#include <pktbuilder/utils.h>
#include <cstdio>
#include <fstream>
#include <memory>
#ifndef WIN32
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <arpa/inet.h>
#endif

namespace pktbuilder {
    std::string getDefaultInterfaceName() {
#ifdef WIN32
        return ""
#else
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("/usr/bin/ip route list", "r"), pclose);
        if (!pipe) {
            return "";
        }
        std::array<char, 512> buf{};
        if (fgets(buf.data(), buf.size(), pipe.get()) == nullptr) {
            return "";
        }
        std::string out_line(buf.cbegin(), buf.cend());
        size_t dev_name_start = out_line.find("dev") + 4;
        size_t dev_name_end = out_line.find(' ', dev_name_start);
        std::string device_name = out_line.substr(dev_name_start, dev_name_end - dev_name_start);
        return device_name;
#endif
    }
    mac_addr_t getInterfaceMAC(std::string const& interface) {
#ifdef WIN32
        return { 0, 0, 0, 0, 0, 0 };
#else
        std::ifstream fstream("/sys/class/net/" + interface + "/address");
        std::string mac{};
        std::getline(fstream, mac);
        return str_to_mac_addr(mac);
#endif
    }
    ipv4_addr_t getInterfaceIPv4Address(std::string const& interface) {
#ifdef WIN32
        return { 0, 0, 0, 0 }
#else
        ifreq req = { 0 };
        if (interface.length() >= IFNAMSIZ) {
            return { 0, 0, 0, 0 };
        }
        std::strcpy(req.ifr_ifrn.ifrn_name, interface.c_str());
        int s = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
        int err = ioctl(s, SIOCGIFADDR, &req);
        if (err != 0) {
            return { 0, 0, 0 ,0 };
        }
        auto addr = std::bit_cast<struct sockaddr_in>(req.ifr_ifru.ifru_addr);
        std::string addr_str(inet_ntoa(addr.sin_addr));
        return str_to_ipv4_addr(addr_str);
#endif
    }
}