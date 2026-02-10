//
// Created by rafael on 2/10/26.
//

#include "mapping.h"

#include <stddef.h>
#include <stdlib.h>

#include "../log.h"

const ipv6_net_t* compute_mappings(const ipv6_net_t* prefix, const ipv6_net_t* networks, int networks_size) {
    if (prefix->mask > 64) {
        error("The detected prefix must be /64 or shorter to be split into /64 subnets.");
        return NULL;
    }

    ipv6_net_t* mapping = malloc(sizeof(ipv6_net_t) * 2 * networks_size);



    return mapping;
}
