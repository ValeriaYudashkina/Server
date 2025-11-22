#include <UnitTest++/UnitTest++.h>
#include "UserDatabase.h"
#include "Logger.h"
#include <fstream>
#include <cstdio>

SUITE(UserDatabaseTest)
{
    TEST(LoadValidFile) { // Тест 1: Загрузка валидного файла базы данных
        Logger logger;
        logger.init("test.log");
        UserDatabase db;

        std::ofstream file("test_valid.conf");
        file << "user:P@ssW0rd\n";
        file << "admin:admin123\n";
        file.close();
        
        CHECK_EQUAL(true, db.load("test_valid.conf", logger));
        
        std::remove("test_valid.conf");
        std::remove("test.log");
    }
    
    TEST(LoadNonExistentFile) { // Тест 2: Загрузка несуществующего файла
        Logger logger;
        logger.init("test.log");
        UserDatabase db;
        
        CHECK_EQUAL(false, db.load("non_existent_file.conf", logger));
        
        std::remove("test.log");
    }
    
    TEST(LoadEmptyFile) { // Тест 3: Загрузка пустого файла
        Logger logger;
        logger.init("test.log");
        UserDatabase db;
        
        std::ofstream file("empty.conf");
        file.close();
        
        CHECK_EQUAL(false, db.load("empty.conf", logger));
        
        std::remove("empty.conf");
        std::remove("test.log");
    }
    
    TEST(ExistingUser) { // Тест 4: Поиск существующего пользователя
        Logger logger;
        logger.init("test.log");
        UserDatabase db;
        
        std::ofstream file("test.conf");
        file << "user:P@ssW0rd\n";
        file << "admin:admin123\n";
        file.close();
        
        db.load("test.conf", logger);
        
        std::string password;
        CHECK_EQUAL(true, db.getPassword("user", password));
        CHECK_EQUAL("P@ssW0rd", password);
        CHECK_EQUAL(true, db.getPassword("admin", password));
        CHECK_EQUAL("admin123", password);
        
        std::remove("test.conf");
        std::remove("test.log");
    }
    
    TEST(NonExistentUser) { // Тест 5: Поиск несуществующего пользователя
        Logger logger;
        logger.init("test.log");
        UserDatabase db;
        
        std::ofstream file("test.conf");
        file << "user:P@ssW0rd\n";
        file.close();
        
        db.load("test.conf", logger);
        
        std::string password;
        CHECK_EQUAL(false, db.getPassword("nonexistent", password));
        
        std::remove("test.conf");
        std::remove("test.log");
    }
    
    TEST(LoadFileWithComments) { // Тест 6: Загрузка файла с комментариями
        Logger logger;
        logger.init("test.log");
        UserDatabase db;
        
        std::ofstream file("with_comments.conf");
        file << "# Первый комментарий\n";
        file << "user:P@ssW0rd\n";
        file << "# Второй комментарий\n";
        file << "admin:admin123\n";
        file.close();
        
        CHECK_EQUAL(true, db.load("with_comments.conf", logger));
        
        std::string password;
        CHECK_EQUAL(true, db.getPassword("user", password));
        CHECK_EQUAL("P@ssW0rd", password);
        
        std::remove("with_comments.conf");
        std::remove("test.log");
    }
    
    TEST(LoadFileWithInvalidLines) { // Тест 7: Загрузка файла с невалидными строками
        Logger logger;
        logger.init("test_invalid.log");
        UserDatabase db;
        
        std::ofstream file("invalid.conf");
        file << "user:P@ssW0rd\n";
        file << "invalid_line\n";
        file << ":empty_password\n";
        file << "empty_login:\n";
        file << "admin:admin123\n";
        file.close();

        CHECK_EQUAL(true, db.load("invalid.conf", logger));
        
        std::string password;
        CHECK_EQUAL(true, db.getPassword("user", password));
        CHECK_EQUAL("P@ssW0rd", password);
        CHECK_EQUAL(true, db.getPassword("admin", password));
        CHECK_EQUAL("admin123", password);
        
        std::remove("invalid.conf");
        std::remove("test_invalid.log");
    }
}