#ifndef UBITIX_IPTABLES_H
#define UBITIX_IPTABLES_H

#include "../common/ipv6.h"

int iptables_append_rules(const ipv6_net_t* private, const ipv6_net_t* public);
int iptables_delete_rules(const ipv6_net_t* private, const ipv6_net_t* public);

#endif //UBITIX_IPTABLES_H