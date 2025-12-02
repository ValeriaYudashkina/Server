/**
 * @file DataProcessor.h
 * @brief Заголовочный файл модуля DataProcessor - обработка числовых данных
 */

#pragma once
#include <vector>
#include <cstdint>

class Logger; ///< Предварительное объявление класса Logger

/**
 * @brief Класс для обработки числовых данных
 * @details Выполняет вычисления над векторами целых чисел
 */
class DataProcessor {
public:
    /**
     * @brief Вычисление среднего арифметического значений вектора
     * @param vector Вектор целых чисел для обработки
     * @param logger Ссылка на объект журнала для записи ошибок
     * @return Среднее арифметическое значений вектора
     * @note при переполнении возвращает INT32_MAX или INT32_Min
     * @warning При пустом векторе возвращает 0
     */
    int32_t calculateAverage(const std::vector<int32_t>& vector, Logger& logger);
};
