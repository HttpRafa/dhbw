#ifndef UBITIX_CONFIG_H
#define UBITIX_CONFIG_H

#include <regex.h>
#include <stdbool.h>

// TOML Configuration
struct gateway_config_t {
    bool ready;
    char* log_file;
    regex_t* regex;
    char* interface;
    char* token;
    char* owner;
    char* repository;
    char* workflow;
    struct {
        char** ptr;     // Pointer to the networks array
        int len;        // Amount of networks
    } networks;
};

typedef struct gateway_config_t gateway_config_t;

gateway_config_t load_gateway_config();
void free_gateway_config(gateway_config_t* config);

#endif //UBITIX_CONFIG_H