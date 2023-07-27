#include <pktbuilder.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
    std::string payload("IPPacketTestPayloadData");
    if (argc > 1) {
        payload = argv[1];
    }
    auto packet = payload | 
                  pktbuilder::IPv4::Packet
                  (
                    {127, 0, 0, 1}, 
                    pktbuilder::IPv4::ProtocolNumber::HOST_INTERNAL
                  );
    std::vector<uint8_t> data = packet.build();
    int s = socket(PF_INET, SOCK_RAW, IPPROTO_RAW);
    sockaddr_in addr{};
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    connect(s, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
    send(s, data.data(), data.size(), 0);
}