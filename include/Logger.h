/**
 * @file Logger.h
 * @brief Заголовочный файл модуля Logger - журналирование работы сервера
 */

#pragma once
#include <string>
#include <fstream>

/**
 * @brief Класс для ведения журнала работы сервера
 * @details Обеспечивает запись информационных сообщений и ошибок в файл
 */
class Logger {
private:
    std::string logPath; ///< Путь к файлу журнала

public:
    /**
     * @brief Инициализация журнала
     * @param log_path Путь к файлу журнала
     */
    void init(const std::string& log_path);

    /**
     * @brief Запись сообщения об ошибке
     * @param message Текст сообщения об ошибке
     * @param isCritical Флаг критичности ошибки
     */
    void logError(const std::string& message, bool isCritical = false);

    /**
     * @brief Запись информационного сообщения
     * @param message Текст информационного сообщения
     */
    void logInfo(const std::string& message);
};
