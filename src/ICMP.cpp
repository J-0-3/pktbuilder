#include <pktbuilder/ICMP.h>
#include <pktbuilder/checksum.h>
#include <cstring>
#include <bit>

namespace pktbuilder {
    namespace ICMP {
        Packet::Packet(uint8_t type, uint8_t code,
                            const std::array<uint8_t, 4> &header_contents) {
            this->type = type;
            this->code = code;
            this->header_contents = header_contents;
        }
        std::vector<uint8_t> Packet::build() const {
            std::vector<uint8_t> data;
            data.push_back(this->type);
            data.push_back(this->code);
            data.push_back(0);
            data.push_back(0);
            for (uint8_t x: this->header_contents) {
                data.push_back(x);
            }
            data.insert(data.end(), this->payload.begin(), this->payload.end());
            uint16_t checksum = calculateInternetChecksum(data);
            uint8_t checksum_bytes[2];
            std::memcpy(checksum_bytes, &checksum, 2);
            if (std::endian::little == std::endian::native) {
                std::reverse(std::begin(checksum_bytes), std::end(checksum_bytes));
            }
            data.at(2) = checksum_bytes[0];
            data.at(3) = checksum_bytes[1];
            return data;
        }
    }
}