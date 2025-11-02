#pragma once
#include <string>
#include <fstream>

class Logger {
private:
    std::string logPath;
public:
    void init(const std::string& log_path);
    void logError(const std::string& message, bool isCritical = false);
    void logInfo(const std::string& message);
};
