cmake_minimum_required(VERSION 3.22)
include(CTest)
link_libraries(pktbuilder)
add_executable(formatDomainName formatDomainName.cpp)
add_test(NAME formatDomainName COMMAND formatDomainName)

add_executable(intToIpv4Addr intToIpv4Addr.cpp)
add_test(NAME intToIpv4Addr COMMAND intToIpv4Addr)

add_executable(ipv4AddrToInt ipv4AddrToInt.cpp)
add_test(NAME ipv4AddrToInt COMMAND ipv4AddrToInt)

add_executable(ipv4AddrToStr ipv4AddrToStr.cpp)
add_test(NAME ipv4AddrToStr COMMAND ipv4AddrToStr)

add_executable(strToIpv4Addr strToIpv4Addr.cpp)
add_test(NAME strToIpv4Addr COMMAND strToIpv4Addr)

add_executable(macAddrToStr macAddrToStr.cpp)
add_test(NAME macAddrToStr COMMAND macAddrToStr)

add_executable(strToMacAddr strToMacAddr.cpp)
add_test(NAME strToMacAddr COMMAND strToMacAddr)

add_executable(getDefaultInterfaceIPv4Address getDefaultInterfaceIPv4Address.cpp)
add_test(NAME getDefaultInterfaceIPv4Address COMMAND getDefaultInterfaceIPv4Address)

add_executable(getDefaultInterfaceMAC getDefaultInterfaceMAC.cpp)
add_test(NAME getDefaultInterfaceMAC COMMAND getDefaultInterfaceMAC)

add_executable(calculateInternetChecksum calculateInternetChecksum.cpp)
add_test(NAME calculateInternetChecksum COMMAND calculateInternetChecksum)