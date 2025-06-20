cmake_minimum_required(VERSION 3.10)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file")

project(TickerPulseWorker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Определение платформы
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(VCPKG_TARGET_TRIPLET "arm64-osx")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(VCPKG_TARGET_TRIPLET "x64-linux")
else()
    message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
endif()

# Поиск зависимостей
find_package(fmt REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(CURL REQUIRED)
find_package(PostgreSQL REQUIRED)

# Исходники из src
file(GLOB SRC_SOURCES
    src/*.cpp
    src/db/*.cpp
    src/models/*.cpp
    src/services/*.cpp
    src/utils/*.cpp
)

# Конфиги
file(GLOB CONFIG_SOURCES
    config/*.cpp
)

# Объединение источников в один список
set(ALL_SOURCES
    ${SRC_SOURCES}
    ${CONFIG_SOURCES}
)

add_executable(TickerPulseWorker ${ALL_SOURCES})

target_include_directories(TickerPulseWorker
    PRIVATE
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/config"
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/include/db"
    "${CMAKE_SOURCE_DIR}/include/models"
    "${CMAKE_SOURCE_DIR}/include/services"
    "${CMAKE_SOURCE_DIR}/include/utils"
)

target_link_libraries(TickerPulseWorker
    fmt::fmt
    nlohmann_json::nlohmann_json
    CURL::libcurl
    PostgreSQL::PostgreSQL
)
