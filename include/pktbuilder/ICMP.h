#pragma once

#include <pktbuilder/Layer.h>
#include <array>

namespace pktbuilder {
    namespace ICMP {
        class Packet: public IntermediaryLayer{
            uint8_t type{};
            uint8_t code{};
            std::array<uint8_t, 4> header_contents{};
        public:
            Packet() = default;
            Packet(uint8_t type, uint8_t code,
                    std::array<uint8_t, 4> const& header_contents);
            [[nodiscard]] std::vector<uint8_t> build() const override;
        };
    }
}