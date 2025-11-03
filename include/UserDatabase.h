#pragma once
#include <string>
#include <map>

class Logger;

class UserDatabase {
private:
    std::map<std::string, std::string> users;
public:
    bool load(const std::string& db_path, Logger& logger);
    bool getPassword(const std::string& login, std::string& out_password) const;
};
