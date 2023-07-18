cmake_minimum_required(VERSION 3.22)
link_libraries(pktbuilder)
add_executable(query query.cpp)
add_executable(response response.cpp)