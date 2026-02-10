#include "config.h"

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../log.h"
#include "../common/state.h"
#include "../toml/tomlc17.h"

#define FILE_KEY "file"
#define REGEX_KEY "regex"
#define INTERFACE_KEY "interface"
#define NETWORKS_KEY "networks"
#define TOKEN_KEY "token"
#define OWNER_KEY "owner"
#define REPOSITORY_KEY "repository"
#define WORKFLOW_KEY "workflow"

gateway_config_t load_gateway_config() {
    FILE* file = fopen("gateway.toml", "r");
    if (file == NULL) {
        error("Error opening gateway.toml");

        return (gateway_config_t){ .ready = false };
    }

    toml_result_t result = toml_parse_file(file);
    fclose(file);
    if (!result.ok) {
        error("Error parsing gateway.toml: %s", result.errmsg);

        toml_free(result);
        return (gateway_config_t){ .ready = false };
    }

    info("Gateway file loaded!");

    gateway_config_t config = { .ready = true };

    {
        const char* raw_regex = try_get_string(REGEX_KEY, toml_get(result.toptab, REGEX_KEY));
        if (raw_regex != NULL) {
            regex_t* regex = malloc(sizeof(regex_t));
            int error_code = regcomp(regex, raw_regex, REG_EXTENDED);
            if (error_code) {
                char buffer[256];
                regerror(error_code, regex, buffer, sizeof(buffer));

                error("Regex compilation failed for '%s': %s", raw_regex, buffer);
                free(regex);
            } else {
                config.regex = regex;
            }
            free((void*)raw_regex);
        }
    }

    config.log_file   = try_get_string(FILE_KEY, toml_get(result.toptab, FILE_KEY));
    config.interface  = try_get_string(INTERFACE_KEY, toml_get(result.toptab, INTERFACE_KEY));
    config.token      = try_get_string(TOKEN_KEY, toml_get(result.toptab, TOKEN_KEY));
    config.owner      = try_get_string(OWNER_KEY, toml_get(result.toptab, OWNER_KEY));
    config.repository = try_get_string(REPOSITORY_KEY, toml_get(result.toptab, REPOSITORY_KEY));
    config.workflow   = try_get_string(WORKFLOW_KEY, toml_get(result.toptab, WORKFLOW_KEY));

    if (!config.log_file || !config.regex || !config.interface || !config.token || !config.owner || !config.repository || !config.workflow) {
        config.ready = false;
    }

    toml_free(result);
    return config;
}

void free_gateway_config(gateway_config_t* config) {
    // Free normal stuff
    free(config->log_file);

    if (config->regex != NULL) {
        regfree(config->regex);
        free(config->regex);
    }

    free(config->interface);
    free(config->token);
    free(config->owner);
    free(config->repository);
    free(config->workflow);

    // Free fancy sst
    if (config->networks.ptr) {
        for (int i = 0; i < config->networks.len; i++) {
            free(config->networks.ptr[i]); // Free the pair {src, dest}
        }
        free(config->networks.ptr); // Free the array of pointers
        config->networks.ptr = NULL;
    }
    config->networks.len = 0;
}