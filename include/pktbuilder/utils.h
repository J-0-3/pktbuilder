#pragma once
#include <array>
#include <algorithm>
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
        if constexpr (std::endian::native == std::endian::little) {
            std::reverse(bytes.begin(), bytes.end());
        }
        return bytes;
    }
    uint16_t combineBytesBigEndian(uint8_t b1, uint8_t b2);
    uint32_t combineBytesBigEndian(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4);
    mac_addr_t getDefaultInterfaceMAC();
    ipv4_addr_t getDefaultInterfaceIPv4Address();
}
