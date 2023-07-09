#pragma once
#include <cstdint>
#include <vector>

namespace pktbuilder {
    uint16_t calculateInternetChecksum(std::vector<uint8_t> header);
}