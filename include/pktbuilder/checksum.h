#pragma once
#include <cstdint>
#include <vector>

namespace pktbuilder {
    uint16_t calculateInternetChecksum(const uint8_t* header, size_t len);
    uint16_t calculateInternetChecksum(std::vector<uint8_t> const& header);
}