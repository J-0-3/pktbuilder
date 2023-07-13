cmake_minimum_required(VERSION 3.22)
link_libraries(pktbuilder)
add_executable(dnsQuery dns/query.cpp)
add_executable(dnsResponse dns/response.cpp)
