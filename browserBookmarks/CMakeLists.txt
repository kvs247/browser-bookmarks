cmake_minimum_required(VERSION 3.28)
project(browserBookmarks)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-Wall -Wextra)

find_package(nlohmann_json REQUIRED)
find_package(CURL REQUIRED)

get_filename_component(PROJECT_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}" ABSOLUTE)
set(BOOKMARKS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bookmarks/")
set(BRAVE_BOOKMARKS_PATH "/.config/BraveSoftware/Brave-Browser/Default/Bookmarks")
set(OPERA_BOOKMARKS_PATH "/.config/opera/Default/Bookmarks")
configure_file(
    "${PROJECT_ROOT_DIR}/cmake/paths.h.in"
    "${CMAKE_BINARY_DIR}/generated/paths.h"
)

set(SOURCES
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    ${CMAKE_BINARY_DIR}/generated
    ${CURL_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    nlohmann_json::nlohmann_json
    ${CURL_LIBRARIES}
)
