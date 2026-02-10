#include "ipv6.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "../log.h"

ipv6_net_t ipv6_from_string(const char *str) {
    ipv6_net_t result = {};

    char buffer[INET6_ADDRSTRLEN + 4];
    strncpy(buffer, str, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    char* slash = strchr(buffer, '/');
    if (slash) {
        *slash = '\0';
        const int m = atoi(slash + 1);
        result.mask = (m >= 0 && m <= 128) ? (unsigned char) m : 128;
    } else {
        result.mask = 128;
    }

    int status = inet_pton(AF_INET6, buffer, &result.address);
    if (status != 1) {
        debug("Failed to parse IPv6 from string: %d", status);
        result.mask = 255;
        memset(&result.address, 0, sizeof(struct in6_addr));
    }

    return result;
}

const char* ipv6_to_string(const ipv6_net_t *net, char *dest, size_t size) {
    char addr_str[INET6_ADDRSTRLEN];

    if (inet_ntop(AF_INET6, &(net->address), addr_str, sizeof(addr_str)) == NULL) {
        return NULL; // Conversion failed
    }

    snprintf(dest, size, "%s/%u", addr_str, net->mask);

    return dest;
}

int ipv6_is_valid(const ipv6_net_t* address) {
    return address->mask <= 128;
}

int ipv6_cmp(const ipv6_net_t* first, const ipv6_net_t* second) {
    return memcmp(&first->address, &second->address, sizeof(struct in6_addr)) == 0 && first->mask == second->mask;
}