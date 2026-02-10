//
// Created by rafael on 2/9/26.
//

#include "state.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../log.h"
#include "../toml/tomlc17.h"

#define PREFIX_KEY "prefix"
#define MAPPING_KEY "mapping"

char* try_get_string(const char* name, toml_datum_t datum) {
    switch (datum.type) {
        case TOML_UNKNOWN:
            warn("MISSING VALUE: %s", name);
            break;
        case TOML_STRING:
            return strdup(datum.u.str.ptr);
        default:
            warn("Type mismatch between expected type of '%s' and provided type.\nExpected: %d Given: %d", name, TOML_STRING, datum.type);
            break;
    }
    return NULL;
}

gateway_state_t load_gateway_state() {
    FILE* file = fopen("state.toml", "r");
    if (file == NULL) {
        info("Failed to open state file, falling back to empty state....");
        return (gateway_state_t) { .prefix = { .address = {}, .mask = 128 }, .mapping = { .ptr = NULL, .len = 0 } };
    }

    toml_result_t result = toml_parse_file(file);
    fclose(file);
    if (!result.ok) {
        error("Error parsing state.toml: %s", result.errmsg);
        goto cleanup;
    }

    info("State file loaded!");

    gateway_state_t state = {};

    {
        const char* raw_prefix = try_get_string(PREFIX_KEY, toml_get(result.toptab, PREFIX_KEY));
        if (raw_prefix != NULL) {
            ipv6_net_t prefix = ipv6_from_string(raw_prefix);
            if (!ipv6_is_valid(&prefix)) {
                error("The prefix found in the state file is not a valid IPv6 subnet: %s", raw_prefix);
                goto cleanup;
            }
            state.prefix = prefix;
            free((void*) raw_prefix);
        } else {
            error("The state file is missing the field %s", PREFIX_KEY);
            goto cleanup;
        }
    }

    {
        toml_datum_t datum = toml_get(result.toptab, MAPPING_KEY);
        if (datum.type == TOML_TABLE) {
            for (unsigned int i = 0; i < datum.u.tab.size; i++) {
            }
        } else {
            error("The state file is missing the table %s", MAPPING_KEY);
            goto cleanup;
        }
    }

    toml_free(result);
    return state;

cleanup:
    toml_free(result);
    warn("Falling back to empty state...");
    return (gateway_state_t) { .prefix = { .address = {}, .mask = 128 }, .mapping = { .ptr = NULL, .len = 0 } };
}

void free_gateway_state(gateway_state_t* state) {
    if (state->mapping.ptr) {
        for (int i = 0; i < state->mapping.len; i++) {
            free(state->mapping.ptr[i]); // Free the pair {src, dest}
        }
        free(state->mapping.ptr); // Free the array of pointers
        state->mapping.ptr = NULL;
    }
    state->mapping.len = 0;
}