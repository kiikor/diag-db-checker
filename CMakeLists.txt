cmake_minimum_required(VERSION 3.16)
project(diag_db_checker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

add_library(checker_lib
    src/checker.cpp
)

target_include_directories(checker_lib PRIVATE include)
target_link_libraries(checker_lib PRIVATE nlohmann_json::nlohmann_json)

add_executable(checker
    src/main.cpp
)

target_include_directories(checker PRIVATE include)
target_link_libraries(checker PRIVATE checker_lib)

enable_testing()

add_executable(test_checker
    tests/test_checker.cpp
)

target_include_directories(test_checker PRIVATE include)
target_link_libraries(test_checker PRIVATE checker_lib)

add_test(NAME checker_unit_tests COMMAND test_checker)