#include <pktbuilder/addresses.h>
#include <cassert>

using namespace pktbuilder;
int main() {
    ipv4_addr_t addr({0xaa, 0xbd, 0x56, 0x32});
    assert(ipv4AddrToInt(addr) == 0xaabd5632);

    addr = {192, 168, 1, 0};
    assert(ipv4AddrToInt(addr) == 0xc0a80100);

    addr = {0, 0, 0, 0};
    assert(ipv4AddrToInt(addr) == 0);

    addr = {255, 255, 255, 255};
    assert(ipv4AddrToInt(addr) == UINT32_MAX);
    return 0;
}