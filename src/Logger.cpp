#include "Logger.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>

void Logger::init(const std::string& log_path) {
    logPath = log_path;
}

void Logger::logError(const std::string& message, bool isCritical) {
    std::string level;
    if (isCritical) {
        level = "CRITICAL";
    } else {
        level = "ERROR";
    }

    std::ofstream file(logPath, std::ios::app);
    if (file.is_open()) {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        file << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S")
             << "; " << level << "; " << message << std::endl;
        file.flush();
        file.close();
    } else {
        std::cerr << "LOGGER ERROR: Cannot write log to " << logPath << std::endl;
    }

    if (isCritical) {
        std::cerr << "CRITICAL ERROR: " << message << std::endl;
    } else {
        std::cerr << "ERROR: " << message << std::endl;
    }
}
void Logger::logInfo(const std::string& message) {
    std::ofstream file(logPath, std::ios::app);
    if (file.is_open()) {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        file << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S")
             << "; INFO; " << message << std::endl;
        file.flush();
        file.close();
    } else {
        std::cerr << "LOGGER ERROR: Cannot write log to " << logPath << std::endl;
    }
}
