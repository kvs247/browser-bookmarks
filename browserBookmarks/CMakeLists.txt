cmake_minimum_required(VERSION 3.28)
project(browserBookmarks)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-Wall -Wextra)

include(FetchContent)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)

FetchContent_MakeAvailable(json)

get_filename_component(PROJECT_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}" ABSOLUTE)
set(BOOKMARKS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bookmarks/")
configure_file(
    "${PROJECT_ROOT_DIR}/cmake/paths.h.in" 
    "${CMAKE_BINARY_DIR}/generated/paths.h"
)

set(SOURCES
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/common
  ${CMAKE_BINARY_DIR}/generated
)

target_link_libraries(${PROJECT_NAME}
    nlohmann_json::nlohmann_json
)