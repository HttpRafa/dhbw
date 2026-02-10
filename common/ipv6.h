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
const char* ipv6_to_string(const ipv6_net_t *net, char *dest, size_t size);

int ipv6_is_valid(const ipv6_net_t* address);
int ipv6_cmp(const ipv6_net_t* first, const ipv6_net_t* second);

void ipv6_fill_bits_high(struct in6_addr* address, int start, int end);
int ipv6_decrement_top(struct in6_addr* address);

#endif //UBITIX_IPV6_H