/**
 * @file Interface.h
 * @brief Заголовочный файл модуля Interface - обработка параметров командной строки
 */

#pragma once
#include <boost/program_options.hpp>
#include <string>
namespace po = boost::program_options;

/**
 * @brief Структура для хранения параметров сервера
 * @details Содержит пути к файлам базы данных и журнала, а также номер порта
 */
struct Params {
    std::string dbFile; ///< Путь к файлу базы данных пользователей
    std::string logFile; ///< Путь к файлу журнала работы сервера
    unsigned short port; ///< Порт
};

/**
 * @brief Класс для обработки параметров командной строки
 * @details Использует библиотеку Boost.program_options для парсинга аргументов
 */
class Interface {
private:
    po::options_description desc; ///< Описание доступных опций командной строки
    po::variables_map vm; ///< Карта для хранения результатов парсинга
    Params params; ///< Экземпляр структуры с параметрами

public:
    /**
     * @brief Конструктор класса Interface
     * @details Инициализирует парсер опциями: help, file, log, port
     */
    Interface();

    /**
     * @brief Парсинг аргументов командной строки
     * @param argc Количество аргументов
     * @param argv Массив аргументов
     * @return true - успешный парсинг рабочих параметров,
     *         false - запрошена справка (help)
     * @throw po::error при ошибках парсинга
     */
    bool Parser(int argc, char** argv);

    /**
     * @brief Получение параметров
     * @return Структура Params с заполненными значениями
     */
    Params getParams() const {
        return params;
    }

    /**
     * @brief Вывод справки по использованию
     */
    void printHelp() const;
};
