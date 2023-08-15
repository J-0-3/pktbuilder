#pragma once
#include <pktbuilder/addresses.h>
#include <pktbuilder/Layer.h>
#include <pktbuilder/IPv4.h>

namespace pktbuilder {
    namespace TCP {
		namespace OptionCode {
			enum OptionCode: uint8_t {
				EOL = 0,
				NOP = 1,
				MAX_SEGMENT_SIZE = 2,
				WINDOW_SCALE = 3,
				SACK_PERM = 4,
				SACK = 5,
				TIMESTAMP = 8
			};
		}

        struct Option {
            uint8_t option_kind;
            std::vector<uint8_t> option_data;
        };

		namespace Flag {
			enum Flag: uint8_t {
				CWR = 0x80,
				ECE = 0x40,
				URG = 0x20,
				ACK = 0x10,
				PSH = 0x08,
				RST = 0x04,
				SYN = 0x02,
				FIN = 0x01
			};
		}

		class Packet: public IntermediaryLayer {
			uint16_t source_port{};
			ipv4_addr_t source_address{};
			uint16_t destination_port{};
			ipv4_addr_t destination_address{};
			uint32_t sequence_number{};
			uint32_t ack_number{};
			uint8_t flags{};
			uint16_t window_size{};
			uint16_t urgent_pointer{};
			std::vector<Option> options;
			public:
			Packet() = default;
			Packet(uint16_t destination_port, uint16_t source_port,
					uint8_t flags, uint16_t window_size = UINT16_MAX, 
					uint32_t sequence_number = 0, 
					std::vector<Option> const& options = {},
					uint32_t ack_number = 0, uint16_t urgent_pointer = 0);
			Packet(uint16_t destination_port, ipv4_addr_t destination_address, 
					uint16_t source_port, uint8_t flags,  
					ipv4_addr_t source_address = { 0, 0, 0, 0 },
					uint16_t window_size = UINT16_MAX, 
					uint32_t sequence_number = 0, 
					std::vector<Option> const& options = {},
					uint32_t ack_number = 0, uint16_t urgent_pointer = 0);
			IPv4::Packet operator|(const IPv4::Packet& other);
			[[nodiscard]] static Packet decodeFrom(const uint8_t* data, size_t length);
			[[nodiscard]] static Packet decodeFrom(std::vector<uint8_t> const& data);
			[[nodiscard]] std::vector<uint8_t> build() const override;
			[[nodiscard]] uint16_t getSourcePort();
			[[nodiscard]] uint16_t getDestinationPort();
			[[nodiscard]] uint32_t getSequenceNumber();
			[[nodiscard]] uint32_t getAckNumber();
			[[nodiscard]] uint8_t getFlags();
			[[nodiscard]] uint16_t getWindowSize();
			[[nodiscard]] uint16_t getUrgentPointer();
			[[nodiscard]] std::vector<Option> const& getOptions();
		};
    }
}
