#include "../toml/tomlc17.h"
#include "gateway.h"

#include <threads.h>

#include "config.h"
#include "../log.h"

void handle_line(char buffer[]) {
    debug("%s", buffer);
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
            handle_line(buffer);
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