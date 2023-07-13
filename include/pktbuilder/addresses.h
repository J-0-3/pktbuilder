#pragma once
#include <array>
#include <cstdint>
#include <string>

namespace pktbuilder {
    typedef std::array<uint8_t, 6> mac_addr_t;
    typedef std::array<uint8_t, 4> ipv4_addr_t;
    std::string ipv4AddrToStr(ipv4_addr_t const& ip_addr);
    ipv4_addr_t strToIpv4Addr(std::string const& ip_addr);
    uint32_t ipv4AddrToInt(ipv4_addr_t const& ip_addr);
    ipv4_addr_t intToIpv4Addr(uint32_t ip_addr);

    std::string macAddrToStr(mac_addr_t const& mac_addr);
    mac_addr_t strToMacAddr(std::string const& mac_addr);
}