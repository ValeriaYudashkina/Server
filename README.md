# Курсовая работа: Программная реализация сетевого сервера

# Сборка проекта
make clean
make

# Просмотр справки
./server --help
./server -h

# Просмотр справки и запуск с параметрами по умолчанию
./server

# Запуск с параметрами
./server --file ../test_users.db --log server.log --port 44444

### Тестирование DataProcessor
# Сборка программы
make test_dataprocessor
# Запуск тестов
./test_dataprocessor
