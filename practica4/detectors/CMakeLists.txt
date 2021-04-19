cmake_minimum_required(VERSION 2.8.14)

add_library(detectors_lib Pair.cpp
                          HARRISPair.cpp
                          ORBPair.cpp
                          SIFTPair.cpp
                          SURFPair.cpp
                          AKAZEPair.cpp)

target_include_directories(detectors_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
