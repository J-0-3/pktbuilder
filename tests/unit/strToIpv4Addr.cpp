#include <pktbuilder/addresses.h>
#include <cassert>
#include <stdexcept>

using namespace pktbuilder;
int main() {
    std::string addr_str = "172.16.0.3";
    assert(strToIpv4Addr(addr_str) == ipv4_addr_t({172, 16, 0, 3}));

    addr_str = "0.0.0.0";
    assert(strToIpv4Addr(addr_str) == ipv4_addr_t({0, 0, 0, 0}));

    addr_str = "255.255.255.255";
    assert(strToIpv4Addr(addr_str) == ipv4_addr_t({255, 255, 255, 255}));
    
    try {
        strToIpv4Addr("invalid");
        return 1;
    } catch(const std::invalid_argument& e) {
        assert(std::string(e.what()) == "not a valid IPv4 address");
    }

    try {
        strToIpv4Addr("10.10.10.");
        return 1;
    } catch(const std::invalid_argument& e) {
        assert(std::string(e.what()) == "not a valid IPv4 address");
    }

    try {
        strToIpv4Addr("256.300.10.323");
        return 1;
    }
    catch (const std::invalid_argument& e) {
        assert(std::string(e.what()) == "not a valid IPv4 address");
    }
    return 0;
}