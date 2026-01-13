cmake_minimum_required(VERSION 4.1)
project(ubitix C)

set(CMAKE_C_STANDARD 99)

add_executable(ubitix main.c
        gateway.h
        action.h
        gateway.c
        action.c
        toml/tomlc17.c)
