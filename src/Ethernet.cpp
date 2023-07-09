#include <pktbuilder/Ethernet.h>
#include <bit>
#include <cstring>

namespace pktbuilder {
    EthernetFrame::EthernetFrame() {
        this->destination_mac_ = mac_addr_t({ 0, 0, 0, 0, 0, 0 });
        this->source_mac_ = mac_addr_t({ 0, 0, 0, 0, 0, 0 });
        this->ethertype_ = EtherType::IPv4;
    }
    EthernetFrame::EthernetFrame(mac_addr_t const& destination_mac, mac_addr_t const& source_mac,
                                 EtherType ethertype) {
        this->destination_mac_ = destination_mac;
        this->source_mac_ = source_mac;
        this->ethertype_ = ethertype;
    }
    std::vector<uint8_t> EthernetFrame::build() const {
        std::vector<uint8_t> data;
        data.reserve(14 + this->payload.size());
        data.insert(data.end(), this->destination_mac_.begin(),
                    this->destination_mac_.end());
        data.insert(data.end(), this->source_mac_.begin(),
                    this->source_mac_.end());
        uint8_t ethertype_bytes[2];
        std::memcpy(ethertype_bytes, &this->ethertype_, 2);
        if (std::endian::native == std::endian::little) {
            std::reverse(
                    std::begin(ethertype_bytes),
                    std::end(ethertype_bytes)
            );
        }
        data.push_back(ethertype_bytes[0]);
        data.push_back(ethertype_bytes[1]);
        data.insert(data.end(), this->payload.begin(),
                    this->payload.end());
        return data;
    }
    const mac_addr_t& EthernetFrame::getDestinationMac() const {
        return this->destination_mac_;
    }
    const mac_addr_t& EthernetFrame::getSourceMac() const {
        return this->source_mac_;
    }
    const EtherType& EthernetFrame::getEthertype() const {
        return this->ethertype_;
    }
}