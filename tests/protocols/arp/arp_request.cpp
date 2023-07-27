#include <pktbuilder.h>
#include <pcap.h>

using namespace pktbuilder;

int main() {
    ipv4_addr_t my_ip = getDefaultInterfaceIPv4Address();
    mac_addr_t my_mac = getDefaultInterfaceMAC(); 
    auto message = ARP::IPv4EthernetRequest(my_ip, my_mac, {192, 168, 1, 1}, {}) |
                   Ethernet::Frame({0xff, 0xff, 0xff, 0xff, 0xff, 0xff});
    std::vector<uint8_t> data = message.build();
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* handle = pcap_open_live("wlp1s0", 65535, 1, 0, errbuf);
    pcap_sendpacket(handle, data.data(), data.size());
}