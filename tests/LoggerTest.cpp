#include <UnitTest++/UnitTest++.h>
#include "Logger.h"
#include <fstream>
#include <string>
#include <cstdio>

SUITE(LoggerTest)
{
    TEST(CreateFile) { // Тест 1: Создание файла при инициализации
        Logger logger;
        std::string filename = "test_create_init.log";
        logger.init(filename);

        logger.logInfo("First message");
        
        std::ifstream file(filename);
        bool file_exists = file.is_open();
        file.close();
        
        if (file_exists) {
            std::remove(filename.c_str());
        }
        
        CHECK(file_exists);
    }
    
    TEST(InfoMessage) { // Тест 2: Запись INFO сообщения
        Logger logger;
        std::string filename = "test_info.log";
        logger.init(filename);
        
        logger.logInfo("Test info message");
        
        std::ifstream file(filename);
        std::string line;
        bool has_info = false;
        bool has_message = false;
        
        if (std::getline(file, line)) {
            has_info = (line.find("INFO") != std::string::npos);
            has_message = (line.find("Test info message") != std::string::npos);
        }
        file.close();
        
        std::remove(filename.c_str());
        
        CHECK(has_info);
        CHECK(has_message);
    }
    
    TEST(ErrorMessage) { // Тест 3: Запись ERROR сообщения
        Logger logger;
        std::string filename = "test_error.log";
        logger.init(filename);
        
        logger.logError("Test error message", false);
        
        std::ifstream file(filename);
        std::string line;
        bool has_error = false;
        bool has_message = false;
        
        if (std::getline(file, line)) {
            has_error = (line.find("ERROR") != std::string::npos);
            has_message = (line.find("Test error message") != std::string::npos);
        }
        file.close();
        
        std::remove(filename.c_str());
        
        CHECK(has_error);
        CHECK(has_message);
    }
    
    TEST(CriticalMessage) { // Тест 4: Запись CRITICAL сообщения
        Logger logger;
        std::string filename = "test_critical.log";
        logger.init(filename);
        
        logger.logError("Test critical message", true);
        
        std::ifstream file(filename);
        std::string line;
        bool has_critical = false;
        bool has_message = false;
        
        if (std::getline(file, line)) {
            has_critical = (line.find("CRITICAL") != std::string::npos);
            has_message = (line.find("Test critical message") != std::string::npos);
        }
        file.close();
        
        std::remove(filename.c_str());
        
        CHECK(has_critical);
        CHECK(has_message);
    }
    
    TEST(CorrectFormat) { // Тест 5: Правильный формат записи
        Logger logger;
        std::string filename = "test_correct_format.log";
        logger.init(filename);
        
        logger.logInfo("Test message");
        
        std::ifstream file(filename);
        std::string line;
        bool correct_format = false;
        
        if (std::getline(file, line)) {
            size_t first_symbol = line.find(";");
            size_t second_symbol = line.find(";", first_symbol + 1);
            
            correct_format = (first_symbol != std::string::npos) && 
                           (second_symbol != std::string::npos) &&
                           (line.find("INFO") != std::string::npos) &&
                           (line.find("Test message") != std::string::npos);
        }
        file.close();
        
        std::remove(filename.c_str());
        
        CHECK(correct_format);
    }
}