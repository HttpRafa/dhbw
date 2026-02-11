cmake_minimum_required(VERSION 4.1)
project(ubitix C)

set(CMAKE_C_STANDARD 99)

add_executable(ubitix main.c
        toml/tomlc17.c
        gateway/gateway.c
        action/action.c
        gateway/config.c
        log.c
        common/state.c
        common/state.h
        common/ipv6.c
        common/ipv6.h
        gateway/mapping.c
        gateway/mapping.h
        gateway/iptables.c
        gateway/iptables.h)
