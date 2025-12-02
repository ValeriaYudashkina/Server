/**
 * @file main.cpp
 * @author Юдашкина В.О.
 * @version 1.0
 * @copyright ИБСТ ПГУ
 * @brief Точка входа серверной программы
 * @mainpage Сетевой сервер обработки данных
 * 
 * Программа реализует сетевой сервер для аутентификации клиентов и вычисления среднего арифметического значений векторов.
 *
 * @section usage_sec Использование
 *
 * Запуск сервера:
 * ./server [--file FILE] [--log FILE] [--port PORT]
 * 
 * Вывод справки:
 * ./server --help
 * ./server -h
 *
 */

#include "Interface.h"
#include "Logger.h"
#include "UserDatabase.h"
#include "DataProcessor.h"
#include "Authenticator.h"
#include "Server.h"
#include <iostream>
#include <string>

/**
 * @brief Проверка валидности номера порта
 * @param port Номер порта для проверки
 * @return true - порт в допустимом диапазоне,
 *         false - порт вне диапазона
 * @note Диапазон допустимых портов: 1024-49151
 */
bool isValidPort(unsigned short port) {
    return port >= 1024 && port <= 49151;
}

/**
 * @brief Главная функция программы
 * @param argc Количество аргументов командной строки
 * @param argv Массив аргументов командной строки
 * @return 0 - успешное завершение,
 *         1 - ошибка инициализации,
 *         2 - ошибка выполнения
 * @details Выполняет инициализацию всех модулей и запускает сервер
 * @throw std::exception при критических ошибках выполнения
 * @note При запуске без параметров выводит справку и запускает сервер
 */
int main(int argc, char** argv) {
    Logger logger; ///< Объект для ведения журнала работы сервера
    Interface iface; ///< Объект для обработки параметров командной строки

    if (argc == 1) {
        iface.printHelp(); // вывод справки и запуск сервера
    }

    /**
     * @brief Парсинг аргументов командной строки
     * @details Если парсинг не удался или запрошена справка, выводит справку и завершает работу
     */
    if (!iface.Parser(argc, argv)) {
        iface.printHelp(); // вывод справки и завершение программы
        return 0;
    }

    /**
     * @brief Получение параметров конфигурации
     * @details Извлекает распарсенные параметры для дальнейшего использования
     */
    Params params = iface.getParams();

    /**
     * @brief Инициализация системы журналирования
     * @details Настраивает журнал на запись в указанный файл
     */
    logger.init(params.logFile);
    logger.logInfo("Server configuration parsing completed");

    /**
     * @brief Загрузка базы данных пользователей
     * @details Загружает пары "логин:пароль" из указанного файла
     */
    UserDatabase userDb;
    if (!userDb.load(params.dbFile, logger)) {
        return 1; ///< Критическая ошибка: не удалось загрузить базу данных
    }

    /**
     * @brief Инициализация модулей обработки
     * @details Создает объекты для аутентификации и обработки данных
     */
    Authenticator auth;
    DataProcessor processor;
    logger.logInfo("Authenticator and DataProcessor initialized");

    /**
     * @brief Проверка валидности порта
     * @details Убеждается, что указанный порт находится в допустимом диапазоне
     */
    if (!isValidPort(params.port)) {
        std::string error_msg = "Invalid port number: " + std::to_string(params.port);
        logger.logError(error_msg, true);
        return 1; ///< Критическая ошибка: неверный порт
    }

    /**
     * @brief Вывод информации о конфигурации
     * @details Отображает в консоли успешно загруженные параметры
     */
    std::cout << "Server configuration loaded successfully" << std::endl;
    std::cout << "Database file: " << params.dbFile << std::endl;
    std::cout << "Log file: " << params.logFile << std::endl;
    std::cout << "Port: " << params.port << std::endl;

    /**
     * @brief Создание и запуск сервера
     * @details Основной блок выполнения программы
     */
    try {
        Server server(params.port, logger, userDb, auth, processor);
        server.run(); ///< Запуск основного цикла сервера
    } catch (const std::exception& e) {
        /**
         * @brief Обработка критических ошибок выполнения
         * @details Записывает в журнал ошибку и завершает работу
         */
        logger.logError("Server fatal error: " + std::string(e.what()), true);
        std::cerr << "Server error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
