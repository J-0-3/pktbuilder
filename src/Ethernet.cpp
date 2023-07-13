#include <pktbuilder/Ethernet.h>
#include <pktbuilder/utils.h>
#include <bit>
#include <cstring>

namespace pktbuilder {
    namespace Ethernet {
        Frame::Frame(mac_addr_t const& destination_mac, uint16_t ethertype, mac_addr_t const& source_mac) {
            this->destination_mac = destination_mac;
            if (source_mac == mac_addr_t({0, 0, 0, 0})) {
                std::string default_nic = getDefaultInterfaceName();
                if (!default_nic.empty()) {
                    this->source_mac = getInterfaceMAC(default_nic);
                } else {
                    this->source_mac = source_mac;
                }
            } else {
                this->source_mac = source_mac;
            }
            this->ethertype = ethertype;
        }
        std::vector<uint8_t> Frame::build() const {
            std::vector<uint8_t> data;
            data.reserve(14 + this->payload.size());
            data.insert(data.end(), this->destination_mac.begin(),
                        this->destination_mac.end());
            data.insert(data.end(), this->source_mac.begin(),
                        this->source_mac.end());
            std::array<uint8_t, 2> ethertype_bytes = splitBytesBigEndian(this->ethertype);
            data.push_back(ethertype_bytes[0]);
            data.push_back(ethertype_bytes[1]);
            data.insert(data.end(), this->payload.begin(),
                        this->payload.end());
            return data;
        }
        const mac_addr_t& Frame::getDestinationMac() const {
            return this->destination_mac;
        }
        const mac_addr_t& Frame::getSourceMac() const {
            return this->source_mac;
        }
        const uint16_t& Frame::getEthertype() const {
            return this->ethertype;
        }
    }
}