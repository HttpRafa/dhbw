#ifndef UBITIX_STATE_H
#define UBITIX_STATE_H

#include "ipv6.h"

// TOML State
struct gateway_state_t {
    ipv6_net_t prefix;
    struct {
        ipv6_net_t** ptr;     // Pointer to the mapping array
        int len;                     // Amount of entries
    } mapping;
};

typedef struct gateway_state_t gateway_state_t;

gateway_state_t load_gateway_state();
void free_gateway_state(gateway_state_t state);

#endif //UBITIX_STATE_H