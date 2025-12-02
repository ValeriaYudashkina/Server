# Курсовая работа: Программная реализация сетевого сервера

# Сборка проекта
make clean
make
make unit_test
make test_userdb
make test_auth
make test_processor
make test_logger
make test_interface

# Просмотр справки
./server --help
./server -h

# Просмотр справки и запуск с параметрами по умолчанию
./server

# Запуск с параметрами
./server --file ../test_users.db --log server.log --port 44444

# Запуск модульного тестирования
./server_tests
