#include <stdio.h>
#include <string.h>

#include "gateway.h"
#include "action.h"

void print_help() {
    printf("--------------- HELP ---------------\n");
    printf("--gateway # Enable Gateway mode\n");
    printf("--action # Enable Gateway mode\n");
    printf("------------------------------------\n");
}

int main(const int argc, char* argv[]) {
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
        printf("Error: Unkown command line argument: %s\n", argv[i]);
    }
    print_help();

    return 0;
}