#pragma once
#include <pktbuilder/Layer.h>
#include <pktbuilder/addresses.h>
#include <pktbuilder/IPv4.h>

namespace pktbuilder {
    class UDPDatagram: public IntermediaryLayer {
        uint16_t source_port = 0;
        uint16_t destination_port = 0;
        ipv4_addr_t source_address = {0, 0, 0, 0};
        ipv4_addr_t destination_address = {0, 0, 0, 0};
        bool no_checksum = false;
    public:
        UDPDatagram() = default;
        explicit UDPDatagram(uint32_t destination_port, uint32_t source_port = 0, bool no_checksum = false);
        UDPDatagram(ipv4_addr_t destination_address, uint32_t destination_port, ipv4_addr_t source_address, uint32_t source_port = 0);
        IPv4Packet operator|(IPv4Packet const& other);
        [[nodiscard]] std::vector<uint8_t> build() const override;
    };
}