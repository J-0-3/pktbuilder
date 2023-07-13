#include <pktbuilder/addresses.h>
#include <cassert>
#include <stdexcept>

using namespace pktbuilder;
int main(void) {
    std::string mac_str = "12:34:56:78:90:ab";
    assert(strToMacAddr(mac_str) == mac_addr_t({0x12, 0x34, 0x56, 0x78, 0x90, 0xab}));
    
    mac_str = "00:00:00:00:00:00";
    assert(strToMacAddr(mac_str) == mac_addr_t({0, 0, 0, 0, 0, 0}));

    mac_str = "01:02:03:04:05:06";
    assert(strToMacAddr(mac_str) == mac_addr_t({1, 2, 3, 4, 5, 6}));

    mac_str = "ff:ff:ff:ff:ff:ff";
    assert(strToMacAddr(mac_str) == mac_addr_t({255, 255, 255, 255, 255, 255}));
    
    mac_str = "invalid";
    try {
        strToMacAddr(mac_str);
        exit(EXIT_FAILURE);
    } catch (std::invalid_argument const& e) {
        assert(std::string(e.what()) == "not a valid MAC address");
    } catch (std::exception const& e) {
        exit(EXIT_FAILURE);
    }

    mac_str = "00:000:00:00:00:00";
    try {
        strToMacAddr(mac_str);
        exit(EXIT_FAILURE);
    } catch (std::invalid_argument const& e) {
        assert(std::string(e.what()) == "not a valid MAC address");
    } catch (std::exception const& e) {
        exit(EXIT_FAILURE);
    }

    mac_str = "1:2:3:4:5:6";
    try {
        strToMacAddr(mac_str);
        exit(EXIT_FAILURE);
    } catch (std::invalid_argument const& e) {
        assert(std::string(e.what()) == "not a valid MAC address");
    } catch (std::exception const& e) {
        exit(EXIT_FAILURE);
    }
    
    mac_str = "01:23:45:67:89";
    try {
        strToMacAddr(mac_str);
        exit(EXIT_FAILURE);
    } catch (std::invalid_argument const& e) {
        assert(std::string(e.what()) == "not a valid MAC address");
    } catch (std::exception const& e) {
        exit(EXIT_FAILURE);
    }
}