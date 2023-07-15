#include <pktbuilder/utils.h>
#include <iostream>

using namespace pktbuilder;
int main(void) {
	mac_addr_t default_mac = getDefaultInterfaceMAC();
	std::string mac_str = macAddrToStr(default_mac);
	std::cout << mac_str << std::endl;
}