#include <pktbuilder/Ethernet.h>
#include <pktbuilder/utils.h>
#include <stdexcept>

namespace pktbuilder {
    namespace Ethernet {
        Frame::Frame(mac_addr_t const& destination_mac, uint16_t ethertype, mac_addr_t const& source_mac) {
            this->destination_mac = destination_mac;
            if (source_mac == mac_addr_t({0, 0, 0, 0})) {
                this->source_mac = getDefaultInterfaceMAC();
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
        Frame Frame::decodeFrom(const uint8_t* data, size_t len) {
            if (len < 14) {
                throw(std::invalid_argument("Ethernet frame too small"));
            }
            Frame frame;
            std::memcpy(frame.destination_mac.data(), data, 6);
            std::memcpy(frame.source_mac.data(), data + 6, 6);
            frame.ethertype = combineBytesBigEndian(data[12], data[13]);
            if (len > 14) {
                frame.payload.insert(frame.payload.end(), data + 14, data + len);
            }
            return frame;
        }
        Frame Frame::decodeFrom(std::vector<uint8_t> const& data) {
            return Frame::decodeFrom(data.data(), data.size());
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