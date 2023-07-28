#pragma once

#include <pktbuilder/Layer.h>
#include <pktbuilder/addresses.h>
#include <pktbuilder/Ethernet.h>
#include <pktbuilder/utils.h>

namespace pktbuilder {
    namespace ARP {
        template <uint8_t hlen, uint8_t plen>
        class Packet: public Layer {
        protected:
            std::array<uint8_t, hlen> sender_hardware_address;
            std::array<uint8_t, hlen> target_hardware_address;
            std::array<uint8_t, plen> sender_protocol_address;
            std::array<uint8_t, plen> target_protocol_address;
            uint16_t hardware_type;
            uint16_t protocol_type;
            uint16_t opcode;
        public:
            Packet() {
                this->sender_hardware_address = std::array<uint8_t, hlen>();
                this->sender_hardware_address.fill(0);
                this->sender_protocol_address = std::array<uint8_t, plen>();
                this->sender_protocol_address.fill(0);
                this->target_hardware_address = std::array<uint8_t, hlen>();
                this->target_hardware_address.fill(0);
                this->target_protocol_address = std::array<uint8_t, plen>();
                this->target_protocol_address.fill(0);
                this->hardware_type = 0;
                this->protocol_type = 0;
                this->opcode = 0;
            }
            Packet(std::array<uint8_t, hlen> sender_hardware_address,
                    std::array<uint8_t, plen> sender_protocol_address,
                    std::array<uint8_t, hlen> target_hardware_address,
                    std::array<uint8_t, plen> target_protocol_address,
                    uint16_t hardware_type, uint16_t protocol_type,
                    uint16_t opcode) {
                this->sender_hardware_address = sender_hardware_address;
                this->sender_protocol_address = sender_protocol_address;
                this->target_hardware_address= target_hardware_address;
                this->target_protocol_address = target_protocol_address;
                this->hardware_type = hardware_type;
                this->protocol_type = protocol_type;
                this->opcode = opcode;
            }
            [[nodiscard]] std::vector<uint8_t> build() const override {
                std::vector<uint8_t> data;
                data.reserve(8 + 2 * (hlen + plen));
                std::array<uint8_t, 2> hwtype_bytes = splitBytesBigEndian(this->hardware_type);
                data.insert(
                        data.end(),
                        std::begin(hwtype_bytes),
                        std::end(hwtype_bytes)
                );
                std::array<uint8_t, 2> prototype_bytes = splitBytesBigEndian(this->protocol_type);
                data.insert(
                        data.end(),
                        std::begin(prototype_bytes),
                        std::end(prototype_bytes)
                );
                data.push_back(hlen);
                data.push_back(plen);
                std::array<uint8_t, 2> opcode_bytes = splitBytesBigEndian(this->opcode);
                data.insert(
                        data.end(),
                        std::begin(opcode_bytes),
                        std::end(opcode_bytes)
                );
                data.insert(
                        data.end(),
                        this->sender_hardware_address.begin(),
                        this->sender_hardware_address.end()
                );
                data.insert(
                        data.end(),
                        this->sender_protocol_address.begin(),
                        this->sender_protocol_address.end()
                );
                data.insert(
                        data.end(),
                        this->target_hardware_address.begin(),
                        this->target_hardware_address.end()
                );
                data.insert(
                        data.end(),
                        this->target_protocol_address.begin(),
                        this->target_protocol_address.end()
                );
                return data;
            }
            Ethernet::Frame operator|(Ethernet::Frame const& other) const {
                uint16_t ethertype = other.getEthertype();
                if (!ethertype) {
                    ethertype = Ethernet::EtherType::ARP;
                }
                Ethernet::Frame frame(other.getDestinationMac(), ethertype, 
                    other.getSourceMac());
                frame.setPayload(this->build());
                return frame;
            }
        };
        template<uint8_t hlen, uint8_t plen>
        class Request: public Packet<hlen, plen> {
        public:
            Request() : Packet<hlen, plen>(){
                this->opcode = 1;
            }
            Request(std::array<uint8_t, hlen> sender_hardware_address,
                    std::array<uint8_t, plen> sender_protocol_address,
                    std::array<uint8_t, hlen> target_hardware_address,
                    std::array<uint8_t, plen> target_protocol_address,
                    uint16_t hardware_type, uint16_t protocol_type)
                    :Packet<hlen, plen>(sender_hardware_address,
                                            sender_protocol_address,
                                            target_hardware_address,
                                            target_protocol_address,
                                            hardware_type, protocol_type, 1) {}
        };
        template<uint8_t hlen, uint8_t plen>
        class Reply: public Packet<hlen, plen> {
        public:
            Reply() : Packet<hlen, plen>(){
                this->opcode = 2;
            }
            Reply(std::array<uint8_t, hlen> sender_hardware_address,
                    std::array<uint8_t, plen> sender_protocol_address,
                    std::array<uint8_t, hlen> target_hardware_address,
                    std::array<uint8_t, plen> target_protocol_address,
                    uint16_t hardware_type, uint16_t protocol_type)
                    :Packet<hlen, plen>(sender_hardware_address,
                                            sender_protocol_address,
                                            target_hardware_address,
                                            target_protocol_address,
                                            hardware_type, protocol_type, 2) {}
        };
        template<uint8_t plen>
        class EthernetRequest: public Request<6, plen> {
        public:
            EthernetRequest(): Request<6, plen>() {
                this->hardware_type = 1;
            }
            EthernetRequest(std::array<uint8_t, 6> sender_mac_address,
                            std::array<uint8_t, plen> sender_protocol_address,
                            std::array<uint8_t, 6> target_mac_address,
                            std::array<uint8_t, plen> target_protocol_address,
                            uint16_t protocol_type)
                            :Request<6, plen>(sender_mac_address,
                                                    sender_protocol_address,
                                                    target_mac_address,
                                                    target_protocol_address,
                                                    1, protocol_type) {}
        };
        template<uint8_t plen>
        class EthernetReply: public Reply<6, plen> {
        public:
            EthernetReply(): Reply<6, plen>() {
                this->hardware_type = 1;
            }
            EthernetReply(mac_addr_t sender_mac_address,
                            std::array<uint8_t, plen> sender_protocol_address,
                            mac_addr_t target_mac_address,
                            std::array<uint8_t, plen> target_protocol_address,
                            uint16_t protocol_type)
                            :Reply<6, plen>(sender_mac_address,
                                                sender_protocol_address,
                                                target_mac_address,
                                                target_protocol_address,
                                                1, protocol_type) {}
        };
        template<uint8_t hlen>
        class IPv4Request: public Request<hlen, 4> {
        public:
            IPv4Request(): Request<hlen, 4>() {
                this->protocol_type = 0x0800;
            }
            IPv4Request(std::array<uint8_t, hlen> sender_hardware_address,
                        ipv4_addr_t sender_ipv4_address,
                        std::array<uint8_t, hlen> target_hardware_address,
                        ipv4_addr_t target_ipv4_address,
                        uint16_t hardware_type)
                        :Request<hlen, 4>(sender_hardware_address,
                                                sender_ipv4_address,
                                                target_hardware_address,
                                                target_ipv4_address,
                                                hardware_type, 0x0800) {}
        };
        template<uint8_t hlen>
        class IPv4Reply: public Reply<hlen, 4> {
        public:
            IPv4Reply(): Reply<hlen, 4>() {
                this->protocol_type = 0x0800;
            }
            IPv4Reply(std::array<uint8_t, hlen> sender_hardware_address,
                        ipv4_addr_t sender_ipv4_address,
                        std::array<uint8_t, hlen> target_hardware_address,
                        ipv4_addr_t target_ipv4_address,
                        uint16_t hardware_type)
                        :Reply<hlen, 4>(sender_hardware_address,
                                            sender_ipv4_address,
                                            target_hardware_address,
                                            target_ipv4_address,
                                            hardware_type, 0x0800) {}
        };
        class IPv4EthernetRequest: public EthernetRequest<4> {
        public:
            IPv4EthernetRequest() {
                this->protocol_type = 0x0800;
            }
            IPv4EthernetRequest(ipv4_addr_t sender_ipv4_address,
                                mac_addr_t sender_mac_address,
                                ipv4_addr_t target_ipv4_address,
                                mac_addr_t target_mac_address = {0, 0, 0, 0, 0, 0})
                                :EthernetRequest<4>(sender_mac_address,
                                                        sender_ipv4_address,
                                                        target_mac_address,
                                                        target_ipv4_address,
                                                        0x0800) {}
        };
        class IPv4EthernetReply: public EthernetReply<4> {
        public:
            IPv4EthernetReply() {
                this->protocol_type = 0x0800;
            }
            IPv4EthernetReply(ipv4_addr_t sender_ipv4_address,
                                mac_addr_t sender_mac_address,
                                ipv4_addr_t target_ipv4_address,
                                mac_addr_t target_mac_address)
                                :EthernetReply<4>(sender_mac_address,
                                                    sender_ipv4_address,
                                                    target_mac_address,
                                                    target_ipv4_address,
                                                    0x0800) {}
        };
    }
}