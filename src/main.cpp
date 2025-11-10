#include "Interface.h"
#include "Logger.h"
#include "UserDatabase.h"
#include "DataProcessor.h"
#include "Authenticator.h"
#include "Server.h"
#include <iostream>
#include <string>

bool isValidPort(unsigned short port) {
    return port >= 1024 && port <= 49151;
}

int main(int argc, char** argv) {
    Logger logger;
    Interface iface;

    if (argc == 1) {
        iface.printHelp(); // вывод справки и запуск сервера
    }

    if (!iface.Parser(argc, argv)) {
        iface.printHelp(); // вывод справки и завершение программы
        return 0;
    }
    Params params = iface.getParams();
    logger.init(params.logFile);
    logger.logInfo("Server configuration parsing completed");

    UserDatabase userDb;
    if (!userDb.load(params.dbFile, logger)) {
        return 1;
    }

    Authenticator auth;
    DataProcessor processor;
    logger.logInfo("Authenticator and DataProcessor initialized");

    if (!isValidPort(params.port)) {
        std::string error_msg = "Invalid port number: " + std::to_string(params.port);
        logger.logError(error_msg, true);
        return 1;
    }

    std::cout << "Server configuration loaded successfully" << std::endl;
    std::cout << "Database file: " << params.dbFile << std::endl;
    std::cout << "Log file: " << params.logFile << std::endl;
    std::cout << "Port: " << params.port << std::endl;

    try {
        Server server(params.port, logger, userDb, auth, processor);
        server.run();
    } catch (const std::exception& e) {
        logger.logError("Server fatal error: " + std::string(e.what()), true);
        std::cerr << "Server error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
