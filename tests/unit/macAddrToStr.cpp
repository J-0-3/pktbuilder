#include <pktbuilder/addresses.h>
#include <cassert>

using namespace pktbuilder;
int main() {
    mac_addr_t mac{0xab, 0xcd, 0xef, 0x12, 0x34, 0x56};
    assert(macAddrToStr(mac) == "ab:cd:ef:12:34:56");
    
    mac = {0, 0, 0, 0, 0, 0};
    assert(macAddrToStr(mac) == "00:00:00:00:00:00");

    mac = {255, 255, 255, 255, 255, 255};
    assert(macAddrToStr(mac) == "ff:ff:ff:ff:ff:ff");

    mac = {1, 2, 3, 4, 5, 6};
    assert(macAddrToStr(mac) == "01:02:03:04:05:06");
    return 0;
}