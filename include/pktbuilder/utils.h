#pragma once
#include <array>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <bit>
#include <pktbuilder/addresses.h>

namespace pktbuilder {
    template<typename T>
    std::array<uint8_t, sizeof(T)/sizeof(uint8_t)> splitBytesBigEndian(T x) {
        constexpr std::size_t NBYTES = sizeof(T) / sizeof(uint8_t);
        std::array<uint8_t, NBYTES> bytes;
        std::memcpy(bytes.data(), &x, NBYTES);
        if (std::endian::native == std::endian::little) {
            std::reverse(bytes.begin(), bytes.end());
        }
        return bytes;
    }
    std::string getDefaultInterfaceName();
    mac_addr_t getInterfaceMAC(std::string const& interface);
    ipv4_addr_t getInterfaceIPv4Address(std::string const& interface);
}