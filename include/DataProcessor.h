#pragma once
#include <vector>
#include <cstdint>

class Logger;

/**
 * Работа с двоичными данными вынесена в модуль Server
*/
class DataProcessor {
public:
    int32_t calculateAverage(const std::vector<int32_t>& vector, Logger& logger);
};
