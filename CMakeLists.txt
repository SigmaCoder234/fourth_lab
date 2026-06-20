cmake_minimum_required(VERSION 3.14)
project(fourth_lab CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Импорт Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(src)
include_directories(second_lab/src)

# Главный исполняемый файл 
add_executable(app
    main.cpp
    # Базовые коллекции из ЛР-2
    second_lab/src/DynamicArray.h
    second_lab/src/LinkedList.h
    second_lab/src/Option.h
    second_lab/src/Sequence.h
    second_lab/src/ArraySequence.h
    second_lab/src/ListSequence.h
    # Новые файлы для ЛР-4
    src/Functors.h
    src/Generator.h
    src/LazySequence.h
    src/Stream.h
    src/PriorityQueue.h
    src/OnlineStatistics.h
)

# Исполняемый файл для тестов
add_executable(run_tests
    tests/test_lazy.cpp
    tests/test_generator.cpp
    tests/test_queue.cpp
    tests/test_streams.cpp
    tests/test_statistics.cpp
    tests/test_ordinal.cpp
)
target_link_libraries(run_tests gtest_main)
include(CTest)
enable_testing()
add_test(NAME AllTests COMMAND run_tests)
