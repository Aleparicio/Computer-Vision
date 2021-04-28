cmake_minimum_required(VERSION 3.13.0)
project(DisplayImage)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(detectors)

if (HAVE_XFEATURES2D_NONFREE_H)
    add_definitions(-DHAVE_XFEATURES2D_NONFREE_H)
endif()

add_executable(panorama main.cpp Ransac.cpp Panorama.cpp)
add_executable(chessboard Chessboard.cpp Ransac.cpp)
target_link_libraries( panorama detectors_lib ${OpenCV_LIBS})
target_link_libraries( chessboard detectors_lib ${OpenCV_LIBS})
