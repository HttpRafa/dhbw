#ifndef UBITIX_MAPPING_H
#define UBITIX_MAPPING_H

#include "../common/ipv6.h"

ipv6_net_t* compute_mappings(const ipv6_net_t* prefix, const ipv6_net_t* networks, int networks_size);

#endif //UBITIX_MAPPING_H