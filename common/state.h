#ifndef UBITIX_STATE_H
#define UBITIX_STATE_H

#include "../toml/tomlc17.h"
#include "ipv6.h"

// TOML State
struct gateway_state_t {
    ipv6_net_t prefix;
    struct {
        ipv6_net_t* ptr;     // Array of pointers to {src, dest} pairs
        int len;              // Amount of entries
    } mapping;
};

typedef struct gateway_state_t gateway_state_t;

char* try_get_string(const char* name, toml_datum_t datum);

void state_update(gateway_state_t* state, ipv6_net_t prefix, ipv6_net_t* mappings, int mappings_len);

gateway_state_t load_gateway_state();
int save_gateway_state(const gateway_state_t* state);
void free_gateway_state(gateway_state_t* state);

#endif //UBITIX_STATE_H