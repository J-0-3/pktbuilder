#include <pktbuilder/IPv4.h>
#include <pktbuilder/checksum.h>
#include <random>
#include <chrono>
#include <stdexcept>
#include "pktbuilder/utils.h"

namespace pktbuilder {
    namespace IPv4 {
        Packet::Packet(ipv4_addr_t destination_ip, uint8_t protocol, ipv4_addr_t source_ip) {
            if (source_ip == ipv4_addr_t({0, 0, 0, 0})) {
                this->source_ip = getDefaultInterfaceIPv4Address();
            } else {
                this->source_ip = source_ip;
            }
            this->destination_ip = destination_ip;
            this->protocol = protocol;
            this->dscp = 0;
            this->ecn = ECNCodePoint::NOT_ECT;
            this->ttl = 64;
            std::mt19937 mt{std::random_device{}()};
            std::uniform_int_distribution<uint16_t> ident_vals{0, UINT16_MAX};
            this->identification = ident_vals(mt);
            this->dont_fragment = true;
            this->more_fragments = false;
            this->options = std::vector<Option>();
            this->fragment_offset = 0;
        }
        Packet::Packet(ipv4_addr_t destination_ip, ipv4_addr_t source_ip,
                            uint8_t protocol, uint16_t identification,
                            uint8_t dscp, ECNCodePoint ecn,
                            bool df_flag, bool mf_flag, uint16_t fragment_offset,
                            uint8_t ttl,
                            const std::vector<Option> *options){
            this->source_ip = source_ip;
            this->destination_ip = destination_ip;
            this->protocol = protocol;
            this->dscp = dscp;
            this->ecn = ecn;
            this->ttl = ttl;
            this->identification = identification;
            this->dont_fragment = df_flag;
            this->more_fragments = mf_flag;
            this->fragment_offset = fragment_offset;
            this->options = *options;
        }
        std::vector<uint8_t> Packet::build() const {
            std::vector<uint8_t> data;
            data.reserve(20);
            uint8_t ihl_bytes = 20;
            uint8_t option_padding = 0;
            for (const auto& option: options) {
                ihl_bytes += 2 + option.option_value.size();
            }
            if (ihl_bytes % 4 != 0) {
                option_padding = static_cast<uint8_t>(std::ceil(ihl_bytes / 4.)) * 4 - ihl_bytes;
                ihl_bytes += option_padding;
            }
            const uint8_t ihl = ihl_bytes / 4;
            if (ihl != (ihl & 0x0f)) {
                throw(std::runtime_error("IPv4 header too large"));
            }
            data.push_back(0x40 | ihl);
            data.push_back(static_cast<uint8_t>(this->dscp << 2) | static_cast<uint8_t>(ecn));
            const uint16_t total_length = ihl * 4ul + static_cast<uint16_t>(this->payload.size());
            const std::array<uint8_t, 2> length_bytes = splitBytesBigEndian(total_length);
            data.push_back(length_bytes[0]);
            data.push_back(length_bytes[1]);
            const std::array<uint8_t, 2> identification_bytes = splitBytesBigEndian(this->identification);
            data.push_back(identification_bytes[0]);
            data.push_back(identification_bytes[1]);
            const uint8_t flags = static_cast<uint8_t>(this->dont_fragment << 1) |
                            static_cast<uint8_t>(this->more_fragments);
            const uint16_t flags_frag_offset = static_cast<uint16_t>(flags << 13) | this->fragment_offset;
            const std::array<uint8_t, 2> flags_frag_offset_b = splitBytesBigEndian(flags_frag_offset);
            data.push_back(flags_frag_offset_b[0]);
            data.push_back(flags_frag_offset_b[1]);
            data.push_back(this->ttl);
            data.push_back(this->protocol);
            data.push_back(0);
            data.push_back(0);
            for(uint8_t octet: this->source_ip) {
                data.push_back(octet);
            }
            for(uint8_t octet: this->destination_ip) {
                data.push_back(octet);
            }
            for(Option option : this->options) {
                data.push_back(option.option_type);
                if (option.option_value.size() > UINT8_MAX)
                {
                    throw(std::runtime_error("IPv4 option data too large"));
                }
                data.push_back(static_cast<uint8_t>(option.option_value.size()));
                data.insert(data.end(), option.option_value.begin(),
                            option.option_value.end());
            }
            for (auto i = 0; i < option_padding; i++) {
                data.push_back(0);
            }
            const uint16_t checksum = calculateInternetChecksum(data);
            const std::array<uint8_t, 2> checksum_b = splitBytesBigEndian(checksum);
            data.at(10) = checksum_b[0];
            data.at(11) = checksum_b[1];
            data.insert(data.end(), this->payload.begin(), this->payload.end());
            return data;
        }

        Ethernet::Frame Packet::operator| (const Ethernet::Frame& other) const {
            uint16_t ethertype = other.getEthertype();
            if (!ethertype) {
                ethertype = Ethernet::EtherType::IPv4;
            }
            Ethernet::Frame new_frame(other.getDestinationMac(), ethertype, other.getSourceMac());
            new_frame.setPayload(this->build());
            return new_frame;
        }
        ipv4_addr_t const &Packet::getSourceAddress() const {
            return this->source_ip;
        }

        ipv4_addr_t const &Packet::getDestinationAddress() const {
            return this->destination_ip;
        }

        uint8_t const &Packet::getProtocolNumber() const {
            return this->protocol;
        }

        void Packet::setProtocolNumber(uint8_t p) {
            this->protocol = p;
        }
    }

}
