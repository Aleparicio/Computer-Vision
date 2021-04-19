cmake_minimum_required(VERSION 2.8.14)
project(DisplayImage)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(detectors)

add_executable(Keypoints Keypoints.cpp)
target_link_libraries( Keypoints detectors_lib ${OpenCV_LIBS})
