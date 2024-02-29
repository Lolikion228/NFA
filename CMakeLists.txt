cmake_minimum_required(VERSION 3.27)
project(DFA)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=native -O3 -lm")

add_executable(NFA
        nfa.h
        big_int/big_int.c
        main.c
        nfa.c
        other/linked_list.h
        other/linked_list.c)

target_link_libraries(NFA m)
target_include_directories(NFA PUBLIC .)