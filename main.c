#include <stdio.h>
#include <string.h>

#include "log.h"
#include "gateway/gateway.h"
#include "action/action.h"

void print_help() {
    warn("--------------- HELP ---------------");
    warn("--gateway # Enable Gateway mode");
    warn("--action # Enable Gateway mode");
    warn("------------------------------------");
}

int main(const int argc, char* argv[], char* env[]) {
    if (argc < 2) {
        print_help();
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--gateway") == 0) {
            start_gateway();
            return 0;
        }
        if (strcmp(argv[i], "--action") == 0) {
            start_action();
            return 0;
        }
        error("Unkown command line argument: %s", argv[i]);
    }
    print_help();

    return 0;
}