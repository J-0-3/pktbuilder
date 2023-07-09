#include <pktbuilder/checksum.h>

namespace pktbuilder {
    uint16_t calculateInternetChecksum(std::vector<uint8_t> header) {
        uint32_t sum = 0;
        if (header.size() % 2 != 0) {
            header.push_back(0);
        }
        std::vector<uint16_t> header_words;
        for (int i = 0; i < header.size() - 1; i+=2) {
            uint16_t word = static_cast<uint16_t>(header.at(i)) << 8 |
                            static_cast<uint16_t>(header.at(i + 1));
            header_words.push_back(word);
        }
        for (uint16_t word: header_words) {
            sum += word;
        }
        sum = (sum >> 16) + (sum & 0xffff);
        sum += sum >> 16;
        return ~sum;
    }
}