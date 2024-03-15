#include <pktbuilder/IPv4.h>
#include <pktbuilder/checksum.h>
#include <random>
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
                            std::vector<Option> const& options){
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
            this->options = options;
        }
        Packet Packet::decodeFrom(const uint8_t* data, size_t len, bool verify_checksum) {
            if (len < 20) {
                throw(std::invalid_argument("IPv4 header too short"));
            }
            Packet packet;
            const uint8_t header_length = (data[0] & 0x0f) * 4;
            if (header_length < 20 || header_length > len) {
                throw(std::invalid_argument("IPv4 header length field too small"));
            }
            if (verify_checksum) {
                if (calculateInternetChecksum(data, header_length) != 0) {
                    throw(std::invalid_argument("IPv4 checksum incorrect"));
                }
            }
            packet.dscp = (data[1] & 0xfc) >> 2;
            packet.ecn = static_cast<ECNCodePoint>(data[1] & 0x03);
            const uint16_t total_length = combineBytesBigEndian(data[2], data[3]);
            if (total_length != len) {
                throw(std::invalid_argument("IPv4 total length incorrect"));
            }
            packet.identification = combineBytesBigEndian(data[4], data[5]);
            const uint16_t flags_frag_offset = combineBytesBigEndian(data[6], data[7]);
            packet.dont_fragment = static_cast<bool>(flags_frag_offset & 0x4000);
            packet.more_fragments = static_cast<bool>(flags_frag_offset & 0x2000);
            packet.fragment_offset = flags_frag_offset & 0x1fff;
            packet.ttl = data[8];
            packet.protocol = data[9];
            std::copy(data + 12, data + 16, packet.source_ip.data());
            std::copy(data + 16, data + 20, packet.destination_ip.data());
            size_t prev_opt_end = 20;
            while (prev_opt_end < header_length - 1) {
                Option option{};
                option.option_type = data[prev_opt_end];
                if (option.option_type != OptionType::EOOL && option.option_type != OptionType::NOP) {
                    uint8_t option_len = data[prev_opt_end + 1];
                    if (prev_opt_end + option_len >= header_length)
                    {
                        throw(std::invalid_argument("IPv4 Option too long"));
                    }
                    uint8_t option_value_len = option_len - 2;
                    if (option_value_len != 0) {
                        option.option_value.insert(
                            option.option_value.end(),
                            data + prev_opt_end + 2,
                            data + prev_opt_end + option_len);
                    }
                    packet.options.push_back(option);
                    prev_opt_end += option_value_len + 2;
                } else {
                    if (option.option_type == OptionType::NOP) {
                        packet.options.push_back(option);
                    } // don't ignore NOPs but ignore EOOL as used for padding
                    prev_opt_end++;
                }
            }
            if (total_length > header_length) {
                packet.payload.insert(
                    packet.payload.end(),
                    data + header_length,
                    data + total_length
                );
            }
            return packet;
        }
        Packet Packet::decodeFrom(std::vector<uint8_t> const& data, bool verify_checksum) {
            return decodeFrom(data.data(), data.size(), verify_checksum);
        }
        std::vector<uint8_t> Packet::build() const {
            std::vector<uint8_t> data;
            data.reserve(20);
            uint8_t ihl_bytes = 20;
            uint8_t option_padding = 0;
            for (const auto& option: options) {
                if (option.option_type == OptionType::NOP || option.option_type == OptionType::EOOL) {
                    ihl_bytes += 1;
                } else {
                    ihl_bytes += 2 + option.option_value.size();
                }
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
                if (option.option_value.size() > UINT8_MAX - 2)
                {
                    throw(std::runtime_error("IPv4 option data too large"));
                }
                if (option.option_type != OptionType::NOP && option.option_type != OptionType::EOOL) {
                    data.push_back(static_cast<uint8_t>(option.option_value.size() + 2));
                    data.insert(data.end(), option.option_value.begin(),
                            option.option_value.end());
                }
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
        ipv4_addr_t const& Packet::getSourceAddress() const {
            return this->source_ip;
        }

        ipv4_addr_t const& Packet::getDestinationAddress() const {
            return this->destination_ip;
        }

        uint8_t Packet::getProtocolNumber() const {
            return this->protocol;
        }

        uint8_t Packet::getTTL() const {
            return this->ttl;
        }

        ECNCodePoint Packet::getECN() const {
            return this->ecn;
        }

        uint8_t Packet::getDSCP() const {
            return this->dscp;
        }

        uint16_t Packet::getFragmentOffset() const {
            return this->fragment_offset;
        }

        bool Packet::isFlagDontFragment() const {
            return this->dont_fragment;
        }

        bool Packet::isFlagMoreFragments() const {
            return this->more_fragments;
        }

        std::vector<Option> const& Packet::getOptions() const {
            return this->options;
        }

        uint16_t Packet::getIdentification() const {
            return this->identification;
        }
        
        void Packet::setProtocolNumber(uint8_t p) {
            this->protocol = p;
        }
    }
}
