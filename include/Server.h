/**
 * @file Server.h
 * @brief Заголовочный файл модуля Server - основной серверный модуль
 */

#pragma once
#include "UserDatabase.h"
#include "Authenticator.h"
#include "DataProcessor.h"
#include "Logger.h"
#include <memory>
#include <vector>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>

/**
 * @brief Базовый класс исключений сервера
 * @details Наследуется от std::runtime_error
 */
class server_error : public std::runtime_error {
public:
    /**
     * @brief Конструктор исключения
     * @param message Сообщение об ошибке
     */
    explicit server_error(const std::string& message) 
        : std::runtime_error(message) {}
};

/**
 * @brief Исключение для ошибок аутентификации
 */
class auth_error : public server_error {
public:
    /**
     * @brief Конструктор исключения аутентификации
     * @param message Сообщение об ошибке аутентификации
     */
    explicit auth_error(const std::string& message) 
        : server_error("Auth error: " + message) {}
};

/**
 * @brief Исключение для ошибок обработки векторов
 */
class vector_error : public server_error {
public:
    /**
     * @brief Конструктор исключения ошибок обработки векторов
     * @param message Сообщение об ошибке обработки векторов
     */
    explicit vector_error(const std::string& message) 
        : server_error("Vector error: " + message) {}
};

/**
 * @brief Основной класс сервера
 * @details Реализует сетевой сервер с аутентификацией и обработкой данных
 */
class Server {
public:
    static const unsigned short MIN_PORT = 1024; ///< Минимальный допустимый порт
    static const unsigned short MAX_PORT = 49151;  ///< Максимальный допустимый порт

    /**
     * @brief Конструктор сервера
     * @param port Порт
     * @param logger Ссылка на журнал
     * @param userDb Ссылка на базу данных пользователей
     * @param authenticator Ссылка на аутентификатор
     * @param processor Ссылка на обработчик данных
     * @throw std::runtime_error при невалидном порте
     */
    Server(unsigned short port, Logger& logger, UserDatabase& userDb, 
           Authenticator& authenticator, DataProcessor& processor);

    /**
     * @brief Деструктор сервера
     * @details Закрывает сокеты и освобождает ресурсы
     */
    ~Server();

    /**
     * @brief Основной метод запуска сервера
     * @details Запускает бесконечный цикл обработки подключений
     * @note Метод не возвращает управление
     * @throw std::system_error при ошибках сетевого взаимодействия
     */
    void run();
    
private:
    unsigned short port; ///< Порт сервера
    Logger& logger; ///< Ссылка на журнал
    UserDatabase& userDb; ///< Ссылка на базу данных пользователей
    Authenticator& authenticator; ///< Ссылка на аутентификатор
    DataProcessor& processor; ///< Ссылка на обработчик данных
    
    int listen_sock; ///< Сокет
    std::unique_ptr<sockaddr_in> self_addr; ///< Адрес сервера
    std::unique_ptr<sockaddr_in> foreign_addr; ///< Адрес клиента

    /**
     * @brief Проверка валидности номера порта
     * @param p Проверяемый порт
     * @throw std::runtime_error при невалидном порте
     */
    void validatePort(unsigned short p) const;

    /**
     * @brief Инициализация и запуск сокета
     * @throw std::system_error при ошибках создания сокета
     */
    void startListening();

    /**
     * @brief Обработка одного клиента
     * @param client_sock Сокет подключенного клиента
     * @throw auth_error при ошибках аутентификации
     * @throw vector_error при ошибках обработки векторов
     */
    void handleClient(int client_sock);

    /**
     * @brief Обработка векторов данных от клиента
     * @param client_sock Сокет клиента
     * @throw vector_error при ошибках обработки векторов
     */
    void processVectors(int client_sock);

    /**
     * @brief Чтение текстового сообщения от клиента
     * @param sock Сокет клиента
     * @return Прочитанная строка
     * @throw std::system_error при ошибках чтения
     */
    std::string readTextMessage(int sock) const;

    /**
     * @brief Отправка сообщения об ошибке клиенту
     * @param client_sock Сокет клиента
     * @param message Текст сообщения для записи в журнал
     */
    void sendError(int client_sock, const std::string& message) const;
};