#include "Interface.h"
#include <iostream>
#include <string>

bool isValidPort(unsigned short port) {
    return port >= 1024 && port <= 49151;
}

int main(int argc, char** argv) {
    Interface iface;

    if (argc == 1) {
        iface.printHelp();
    }

    if (!iface.Parser(argc, argv)) {
        iface.printHelp();
        return 0;
    }
    Params params = iface.getParams();

    if (!isValidPort(params.port)) {
        std::cerr << "Error: Invalid port number" << std::endl;
        return 1;
    }

    std::cout << "Server configuration loaded successfully" << std::endl;
    std::cout << "Database file: " << params.dbFile << std::endl;
    std::cout << "Log file: " << params.logFile << std::endl;
    std::cout << "Port: " << params.port << std::endl;
    return 0;
}
