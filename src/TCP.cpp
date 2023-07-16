#include <pktbuilder/TCP.h>
#include <pktbuilder/checksum.h>
#include <pktbuilder/utils.h>
#include <cmath>
#include <stdexcept>

namespace pktbuilder {
    namespace TCP {

        Packet::Packet(uint16_t source_port, uint16_t destination_port,
                            uint32_t sequence_number,
                            uint8_t flags, uint16_t window_size,
                            const std::vector<Option> &options,
                            uint32_t ack_number, uint16_t urgent_pointer) {
            this->source_port = source_port;
            this->destination_port = destination_port;
            this->sequence_number = sequence_number;
            this->ack_number = ack_number;
            this->flags = flags;
            this->window_size = window_size;
            this->options = options;
            this->urgent_pointer = urgent_pointer;
            this->source_address = { 0, 0, 0, 0 };
            this->destination_address = { 0, 0, 0, 0 };
        }

        Packet::Packet(uint16_t destination_port,
                            ipv4_addr_t destination_address, uint16_t source_port,
                            uint32_t sequence_number, uint8_t flags,
                            uint16_t window_size,
                            const std::vector<Option> &options,
                            ipv4_addr_t source_address, uint32_t ack_number,
                            uint16_t urgent_pointer) {
            if (source_address == ipv4_addr_t({0, 0, 0, 0})) {
                this->source_address = getDefaultInterfaceIPv4Address();
            } else {
                this->source_address = source_address;
            }
            this->source_port = source_port;
            this->destination_address = destination_address;
            this->destination_port = destination_port;
            this->sequence_number = sequence_number;
            this->ack_number = ack_number;
            this->flags = flags;
            this->window_size = window_size;
            this->options = options;
            this->urgent_pointer = urgent_pointer;
        }

        IPv4::Packet Packet::operator| (const IPv4::Packet& other) {
            this->source_address = other.getSourceAddress();
            this->destination_address = other.getDestinationAddress();
            IPv4::Packet new_packet = other;
            if (other.getProtocolNumber()) {
                new_packet.setProtocolNumber(other.getProtocolNumber());
            } else {
                new_packet.setProtocolNumber(IPv4::ProtocolNumber::TCP);
            }
            new_packet.setPayload(this->build());
            return new_packet;
        }

        std::vector<uint8_t> Packet::build() const {
            std::vector<uint8_t> data;
            std::array<uint8_t, 2> src_port_bytes = splitBytesBigEndian(this->source_port);
            data.insert(data.end(), src_port_bytes.begin(), src_port_bytes.end());
            std::array<uint8_t, 2> dest_port_bytes = splitBytesBigEndian(this->destination_port);
            data.insert(data.end(), dest_port_bytes.begin(), dest_port_bytes.end());
            std::array<uint8_t, 4> seq_num_bytes = splitBytesBigEndian(this->sequence_number);
            data.insert(data.end(), seq_num_bytes.begin(), seq_num_bytes.end());
            std::array<uint8_t, 4> ack_num_bytes = splitBytesBigEndian(this->ack_number);
            data.insert(data.end(), ack_num_bytes.begin(), ack_num_bytes.end());
            uint8_t header_size = 20;
            for (Option const &option: this->options) {
                header_size += 2 + option.option_data.size(); // nop + type + data
            }
            uint8_t padding_bytes = 0;
            if (header_size % 4 != 0) {
                padding_bytes = static_cast<uint8_t>(std::ceil(header_size / 4.) * 4 - header_size);
            }
            header_size += padding_bytes;
            const uint8_t data_offset = header_size / 4;
            if (data_offset != (data_offset & 0xffff)) {
                throw(std::runtime_error("TCP Header too long"));
            }
            data.push_back(data_offset << 4u);
            data.push_back(this->flags);
            std::array<uint8_t, 2> window_size_bytes = splitBytesBigEndian(this->window_size);
            data.insert(data.end(), window_size_bytes.begin(), window_size_bytes.end());
            data.push_back(0);
            data.push_back(0);
            std::array<uint8_t, 2> urg_ptr_bytes = splitBytesBigEndian(this->urgent_pointer);
            data.insert(data.end(), urg_ptr_bytes.begin(), urg_ptr_bytes.end());
            for (Option const &option : this->options) {
                data.push_back(0x01);
                data.push_back(static_cast<uint8_t>(option.option_kind));
                data.insert(data.end(), option.option_data.begin(), option.option_data.end());
            }
            for (auto i = 0; i < padding_bytes; i++) {
                data.push_back(0);
            }
            data.insert(data.end(), this->payload.begin(), this->payload.end());
            std::vector<uint8_t> pseudo_header;
            pseudo_header.reserve(12);
            pseudo_header.insert(pseudo_header.end(), this->source_address.begin(), this->source_address.end());
            pseudo_header.insert(pseudo_header.end(), this->destination_address.begin(), this->destination_address.end());
            pseudo_header.push_back(0);
            pseudo_header.push_back(static_cast<uint8_t>(IPv4::ProtocolNumber::TCP));
            if (data.size() > UINT16_MAX) {
                throw(std::runtime_error("TCP Packet too large for pseudoheader"));
            }
            const auto tcp_length = static_cast<uint16_t>(data.size());
            std::array<uint8_t, 2> length_bytes = splitBytesBigEndian(tcp_length);
            pseudo_header.insert(pseudo_header.end(), length_bytes.begin(), length_bytes.end());
            std::vector<uint8_t> to_checksum;
            to_checksum.reserve(data.size() + pseudo_header.size());
            to_checksum.insert(to_checksum.end(), pseudo_header.begin(), pseudo_header.end());
            to_checksum.insert(to_checksum.end(), data.begin(), data.end());
            const uint16_t checksum = calculateInternetChecksum(to_checksum);
            const std::array<uint8_t, 2> checksum_bytes = splitBytesBigEndian(checksum);
            data.at(16) = checksum_bytes[0];
            data.at(17) = checksum_bytes[1];
            return data;
        }
    }
}