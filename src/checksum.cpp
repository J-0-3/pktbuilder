#include <pktbuilder/checksum.h>

namespace pktbuilder {
    uint16_t calculateInternetChecksum(const uint8_t* header, size_t len)
    {   
        uint32_t sum = 0;
        for (size_t i = 0; i < len - 1; i+=2) {
            sum += (static_cast<uint32_t>(header[i]) << 8) | (static_cast<uint32_t>(header[i + 1]));
        }
        if (len % 2 != 0) {
            sum += header[len - 1] << 8;
        }
        sum = (sum >> 16) + (sum & 0xffff);
        sum += sum >> 16;
        return static_cast<uint16_t>(~sum);
    }
    uint16_t calculateInternetChecksum(std::vector<uint8_t> const& header) {
        return calculateInternetChecksum(header.data(), header.size());
    }
}