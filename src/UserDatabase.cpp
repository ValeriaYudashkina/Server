#include "UserDatabase.h"
#include "Logger.h"
#include <fstream>
#include <iostream>

bool UserDatabase::load(const std::string& db_path, Logger& logger) {
    std::ifstream file(db_path);
    if (!file.is_open()) {
        logger.logError("Cannot open database file: " + db_path, true);
        return false;
    }

    std::string line;
    int line_number = 0;
    while (std::getline(file, line)) {
        line_number++;
        if (line.empty() || line[0] == '#') {
            continue;
        }
        size_t pos = line.find(':');
        if (pos == std::string::npos) {
            logger.logError("Invalid string format " + std::to_string(line_number), false);
            continue;
        }
        std::string login = line.substr(0, pos);
        std::string password = line.substr(pos + 1);
        if (login.empty() || password.empty()) {
            logger.logError("Empty login or password on line " + std::to_string(line_number), false);
            continue;
        }
        users[login] = password;
    }
    file.close();

    if (users.empty()) {
        logger.logError("User database is empty", true);
        return false;
    }
    logger.logInfo("Loaded " + std::to_string(users.size()) + " users from database");
    return true;
}

bool UserDatabase::getPassword(const std::string& login, std::string& out_password) const {
    auto it = users.find(login);
    if (it != users.end()) {
        out_password = it->second;
        return true;
    }
    return false;
}
