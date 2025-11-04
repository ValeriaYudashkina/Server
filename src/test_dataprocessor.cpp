#include "DataProcessor.h"
#include "Logger.h"
#include <iostream>
#include <vector>

void testDataProcessor() {
    Logger logger;
    logger.init("dp_test.log");
    DataProcessor processor;
    
    // Тест 1: Нормальное вычисление
    std::vector<int32_t> test1 = {10, 20, 30, 40, 50};
    int32_t result1 = processor.calculateAverage(test1, logger);
    std::cout << "Normal [10,20,30,40,50]: " << result1 << " (expected: 30)" << std::endl;
    
    // Тест 2: Переполнение вверх
    std::vector<int32_t> test2 = {2147483647, 2147483647};
    int32_t result2 = processor.calculateAverage(test2, logger);
    std::cout << "Overflow up [MAX,MAX]: " << result2 << " (expected: 2147483647)" << std::endl;
    
    // Тест 3: Переполнение вниз
    std::vector<int32_t> test3 = {-2147483648, -2147483648};
    int32_t result3 = processor.calculateAverage(test3, logger);
    std::cout << "Overflow down [MIN,MIN]: " << result3 << " (expected: -2147483648)" << std::endl;
    
    // Тест 4: Компенсирующие значения
    std::vector<int32_t> test4 = {-2147483647, 2147483647};
    int32_t result4 = processor.calculateAverage(test4, logger);
    std::cout << "Zero [-MAX,MAX]: " << result4 << " (expected: 0)" << std::endl;
    
    // Тест 5: Большие числа без переполнения
    std::vector<int32_t> test5 = {2147483647, 1};
    int32_t result5 = processor.calculateAverage(test5, logger);
    std::cout << "Large [MAX,1]: " << result5 << " (expected: 1073741824)" << std::endl;
    
    // Тест 6: Пустой вектор
    std::vector<int32_t> test6 = {};
    int32_t result6 = processor.calculateAverage(test6, logger);
    std::cout << "Empty: " << result6 << " (expected: 0)" << std::endl;
}

int main() {
    testDataProcessor();
    return 0;
}
