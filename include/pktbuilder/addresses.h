#pragma once
#include <array>
#include <cstdint>
#include <string>

namespace pktbuilder {
    typedef std::array<uint8_t, 6> mac_addr_t;
    typedef std::array<uint8_t, 4> ipv4_addr_t;
    std::string ipv4_addr_to_str(ipv4_addr_t const& ip_addr);
    ipv4_addr_t str_to_ipv4_addr(std::string const& ip_addr);
    uint32_t ipv4_addr_to_int(ipv4_addr_t const& ip_addr);
    ipv4_addr_t int_to_ipv4_addr(uint32_t ip_addr);

    std::string mac_addr_to_str(mac_addr_t const& mac_addr);
    mac_addr_t str_to_mac_addr(std::string const& mac_addr);
}