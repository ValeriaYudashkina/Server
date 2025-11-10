#include "Server.h"
#include <cstring>
#include <system_error>
#include <arpa/inet.h>
#include <vector>
#include <unistd.h>
#include <algorithm>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFLEN 1024 // Максимальный размер буфера для текстового сообщения аутентификации
#define QLEN 10 // Стандартная очередь для listen
#define AUTH_DATA_LENGTH (16 + 40) // 16 символов SALT + 40 символов HASH

Server::Server(unsigned short port, Logger& logger, UserDatabase& userDb, 
               Authenticator& authenticator, DataProcessor& processor)
    : port(port), logger(logger), userDb(userDb), 
      authenticator(authenticator), processor(processor), 
      listen_sock(-1), self_addr(new sockaddr_in), foreign_addr(new sockaddr_in)
{
    validatePort(port); 
}

Server::~Server() {
    if (listen_sock != -1) {
        close(listen_sock);
        logger.logInfo("Server socket closed");
    }
}

void Server::validatePort(unsigned short p) const {
    if (p < MIN_PORT || p > MAX_PORT) {
        std::string err_msg = "Port (" + std::to_string(p) + ") out of range " + 
                              std::to_string(MIN_PORT) + "-" + std::to_string(MAX_PORT);
        logger.logError(err_msg, true);
        throw std::runtime_error(err_msg);
    }
}

void Server::sendError(int client_sock, const std::string& message) const {
    const char* err_msg = "ERR";
    send(client_sock, err_msg, strlen(err_msg), 0);
    logger.logError("Error sent to client: " + message, false);
}

std::string Server::readTextMessage(int sock) const {
    char buffer[BUFLEN];
    ssize_t rc = recv(sock, buffer, BUFLEN - 1, 0); 
    if (rc == -1) {
        throw std::system_error(errno, std::generic_category(), "recv error reading MSG");
    }
    if (rc == 0) return "";
    
    buffer[rc] = '\0';
    std::string message(buffer);
    
    message.erase(std::remove_if(message.begin(), message.end(), 
                                 [](char c){ return c == '\n' || c == '\r'; }), message.end());
    return message;
}

void Server::startListening() {
    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == -1) {
        throw std::system_error(errno, std::generic_category(), "socket creation failed");
    }

    int on = 1;
    if (setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&on, sizeof on) == -1) {
        logger.logError("setsockopt SO_REUSEADDR failed, continuing", false);
    }

    self_addr->sin_family = AF_INET;
    self_addr->sin_port = htons(port);
    self_addr->sin_addr.s_addr = INADDR_ANY;

    if (bind(listen_sock, reinterpret_cast<const sockaddr*>(self_addr.get()), sizeof(sockaddr_in)) == -1) {
        close(listen_sock);
        throw std::system_error(errno, std::generic_category(), "bind failed");
    }
    if (listen(listen_sock, QLEN) == -1) {
        close(listen_sock);
        throw std::system_error(errno, std::generic_category(), "listen failed");
    }
    logger.logInfo("Server started and listening on port " + std::to_string(port));
}

void Server::run() {
    startListening();
    socklen_t socklen = sizeof(sockaddr_in);
    while(true) {
        int work_sock = -1;
        try {
            logger.logInfo("Waiting for new client...");
            work_sock = accept(listen_sock, reinterpret_cast<sockaddr*>(foreign_addr.get()), &socklen);
            if (work_sock == -1) {
                logger.logError("Accept error: " + std::string(strerror(errno)), false);continue; 
            }
            std::string ip_addr(inet_ntoa(foreign_addr->sin_addr));
            logger.logInfo("Connection established with " + ip_addr);
            handleClient(work_sock);
        } catch (const std::exception& e) {
            logger.logError("Error in server loop: " + std::string(e.what()), false);
        }
        if (work_sock != -1) {
            close(work_sock);
            logger.logInfo("Connection closed");
        }
    }
}

void Server::handleClient(int client_sock) {
    try {
        std::string full_msg = readTextMessage(client_sock);
        if (full_msg.empty()) {
            logger.logError("Client disconnected during authentication", false);
            return;
        }
        if (full_msg.length() < AUTH_DATA_LENGTH) {
            throw auth_error("Auth message too short");
        }
        std::string auth_data = full_msg.substr(full_msg.length() - AUTH_DATA_LENGTH);
        std::string login = full_msg.substr(0, full_msg.length() - AUTH_DATA_LENGTH);

        if (authenticator.verify(login, auth_data, userDb, logger)) {
            const char* ok_msg = "OK";
            send(client_sock, ok_msg, strlen(ok_msg), 0); 
            logger.logInfo("Client '" + login + "' authenticated successfully");
        } else {
            throw auth_error("Authentication failed for login " + login);
        }
        processVectors(client_sock);
    } catch (const auth_error& e) {
        sendError(client_sock, e.what());
        throw;
    } catch (const std::exception& e) {
        sendError(client_sock, "Protocol processing error");
        throw;
    }
}

void Server::processVectors(int sock) {
    uint32_t num_vectors;
    ssize_t rc;

    rc = recv(sock, &num_vectors, sizeof(num_vectors), 0);
    if (rc != sizeof(num_vectors)) {
        throw vector_error("Failed to receive number of vectors");
    }
    num_vectors = ntohl(num_vectors);
    logger.logInfo("Receiving " + std::to_string(num_vectors) + " vectors");
    for (uint32_t i = 0; i < num_vectors; ++i) {
        uint32_t vector_len;
        rc = recv(sock, &vector_len, sizeof(vector_len), 0);
        if (rc != sizeof(vector_len)) {
             throw vector_error("Failed to receive vector length");
        }
        vector_len = ntohl(vector_len);
        
        size_t total_bytes_needed = vector_len * sizeof(int32_t);

        if (vector_len == 0 || total_bytes_needed > 4000000000) { 
             throw vector_error("Vector size invalid or too large");
        }
        std::vector<int32_t> data(vector_len);
        rc = recv(sock, data.data(), total_bytes_needed, 0);
        if (rc != (ssize_t)total_bytes_needed) {
            throw vector_error("Vector data size mismatch");
        }

        for (auto& value : data) {
            value = ntohl(value);
        }
        int32_t result = processor.calculateAverage(data, logger);

        uint32_t num_results = htonl(1);
        int32_t net_result = htonl(result);

        send(sock, &num_results, sizeof(num_results), 0);
        send(sock, &net_result, sizeof(net_result), 0);
        
        logger.logInfo("Processed vector " + std::to_string(i+1) + ", result: " + std::to_string(result));
    }
}