cmake_minimum_required(VERSION 3.23)
project(Go)

set(CMAKE_C_STANDARD 23)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(Go go.c)
target_link_libraries(Go ${CURSES_LIBRARIES})