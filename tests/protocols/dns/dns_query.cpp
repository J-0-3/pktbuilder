#include <pktbuilder.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main() {
    int s = socket(PF_INET, SOCK_DGRAM, 0);
    sockaddr_in addr{};
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(53);
    inet_aton("127.0.0.1", &addr.sin_addr);
    connect(s, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
    auto message = pktbuilder::DNS::Message(1, pktbuilder::DNS::MessageType::QUERY, 
        pktbuilder::DNS::Opcode::QUERY, pktbuilder::DNS::Flag::RD, 0, 
        {
            pktbuilder::DNS::Question{
                .domain_name = "google.com",
                .qtype = pktbuilder::DNS::Type::A,
                .qclass = pktbuilder::DNS::Class::IN 
            },
            pktbuilder::DNS::Question{
                .domain_name = "amazon.co.uk",
                .qtype = pktbuilder::DNS::Type::AAAA,
                .qclass = pktbuilder::DNS::Class::IN
            }
        }, {}, {}, {}
    );
    std::vector<uint8_t> data = message.build();
    send(s, data.data(), data.size(), 0);
    return 0;
}