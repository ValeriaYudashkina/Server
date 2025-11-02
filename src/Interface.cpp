#include "Interface.h"
#include <iostream>

Interface::Interface() : desc("Allowed options") {
    desc.add_options()
    ("help,h", "Show help")
    ("file,f", po::value<std::string>(&params.dbFile)->default_value("/etc/vcalc.conf"), "User database file")
    ("log,l", po::value<std::string>(&params.logFile)->default_value("/var/log/vcalc.log"), "Log file")
    ("port,p", po::value<unsigned short>(&params.port)->default_value(33333), "Server port");
}

bool Interface::Parser(int argc, char** argv) {
    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        if (vm.count("help")) {
            return false; //печать справки
        }
        po::notify(vm);
    } catch (const std::exception& e) {
        std::cerr << "Error parsing arguments: " << e.what() << std::endl;
        return false;
    }
    return true;
}
void Interface::printHelp() const {
    std::cout << desc << std::endl;
}
