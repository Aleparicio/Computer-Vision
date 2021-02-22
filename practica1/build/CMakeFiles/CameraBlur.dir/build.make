# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/alejandro/Desktop/Vision/practica1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/alejandro/Desktop/Vision/practica1/build

# Include any dependencies generated for this target.
include CMakeFiles/CameraBlur.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/CameraBlur.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/CameraBlur.dir/flags.make

CMakeFiles/CameraBlur.dir/CameraBlur.cpp.o: CMakeFiles/CameraBlur.dir/flags.make
CMakeFiles/CameraBlur.dir/CameraBlur.cpp.o: ../CameraBlur.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alejandro/Desktop/Vision/practica1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/CameraBlur.dir/CameraBlur.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/CameraBlur.dir/CameraBlur.cpp.o -c /home/alejandro/Desktop/Vision/practica1/CameraBlur.cpp

CMakeFiles/CameraBlur.dir/CameraBlur.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CameraBlur.dir/CameraBlur.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/alejandro/Desktop/Vision/practica1/CameraBlur.cpp > CMakeFiles/CameraBlur.dir/CameraBlur.cpp.i

CMakeFiles/CameraBlur.dir/CameraBlur.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CameraBlur.dir/CameraBlur.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/alejandro/Desktop/Vision/practica1/CameraBlur.cpp -o CMakeFiles/CameraBlur.dir/CameraBlur.cpp.s

# Object files for target CameraBlur
CameraBlur_OBJECTS = \
"CMakeFiles/CameraBlur.dir/CameraBlur.cpp.o"

# External object files for target CameraBlur
CameraBlur_EXTERNAL_OBJECTS =

CameraBlur: CMakeFiles/CameraBlur.dir/CameraBlur.cpp.o
CameraBlur: CMakeFiles/CameraBlur.dir/build.make
CameraBlur: /usr/local/lib/libopencv_dnn.so.4.4.0
CameraBlur: /usr/local/lib/libopencv_gapi.so.4.4.0
CameraBlur: /usr/local/lib/libopencv_highgui.so.4.4.0
CameraBlur: /usr/local/lib/libopencv_ml.so.4.4.0
CameraBlur: /usr/local/lib/libopencv_objdetect.so.4.4.0
CameraBlur: /usr/local/lib/libopencv_photo.so.4.4.0
CameraBlur: /usr/local/lib/libopencv_stitching.so.4.4.0
CameraBlur: /usr/local/lib/libopencv_video.so.4.4.0
CameraBlur: /usr/local/lib/libopencv_videoio.so.4.4.0
CameraBlur: /usr/local/lib/libopencv_imgcodecs.so.4.4.0
CameraBlur: /usr/local/lib/libopencv_calib3d.so.4.4.0
CameraBlur: /usr/local/lib/libopencv_features2d.so.4.4.0
CameraBlur: /usr/local/lib/libopencv_flann.so.4.4.0
CameraBlur: /usr/local/lib/libopencv_imgproc.so.4.4.0
CameraBlur: /usr/local/lib/libopencv_core.so.4.4.0
CameraBlur: CMakeFiles/CameraBlur.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/alejandro/Desktop/Vision/practica1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable CameraBlur"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/CameraBlur.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/CameraBlur.dir/build: CameraBlur

.PHONY : CMakeFiles/CameraBlur.dir/build

CMakeFiles/CameraBlur.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/CameraBlur.dir/cmake_clean.cmake
.PHONY : CMakeFiles/CameraBlur.dir/clean

CMakeFiles/CameraBlur.dir/depend:
	cd /home/alejandro/Desktop/Vision/practica1/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/alejandro/Desktop/Vision/practica1 /home/alejandro/Desktop/Vision/practica1 /home/alejandro/Desktop/Vision/practica1/build /home/alejandro/Desktop/Vision/practica1/build /home/alejandro/Desktop/Vision/practica1/build/CMakeFiles/CameraBlur.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/CameraBlur.dir/depend

