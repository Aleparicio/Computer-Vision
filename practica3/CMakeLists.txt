cmake_minimum_required(VERSION 2.8.14)
project( DisplayImage )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "-O3")

set(SOURCES descriptors.cpp mahalanobis_classifier.cpp)
add_executable( main main.cpp ${SOURCES})
target_link_libraries( main ${OpenCV_LIBS} )

add_executable( aprender aprender.cpp ${SOURCES})
target_link_libraries( aprender ${OpenCV_LIBS} )

add_executable( reconocer reconocer.cpp ${SOURCES})
target_link_libraries( reconocer ${OpenCV_LIBS} )
