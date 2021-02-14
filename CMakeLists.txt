# cmake_minimum_required(VERSION <specify CMake version here>)
project(hash C)

set(CMAKE_C_STANDARD 11)

add_executable(hash main.c handle_connection.c socket_pool.c socket_queue.c service.h service.c)
target_link_libraries(hash pthread)