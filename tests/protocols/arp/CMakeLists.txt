add_executable(arp_reply arp_reply.cpp)
add_executable(arp_request arp_request.cpp)
target_link_libraries(arp_reply pcap)
target_link_libraries(arp_request pcap)