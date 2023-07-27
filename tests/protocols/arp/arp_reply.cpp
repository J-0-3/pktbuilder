#include <pktbuilder.h>
#include <pcap.h>

int main() {
    pktbuilder::ipv4_addr_t my_ip = pktbuilder::getDefaultInterfaceIPv4Address();
    pktbuilder::mac_addr_t my_mac = pktbuilder::getDefaultInterfaceMAC();
    auto message = pktbuilder::ARP::IPv4EthernetReply(
            my_ip,
            my_mac,
            {0, 0, 0, 0},
            {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}) |
        pktbuilder::Ethernet::Frame({0xff, 0xff, 0xff, 0xff, 0xff, 0xff}); 
    ;
    std::vector<uint8_t> data = message.build();
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* handle = pcap_open_live("wlp1s0", 65535, 1, 0, errbuf);
    pcap_sendpacket(handle, data.data(), data.size());
}