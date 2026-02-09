#ifndef UBITIX_IPV6_H
#define UBITIX_IPV6_H

#include <netinet/in.h>

// IPv6 Subnet
struct ipv6_net_t {
    struct in6_addr address;
    unsigned char mask;
};
typedef struct ipv6_net_t ipv6_net_t;

ipv6_net_t ipv6_from_string(const char *str);

int ipv6_cmp(const ipv6_net_t* first, const ipv6_net_t* second);

#endif //UBITIX_IPV6_H