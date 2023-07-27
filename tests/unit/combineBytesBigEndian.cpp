#include <pktbuilder/utils.h>
#include <random>
#include <cassert>

int main() {
    std::mt19937 mt(std::random_device{}());
    std::uniform_int_distribution<uint16_t> dist(0);
    for (int i = 0; i < 5; i++) {
        uint16_t n = dist(mt);
        std::array<uint8_t, 2> bytes = pktbuilder::splitBytesBigEndian(n);
        assert(pktbuilder::combineBytesBigEndian(bytes[0], bytes[1]) == n);
    }
}