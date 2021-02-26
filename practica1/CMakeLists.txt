cmake_minimum_required(VERSION 2.8.14)
project( DisplayImage )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

set (CMAKE_CXX_STANDARD 17)

set(SOURCES App.cpp Filters.cpp)
add_executable( App ${SOURCES})
target_link_libraries( App ${OpenCV_LIBS} )
