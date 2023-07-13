#include <pktbuilder/addresses.h>
#include <cassert>

using namespace pktbuilder;
int main(void) {
    uint32_t addr_int = 0xffbbae20;
    assert(intToIpv4Addr(addr_int) == ipv4_addr_t({0xff, 0xbb, 0xae, 0x20}));

    addr_int = 0xc0a80101;
    assert(intToIpv4Addr(addr_int) == ipv4_addr_t({192, 168, 1, 1}));

    addr_int = 0;
    assert(intToIpv4Addr(addr_int) == ipv4_addr_t({0, 0, 0, 0}));

    addr_int = UINT32_MAX;
    assert(intToIpv4Addr(addr_int) == ipv4_addr_t({255, 255, 255, 255}));

    return 0;
}