#include <pktbuilder.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main() {
    int s = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
    sockaddr_in addr{};
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    inet_aton("127.0.0.1", &addr.sin_addr);
    connect(s, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
    auto message = std::array<uint8_t, 4>{0xde, 0xad, 0xbe, 0xef} | 
                    pktbuilder::ICMP::Packet(pktbuilder::ICMP::Type::ECHO_REQUEST, 
                        pktbuilder::ICMP::Code::EchoRequest::ECHO_REQUEST, {0, 0, 0, 0});
    
    std::vector<uint8_t> data = message.build();
    send(s, data.data(), data.size(), 0);
}