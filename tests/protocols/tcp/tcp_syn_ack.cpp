#include <pktbuilder.h>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace pktbuilder;
using namespace TCP;
int main() {
    auto packet = Packet(6969, {127, 0, 0, 1}, 420, Flag::SYN | Flag::ACK, {127, 0, 0, 1});
    std::vector<uint8_t> data = packet.build();
    int s = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
    sockaddr_in addr{};
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    connect(s, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
    send(s, data.data(), data.size(), 0);
}