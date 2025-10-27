#include "Interface.h"
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    Interface iface;
    if (!iface.Parser(argc, argv)) {
        iface.printHelp();
        return 0;
    }
    Params params = iface.getParams();
    std::cout << "Server configuration loaded successfully" << std::endl;
    std::cout << "Database file: " << params.dbFile << std::endl;
    std::cout << "Log file: " << params.logFile << std::endl;
    std::cout << "Port: " << params.port << std::endl;
    return 0;
}
