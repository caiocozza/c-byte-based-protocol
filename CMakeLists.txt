set(CMAKE_C_COMPILER gcc)
cmake_minimum_required(VERSION 3.8)
project(protocol_impl C)

set(CMAKE_C_STANDARD 11)

add_executable(protocol_impl main.c handle_connection.c socket_pool.c socket_queue.c service.h service.c errors.h errors.c key_manager.c key_manager.h)
target_link_libraries(protocol_impl pthread)