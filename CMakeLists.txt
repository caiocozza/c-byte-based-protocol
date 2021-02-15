set(CMAKE_C_COMPILER gcc)
cmake_minimum_required(VERSION 3.8)
project(hash C)

set(CMAKE_C_STANDARD 11)

add_executable(hash main.c handle_connection.c socket_pool.c socket_queue.c service.h service.c errors.h errors.c)
target_link_libraries(hash pthread)