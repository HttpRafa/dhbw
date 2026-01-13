#include "toml/tomlc17.h"
#include "gateway.h"

#include <stdlib.h>

#define FILE_KEY "file"
#define REGEX_KEY "regex"
#define NETWORKS_KEY "networks"
#define TOKEN_KEY "token"
#define OWNER_KEY "owner"
#define REPOSITORY_KEY "repository"
#define WORKFLOW_KEY "workflow"

void start_gateway() {
    FILE* file = fopen("gateway.toml", "r");
    if (file == NULL) {
        printf("Error opening gateway.toml\n");
    }

    toml_result_t result = toml_parse_file(file);
    if (!result.ok) {
        printf("Error parsing gateway.toml: %s\n", result.errmsg);
        return;
    }

    printf("Gateway file loaded!\n");

    toml_datum_t name = toml_get(result.toptab, FILE_KEY);
    toml_datum_t name = toml_get(result.toptab, REGEX_KEY);
    toml_datum_t name = toml_get(result.toptab, NETWORKS_KEY);
    toml_datum_t name = toml_get(result.toptab, TOKEN_KEY);
    toml_datum_t name = toml_get(result.toptab, OWNER_KEY);
    toml_datum_t name = toml_get(result.toptab, REPOSITORY_KEY);
    toml_datum_t name = toml_get(result.toptab, WORKFLOW_KEY);
    if (name.type == TOML_STRING) {
        printf("%s\n", name.u.str.ptr);
    }

    toml_free(result);
    fclose(file);
}
