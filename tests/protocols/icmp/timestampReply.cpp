#include <pktbuilder.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <chrono>
#include <random>

using namespace pktbuilder::ICMP;
int main() {
    int s = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
    sockaddr_in addr{};
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    inet_aton("127.0.0.1", &addr.sin_addr);
    connect(s, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
    std::mt19937_64 rng(std::random_device{}());
    std::uniform_int_distribution<uint16_t> ident_dest(0, UINT16_MAX);
    uint16_t identifier = ident_dest(rng);
    uint32_t originate_timestamp = 0;
    auto message = createTimestampReplyMessage(originate_timestamp) |
        Packet(Type::TIMESTAMP_REPLY, Code::TimestampReply::TIMESTAMP_REPLY, identifier, 0);
    std::vector<uint8_t> data = message.build();
    send(s, data.data(), data.size(), 0);
}