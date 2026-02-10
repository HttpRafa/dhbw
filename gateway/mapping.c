//
// Created by rafael on 2/10/26.
//

#include "mapping.h"

#include <stdlib.h>
#include <string.h>

#include "../log.h"

int calc_subnets(const ipv6_net_t* parent, ipv6_net_t* results, const int n) {
    if (parent->mask >= 64) {
        // This should be catched by the compute_mappings function
        return 1;
    }

    struct in6_addr current = parent->address;

    memset(&current.s6_addr[8], 0, 8);
    ipv6_fill_bits_high(&current, parent->mask, 64);

    for (int i = 0; i < n; i++) {
        results[i].address = current;
        results[i].mask = 64;

        if (ipv6_decrement_top(&current) != 0) {
            // Stop if we underflowed (shouldn't happen given valid inputs)
            break;
        }
    }

    return 0;
}

const ipv6_net_t* compute_mappings(const ipv6_net_t* prefix, const ipv6_net_t* networks, int networks_size) {
    if (prefix->mask >= 64) {
        error("The detected prefix must shorter to be split into /64 subnets.");
        return NULL;
    }

    ipv6_net_t* mapping = malloc(sizeof(ipv6_net_t) * 2 * networks_size);
    ipv6_net_t* subnets = malloc(sizeof(ipv6_net_t) * networks_size);

    calc_subnets(prefix, subnets, networks_size);

    return mapping;
}
