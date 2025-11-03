#include "Interface.h"
#include "Logger.h"
#include "UserDatabase.h"
#include <iostream>
#include <string>

bool isValidPort(unsigned short port) {
    return port >= 1024 && port <= 49151;
}

int main(int argc, char** argv) {
    Logger logger;
    Interface iface;

    if (argc == 1) {
        iface.printHelp();
    }
    if (!iface.Parser(argc, argv)) {
        iface.printHelp();
        return 0;
    }
    Params params = iface.getParams();
    logger.init(params.logFile);
    logger.logInfo("Server configuration parsing completed");

    UserDatabase userDb;
    if (!userDb.load(params.dbFile, logger)) {
        return 1;
    }

    if (!isValidPort(params.port)) {
        std::string error_msg = "Invalid port number: " + std::to_string(params.port);
        logger.logError(error_msg, true);
        return 1;
    }

    std::cout << "Server configuration loaded successfully" << std::endl;
    std::cout << "Database file: " << params.dbFile << std::endl;
    std::cout << "Log file: " << params.logFile << std::endl;
    std::cout << "Port: " << params.port << std::endl;
    return 0;
}
