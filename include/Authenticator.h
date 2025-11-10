#pragma once
#include <string>

class UserDatabase; 
class Logger;

class Authenticator {
public:
    bool verify(const std::string& login, const std::string& salt_hash_client, 
                UserDatabase& db, Logger& logger) const;
private:
    bool isValidHex(const std::string& str) const;
    static const int SALT16_LENGTH = 16;
    static const int SHA1_HEX_LENGTH = 40;
};