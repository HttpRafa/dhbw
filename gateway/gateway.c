#include "../toml/tomlc17.h"
#include "gateway.h"

#include <threads.h>
#include <regex.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "../log.h"

void handle_line(const gateway_config_t* config, char buffer[]) {
    buffer[strcspn(buffer, "\r\n")] = '\0';
    debug("[LINE] %s", buffer);

    regmatch_t matches[3];
    if (regexec(config->regex, buffer, 3, matches, 0) == 0 && matches[1].rm_so != -1 && matches[2].rm_so != -1) {
        int prefix_len = matches[1].rm_eo - matches[1].rm_so;
        int interface_len = matches[2].rm_eo - matches[2].rm_so;

        char* prefix = malloc(prefix_len + 1);
        strncpy(prefix, buffer + matches[1].rm_so, prefix_len);
        prefix[prefix_len] = '\0';

        char* interface = malloc(interface_len + 1);
        strncpy(interface, buffer + matches[2].rm_so, interface_len);
        interface[interface_len] = '\0';

        if (strcmp(interface, config->interface) != 0) {
            info("Prefix change detected on a different interface (%s): Unknown -> %s (no action taken)", interface, prefix);
            goto cleanup;
        }

        info("Found prefix: %s, interface: %s", prefix, interface);

    cleanup:
        free(prefix);
        free(interface);
    }
}

void start_watcher(gateway_config_t config) {
    FILE* file = fopen(config.log_file, "r");
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
            handle_line(&config, buffer);
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
    if (!config.ready) {
        error("Config not ready. Abort....");
        return;
    }

    info("Starting to tail provided file: %s", config.log_file);
    start_watcher(config);

    free_gateway_config(config);
}