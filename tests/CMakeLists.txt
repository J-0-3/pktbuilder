cmake_minimum_required(VERSION 3.22)
add_subdirectory(unit)

if (!WIN32)
    add_subdirectory(protocols)
endif()