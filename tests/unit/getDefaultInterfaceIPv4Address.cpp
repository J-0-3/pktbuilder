#include <pktbuilder/utils.h>
#include <cassert>

using namespace pktbuilder;
int main() {
	const ipv4_addr_t default_ip = getDefaultInterfaceIPv4Address();
	assert(default_ip != ipv4_addr_t({ 0, 0, 0, 0 }));
	assert(default_ip[0] != 127); // not loopback
}