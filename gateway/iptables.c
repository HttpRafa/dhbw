#include "iptables.h"

#include <stdio.h>
#include <stdlib.h>

#include "../log.h"
#include "../common/ipv6.h"

int iptables_append_rules(const ipv6_net_t* private, const ipv6_net_t* public, const int dry_run) {
    char s_private[INET6_ADDRSTRLEN + 5];
    char s_public[INET6_ADDRSTRLEN + 5];
    if (ipv6_to_string(private, s_private, sizeof(s_private)) == NULL) {
        error("Failed to convert private IPv6 network to string");
        return 1;
    }
    if (ipv6_to_string(public, s_public, sizeof(s_public)) == NULL) {
        error("Failed to convert public IPv6 network to string");
        return 1;
    }

    info("+: %s <---> %s", s_public, s_private);

    char command[512];
    {
        snprintf(command, sizeof(command), "ip6tables -t nat -A POSTROUTING -s %s -j NETMAP --to %s", s_private, s_public);
        debug(" - # 1/2: %s", command);
        if (!dry_run) {
            int result = system(command);
            if (result != 0) {
                error("Failed to execute NETMAP append POSTROUTING rule. Return code: %d", result);
            }
        }
    }

    {
        snprintf(command, sizeof(command), "ip6tables -t nat -A PREROUTING -d %s -j NETMAP --to %s", s_public, s_private);
        debug(" - # 2/2: %s", command);
        if (!dry_run) {
            int result = system(command);
            if (result != 0) {
                error("Failed to execute NETMAP append PREROUTING rule. Return code: %d", result);
            }
        }
    }

    return 0;
}

int iptables_delete_rules(const ipv6_net_t* private, const ipv6_net_t* public, const int dry_run) {
    char s_private[INET6_ADDRSTRLEN + 5];
    char s_public[INET6_ADDRSTRLEN + 5];
    if (ipv6_to_string(private, s_private, sizeof(s_private)) == NULL) {
        error("Failed to convert private IPv6 network to string");
        return 1;
    }
    if (ipv6_to_string(public, s_public, sizeof(s_public)) == NULL) {
        error("Failed to convert public IPv6 network to string");
        return 1;
    }

    info("-: %s <---> %s", s_public, s_private);

    char command[512];
    {
        snprintf(command, sizeof(command), "ip6tables -t nat -D POSTROUTING -s %s -j NETMAP --to %s", s_private, s_public);
        debug(" - # 1/2: %s", command);
        if (!dry_run) {
            int result = system(command);
            if (result != 0) {
                warn("Failed to delete NETMAP POSTROUTING rule (it might not exist). Return code: %d", result);
            }
        }
    }

    {
        snprintf(command, sizeof(command), "ip6tables -t nat -D PREROUTING -d %s -j NETMAP --to %s", s_public, s_private);
        debug(" - # 2/2: %s", command);
        if (!dry_run) {
            int result = system(command);
            if (result != 0) {
                warn("Failed to delete NETMAP PREROUTING rule (it might not exist). Return code: %d", result);
            }
        }
    }

    return 0;
}