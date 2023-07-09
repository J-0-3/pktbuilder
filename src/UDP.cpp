#include <stdexcept>
#include <pktbuilder/UDP.h>
#include <pktbuilder/utils.h>
#include <pktbuilder/checksum.h>

namespace pktbuilder {
    UDPDatagram::UDPDatagram(uint32_t destination_port, uint32_t source_port, bool no_checksum) {
        this->source_port = source_port;
        this->destination_port = destination_port;
        this->source_address = { 0, 0, 0, 0 };
        this->destination_address = { 0, 0, 0, 0 };
        this->no_checksum = no_checksum;
    }
    UDPDatagram::UDPDatagram(ipv4_addr_t destination_address,
                uint32_t destination_port, ipv4_addr_t source_address,
                uint32_t source_port) {
        this->source_port = source_port;
        this->source_address = source_address;
        this->destination_port = destination_port;
        this->destination_address = destination_address;
        this->no_checksum = false;
    }

    IPv4Packet UDPDatagram::operator|(const IPv4Packet &other) {
        this->source_address = other.getSourceAddress();
        this->destination_address = other.getDestinationAddress();
        IPv4Packet new_packet = other;
        new_packet.setPayload(this->build());
        return new_packet;
    }

    std::vector<uint8_t> UDPDatagram::build() const {
        std::vector<uint8_t> data;
        data.reserve(8 + this->payload.size());
        std::array<uint8_t, 2> src_port_bytes = splitBytesBigEndian(this->source_port);
        data.insert(data.end(), src_port_bytes.begin(), src_port_bytes.end());
        std::array<uint8_t, 2> dst_port_bytes = splitBytesBigEndian(this->destination_port);
        data.insert(data.end(), dst_port_bytes.begin(), dst_port_bytes.end());
        if (this->payload.size() + 8 > UINT16_MAX) {
            throw(std::runtime_error("UDP Datagram too large"));
        }
        uint16_t udp_length = static_cast<uint8_t>(this->payload.size()) + 8;
        std::array<uint8_t, 2> udp_length_bytes = splitBytesBigEndian(udp_length);
        data.insert(data.end(), udp_length_bytes.begin(), udp_length_bytes.end());
        if (this->no_checksum) {
            data.push_back(0);
            data.push_back(0);
        } else {
            // construct IPv4 pseudoheader
            std::vector<uint8_t> to_checksum;
            to_checksum.reserve(12);
            to_checksum.insert(to_checksum.end(), this->source_address.begin(), this->source_address.end());
            to_checksum.insert(to_checksum.end(), this->destination_address.begin(), this->destination_address.end());
            to_checksum.push_back(0);
            to_checksum.push_back(static_cast<uint8_t>(IPv4ProtocolNumber::UDP));
            to_checksum.insert(to_checksum.end(), udp_length_bytes.begin(), udp_length_bytes.end());
            to_checksum.insert(to_checksum.end(), data.begin(), data.end());
            to_checksum.insert(to_checksum.end(), this->payload.begin(), this->payload.end());
            uint16_t checksum = calculateInternetChecksum(to_checksum);
            std::array<uint8_t, 2> checksum_bytes = splitBytesBigEndian(checksum);
            data.insert(data.end(), checksum_bytes.begin(), checksum_bytes.end());
        }
        data.insert(data.end(), this->payload.begin(), this->payload.end());
        return data;
    }

}