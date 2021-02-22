cmake_minimum_required(VERSION 2.8.14)
project( DisplayImage )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

set (CMAKE_CXX_STANDARD 11)

add_executable( CameraBlur CameraBlur.cpp )
target_link_libraries( CameraBlur ${OpenCV_LIBS} )

add_executable( CameraBlur2 CameraBlur2.cpp )
target_link_libraries( CameraBlur2 ${OpenCV_LIBS} )
