#include <pktbuilder.h>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace pktbuilder::TCP;

int main(int argc, char** argv) {
    std::string payload("TCP Packet Test Data");
    if (argc > 1) {
        payload = argv[1];
    }
    auto packet = payload | Packet(420, {127, 0, 0, 1}, 6969, Flag::PSH, {127, 0, 0, 1});
    std::vector<uint8_t> data = packet.build();
    int s = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
    sockaddr_in addr{};
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    connect(s, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
    send(s, data.data(), data.size(), 0);
}