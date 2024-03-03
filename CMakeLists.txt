cmake_minimum_required(VERSION 3.27)
project(NFA)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=native -O3 -lm")

add_executable(NFA
        nfa.h
        other/big_int.c
        main.c
        nfa.c
        other/linked_list.h
        other/linked_list.c)

target_link_libraries(NFA m)
target_include_directories(NFA PUBLIC .)