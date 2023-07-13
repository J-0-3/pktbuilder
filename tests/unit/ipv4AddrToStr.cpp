#include <pktbuilder/addresses.h>
#include <cassert>

using namespace pktbuilder;
int main(void) {
    ipv4_addr_t addr({192, 168, 1, 2});
    assert(ipv4AddrToStr(addr) == "192.168.1.2");
    
    addr = {10, 0, 0, 0};
    assert(ipv4AddrToStr(addr) == "10.0.0.0");

    addr = {0, 0, 0, 0};
    assert(ipv4AddrToStr(addr) == "0.0.0.0");

    addr = {0xff, 0xff, 0xff, 0xff};
    assert(ipv4AddrToStr(addr) == "255.255.255.255");
    return 0;
}