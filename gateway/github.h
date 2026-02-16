#ifndef UBITIX_GITHUB_H
#define UBITIX_GITHUB_H

#include "config.h"
#include "../common/ipv6.h"

int github_dispatch_workflow(const gateway_config_t* config, ipv6_net_t* prefix, ipv6_net_t* mappings, int mappings_len);

#endif //UBITIX_GITHUB_H