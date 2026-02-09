cmake_minimum_required(VERSION 4.1)
project(ubitix C)

set(CMAKE_C_STANDARD 99)

add_executable(ubitix main.c
        toml/tomlc17.c
        gateway/gateway.c
        action/action.c
        gateway/config.c
        log.c
        gateway/state.c
        gateway/state.h)
