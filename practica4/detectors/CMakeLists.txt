cmake_minimum_required(VERSION 3.13.0)

include(CheckIncludeFileCXX)
set(CMAKE_REQUIRED_LIBRARIES ${OpenCV_LIBS})
# set(CMAKE_REQUIRED_INCLUDES ${OpenCV_INCLUDE_DIRS})
CHECK_INCLUDE_FILE_CXX(opencv2/xfeatures2d/nonfree.hpp HAVE_XFEATURES2D_NONFREE_H)

add_library(detectors_lib Pair.cpp
                          HARRISPair.cpp
                          ORBPair.cpp
                          SIFTPair.cpp
                          AKAZEPair.cpp)

if (HAVE_XFEATURES2D_NONFREE_H)
    target_sources(detectors_lib PUBLIC SURFPair.cpp)
endif()

target_include_directories(detectors_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
