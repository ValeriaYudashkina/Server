#include <UnitTest++/UnitTest++.h>
#include "Interface.h"
#include <vector>
#include <string>

SUITE(InterfaceTest)
{
    TEST(DefaultValues) { // Тест 1: Запуск без параметров
        Interface iface;

        const char* argv[] = {"test_program"};
        int argc = 1; 

        CHECK_EQUAL(true, iface.Parser(argc, const_cast<char**>(argv)));
        
        Params p = iface.getParams();

        CHECK_EQUAL("etc/vcalc.conf", p.dbFile);
        CHECK_EQUAL("var/log/vcalc.log", p.logFile);
        CHECK_EQUAL(33333, p.port);
    }

    TEST(CustomAllParams) { // Тест 2: Валидные параметры
        Interface iface;
        
        std::string custom_db = "/tmp/my.conf";
        std::string custom_log = "my/custom.log";
        const unsigned short custom_port = 55555;
        
        const char* argv[] = {
            "test_program", 
            "-f", custom_db.c_str(),
            "-l", custom_log.c_str(),
            "-p", "55555"
        };
        int argc = 7; 
        
        CHECK_EQUAL(true, iface.Parser(argc, const_cast<char**>(argv)));
        
        Params p = iface.getParams();
        
        CHECK_EQUAL(custom_db, p.dbFile);
        CHECK_EQUAL(custom_log, p.logFile);
        CHECK_EQUAL(custom_port, p.port);
    }
    
    TEST(HelpShortOption) { // Тест 3: Справка -h
        Interface iface;
        
        const char* argv[] = {"test_program", "-h"};
        int argc = 2;
        
        CHECK_EQUAL(false, iface.Parser(argc, const_cast<char**>(argv)));
    }
    
    TEST(HelpLongOption) { // Тест 4: Справка --help
        Interface iface;
        
        const char* argv[] = {"test_program", "--help"};
        int argc = 2;
        
        CHECK_EQUAL(false, iface.Parser(argc, const_cast<char**>(argv)));
    }
    
    TEST(InvalidPortValue) { // Тест 5: Невалидный порт (не число)
        Interface iface;
        
        const char* argv[] = {"test_program", "-p", "invalid_port"}; 
        int argc = 3;

        CHECK_EQUAL(false, iface.Parser(argc, const_cast<char**>(argv)));
    }
    
    TEST(UnknownArgument) { // Тест 6: Невалидный аргумент
        Interface iface;
        
        const char* argv[] = {"test_program", "-z"};
        int argc = 2;
        
        CHECK_EQUAL(false, iface.Parser(argc, const_cast<char**>(argv)));
    }
    
    TEST(MissingArgumentValue) { // Тест 7: Отсутствие значения параметра
        Interface iface;
        
        const char* argv[] = {"test_program", "-f"};
        int argc = 2;
        
        CHECK_EQUAL(false, iface.Parser(argc, const_cast<char**>(argv)));
    }
}