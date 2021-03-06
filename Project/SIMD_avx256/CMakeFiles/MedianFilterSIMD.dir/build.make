# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/vip/Optimization-of-Median-Filter/SIMD_avx256

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/vip/Optimization-of-Median-Filter/SIMD_avx256

# Include any dependencies generated for this target.
include CMakeFiles/MedianFilterSIMD.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/MedianFilterSIMD.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MedianFilterSIMD.dir/flags.make

CMakeFiles/MedianFilterSIMD.dir/MedianFilter_SIMD_sort.cpp.o: CMakeFiles/MedianFilterSIMD.dir/flags.make
CMakeFiles/MedianFilterSIMD.dir/MedianFilter_SIMD_sort.cpp.o: MedianFilter_SIMD_sort.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/vip/Optimization-of-Median-Filter/SIMD_avx256/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/MedianFilterSIMD.dir/MedianFilter_SIMD_sort.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MedianFilterSIMD.dir/MedianFilter_SIMD_sort.cpp.o -c /home/vip/Optimization-of-Median-Filter/SIMD_avx256/MedianFilter_SIMD_sort.cpp

CMakeFiles/MedianFilterSIMD.dir/MedianFilter_SIMD_sort.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MedianFilterSIMD.dir/MedianFilter_SIMD_sort.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/vip/Optimization-of-Median-Filter/SIMD_avx256/MedianFilter_SIMD_sort.cpp > CMakeFiles/MedianFilterSIMD.dir/MedianFilter_SIMD_sort.cpp.i

CMakeFiles/MedianFilterSIMD.dir/MedianFilter_SIMD_sort.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MedianFilterSIMD.dir/MedianFilter_SIMD_sort.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/vip/Optimization-of-Median-Filter/SIMD_avx256/MedianFilter_SIMD_sort.cpp -o CMakeFiles/MedianFilterSIMD.dir/MedianFilter_SIMD_sort.cpp.s

CMakeFiles/MedianFilterSIMD.dir/MedianFilter_SIMD_sort.cpp.o.requires:

.PHONY : CMakeFiles/MedianFilterSIMD.dir/MedianFilter_SIMD_sort.cpp.o.requires

CMakeFiles/MedianFilterSIMD.dir/MedianFilter_SIMD_sort.cpp.o.provides: CMakeFiles/MedianFilterSIMD.dir/MedianFilter_SIMD_sort.cpp.o.requires
	$(MAKE) -f CMakeFiles/MedianFilterSIMD.dir/build.make CMakeFiles/MedianFilterSIMD.dir/MedianFilter_SIMD_sort.cpp.o.provides.build
.PHONY : CMakeFiles/MedianFilterSIMD.dir/MedianFilter_SIMD_sort.cpp.o.provides

CMakeFiles/MedianFilterSIMD.dir/MedianFilter_SIMD_sort.cpp.o.provides.build: CMakeFiles/MedianFilterSIMD.dir/MedianFilter_SIMD_sort.cpp.o


# Object files for target MedianFilterSIMD
MedianFilterSIMD_OBJECTS = \
"CMakeFiles/MedianFilterSIMD.dir/MedianFilter_SIMD_sort.cpp.o"

# External object files for target MedianFilterSIMD
MedianFilterSIMD_EXTERNAL_OBJECTS =

MedianFilterSIMD: CMakeFiles/MedianFilterSIMD.dir/MedianFilter_SIMD_sort.cpp.o
MedianFilterSIMD: CMakeFiles/MedianFilterSIMD.dir/build.make
MedianFilterSIMD: /usr/local/lib/libopencv_gapi.so.4.5.1
MedianFilterSIMD: /usr/local/lib/libopencv_highgui.so.4.5.1
MedianFilterSIMD: /usr/local/lib/libopencv_ml.so.4.5.1
MedianFilterSIMD: /usr/local/lib/libopencv_objdetect.so.4.5.1
MedianFilterSIMD: /usr/local/lib/libopencv_photo.so.4.5.1
MedianFilterSIMD: /usr/local/lib/libopencv_stitching.so.4.5.1
MedianFilterSIMD: /usr/local/lib/libopencv_video.so.4.5.1
MedianFilterSIMD: /usr/local/lib/libopencv_videoio.so.4.5.1
MedianFilterSIMD: /usr/local/lib/libopencv_dnn.so.4.5.1
MedianFilterSIMD: /usr/local/lib/libopencv_imgcodecs.so.4.5.1
MedianFilterSIMD: /usr/local/lib/libopencv_calib3d.so.4.5.1
MedianFilterSIMD: /usr/local/lib/libopencv_features2d.so.4.5.1
MedianFilterSIMD: /usr/local/lib/libopencv_flann.so.4.5.1
MedianFilterSIMD: /usr/local/lib/libopencv_imgproc.so.4.5.1
MedianFilterSIMD: /usr/local/lib/libopencv_core.so.4.5.1
MedianFilterSIMD: CMakeFiles/MedianFilterSIMD.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/vip/Optimization-of-Median-Filter/SIMD_avx256/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable MedianFilterSIMD"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MedianFilterSIMD.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/MedianFilterSIMD.dir/build: MedianFilterSIMD

.PHONY : CMakeFiles/MedianFilterSIMD.dir/build

CMakeFiles/MedianFilterSIMD.dir/requires: CMakeFiles/MedianFilterSIMD.dir/MedianFilter_SIMD_sort.cpp.o.requires

.PHONY : CMakeFiles/MedianFilterSIMD.dir/requires

CMakeFiles/MedianFilterSIMD.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/MedianFilterSIMD.dir/cmake_clean.cmake
.PHONY : CMakeFiles/MedianFilterSIMD.dir/clean

CMakeFiles/MedianFilterSIMD.dir/depend:
	cd /home/vip/Optimization-of-Median-Filter/SIMD_avx256 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/vip/Optimization-of-Median-Filter/SIMD_avx256 /home/vip/Optimization-of-Median-Filter/SIMD_avx256 /home/vip/Optimization-of-Median-Filter/SIMD_avx256 /home/vip/Optimization-of-Median-Filter/SIMD_avx256 /home/vip/Optimization-of-Median-Filter/SIMD_avx256/CMakeFiles/MedianFilterSIMD.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/MedianFilterSIMD.dir/depend

