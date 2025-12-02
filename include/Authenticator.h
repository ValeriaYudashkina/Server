/**
 * @file Authenticator.h
 * @brief Заголовочный файл модуля Authenticator - аутентификация пользователей
 */

#pragma once
#include <string>

class UserDatabase; ///< Предварительное объявление класса UserDatabase
class Logger; ///< Предварительное объявление класса Logger

/**
 * @brief Класс для аутентификации пользователей
 * @details Реализует аутентификацию с использованием хеш-функции SHA-1 и соли, формируемой клиентом
 */
class Authenticator {
public:
    /**
     * @brief Проверка аутентификационных данных
     * @param login Логин пользователя
     * @param salt_hash_client Строка формата SALT16 + HASH (56 символов)
     * @param db Ссылка на базу данных пользователей
     * @param logger Ссылка на журнал для записи событий
     * @return true - аутентификация успешна,
     *         false - аутентификация не пройдена
     */
    bool verify(const std::string& login, const std::string& salt_hash_client, 
                UserDatabase& db, Logger& logger) const;

private:
    /**
     * @brief Проверка строки на соотвествие шестнадцатеричному формату
     * @param str Проверяемая строка
     * @return true - строка содержит только hex-символы,
     *         false - строка содержит недопустимые символы
     */
    bool isValidHex(const std::string& str) const;

    static const int SALT16_LENGTH = 16; ///< Длина строки SALT в hex-формате
    static const int SHA1_HEX_LENGTH = 40; ///< Длина хеша SHA-1 в hex-формате
};