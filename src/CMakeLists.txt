cmake_minimum_required(VERSION 3.22)
project(pktbuilder)
add_library(pktbuilder Ethernet.cpp DNS.cpp addresses.cpp Layer.cpp IPv4.cpp TCP.cpp UDP.cpp ICMP.cpp checksum.cpp utils.cpp pktbuilder.cpp)
