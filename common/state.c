//
// Created by rafael on 2/9/26.
//

#include "state.h"

#include <stdlib.h>

gateway_state_t load_gateway_state() {
    gateway_state_t state = { .prefix = { .address = {0, 0}, .mask = 128 }, .mapping = { .ptr = NULL, .len = 0 } };
    return state;
}

void free_gateway_state(gateway_state_t state) {
    for (int i = 0; i < state.mapping.len; i++) {
        free(state.mapping.ptr[i]);
    }
    free(state.mapping.ptr);
}