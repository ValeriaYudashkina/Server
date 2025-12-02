/**
 * @file DataProcessor.cpp
 * @brief Реализация класса DataProcessor для обработки числовых данных
 */

#include "DataProcessor.h"
#include "Logger.h"

/**
 * @brief Вычисление среднего арифметического значений вектора
 * @param vector Вектор целых чисел для обработки
 * @param logger Ссылка на журнал для записи ошибок
 * @return Среднее арифметическое значений вектора
 * @details Алгоритм:
 *          1. Проверка на пустой вектор (возвращает 0)
 *          2. Суммирование значений с использованием int64_t для предотвращения переполнения
 *          3. Деление суммы на количество элементов
 *          4. Проверка границ
 * @note При переполнении возвращает:
 *       - 2147483647 (INT32_MAX) при переполнении вверх
 *       - -2147483647 (INT32_MIN) при переполнении вниз
 * @warning При пустом векторе возвращает 0 и записывает предупреждение в журнал
 */
int32_t DataProcessor::calculateAverage(const std::vector<int32_t>& vector, Logger& logger) {
    if (vector.empty()) {
        logger.logError("Vector is empty", false);
        return 0;
    }
    
    int64_t sum = 0; //предотвращение промежуточного переполнения
    for (int32_t val : vector) {
        sum += val;
    }
    
    int64_t avrg = sum / static_cast<int64_t>(vector.size());
    
    if (avrg > 2147483647) { // 2^(31-1)
        logger.logError("Overflow detected (upwards)", false);
        return 2147483647;
    }
    if (avrg < -2147483648) { // -2^31
        logger.logError("Overflow detected (downwards)", false);
        return -2147483648;
    }
    
    return static_cast<int32_t>(avrg);
}
