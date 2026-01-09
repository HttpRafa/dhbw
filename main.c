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
    if (argc != 2) {
        print_help();
        return 1;
    }

    if (strcmp(argv[1], "--gateway") == 0) {
        start_gateway();
    } else if (strcmp(argv[1], "--action") == 0) {
        start_action();
    } else {
        print_help();
    }

    return 0;
}