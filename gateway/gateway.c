#include "../toml/tomlc17.h"
#include "gateway.h"

#include <threads.h>
#include <regex.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "iptables.h"
#include "mapping.h"
#include "../common/state.h"
#include "../log.h"

void handle_line(const gateway_config_t* config, const gateway_state_t* state, char buffer[]) {
    buffer[strcspn(buffer, "\r\n")] = '\0';
    debug("[LINE] %s", buffer);

    regmatch_t matches[3];
    if (regexec(config->regex, buffer, 3, matches, 0) == 0 && matches[1].rm_so != -1 && matches[2].rm_so != -1) {
        int prefix_len = matches[1].rm_eo - matches[1].rm_so;
        int interface_len = matches[2].rm_eo - matches[2].rm_so;

        char* raw_prefix = malloc(prefix_len + 1);
        strncpy(raw_prefix, buffer + matches[1].rm_so, prefix_len);
        raw_prefix[prefix_len] = '\0';

        char* interface = malloc(interface_len + 1);
        strncpy(interface, buffer + matches[2].rm_so, interface_len);
        interface[interface_len] = '\0';

        // Parse prefix string to prefix
        ipv6_net_t prefix = ipv6_from_string(raw_prefix);
        if (!ipv6_is_valid(&prefix)) {
            error("Error: Invalid IPv6 string format. (%s)", raw_prefix);
            goto cleanup;
        }

        if (strcmp(interface, config->interface) != 0) {
            info("Prefix change detected on a different interface (%s): Unknown -> %s (no action taken)", interface, prefix);
            goto cleanup;
        }

        char old_prefix[128];
        ipv6_to_string(&state->prefix, old_prefix, sizeof(old_prefix));
        if (!ipv6_cmp(&state->prefix, &prefix)) {
            info("Prefix updated on target interface %s: %s -> %s", interface, old_prefix, raw_prefix);

            // Delete IP Tables rules

            // Calc Mappings
            const ipv6_net_t* mappings = compute_mappings(&prefix, config->networks.ptr, config->networks.len);
            if (mappings == NULL) {
                goto cleanup;
            }

            info("Appending %d new NPTv6 rules:", config->networks.len * 2);
            for (int i = 0; i < config->networks.len; i++) {
                ipv6_net_t private = mappings[i * 2];
                ipv6_net_t public = mappings[i * 2 + 1];

                if (iptables_append_rules(&private, &public)) {
                    error("Failed to append rules to IPv6 rules for mapping at index %d", i);
                }
            }

            // Append IP Tables rules

            // Dispatch Github Workflow...

            // Update state on disk

            free((void*)mappings);
        } else {
            warn("Received redundant prefix change for interface %s: %s -> %s (no action taken)", interface, old_prefix, raw_prefix);
        }

    cleanup:
        free(raw_prefix);
        free(interface);
    }
}

void start_watcher(const gateway_config_t* config, const gateway_state_t* state) {
    FILE* file = fopen(config->log_file, "r");
    char buffer[2048];

    if (file == NULL) {
        error("Log file not found. Abort....");
        return;
    }

    struct timespec sleep_duration = {
        .tv_sec = 0,
        .tv_nsec = 100000000
    };

    fseek(file, 0, SEEK_END);

    while (true) {
        if (fgets(buffer, sizeof(buffer), file)) {
            handle_line(config, state, buffer);
        } else {
            if (feof(file)) {
                clearerr(file);
                thrd_sleep(&sleep_duration, NULL);
            } else {
                break;
            }
        }
    }

    fclose(file);
}

void start_gateway() {
    gateway_config_t config = load_gateway_config();
    gateway_state_t state = load_gateway_state();
    if (!config.ready) {
        error("Config not ready. Abort....");
        return;
    }

    info("Starting to tail provided file: %s", config.log_file);
    start_watcher(&config, &state);

    free_gateway_config(&config);
}