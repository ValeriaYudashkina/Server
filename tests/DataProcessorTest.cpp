#include <UnitTest++/UnitTest++.h>
#include "DataProcessor.h"
#include "Logger.h"
#include <fstream>
#include <vector>
#include <climits>

SUITE(DataProcessorTest)
{
    struct DataProcessorFixture {
        Logger logger;
        DataProcessor processor;
        std::string test_log_file;
        
        DataProcessorFixture() : test_log_file("test_processor.log") {
            logger.init(test_log_file);
        }
        
        ~DataProcessorFixture() {
            std::remove(test_log_file.c_str());
        }
    };

    TEST_FIXTURE(DataProcessorFixture, ValidValue) { // Тест 1: Валидные значения
        std::vector<int32_t> data = {1, 2, 3, 4, 5};
        int32_t result = processor.calculateAverage(data, logger);
        CHECK_EQUAL(3, result); // (1+2+3+4+5)/5 = 3
    }
    
    TEST_FIXTURE(DataProcessorFixture, SingleElement) { // Тест 2: Один элемент
        std::vector<int32_t> data = {42};
        int32_t result = processor.calculateAverage(data, logger);
        CHECK_EQUAL(42, result);
    }
    
    TEST_FIXTURE(DataProcessorFixture, EmptyVector) { // Тест 3: Пустой вектор
        std::vector<int32_t> data;
        int32_t result = processor.calculateAverage(data, logger);
        CHECK_EQUAL(0, result);
    }
    
    TEST_FIXTURE(DataProcessorFixture, NegativeNumbers) { // Тест 4: Отрицательные числа
        std::vector<int32_t> data = {-1, -2, -3, -4};
        int32_t result = processor.calculateAverage(data, logger);
        CHECK_EQUAL(-2, result); // (-1-2-3-4)/4 = -2
    }
    
    TEST_FIXTURE(DataProcessorFixture, PositiveNegativeNumbers) { // Тест 5: положительные и отрицательные числа
        std::vector<int32_t> data = {-10, 0, 10};
        int32_t result = processor.calculateAverage(data, logger);
        CHECK_EQUAL(0, result); // (-10+0+10)/3 = 0
    }
    
    TEST_FIXTURE(DataProcessorFixture, LargeNumbers) { // Тест 6: Большие числа
        std::vector<int32_t> data = {1000000, 2000000, 3000000};
        int32_t result = processor.calculateAverage(data, logger);
        CHECK_EQUAL(2000000, result); // 6000000/3 = 2000000
    }
        
    TEST_FIXTURE(DataProcessorFixture, OverflowUp) { // Тест 7: Переполнение вверх
        std::vector<int32_t> data = {INT_MAX, INT_MAX};
        int32_t result = processor.calculateAverage(data, logger);
        CHECK_EQUAL(INT_MAX, result);
    }
    
    TEST_FIXTURE(DataProcessorFixture, OverflowDown) { // Тест 8: Переполнение вниз
        std::vector<int32_t> data = {INT_MIN, INT_MIN};
        int32_t result = processor.calculateAverage(data, logger);
        CHECK_EQUAL(INT_MIN, result);
    }
}