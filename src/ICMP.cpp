#include <pktbuilder/ICMP.h>
#include <pktbuilder/checksum.h>
#include <pktbuilder/utils.h>
#include <chrono>

namespace pktbuilder {
    namespace ICMP {
        Packet::Packet(uint8_t type, uint8_t code,
                            const std::array<uint8_t, 4> &header_contents) {
            this->type = type;
            this->code = code;
            this->header_contents = header_contents;
        }
        Packet::Packet(uint8_t type, uint8_t code, uint16_t identifier, uint16_t sequence_number) {
            this->type = type;
            this->code = code;
            std::array<uint8_t, 2> ident_b = splitBytesBigEndian(identifier);
            std::array<uint8_t, 2> seq_num_b = splitBytesBigEndian(sequence_number);
            this->header_contents = {ident_b[0], ident_b[1], seq_num_b[0], seq_num_b[1]};
        }
        std::vector<uint8_t> Packet::build() const {
            std::vector<uint8_t> data;
            data.push_back(this->type);
            data.push_back(this->code);
            data.push_back(0);
            data.push_back(0);
            for (uint8_t x: this->header_contents) {
                data.push_back(x);
            }
            data.insert(data.end(), this->payload.begin(), this->payload.end());
            const uint16_t checksum = calculateInternetChecksum(data);
            const std::array<uint8_t, 2> checksum_bytes = splitBytesBigEndian(checksum);
            data.at(2) = checksum_bytes[0];
            data.at(3) = checksum_bytes[1];
            return data;
        }
        IPv4::Packet Packet::operator|(IPv4::Packet const& other) {
            uint16_t proto_num = other.getProtocolNumber();
            if (!proto_num) {
                proto_num = IPv4::ProtocolNumber::ICMP;
            }
            IPv4::Packet packet = other;
            packet.setProtocolNumber(proto_num);
            packet.setPayload(this->build());
            return packet;
        }
        std::array<uint8_t, 12> createTimestampMessage(uint32_t originate_timestamp) {
            std::array<uint8_t, 12> data{};
            std::array<uint8_t, 4> ts_bytes = splitBytesBigEndian(originate_timestamp);
            std::copy(ts_bytes.begin(), ts_bytes.end(), data.begin());
            return data;
        }
        std::array<uint8_t, 12> createTimestampMessage() {
            auto day = std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now());
            auto time = std::chrono::floor<std::chrono::milliseconds>(std::chrono::system_clock::now());
            std::chrono::milliseconds since_midnight = time - day;
            return createTimestampMessage(since_midnight.count());
        }
        std::array<uint8_t, 12> createTimestampReplyMessage(uint32_t originate_timestamp,
                uint32_t receive_timestamp, uint32_t transmit_timestamp) {
            std::array<uint8_t, 12> data{};
            std::array<uint8_t, 4> or_bytes = splitBytesBigEndian(originate_timestamp);
            std::array<uint8_t, 4> re_bytes = splitBytesBigEndian(receive_timestamp);
            std::array<uint8_t, 4> tr_bytes = splitBytesBigEndian(transmit_timestamp);
            std::copy(or_bytes.begin(), or_bytes.end(), data.begin());
            std::copy(re_bytes.begin(), re_bytes.end(), data.begin() + 4);
            std::copy(tr_bytes.begin(), tr_bytes.end(), data.begin() + 8);
            return data;
        }
        std::array<uint8_t, 12> createTimestampReplyMessage(uint32_t originate_timestamp) {
            auto day = std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now());
            auto time = std::chrono::floor<std::chrono::milliseconds>(std::chrono::system_clock::now());
            std::chrono::milliseconds since_midnight = time - day;
            return createTimestampReplyMessage(originate_timestamp, 
                since_midnight.count(), since_midnight.count());
        }
    }
}