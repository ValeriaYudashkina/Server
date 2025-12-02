/**
 * @file Authenticator.cpp
 * @brief Реализация класса Authenticator для аутентификации пользователей
 */

#include "Authenticator.h"
#include "UserDatabase.h"
#include "Logger.h"
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>
#include <iostream>
#include <algorithm>
#include <cctype>

namespace CPP = CryptoPP;

/**
 * @brief Проверка строки на соответствие шестнадцатеричному формату
 * @param str Проверяемая строка
 * @param logger Ссылка на журнал для записи ошибок
 * @return true - строка содержит только hex-символы,
 *         false - строка содержит недопустимые символы
 * @details Допустимые символы: 0-9, A-F, a-f
 */
bool Authenticator::isValidHex(const std::string& str) const {
    return std::all_of(str.begin(), str.end(), [](char c) {
        return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
    });
}

/**
 * @brief Проверка аутентификационных данных пользователя
 * @param login Логин пользователя
 * @param salt_hash_client Строка формата SALT16 + HASH (56 символов)
 * @param db Ссылка на базу данных пользователей
 * @param logger Ссылка на журнал для записи ошибок
 * @return true - аутентификация успешна,
 *         false - аутентификация не пройдена
 * @details Алгоритм проверки:
 *          1. Проверка длины сообщения (16+40=56 символов)
 *          2. Валидация hex-формата SALT и HASH
 *          3. Поиск пользователя в базе данных
 *          4. Вычисление хеша SHA-1 от (SALT + PASSWORD)
 *          5. Сравнение вычисленного хеша с полученным
 * @note Используется схема: HASH = SHA1(SALT || PASSWORD)
 * @warning При несовпадении хешей в журнал записывается ошибка аутентификации
 */
bool Authenticator::verify(const std::string& login, const std::string& salt_hash_client, 
                           UserDatabase& db, Logger& logger) const {
    
    if (salt_hash_client.length() != SALT16_LENGTH + SHA1_HEX_LENGTH) {
        logger.logError("Authenticator: Message length mismatch. Expected: " + 
                       std::to_string(SALT16_LENGTH + SHA1_HEX_LENGTH) + 
                       ", got: " + std::to_string(salt_hash_client.length()), false);
        return false;
    }
    
    std::string salt16 = salt_hash_client.substr(0, SALT16_LENGTH);
    std::string clientHash16 = salt_hash_client.substr(SALT16_LENGTH);
    
    if (!isValidHex(salt16)) {
        logger.logError("Authenticator: Invalid hex format in SALT16: " + salt16, false);
        return false;
    }
    
    if (!isValidHex(clientHash16)) {
        logger.logError("Authenticator: Invalid hex format in client hash", false);
        return false;
    }
    
    std::string password;
    if (!db.getPassword(login, password)) {
        logger.logError("Authenticator: Login " + login + " not found", false);
        return false;
    }
    
    std::string input = salt16 + password;
    std::string serverHash16;
    
    try {
        CPP::SHA1 hash;
        
        CPP::StringSource(input, true,
            new CPP::HashFilter(hash, 
                new CPP::HexEncoder(
                    new CPP::StringSink(serverHash16))));
        
    } catch (const CPP::Exception& e) {
        logger.logError(std::string("Crypto++ error: ") + e.what(), true);
        return false;
    }
    
    std::transform(clientHash16.begin(), clientHash16.end(), clientHash16.begin(), ::toupper);
    std::transform(serverHash16.begin(), serverHash16.end(), serverHash16.begin(), ::toupper);
    
    if (serverHash16 == clientHash16) {
        logger.logInfo("Authenticator: Success for login " + login);
        return true;
    } else {
        logger.logError("Authenticator: Password mismatch for login " + login, false);
        return false;
    }
}
