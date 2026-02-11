#include "iptables.h"

#include <stddef.h>

#include "../log.h"
#include "../common/ipv6.h"

int iptables_append_rules(const ipv6_net_t* private, const ipv6_net_t* public) {
    char s_private[39 + 1 + 3 + 1];
    char s_public[39 + 1 + 3 + 1];
    if (ipv6_to_string(private, s_private, sizeof(s_private)) == NULL) {
        error("Failed to convert private IPv6 network to string");
        return 1;
    }
    if (ipv6_to_string(public, s_public, sizeof(s_public)) == NULL) {
        error("Failed to convert public IPv6 network to string");
        return 1;
    }

    info("+: %s <---> %s", s_public, s_private);

    // TODO: Add rules using iptables command
    // https://github.com/HttpRafa/ubitix/blob/b917458c7083171260a9b1c7b847c2f9a736de99/src/gateway/rules.rs#L37

    return 0;
}

int iptables_delete_rules(const ipv6_net_t* private, const ipv6_net_t* public) {
    char s_private[39 + 1 + 3 + 1];
    char s_public[39 + 1 + 3 + 1];
    if (ipv6_to_string(private, s_private, sizeof(s_private)) == NULL) {
        error("Failed to convert private IPv6 network to string");
        return 1;
    }
    if (ipv6_to_string(public, s_public, sizeof(s_public)) == NULL) {
        error("Failed to convert public IPv6 network to string");
        return 1;
    }

    info("-: %s <---> %s", s_public, s_private);

    // TODO: Delete rules using iptables command
    // https://github.com/HttpRafa/ubitix/blob/b917458c7083171260a9b1c7b847c2f9a736de99/src/gateway/rules.rs#L55

    return 0;
}