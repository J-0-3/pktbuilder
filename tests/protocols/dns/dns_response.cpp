#include <pktbuilder.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(void) {
    int s = socket(PF_INET, SOCK_DGRAM, 0);
    sockaddr_in addr{.sin_family = AF_INET, .sin_port = htons(9999)};
    inet_aton("127.0.0.1", &addr.sin_addr);
    connect(s, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
    auto response = pktbuilder::DNS::Message(0, pktbuilder::DNS::MessageType::RESPONSE,
        0, pktbuilder::DNS::Flag::RA | pktbuilder::DNS::Flag::AA, 
        pktbuilder::DNS::ResponseCode::OK, {}, 
        { 
            pktbuilder::DNS::ResourceRecord{
                .domain_name = "myhost.mydomain.com",
                .rrtype = pktbuilder::DNS::Type::A,
                .rrclass = pktbuilder::DNS::Class::IN,
                .ttl = 30, 
                .rdata = {192, 168, 2, 10}
            },
            pktbuilder::DNS::ResourceRecord{
                .domain_name = "myhost.mydomain.com",
                .rrtype = pktbuilder::DNS::Type::CNAME,
                .rrclass = pktbuilder::DNS::Class::IN,
                .ttl = 30,
                .rdata = pktbuilder::DNS::formatDomainName("myhost.cdn.mydomain.com")
            } 
        }, {}, {});
    std::vector<uint8_t> data = response.build();
    send(s, data.data(), data.size(), 0);
    return 0;
}