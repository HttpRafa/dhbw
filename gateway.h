#ifndef UBITIX_GATEWAY_H
#define UBITIX_GATEWAY_H

// TOML Con
struct gateway_config_t {
    const char* file;
    const char* regex;
    const char* token;
    const char* owner;
    const char* repository;
    const char* workflow;
    struct {
        const char** ptr; // Pointer to the networks array
        int len;          // Amount of networks
    } networks;
};
typedef struct gateway_config_t gateway_config_t;

void start_gateway();

#endif //UBITIX_GATEWAY_H