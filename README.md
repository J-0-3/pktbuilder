# PKTBuilder
## A simple, intuitive C++ packet crafting/decoding library.

PKTBuilder makes it easy to construct and decode network packets at any layer of
the network stack using a simple, clean syntax.

It can construct and decode packets from an ever increasing number of network protocols, 
from the application layer to the link layer. 

It uses sane default values, and lower level protocols are able to infer 
values from the higher level protocols contained within them. For example,
an IPv4 packet will automatically set its protocol number to the correct value
when a Layer 4 protocol is added on top. 

PKTBuilder supports both Linux and Windows.

## Currently Implemented Protocols

PKTBuilder currently has implementations of:
- Ethernet
- IPv4
- TCP
- UDP (no decoding yet)
- ICMP (no decoding yet)
- ARP (no decoding yet)
- DNS (no decoding yet)

## Roadmap

Protcols which will be implemented in the future include:
- DHCP
- HTTP
- FTP
- IPv6
- AX.25

Further features I aim to add include:

- Packet decoding for all protocols
- Sending packets at various layers
- Utility functions for certain protocols (such as performing the TCP 3-way handshake)

## Installation

While precompiled static binaries are available, it is recommended that you 
compile PKTBuilder from source using CMake. PKTBuilder requires a compiler 
with support for C++20 (you can check which compilers are supported 
[here](https://en.cppreference.com/w/cpp/compiler_support/20)).

To build PKTBuilder you will also need CMake 3.22 or greater.

### Compiling PKTBuilder

To compile PKTBuilder, first clone the repository with:

```bash
git clone https://github.com/joedthomas2005/pktbuilder
```

PKTBuilder can be built using either the command line (recommended)
or various IDEs.

I have tested the following IDEs, and confirmed that they should 
automatically detect the build configuration: 

- Visual Studio 2022
- JetBrains CLion*
- Visual Studio Code with CMake Tools (**Linux only**).

\*In order to use JetBrains CLion, it is recommended that you follow these steps:

- Delete the automatically created `Debug` or `Release` profile, and select instead the `windows-release` or `linux-release` profile
(otherwise CLion will build it twice).
- Set your default toolchain (`Settings -> Build, Execution, Deployment -> Toolchains`) to `Visual Studio` instead of `MinGW`.

---

#### Building with the Command Line

If you are on Windows, make sure these commands are run from the `Developer Command Prompt for VS`.

Move into the `pktbuilder` directory with:

```
cd pktbuilder
```

Configure CMake with:

Windows:

```
cmake --preset windows-release
```

Linux:

```bash
cmake --preset linux-release
```

Build PKTBuilder with:

Windows:

```
cmake --build --preset windows-release
```

Linux:

```
cmake --build --preset linux-release
```

Inside the `build/linux-release` or `build/windows-release` folder
the compiled library will be called `libpktbuilder.a`/`libpktbuilder.so`
if you are using Linux or `pktbuilder.lib`/`pktbuilder.dll` if you are 
using Windows.

### Linking to PKTBuilder

To use PKTBuilder in your program, first this library file must be placed in a 
directory where your compiler can link to it. This could either be in the system-wide
default library path (usually `/usr/lib` on Linux) or a local directory for your
program (recommended). 

If the library is not placed in your default system library path, you must add 
the directory containing it to your compiler's library path. If you are using
g++, add `-L<PATH TO FOLDER CONTAINING LIBRARY>` to your compiler command (e.g. 
if library was stored in your project directory under `lib/pktbuilder`
then you would add `-L./lib/pktbuilder` to your g++ command). If your project
is using CMake then add a line to your `CMakeLists.txt` which adds the directory
to your linker path (`link_directories(<PATH TO FOLDER> <OTHER LIBRARY PATHS>...)`).

In order to use the library you will need to link to it at compile time. In order
to do this using g++ you should add `-lpktbuilder` to your compiler command. To 
link to the library using CMake you should add a line to your `CMakeLists.txt`
add it to the libraries for your CMake target (`target_link_libraries(<YOUR TARGET>
pktbuilder <OTHER LIBRARIES...>)`).

You will also need the PKTBuilder headers found in this repository. Simply copy 
the entire contents of this repository's `include` directory and add it to 
somewhere in your include path. This could either be the system-wide include path
(usually `/usr/include` on Linux), or your program's local include path.

Make sure the directory you have placed the headers in is added to your 
compiler's include path. If your compiler is g++, add `-I<PATH TO HEADERS>` to
your compiler command (e.g. if the headers were stored in your project directory
under `lib/pktbuilder/include` then you would add `-I./lib/pktbuilder/include` to 
your compile command), If your project is using CMake then add a line to your 
`CMakeLists.txt` which adds the directory to your include path (
`include_directories(<PATH TO HEADERS> <OTHER INCLUDE DIRECTORIES...>)`).

In order to build a project using PKTBuilder you **must** enable C\+\+20 support.
To do this with g\+\+, add `-std=c++20` to your compile command. If you 
are using CMake this can be done by adding the following line to your 
`CMakeLists.txt`: 
```
set(CMAKE_CXX_STANDARD 20)
```

In order to use pktbuilder in your project, simply `#include <pktbuilder.h>`.

## Getting Started

### Building Packets

PKTBuilder uses a simple syntax to represent network packets, in which layers of 
protocols are separated using the `|` character in descending order (i.e. the 
first protocol will be contained within the next).

This allows protocol stacks to be represented visually with the higher level
protocols at the top and the lower level protocols at the bottom like so:

```c++
auto packet = std::string("hello world") |
              pktbuilder::UDP::Datagram(8080) |
              pktbuilder::IPv4::Packet({192, 168, 1, 1}) |
              pktbuilder::Ethernet::Frame({0x01, 0x23, 0x45, 0x67, 0x89, 0xab});
```

In this example, it can be seen that the "application" data (the string 
"hello world") is at the top, and is then encapsulated within a UDP Datagram 
with destination port `8080` (the UDP source port is optional),
which is encapsulated within an IPv4 packet with 
destination address `192.168.1.1` which is then itself contained within an 
Ethernet frame with destination address `01:23:45:67:89:ab`.

Notice also that the source hardware address for the ethernet frame and the 
source IP address for the IPv4 packet were not required. This is because
pktbuilder is able to automatically fill them based on your device's MAC
and IP address. 

The UDP datagram would also usually require the source and destination IP 
addresses in order to calculate the checksum, but these are set automatically
by the IPv4 packet below it. 

Furthermore, an Ethernet frame is required to contain an `ethertype` field which 
indicates which layer 3 protocol is encapsulated within it and IPv4 packet is 
required to contain a `protocol number` field which indicates the layer 4 protocol.
These also did not need to be supplied as they are automatically filled by the 
upper level protocols (IPv4 and UDP respectively). 

PKTBuilder does not currently handle sending packets, and so to send the packet 
you must first call the `build()` method on it to get an `std::vector` containing 
the raw packet bytes:

```c++
std::vector<uint8_t> data = packet.build();
```

This can then be sent using whatever means you wish, for example, here I use the 
`pcap` library to send the raw ethernet frame directly from the `eth0` network
interface.

```c++
    char errbuf[PCAP_BUF_SIZE];
    pcap_t* handle = pcap_open_live("eth0", BUFSIZ, 1, 1000, errbuf);
    pcap_sendpacket(handle, data.data(), data.size());
```

If you were not using such low level protocols, you could also use a BSD 
socket, for example:

```c++
auto packet = std::array<uint8_t, 4>({0xab, 0xff, 0xde, 0x1}) |
              pktbuilder::TCP::Packet(80, {192, 168, 1, 1}, 200, 4000000000,
                  pktbuilder::TCP::Flag::PSH | pktbuilder::TCP::Flag::RST, 1024, {});

std::vector<uint8_t> data = packet.build();

int s = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
struct sockaddr_in addr = { 0 };
addr.sin_family = AF_INET;
inet_aton("192.168.1.1", &addr.sin_addr);
connect(s, (sockaddr*)&addr, sizeof(addr));
send(s, data.data(), data.size(), 0);
```

This example constructs a TCP packet from port 200 to port 80 (the
destination IP address does need to be specified this time as there is no
IPv4 packet to supply it. The source address can be set automatically, however)
 containing the payload `ab ff de 01`  with a sequence number of `4000000000`, 
with the `PSH` and `RST` flags set, a window size of `1024` and no TCP options.
The TCP packet is then sent using a raw layer 3 IPv4 socket to `192.168.1.1`.

PKTBuilder is extremely flexible, and allows for effectively any combination of
protocols in any order to suit any use case. Any protocol which accepts a payload
can have any other protocol contained within it. There are certain protocols which
do not accept a payload, such as `ARP`, and these can only ever be the very top
protocol in the stack, but aside from this limitation, any protocol can be added 
to any other protocol in any order. 

Want an AppleTalk ethertype Ethernet frame contained in an ICMP echo request in
a UDP datagram?

```c++
auto packet = pktbuilder::EthernetFrame({0xab, 0xcd, 0xef, 0x12, 0x34, 0x56},
                                        pktbuilder::Ethernet::EtherType::AppleTalk,
                                        {0xde, 0xad, 0xbe, 0xef, 0, 0}) |
                                        pktbuilder::ICMP::Packet(8, 0, {0, 0, 0, 0}) |
                                        pktbuilder::UDP::Datagram(1024, 800);
```

This flexibility allows any part of the network stack to be switched out for 
any other without affecting the higher layers. 
DNS over UDP and DNS over TCP are only 1 line of code different:

 ```c++
auto dns = pktbuilder::DNS::Message(
                1, 
                pktbuilder::DNS::MessageType::QUERY, 
                pktbuilder::DNS::Opcode::QUERY, 
                pktbuilder::DNS::Flag::RD,
                0, 
                {
                    pktbuilder::DNS::Question{
                        .domain_name = "google.com",
                        .qtype = pktbuilder::DNS::Type::A,
                        .qclass = pktbuilder::DNS::Class::IN 
                    },
                }, 
                {},
                {}, 
                {}) | 
            pktbuilder::UDP::Datagram(53);
```

```c++
auto dns = pktbuilder::DNS::Message(
                1, 
                pktbuilder::DNS::MessageType::QUERY, 
                pktbuilder::DNS::Opcode::QUERY, 
                pktbuilder::DNS::Flag::RD,
                0, 
                {
                    pktbuilder::DNS::Question{
                        .domain_name = "google.com",
                        .qtype = pktbuilder::DNS::Type::A,
                        .qclass = pktbuilder::DNS::Class::IN 
                    },
                }, 
                {},
                {}, 
                {}) | 
            pktbuilder::TCP::Packet(53, 50000, pktbuilder::TCP::Flag::PSH)
```

### Decoding Packets

Layer classes have a static method called `decodeFrom`, which can be used to 
construct an instance of that class from raw packet bytes. For example, this 
unit test (from [here](tests/unit/decoding/ethernetFrame.cpp)) decodes the bytes
of an ethernet frame which was captured in wireshark and compares the header 
field values with those identified by wireshark:

```c++
int main() {
    std::vector<uint8_t> frameData({
        0xaa,0xbb,0xcc,0xdd,0xee,0xff,0xfe,0xdc,0xba,0x09,0x87,0x65,0x08,0x00,0x45,0x00,
        0x00,0x54,0xdf,0x7e,0x40,0x00,0x40,0x01,0xf4,0x9f,0xc0,0xa8,0x01,0x59,0xd8,0x3a,
        0xcc,0x4e,0x08,0x00,0x8d,0xaa,0x00,0x01,0x00,0x01,0xa0,0xaf,0xc2,0x64,0x00,0x00,
        0x00,0x00,0x47,0x6c,0x01,0x00,0x00,0x00,0x00,0x00,0x10,0x11,0x12,0x13,0x14,0x15,
        0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,0x20,0x21,0x22,0x23,0x24,0x25,
        0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,0x30,0x31,0x32,0x33,0x34,0x35,
        0x36,0x37
    });
    pktbuilder::Ethernet::Frame frame = pktbuilder::Ethernet::Frame::decodeFrom(frameData);
    assert(frame.getDestinationMac() == pktbuilder::mac_addr_t({0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff}));
    assert(frame.getSourceMac() == pktbuilder::mac_addr_t({0xfe,0xdc,0xba,0x09,0x87,0x65}));
    assert(frame.getEthertype() == pktbuilder::Ethernet::EtherType::IPv4);
    assert(frame.getPayload() == std::vector<uint8_t>({0x45,0x00,0x00,0x54,0xdf,0x7e,0x40,0x00,0x40, 
        0x01,0xf4,0x9f,0xc0,0xa8,0x01,0x59,0xd8,0x3a,0xcc,0x4e,0x08,0x00,0x8d, 
        0xaa,0x00,0x01,0x00,0x01,0xa0,0xaf,0xc2,0x64,0x00,0x00,0x00,0x00,0x47,
        0x6c,0x01,0x00,0x00,0x00,0x00,0x00,0x10,0x11,0x12,0x13,0x14,0x15,0x16,
        0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,0x20,0x21,0x22,0x23,0x24,
        0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,0x30,0x31,0x32,
        0x33,0x34,0x35,0x36,0x37}));
}
```

As you can see, all the ethernet header data is parsed and stored in the 
Frame object, with the rest of the data being stored in the payload field.

The data stored in the payload field could then itself be parsed as an IPv4
packet, as it is in this other unit test (from [here](tests/unit/decoding/ipv4Packet.cpp)):

```c++
using namespace pktbuilder::IPv4;
int main() {
    std::vector<uint8_t> data({
        0x45,0x00,0x00,0x54,0xdf,0x7e,0x40,0x00,0x40,0x01,0xf4,0x9f,0xc0,0xa8,
        0x01,0x59,0xd8,0x3a,0xcc,0x4e,0x08,0x00,0x8d,0xaa,0x00,0x01,0x00,0x01,
        0xa0,0xaf,0xc2,0x64,0x00,0x00,0x00,0x00,0x47,0x6c,0x01,0x00,0x00,0x00,
        0x00,0x00,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,
        0x1c,0x1d,0x1e,0x1f,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,
        0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37
    });
    Packet packet = Packet::decodeFrom(data);
    assert(packet.getDestinationAddress() == ipv4_addr_t({216, 58, 204, 78}));
    assert(packet.getSourceAddress() == ipv4_addr_t({192, 168, 1, 89}));
    assert(packet.getProtocolNumber() == ProtocolNumber::ICMP);
    assert(packet.getTTL() == 64);
    assert(packet.getIdentification() == 0xdf7e);
    assert(packet.isFlagDontFragment());
    assert(!packet.isFlagMoreFragments());
    assert(packet.getECN() == ECNCodePoint::NOT_ECT);
    assert(packet.getDSCP() == 0);
    assert(packet.getFragmentOffset() == 0);
    assert(packet.getOptions().empty());
    assert(packet.getPayload() == std::vector<uint8_t>({
        0x08,0x00,0x8d,0xaa,0x00,0x01,0x00,0x01,0xa0,0xaf,0xc2,0x64,0x00,0x00,
        0x00,0x00,0x47,0x6c,0x01,0x00,0x00,0x00,0x00,0x00,0x10,0x11,0x12,0x13,
        0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,0x20,0x21,
        0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,
        0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37
    }));
```

Once again, the IPv4 header is parsed and all relevant data is extracted, with
the rest of the packet (an ICMP echo request to google.com) being placed in
the payload field.
