cmake_minimum_required(VERSION 2.8.14)
project( DisplayImage )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "-O3")

set(SOURCES canny_operator.cpp vanishing_points.cpp ransac.cpp)
add_executable( Edges Edges.cpp ${SOURCES})
target_link_libraries( Edges ${OpenCV_LIBS} )

set(SOURCES canny_operator.cpp vanishing_points.cpp ransac.cpp)
add_executable( kernel_test kernel_test.cpp ${SOURCES})
target_link_libraries( kernel_test ${OpenCV_LIBS} )
