#include "DataProcessor.h"
#include "Logger.h"

/**
 * Работа с двоичными файлами вынесена в модуль Server
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
