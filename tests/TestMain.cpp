#include <UnitTest++/UnitTest++.h>
#include <iostream>

int main(int argc, char** argv) {
    std::cout << "Модульное тестирование сервера" << std::endl;
    int result = UnitTest::RunAllTests();
    if (result == 0) {
        std::cout << "Все тесты пройдены успешно" << std::endl;
    } else {
        std::cout << "Некоторые тесты не были пройдены" << std::endl;
    }
    return result;
}
