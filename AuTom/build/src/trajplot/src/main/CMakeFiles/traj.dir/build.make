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
include src/trajplot/src/main/CMakeFiles/traj.dir/depend.make

# Include the progress variables for this target.
include src/trajplot/src/main/CMakeFiles/traj.dir/progress.make

# Include the compile flags for this target's objects.
include src/trajplot/src/main/CMakeFiles/traj.dir/flags.make

src/trajplot/src/main/CMakeFiles/traj.dir/main.cpp.o: src/trajplot/src/main/CMakeFiles/traj.dir/flags.make
src/trajplot/src/main/CMakeFiles/traj.dir/main.cpp.o: ../src/trajplot/src/main/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/trajplot/src/main/CMakeFiles/traj.dir/main.cpp.o"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/trajplot/src/main && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/traj.dir/main.cpp.o -c /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/trajplot/src/main/main.cpp

src/trajplot/src/main/CMakeFiles/traj.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/traj.dir/main.cpp.i"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/trajplot/src/main && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/trajplot/src/main/main.cpp > CMakeFiles/traj.dir/main.cpp.i

src/trajplot/src/main/CMakeFiles/traj.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/traj.dir/main.cpp.s"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/trajplot/src/main && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/trajplot/src/main/main.cpp -o CMakeFiles/traj.dir/main.cpp.s

src/trajplot/src/main/CMakeFiles/traj.dir/main.cpp.o.requires:

.PHONY : src/trajplot/src/main/CMakeFiles/traj.dir/main.cpp.o.requires

src/trajplot/src/main/CMakeFiles/traj.dir/main.cpp.o.provides: src/trajplot/src/main/CMakeFiles/traj.dir/main.cpp.o.requires
	$(MAKE) -f src/trajplot/src/main/CMakeFiles/traj.dir/build.make src/trajplot/src/main/CMakeFiles/traj.dir/main.cpp.o.provides.build
.PHONY : src/trajplot/src/main/CMakeFiles/traj.dir/main.cpp.o.provides

src/trajplot/src/main/CMakeFiles/traj.dir/main.cpp.o.provides.build: src/trajplot/src/main/CMakeFiles/traj.dir/main.cpp.o


# Object files for target traj
traj_OBJECTS = \
"CMakeFiles/traj.dir/main.cpp.o"

# External object files for target traj
traj_EXTERNAL_OBJECTS =

bin/traj: src/trajplot/src/main/CMakeFiles/traj.dir/main.cpp.o
bin/traj: src/trajplot/src/main/CMakeFiles/traj.dir/build.make
bin/traj: lib/libutil.a
bin/traj: lib/libdataf.a
bin/traj: src/trajplot/lib/libtrajectory.a
bin/traj: lib/libmrcimg.a
bin/traj: lib/libdataf.a
bin/traj: lib/libutil.a
bin/traj: src/trajplot/src/main/CMakeFiles/traj.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../../../bin/traj"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/trajplot/src/main && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/traj.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/trajplot/src/main/CMakeFiles/traj.dir/build: bin/traj

.PHONY : src/trajplot/src/main/CMakeFiles/traj.dir/build

src/trajplot/src/main/CMakeFiles/traj.dir/requires: src/trajplot/src/main/CMakeFiles/traj.dir/main.cpp.o.requires

.PHONY : src/trajplot/src/main/CMakeFiles/traj.dir/requires

src/trajplot/src/main/CMakeFiles/traj.dir/clean:
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/trajplot/src/main && $(CMAKE_COMMAND) -P CMakeFiles/traj.dir/cmake_clean.cmake
.PHONY : src/trajplot/src/main/CMakeFiles/traj.dir/clean

src/trajplot/src/main/CMakeFiles/traj.dir/depend:
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yangzhidong/haoyu/AuTom_20181119_withmrcview /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/trajplot/src/main /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/trajplot/src/main /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/trajplot/src/main/CMakeFiles/traj.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/trajplot/src/main/CMakeFiles/traj.dir/depend

