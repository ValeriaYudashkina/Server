#pragma once
#include "UserDatabase.h"
#include "Authenticator.h"
#include "DataProcessor.h"
#include "Logger.h"
#include <memory>
#include <vector>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>

class server_error : public std::runtime_error {
public:
    explicit server_error(const std::string& message) 
        : std::runtime_error(message) {}
};

class auth_error : public server_error {
public:
    explicit auth_error(const std::string& message) 
        : server_error("Auth error: " + message) {}
};

class vector_error : public server_error {
public:
    explicit vector_error(const std::string& message) 
        : server_error("Vector error: " + message) {}
};

class Server {
public:
    static const unsigned short MIN_PORT = 1024;
    static const unsigned short MAX_PORT = 49151; 
    Server(unsigned short port, Logger& logger, UserDatabase& userDb, 
           Authenticator& authenticator, DataProcessor& processor);
    ~Server();
    void run();
    
private:
    unsigned short port;
    Logger& logger;
    UserDatabase& userDb;
    Authenticator& authenticator;
    DataProcessor& processor;
    
    int listen_sock;
    std::unique_ptr<sockaddr_in> self_addr;
    std::unique_ptr<sockaddr_in> foreign_addr;

    void validatePort(unsigned short p) const;
    void startListening();
    void handleClient(int client_sock);
    void processVectors(int client_sock);
    std::string readTextMessage(int sock) const;
    void sendError(int client_sock, const std::string& message) const;
};