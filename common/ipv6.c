//
// Created by rafael on 2/9/26.
//

#include "ipv6.h"

ipv6_net_t ipv6_from_string(const char *str) {

}

int ipv6_cmp(const ipv6_net_t* first, const ipv6_net_t* second) {
    return first->address[0] == second->address[0] && first->address[1] == second->address[1];
}