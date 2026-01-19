#include "../toml/tomlc17.h"
#include "gateway.h"

#include <threads.h>

#include "config.h"

void start_gateway() {
    gateway_config_t config = load_gateway_config();
    if (!config.ready) {
        printf("Config not ready. Abort....\n");
        return;
    }

    printf("Starting to tail provided file: %s\n", config.log_file);

    FILE* file = fopen(config.log_file, "r");
    char buffer[1024];

    if (file == NULL) {
        printf("Log file not found. Abort....\n");
        return;
    }

    struct timespec sleep_duration = {
        .tv_sec = 0,
        .tv_nsec = 100000000
    };

    fseek(file, 0, SEEK_END);

    while (true) {
        if (fgets(buffer, sizeof(buffer), file)) {
            printf("%s", buffer);
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
    free_gateway_config(config);
}