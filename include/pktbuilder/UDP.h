#pragma once
#include <pktbuilder/Layer.h>
#include <pktbuilder/addresses.h>
#include <pktbuilder/IPv4.h>

namespace pktbuilder {
    namespace UDP {
        class Datagram: public IntermediaryLayer {
            uint16_t source_port = 0;
            uint16_t destination_port = 0;
            ipv4_addr_t source_address = {0, 0, 0, 0};
            ipv4_addr_t destination_address = {0, 0, 0, 0};
            bool no_checksum = false;
        public:
            Datagram() = default;
            explicit Datagram(uint32_t destination_port, uint32_t source_port = 0, bool no_checksum = false);
            Datagram(ipv4_addr_t destination_address, uint32_t destination_port, ipv4_addr_t source_address, uint32_t source_port = 0);
            IPv4::Packet operator|(IPv4::Packet const& other);
            [[nodiscard]] std::vector<uint8_t> build() const override;
        };
    }
}