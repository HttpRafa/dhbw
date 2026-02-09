#ifndef UBITIX_STATE_H
#define UBITIX_STATE_H

// TOML State
struct gateway_state_t {
    bool ready;
};
typedef struct gateway_state_t gateway_state_t;

gateway_state_t load_gateway_state();
void free_gateway_state(gateway_state_t state);

#endif //UBITIX_STATE_H