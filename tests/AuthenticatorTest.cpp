#include <UnitTest++/UnitTest++.h>
#include "Authenticator.h"
#include "UserDatabase.h"
#include "Logger.h"
#include <fstream>
#include <cstdio>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>

using namespace CryptoPP;

SUITE(AuthenticatorTest)
{
    struct AuthTestFixture {
        Logger logger;
        UserDatabase db;
        Authenticator auth;
        std::string test_db_file;
        
        AuthTestFixture() : test_db_file("test_auth.conf") {
            logger.init("test_auth.log");
            createTestDatabase();
            db.load(test_db_file, logger);
        }
        
        ~AuthTestFixture() {
            std::remove(test_db_file.c_str());
            std::remove("test_auth.log");
        }
        
        void createTestDatabase() {
            std::ofstream db_file(test_db_file);
            db_file << "user:P@ssW0rd\n";
            db_file << "testuser:testpassword\n";
            db_file.close();
        }
        
        std::string computeValidHash(const std::string& salt, const std::string& password) {
            std::string input = salt + password;
            std::string hash;
            
            SHA1 sha1;
            StringSource(input, true,
                new HashFilter(sha1,
                    new HexEncoder(
                        new StringSink(hash))));
            
            return hash;
        }
    };

    TEST_FIXTURE(AuthTestFixture, ValidMessageLength) { // Тест 1: Неверное сообщение аутентификации
        std::string valid_length_msg(56, 'A'); // 16 соль + 40 хеш, невалидный хеш
        CHECK_EQUAL(false, auth.verify("user", valid_length_msg, db, logger));
    }
    
    TEST_FIXTURE(AuthTestFixture, ShortMessage) { // Тест 2: Короткое сообщение
        std::string short_msg = "short"; // Меньше 56 символов
        CHECK_EQUAL(false, auth.verify("user", short_msg, db, logger));
    }
    
    TEST_FIXTURE(AuthTestFixture, Authenticator_Verify_LongMessage_ReturnsFalse) { // Тест 3: Длинное сообщение
        std::string long_msg(100, 'A'); // Больше 56 символов
        CHECK_EQUAL(false, auth.verify("user", long_msg, db, logger));
    }
    
    TEST_FIXTURE(AuthTestFixture, NonExistentUser) { // Тест 4: Несуществующий пользователь
        std::string auth_data(56, 'A');
        CHECK_EQUAL(false, auth.verify("nonexistent", auth_data, db, logger));
    }
    
    TEST_FIXTURE(AuthTestFixture, InvalidSalt) { // Тест 5: Невалидная соль
        std::string invalid_salt = "INVALID_SALT!!"; // Не hex символы
        std::string hash(40, '0'); // Валидный хеш
        std::string auth_data = invalid_salt + hash;
        
        CHECK_EQUAL(false, auth.verify("user", auth_data, db, logger));
    }
    
    TEST_FIXTURE(AuthTestFixture, InvalidHash) { // Тест 6: Невалидный хеш
        std::string valid_salt = "1234567890ABCDEF"; // Валидная соль
        std::string invalid_hash(40, 'X'); // Не hex символы
        std::string auth_data = valid_salt + invalid_hash;
        
        CHECK_EQUAL(false, auth.verify("user", auth_data, db, logger));
    }
    
    TEST_FIXTURE(AuthTestFixture, EmptyLogin) { // Тест 7: Пустой логин
        std::string auth_data(56, 'A');
        CHECK_EQUAL(false, auth.verify("", auth_data, db, logger));
    }

    TEST_FIXTURE(AuthTestFixture, ValidAuthentication) { // Тест 8: Успешная аутентификация
        std::string valid_salt = "1234567890ABCDEF";
        std::string password;
        bool user_exists = db.getPassword("user", password);
        CHECK(user_exists);
        std::string valid_hash = computeValidHash(valid_salt, password);
        std::string auth_data = valid_salt + valid_hash;
        CHECK_EQUAL(true, auth.verify("user", auth_data, db, logger));
    }
}