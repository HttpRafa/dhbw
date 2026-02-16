cmake_minimum_required(VERSION 3.15)
project(ubitix C)

set(CMAKE_C_STANDARD 99)

# --- 1. Set Default Build Type to Release ---
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()

# --- 2. Create the Executable ---
add_executable(ubitix
        main.c
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
        gateway/iptables.h
        gateway/github.c
        gateway/github.h
)

# --- 3. Add Include Directories ---
target_include_directories(ubitix PRIVATE
        "${CMAKE_SOURCE_DIR}"
        "${CMAKE_SOURCE_DIR}/common"
        "${CMAKE_SOURCE_DIR}/gateway"
        "${CMAKE_SOURCE_DIR}/toml"
        "${CMAKE_SOURCE_DIR}/action"
)

# --- 4. Enable Link Time Optimization (LTO) ---
include(CheckIPOSupported)
check_ipo_supported(RESULT result OUTPUT output)

if(result)
    set_property(TARGET ubitix PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
    message(STATUS "Link Time Optimization (LTO) enabled for Release build.")
else()
    message(WARNING "LTO not supported: ${output}")
endif()