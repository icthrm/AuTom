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
CMAKE_SOURCE_DIR = /home/yangzhidong/haoyu/AuTom_20181119_withmrcview

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build

# Include any dependencies generated for this target.
include src/tomomain/hatpixel/CMakeFiles/hat_pixel.dir/depend.make

# Include the progress variables for this target.
include src/tomomain/hatpixel/CMakeFiles/hat_pixel.dir/progress.make

# Include the compile flags for this target's objects.
include src/tomomain/hatpixel/CMakeFiles/hat_pixel.dir/flags.make

src/tomomain/hatpixel/CMakeFiles/hat_pixel.dir/main.cpp.o: src/tomomain/hatpixel/CMakeFiles/hat_pixel.dir/flags.make
src/tomomain/hatpixel/CMakeFiles/hat_pixel.dir/main.cpp.o: ../src/tomomain/hatpixel/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/tomomain/hatpixel/CMakeFiles/hat_pixel.dir/main.cpp.o"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/tomomain/hatpixel && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/hat_pixel.dir/main.cpp.o -c /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/tomomain/hatpixel/main.cpp

src/tomomain/hatpixel/CMakeFiles/hat_pixel.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/hat_pixel.dir/main.cpp.i"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/tomomain/hatpixel && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/tomomain/hatpixel/main.cpp > CMakeFiles/hat_pixel.dir/main.cpp.i

src/tomomain/hatpixel/CMakeFiles/hat_pixel.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/hat_pixel.dir/main.cpp.s"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/tomomain/hatpixel && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/tomomain/hatpixel/main.cpp -o CMakeFiles/hat_pixel.dir/main.cpp.s

src/tomomain/hatpixel/CMakeFiles/hat_pixel.dir/main.cpp.o.requires:

.PHONY : src/tomomain/hatpixel/CMakeFiles/hat_pixel.dir/main.cpp.o.requires

src/tomomain/hatpixel/CMakeFiles/hat_pixel.dir/main.cpp.o.provides: src/tomomain/hatpixel/CMakeFiles/hat_pixel.dir/main.cpp.o.requires
	$(MAKE) -f src/tomomain/hatpixel/CMakeFiles/hat_pixel.dir/build.make src/tomomain/hatpixel/CMakeFiles/hat_pixel.dir/main.cpp.o.provides.build
.PHONY : src/tomomain/hatpixel/CMakeFiles/hat_pixel.dir/main.cpp.o.provides

src/tomomain/hatpixel/CMakeFiles/hat_pixel.dir/main.cpp.o.provides.build: src/tomomain/hatpixel/CMakeFiles/hat_pixel.dir/main.cpp.o


# Object files for target hat_pixel
hat_pixel_OBJECTS = \
"CMakeFiles/hat_pixel.dir/main.cpp.o"

# External object files for target hat_pixel
hat_pixel_EXTERNAL_OBJECTS =

bin/hat_pixel: src/tomomain/hatpixel/CMakeFiles/hat_pixel.dir/main.cpp.o
bin/hat_pixel: src/tomomain/hatpixel/CMakeFiles/hat_pixel.dir/build.make
bin/hat_pixel: src/tomomain/hatpixel/CMakeFiles/hat_pixel.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../../bin/hat_pixel"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/tomomain/hatpixel && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/hat_pixel.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/tomomain/hatpixel/CMakeFiles/hat_pixel.dir/build: bin/hat_pixel

.PHONY : src/tomomain/hatpixel/CMakeFiles/hat_pixel.dir/build

src/tomomain/hatpixel/CMakeFiles/hat_pixel.dir/requires: src/tomomain/hatpixel/CMakeFiles/hat_pixel.dir/main.cpp.o.requires

.PHONY : src/tomomain/hatpixel/CMakeFiles/hat_pixel.dir/requires

src/tomomain/hatpixel/CMakeFiles/hat_pixel.dir/clean:
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/tomomain/hatpixel && $(CMAKE_COMMAND) -P CMakeFiles/hat_pixel.dir/cmake_clean.cmake
.PHONY : src/tomomain/hatpixel/CMakeFiles/hat_pixel.dir/clean

src/tomomain/hatpixel/CMakeFiles/hat_pixel.dir/depend:
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yangzhidong/haoyu/AuTom_20181119_withmrcview /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/tomomain/hatpixel /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/tomomain/hatpixel /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/tomomain/hatpixel/CMakeFiles/hat_pixel.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/tomomain/hatpixel/CMakeFiles/hat_pixel.dir/depend

