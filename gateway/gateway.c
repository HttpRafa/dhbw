#include "../toml/tomlc17.h"
#include "gateway.h"

#include <threads.h>
#include <regex.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "iptables.h"
#include "mapping.h"
#include "github.h"
#include "../common/state.h"
#include "../log.h"

void handle_line(const gateway_config_t* config, gateway_state_t* state, char buffer[], const int dry_run) {
    buffer[strcspn(buffer, "\r\n")] = '\0';
    debug("[LINE] %s", buffer);

    regmatch_t matches[3];
    if (regexec(config->regex, buffer, 3, matches, 0) == 0 && matches[1].rm_so != -1 && matches[2].rm_so != -1) {
        const int prefix_len = matches[1].rm_eo - matches[1].rm_so;
        const int interface_len = matches[2].rm_eo - matches[2].rm_so;

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
            info("Deleting %d old NPTv6 rules:", state->mapping.len * 2);
            for (int i = 0; i < state->mapping.len; i++) {
                ipv6_net_t private = state->mapping.ptr[i * 2];
                ipv6_net_t public = state->mapping.ptr[i * 2 + 1];

                if (iptables_delete_rules(&private, &public, dry_run)) {
                    error("Failed to delete rules from IPv6 rules for mapping at index %d", i);
                }
            }

            // Calc Mappings
            ipv6_net_t* mappings = compute_mappings(&prefix, config->networks.ptr, config->networks.len);
            if (mappings == NULL) {
                goto cleanup;
            }

            // Append IP Tables rules
            info("Appending %d new NPTv6 rules:", config->networks.len * 2);
            for (int i = 0; i < config->networks.len; i++) {
                ipv6_net_t private = mappings[i * 2];
                ipv6_net_t public = mappings[i * 2 + 1];

                if (iptables_append_rules(&private, &public, dry_run)) {
                    error("Failed to append rules to IPv6 rules for mapping at index %d", i);
                }
            }

            // Dispatch GitHub Workflow...
            github_dispatch_workflow(config, &prefix, mappings, config->networks.len);

            // Update state on disk
            state_update(state, prefix, mappings, config->networks.len);
            save_gateway_state(state);
        } else {
            warn("Received redundant prefix change for interface %s: %s -> %s (no action taken)", interface, old_prefix, raw_prefix);
        }

    cleanup:
        free(raw_prefix);
        free(interface);
    }
}

void start_watcher(const gateway_config_t* config, gateway_state_t* state, const int dry_run) {
    FILE* file = fopen(config->log_file, "r");
    char buffer[2048];

    if (file == NULL) {
        error("Log file not found. Abort....");
        return;
    }

    const struct timespec sleep_duration = {
        .tv_sec = 0,
        .tv_nsec = 100000000
    };

    fseek(file, 0, SEEK_END);

    while (true) {
        if (fgets(buffer, sizeof(buffer), file)) {
            handle_line(config, state, buffer, dry_run);
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

void start_gateway(const int dry_run) {
    gateway_config_t config = load_gateway_config();
    gateway_state_t state = load_gateway_state();
    if (!config.ready) {
        error("Config not ready. Abort....");
        free_gateway_config(&config);
        free_gateway_state(&state);
        return;
    }

    info("Starting to tail provided file: %s", config.log_file);
    start_watcher(&config, &state, dry_run);

    free_gateway_config(&config);
    free_gateway_state(&state);
}