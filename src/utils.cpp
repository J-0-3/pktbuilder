#include <pktbuilder/utils.h>
#ifdef WIN32
#include <Windows.h>
#include <iphlpapi.h>
#else
#include <memory>
#include <unistd.h>
#include <fstream>
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
        const ipv4_addr_t default_routing_interface = getDefaultInterfaceIPv4Address();
        if (default_routing_interface == ipv4_addr_t({ 0, 0, 0, 0 })) {
            return { 0, 0, 0, 0, 0, 0 };
        }
        const std::string default_ip = ipv4AddrToStr(default_routing_interface);
        auto adapter_list = static_cast<PIP_ADAPTER_INFO>(malloc(sizeof(IP_ADAPTER_INFO)));
        if (adapter_list == nullptr) {
            return { 0, 0, 0, 0, 0, 0 };
        }
        ZeroMemory(adapter_list, sizeof(IP_ADAPTER_INFO));
        ULONG required_size = 0;
        if (GetAdaptersInfo(adapter_list, &required_size) != ERROR_SUCCESS) {
            free(adapter_list);
            adapter_list = static_cast<PIP_ADAPTER_INFO>(malloc(required_size));
            if (adapter_list == nullptr) {
                return { 0, 0, 0, 0, 0, 0 };
            }
            ZeroMemory(adapter_list, required_size);
            if (GetAdaptersInfo(adapter_list, &required_size) != ERROR_SUCCESS) {
                free(adapter_list);
                return { 0, 0, 0, 0, 0, 0 };
            }
        }
        PIP_ADAPTER_INFO cur_adapter = adapter_list;
        PIP_ADAPTER_INFO default_adapter = nullptr;
        while (cur_adapter) {
            PIP_ADDR_STRING cur_ip = &(cur_adapter->IpAddressList);
            while (cur_ip) {
                if (std::string(cur_ip->IpAddress.String) == default_ip) {
                    default_adapter = cur_adapter;
                    break;
                }
                cur_ip = cur_ip->Next;
            }
            if (default_adapter) {
                break;
            }
            cur_adapter = cur_adapter->Next;
        }
        if (!default_adapter) {
            free(adapter_list);
            return { 0, 0, 0, 0, 0, 0 };
        }
        if (default_adapter->AddressLength != 6) {
            free(adapter_list);
            return { 0, 0, 0, 0, 0, 0 };
        }
        mac_addr_t default_mac({ 0, 0, 0, 0 ,0 ,0 });
        std::memcpy(default_mac.data(), default_adapter->Address, 6);
        free(adapter_list);
        return default_mac;

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
        // Why is Windows like this?
        std::string cmd = R"(C:\Windows\System32\route.exe print 0.0.0.0 -4)";
        HANDLE stdout_p_read = nullptr;
        HANDLE stdout_p_write = nullptr;
        HANDLE stdin_p_read = nullptr;
        HANDLE stdin_p_write = nullptr;
        SECURITY_ATTRIBUTES sec_attrib;
        sec_attrib.bInheritHandle = true;
        sec_attrib.nLength = sizeof(SECURITY_ATTRIBUTES);
        sec_attrib.lpSecurityDescriptor = nullptr;
        CreatePipe(&stdout_p_read, &stdout_p_write, &sec_attrib, 0);
        SetHandleInformation(stdout_p_read, HANDLE_FLAG_INHERIT, 0);
        CreatePipe(&stdin_p_read, &stdin_p_write, &sec_attrib, 0);
        SetHandleInformation(stdin_p_write, HANDLE_FLAG_INHERIT, 0);
        STARTUPINFO startup_info = {};
        startup_info.cb = sizeof(STARTUPINFO);
        startup_info.hStdOutput = stdout_p_write;
        startup_info.hStdInput = stdin_p_read;
        startup_info.hStdError = stdout_p_write;
        startup_info.dwFlags |= STARTF_USESTDHANDLES;
        PROCESS_INFORMATION pInfo = {};
        if (!CreateProcess(nullptr, cmd.data(), 
            nullptr, nullptr, true, 
            0, nullptr, nullptr, 
            &startup_info, &pInfo)) {
            CloseHandle(pInfo.hProcess);
            CloseHandle(pInfo.hThread);
            return { 0, 0, 0, 0 };
        }
        CloseHandle(pInfo.hProcess);
        CloseHandle(pInfo.hThread);
        char buf[4097];
        ZeroMemory(&buf, 4097);
        DWORD n_read = 0;
        if (!ReadFile(stdout_p_read, buf, 4096, &n_read, nullptr) || n_read == 0) {
            return { 0, 0, 0, 0 };
        }
        std::string cmd_output(buf);
        const size_t dest_begin = cmd_output.find("0.0.0.0");
        const size_t dest_end = cmd_output.find(' ', dest_begin + 1);
        const size_t netmask_begin = cmd_output.find("0.0.0.0", dest_end + 1);
        const size_t netmask_end = cmd_output.find(' ', netmask_begin + 1);
        const size_t gateway_begin = cmd_output.find_first_not_of(' ', netmask_end + 1);
        const size_t gateway_end = cmd_output.find(' ', gateway_begin + 1);
        const size_t interface_begin = cmd_output.find_first_not_of(' ', gateway_end + 1);
        const size_t interface_end = cmd_output.find(' ', interface_begin);
        const std::string if_ip_str = cmd_output.substr(interface_begin, interface_end - interface_begin);
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