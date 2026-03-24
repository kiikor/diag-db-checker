cmake_minimum_required(VERSION 3.16)
project(diag_db_checker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(checker
    src/main.cpp
    src/checker.cpp
)

target_include_directories(checker PRIVATE include)

include(FetchContent)
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

target_link_libraries(checker PRIVATE nlohmann_json::nlohmann_json)
