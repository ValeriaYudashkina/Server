/**
 * @file UserDatabase.h
 * @brief Заголовочный файл модуля UserDatabase - работа с базой пользователей
 */

#pragma once
#include <string>
#include <map>

class Logger; ///< Предварительное объявление класса Logger

/**
 * @brief Класс для работы с базой данных пользователей
 * @details Загружает пары "логин:пароль" из текстового файла и предоставляет доступ к ним для аутентификации
 */
class UserDatabase {
private:
    std::map<std::string, std::string> users; ///< Контейнер для хранения пользователей

public:
    /**
     * @brief Загрузка базы пользователей из файла
     * @param db_path Путь к файлу базы данных
     * @param logger Ссылка на объект журнала для записи ошибок
     * @return true - база успешно загружена,
     *         false - произошла ошибка при загрузке
     */
    bool load(const std::string& db_path, Logger& logger);

    /**
     * @brief Получение пароля пользователя по логину
     * @param login Логин пользователя
     * @param out_password Строка для записи пароля
     * @return true - пользователь найден,
     *         false - пользователь не найден
     */
    bool getPassword(const std::string& login, std::string& out_password) const;
};
