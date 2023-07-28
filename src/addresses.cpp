#include <pktbuilder/addresses.h>
#include <stdexcept>
#include <string>
#include <sstream>
#include <iomanip>

namespace pktbuilder {
    ipv4_addr_t strToIpv4Addr(std::string const& ip_addr) {
        size_t prev_pos = -1;
        ipv4_addr_t addr = { 0, 0, 0, 0 };
        for (auto i = 0; i < 4; i++) {
            size_t period_pos = ip_addr.find('.', prev_pos + 1);
            if (std::string::npos == period_pos) {
                if (3 == i) {
                    period_pos = ip_addr.length();
                } else {
                    throw std::invalid_argument("not a valid IPv4 address");
                }
            }
            const size_t octet_char_length = period_pos - prev_pos - 1;
            std::string octet = ip_addr.substr(prev_pos + 1, octet_char_length);
            if (octet.length() < 1 || octet.length() > 3) {
                throw std::invalid_argument("not a valid IPv4 address");
            }
            int octet_int;
            try {
                octet_int = std::stoi(octet);
            } catch (std::invalid_argument&) {
                throw std::invalid_argument("not a valid IPv4 address");
            }
            if (octet_int < 0 || octet_int > 255) {
                throw std::invalid_argument("not a valid IPv4 address");
            }
            addr[i] = static_cast<uint8_t>(octet_int);
            prev_pos = period_pos;
        }
        return addr;
    }

    std::string ipv4AddrToStr(ipv4_addr_t const& ip_addr) {
        std::stringstream str;
        str << static_cast<unsigned int>(ip_addr[0]) << "."
            << static_cast<unsigned int>(ip_addr[1]) << "."
            << static_cast<unsigned int>(ip_addr[2]) << "."
            << static_cast<unsigned int>(ip_addr[3]);
        return str.str();
    }

    uint32_t ipv4AddrToInt(ipv4_addr_t const& ip_addr) {
        return (
                static_cast<uint32_t>(ip_addr[0]) << 24 |
                static_cast<uint32_t>(ip_addr[1]) << 16 |
                static_cast<uint32_t>(ip_addr[2]) << 8  |
                static_cast<uint32_t>(ip_addr[3])
        );
    }

    ipv4_addr_t intToIpv4Addr(uint32_t ip_addr) {
        return ipv4_addr_t({
            static_cast<uint8_t>((ip_addr & 0xff000000) >> 24),
            static_cast<uint8_t>((ip_addr & 0x00ff0000) >> 16),
            static_cast<uint8_t>((ip_addr & 0x0000ff00) >> 8),
            static_cast<uint8_t>((ip_addr & 0x000000ff))
        });
    }

    mac_addr_t strToMacAddr(std::string const& mac_addr) {
        mac_addr_t addr = { 0, 0, 0, 0, 0, 0 };
        size_t prev_pos = -1;
        for (auto i = 0; i < 6; i++) {
            size_t colon_pos = mac_addr.find(':', prev_pos + 1);
            if (std::string::npos == colon_pos) {
                if (5 == i) {
                    colon_pos = mac_addr.length();
                } else {
                    throw std::invalid_argument("not a valid MAC address");
                }
            }
            const size_t octet_char_length = colon_pos - prev_pos - 1;
            std::string octet = mac_addr.substr(prev_pos + 1, octet_char_length);
            if (octet.length() != 2) {
                throw std::invalid_argument("not a valid MAC address");
            }
            uint8_t octet_val;
            try {
                octet_val = static_cast<uint8_t>(std::stoi(octet, nullptr, 16));
            } catch (std::invalid_argument&) {
                throw std::invalid_argument("not a valid MAC address");
            }
            addr[i] = octet_val;
            prev_pos = colon_pos;
        }
        return addr;
    }

    std::string macAddrToStr(mac_addr_t const& mac_addr) {
        std::stringstream str;
        str << std::setfill('0') << std::setw(2) << std::right << std::hex
            << static_cast<unsigned int>(mac_addr[0]);

        for(auto i = 1; i < 6; i++) {
            str << ":";
            str << std::setfill('0') << std::setw(2) << std::right << std::hex
                << static_cast<unsigned int>(mac_addr[i]);
        }
        return str.str();
    }
}