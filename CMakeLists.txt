cmake_minimum_required(VERSION 4.3)
project(Pila C)

set(CMAKE_C_STANDARD 11)

add_executable(Pila
        main.c,
        src/MAKEPILE/pile.c,
        src/MAKEVALUE/value.c,
        src/MAKEVALUE/DATA/string.c
)
