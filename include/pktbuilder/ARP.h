#pragma once

#include <bit>
#include <cstring>
#include <pktbuilder/Layer.h>
#include <pktbuilder/addresses.h>

namespace pktbuilder {
    template <uint8_t hlen, uint8_t plen>
    class ArpPacket: public Layer {
    protected:
        std::array<uint8_t, hlen> sender_hardware_address;
        std::array<uint8_t, hlen> target_hardware_address;
        std::array<uint8_t, plen> sender_protocol_address;
        std::array<uint8_t, plen> target_protocol_address;
        uint16_t hardware_type;
        uint16_t protocol_type;
        uint16_t opcode;
    public:
        ArpPacket() {
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
        ArpPacket(std::array<uint8_t, hlen> sender_hardware_address,
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
            bool swap_byte_order;
            if (std::endian::native == std::endian::little) {
                swap_byte_order = true;
            } else {
                swap_byte_order = false;
            }
            std::vector<uint8_t> data;
            data.reserve(8 + 2 * (hlen + plen));
            uint8_t hwtype_bytes[2];
            std::memcpy(hwtype_bytes, &this->hardware_type, 2);
            if (swap_byte_order) {
                std::reverse(
                        std::begin(hwtype_bytes),
                        std::end(hwtype_bytes)
                );
            }
            data.insert(
                    data.end(),
                    std::begin(hwtype_bytes),
                    std::end(hwtype_bytes)
            );
            uint8_t prototype_bytes[2];
            std::memcpy(prototype_bytes, &this->protocol_type, 2);
            if (swap_byte_order) {
                std::reverse(
                        std::begin(prototype_bytes),
                        std::end(prototype_bytes)
                );
            }
            data.insert(
                    data.end(),
                    std::begin(prototype_bytes),
                    std::end(prototype_bytes)
            );
            data.push_back(hlen);
            data.push_back(plen);
            uint8_t opcode_bytes[2];
            std::memcpy(opcode_bytes, &this->opcode, 2);
            if (swap_byte_order) {
                std::reverse(std::begin(opcode_bytes), std::end(opcode_bytes));
            }
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
    };
    template<uint8_t hlen, uint8_t plen>
    class ArpRequest: public ArpPacket<hlen, plen> {
    public:
        ArpRequest() : ArpPacket<hlen, plen>(){
            this->opcode = 1;
        }
        ArpRequest(std::array<uint8_t, hlen> sender_hardware_address,
                   std::array<uint8_t, plen> sender_protocol_address,
                   std::array<uint8_t, hlen> target_hardware_address,
                   std::array<uint8_t, plen> target_protocol_address,
                   uint16_t hardware_type, uint16_t protocol_type)
                   :ArpPacket<hlen, plen>(sender_hardware_address,
                                         sender_protocol_address,
                                         target_hardware_address,
                                         target_protocol_address,
                                         hardware_type, protocol_type, 1) {};
    };
    template<uint8_t hlen, uint8_t plen>
    class ArpReply: public ArpPacket<hlen, plen> {
    public:
        ArpReply() : ArpPacket<hlen, plen>(){
            this->opcode = 2;
        }
        ArpReply(std::array<uint8_t, hlen> sender_hardware_address,
                   std::array<uint8_t, plen> sender_protocol_address,
                   std::array<uint8_t, hlen> target_hardware_address,
                   std::array<uint8_t, plen> target_protocol_address,
                   uint16_t hardware_type, uint16_t protocol_type)
                   :ArpPacket<hlen, plen>(sender_hardware_address,
                                          sender_protocol_address,
                                          target_hardware_address,
                                          target_protocol_address,
                                          hardware_type, protocol_type, 1) {};
    };
    template<uint8_t plen>
    class EthernetArpRequest: public ArpRequest<6, plen> {
    public:
        EthernetArpRequest(): ArpRequest<6, plen>() {
            this->hardware_type = 1;
        }
        EthernetArpRequest(std::array<uint8_t, 6> sender_mac_address,
                           std::array<uint8_t, plen> sender_protocol_address,
                           std::array<uint8_t, 6> target_mac_address,
                           std::array<uint8_t, plen> target_protocol_address,
                           uint16_t protocol_type)
                           :ArpRequest<6, plen>(sender_mac_address,
                                                sender_protocol_address,
                                                target_mac_address,
                                                target_protocol_address,
                                                1, protocol_type) {};
    };
    template<uint8_t plen>
    class EthernetArpReply: public ArpReply<6, plen> {
    public:
        EthernetArpReply(): ArpReply<6, plen>() {
            this->hardware_type = 1;
        }
        EthernetArpReply(mac_addr_t sender_mac_address,
                         std::array<uint8_t, plen> sender_protocol_address,
                         mac_addr_t target_mac_address,
                         std::array<uint8_t, plen> target_protocol_address,
                         uint16_t protocol_type)
                         :ArpReply<6, plen>(sender_mac_address,
                                            sender_protocol_address,
                                            target_mac_address,
                                            target_protocol_address,
                                            1, protocol_type) {};
    };
    template<uint8_t hlen>
    class IPv4ArpRequest: public ArpRequest<hlen, 4> {
    public:
        IPv4ArpRequest(): ArpRequest<hlen, 4>() {
            this->protocol_type = 0x0800;
        }
        IPv4ArpRequest(std::array<uint8_t, hlen> sender_hardware_address,
                       ipv4_addr_t sender_ipv4_address,
                       std::array<uint8_t, hlen> target_hardware_address,
                       ipv4_addr_t target_ipv4_address,
                       uint16_t hardware_type)
                       :ArpRequest<hlen, 4>(sender_hardware_address,
                                            sender_ipv4_address,
                                            target_hardware_address,
                                            target_ipv4_address,
                                            hardware_type, 0x0800) {};
    };
    template<uint8_t hlen>
    class IPv4ArpReply: public ArpReply<hlen, 4> {
    public:
        IPv4ArpReply(): ArpReply<hlen, 4>() {
            this->protocol_type = 0x0800;
        }
        IPv4ArpReply(std::array<uint8_t, hlen> sender_hardware_address,
                     ipv4_addr_t sender_ipv4_address,
                     std::array<uint8_t, hlen> target_hardware_address,
                     ipv4_addr_t target_ipv4_address,
                     uint16_t hardware_type)
                     :ArpReply<hlen, 4>(sender_hardware_address,
                                        sender_ipv4_address,
                                        target_hardware_address,
                                        target_ipv4_address,
                                        hardware_type, 0x0800) {};
    };
    class IPv4EthernetArpRequest: public EthernetArpRequest<4> {
    public:
        IPv4EthernetArpRequest(): EthernetArpRequest<4>() {
            this->protocol_type = 0x0800;
        }
        IPv4EthernetArpRequest(ipv4_addr_t sender_ipv4_address,
                               mac_addr_t sender_mac_address,
                               ipv4_addr_t target_ipv4_address,
                               mac_addr_t target_mac_address = {0, 0, 0, 0, 0, 0})
                               :EthernetArpRequest<4>(sender_mac_address,
                                                      sender_ipv4_address,
                                                      target_mac_address,
                                                      target_ipv4_address,
                                                      0x0800) {};
    };
    class IPv4EthernetArpReply: public EthernetArpReply<4> {
    public:
        IPv4EthernetArpReply(): EthernetArpReply<4>() {
            this->protocol_type = 0x0800;
        }
        IPv4EthernetArpReply(ipv4_addr_t sender_ipv4_address,
                             mac_addr_t sender_mac_address,
                             ipv4_addr_t target_ipv4_address,
                             mac_addr_t target_mac_address)
                             :EthernetArpReply<4>(sender_mac_address,
                                                  sender_ipv4_address,
                                                  target_mac_address,
                                                  target_ipv4_address,
                                                  0x0800) {};
    };
}