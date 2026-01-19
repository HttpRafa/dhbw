#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../toml/tomlc17.h"

#define FILE_KEY "file"
#define REGEX_KEY "regex"
#define NETWORKS_KEY "networks"
#define TOKEN_KEY "token"
#define OWNER_KEY "owner"
#define REPOSITORY_KEY "repository"
#define WORKFLOW_KEY "workflow"

char* try_get_string(const char* name, toml_datum_t datum) {
    switch (datum.type) {
        case TOML_UNKNOWN:
            printf("MISSING VALUE: %s\n", name);
            break;
        case TOML_STRING:
            return strdup(datum.u.str.ptr);
        default:
            printf("Type mismatch between expected type of '%s' and provided type.\nExpected: %d Given: %d\n", name, TOML_STRING, datum.type);
            break;
    }
    return NULL;
}

gateway_config_t load_gateway_config() {
    FILE* file = fopen("gateway.toml", "r");
    if (file == NULL) {
        printf("Error opening gateway.toml\n");

        return (gateway_config_t){ .ready = false };
    }

    toml_result_t result = toml_parse_file(file);
    fclose(file);
    if (!result.ok) {
        printf("Error parsing gateway.toml: %s\n", result.errmsg);

        toml_free(result);
        return (gateway_config_t){ .ready = false };
    }

    printf("Gateway file loaded!\n");

    gateway_config_t config = { .ready = true };

    config.log_file   = try_get_string(FILE_KEY, toml_get(result.toptab, FILE_KEY));
    config.regex      = try_get_string(REGEX_KEY, toml_get(result.toptab, REGEX_KEY));
    config.token      = try_get_string(TOKEN_KEY, toml_get(result.toptab, TOKEN_KEY));
    config.owner      = try_get_string(OWNER_KEY, toml_get(result.toptab, OWNER_KEY));
    config.repository = try_get_string(REPOSITORY_KEY, toml_get(result.toptab, REPOSITORY_KEY));
    config.workflow   = try_get_string(WORKFLOW_KEY, toml_get(result.toptab, WORKFLOW_KEY));

    if (!config.log_file || !config.regex || !config.token || !config.owner || !config.repository || !config.workflow) {
        config.ready = false;
    }

    toml_free(result);
    return config;
}

void free_gateway_config(gateway_config_t config) {
    // Free normal stuff
    free(config.log_file);
    free(config.regex);
    free(config.token);
    free(config.owner);
    free(config.repository);
    free(config.workflow);

    // Free fancy sst
    for (int i = 0; i < config.networks.len; i++) {
        free(config.networks.ptr[i]);
    }
    free(config.networks.ptr);
}