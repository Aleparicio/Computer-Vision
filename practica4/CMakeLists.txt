cmake_minimum_required(VERSION 2.8.14)
project( DisplayImage )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

set (CMAKE_CXX_STANDARD 11)

set(SRCS_MODULES
   AKAZEPair.cpp
   SIFTPair.cpp
)

add_library(SOURCES_CODE SHARED ${SRCS_MODULES})
add_executable( Keypoints Keypoints.cpp )
target_link_libraries( Keypoints ${OpenCV_LIBS} SOURCES_CODE)
