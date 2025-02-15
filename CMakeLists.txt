cmake_minimum_required(VERSION 3.28)
project(browser-bookmarks)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-Wall -Wextra)

find_package(nlohmann_json REQUIRED)
find_package(AWSSDK REQUIRED COMPONENTS s3)
find_package(SQLite3 REQUIRED)

get_filename_component(PROJECT_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}" ABSOLUTE)
set(CHROMIUM_BOOKMARKS_FILENAME "Bookmarks")
set(BRAVE_DIR_PATH "/.config/BraveSoftware")
set(OPERA_DIR_PATH "/.config/opera")
set(FIREFOX_BOOKMARKS_FILENAME "places.sqlite")
set(FIREFOX_DIR_PATH "/.mozilla/firefox")
configure_file(
    "${PROJECT_ROOT_DIR}/cmake/paths.h.in"
    "${CMAKE_BINARY_DIR}/generated/paths.h"
)

set(SOURCES
    src/main.cpp
    src/Parser/Parser.cpp
    src/Parser/ChromiumParser.cpp
    src/Parser/FirefoxParser.cpp
    src/AwsS3.cpp
    src/Formatter.cpp
    src/Manager.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Parser
    ${CMAKE_BINARY_DIR}/generated
    ${CURL_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    nlohmann_json::nlohmann_json
    ${AWSSDK_LIBRARIES}
    SQLite::SQLite3
)
