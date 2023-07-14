#include <pktbuilder/utils.h>
#include <cstdio>
#include <fstream>
#include <memory>
#include <iostream>
#ifdef WIN32
#include <Windows.h>
#else
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <arpa/inet.h>
#endif

#ifndef WIN32
namespace {
    std::string getDefaultInterfaceName() {
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
    }
}
#endif
namespace pktbuilder {

    mac_addr_t getDefaultInterfaceMAC() {
#ifdef WIN32
        return { 0, 0, 0, 0, 0, 0 };
#else
        std::string inter = getDefaultInterfaceName();
        std::ifstream fstream("/sys/class/net/" + inter + "/address");
        std::string mac{};
        std::getline(fstream, mac);
        return strToMacAddr(mac);
#endif
    }
    ipv4_addr_t getDefaultInterfaceIPv4Address() {
#ifdef WIN32
        std::string cmd = "C:\\Windows\\System32\\route.exe print 0.0.0.0 -4";
        HANDLE stdout_p_read = NULL;
        HANDLE stdout_p_write = NULL;
        HANDLE stdin_p_read = NULL;
        HANDLE stdin_p_write = NULL;
        SECURITY_ATTRIBUTES sec_attrib = { 0 };
        sec_attrib.bInheritHandle = true;
        sec_attrib.nLength = sizeof(SECURITY_ATTRIBUTES);
        sec_attrib.lpSecurityDescriptor = NULL;
        CreatePipe(&stdout_p_read, &stdout_p_write, &sec_attrib, 0);
        SetHandleInformation(stdout_p_read, HANDLE_FLAG_INHERIT, 0);
        CreatePipe(&stdin_p_read, &stdin_p_write, &sec_attrib, 0);
        SetHandleInformation(stdin_p_write, HANDLE_FLAG_INHERIT, 0);
        STARTUPINFO startup_info = { 0 };
        startup_info.cb = sizeof(STARTUPINFO);
        startup_info.hStdOutput = stdout_p_write;
        startup_info.hStdInput = stdin_p_read;
        startup_info.hStdError = stdout_p_write;
        startup_info.dwFlags |= STARTF_USESTDHANDLES;
        PROCESS_INFORMATION pInfo = { 0 };
        CreateProcess(NULL, cmd.data(), NULL, NULL, true, 0, NULL, NULL, &startup_info, &pInfo);
        char buf[4096];
        DWORD n_read = 0;
        BOOL success = ReadFile(stdout_p_read, buf, 4096, &n_read, NULL);
        std::string cmd_output(buf);
        size_t dest_begin = cmd_output.find("0.0.0.0");
        size_t dest_end = cmd_output.find(' ', dest_begin + 1);
        size_t netmask_begin = cmd_output.find("0.0.0.0", dest_end + 1);
        size_t netmask_end = cmd_output.find(' ', netmask_begin + 1);
        size_t gateway_begin = cmd_output.find_first_not_of(' ', netmask_end + 1);
        size_t gateway_end = cmd_output.find(' ', gateway_begin + 1);
        size_t interface_begin = cmd_output.find_first_not_of(' ', gateway_end + 1);
        size_t interface_end = cmd_output.find(' ', interface_begin);
        std::string if_ip_str = cmd_output.substr(interface_begin, interface_end - interface_begin);
        return strToIpv4Addr(if_ip_str);
#else
        std::string inter = getDefaultInterfaceName();
        ifreq req = { 0 };
        if (inter.length() >= IFNAMSIZ) {
            return { 0, 0, 0, 0 };
        }
        std::strcpy(req.ifr_ifrn.ifrn_name, inter.c_str());
        int s = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
        int err = ioctl(s, SIOCGIFADDR, &req);
        if (err != 0) {
            return { 0, 0, 0 ,0 };
        }
        auto addr = std::bit_cast<struct sockaddr_in>(req.ifr_ifru.ifru_addr);
        std::string addr_str(inet_ntoa(addr.sin_addr));
        return strToIpv4Addr(addr_str);
#endif
    }
}