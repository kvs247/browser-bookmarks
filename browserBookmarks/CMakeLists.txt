cmake_minimum_required(VERSION 3.28)
project(browserBookmarks)

add_compile_options(-Wall -Wextra)

set(SOURCES
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})