#include <pktbuilder/utils.h>
#include <iostream>

using namespace pktbuilder;
int main() {
	const ipv4_addr_t default_ip = getDefaultInterfaceIPv4Address();
	const std::string ip_str = ipv4AddrToStr(default_ip);
	std::cout << ip_str << std::endl;
}