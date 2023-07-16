#include <pktbuilder/utils.h>
#include <cassert>

using namespace pktbuilder;
int main() {
	const mac_addr_t default_mac = getDefaultInterfaceMAC();
	assert(default_mac != mac_addr_t({0, 0, 0, 0, 0, 0}));
	assert(default_mac != mac_addr_t({ 255, 255, 255, 255, 255, 255 })); // not brdcast
}