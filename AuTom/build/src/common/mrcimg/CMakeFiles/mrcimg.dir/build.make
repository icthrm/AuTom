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
include src/common/mrcimg/CMakeFiles/mrcimg.dir/depend.make

# Include the progress variables for this target.
include src/common/mrcimg/CMakeFiles/mrcimg.dir/progress.make

# Include the compile flags for this target's objects.
include src/common/mrcimg/CMakeFiles/mrcimg.dir/flags.make

src/common/mrcimg/CMakeFiles/mrcimg.dir/GraphUtils.cpp.o: src/common/mrcimg/CMakeFiles/mrcimg.dir/flags.make
src/common/mrcimg/CMakeFiles/mrcimg.dir/GraphUtils.cpp.o: ../src/common/mrcimg/GraphUtils.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/common/mrcimg/CMakeFiles/mrcimg.dir/GraphUtils.cpp.o"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/mrcimg && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mrcimg.dir/GraphUtils.cpp.o -c /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/common/mrcimg/GraphUtils.cpp

src/common/mrcimg/CMakeFiles/mrcimg.dir/GraphUtils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mrcimg.dir/GraphUtils.cpp.i"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/mrcimg && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/common/mrcimg/GraphUtils.cpp > CMakeFiles/mrcimg.dir/GraphUtils.cpp.i

src/common/mrcimg/CMakeFiles/mrcimg.dir/GraphUtils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mrcimg.dir/GraphUtils.cpp.s"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/mrcimg && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/common/mrcimg/GraphUtils.cpp -o CMakeFiles/mrcimg.dir/GraphUtils.cpp.s

src/common/mrcimg/CMakeFiles/mrcimg.dir/GraphUtils.cpp.o.requires:

.PHONY : src/common/mrcimg/CMakeFiles/mrcimg.dir/GraphUtils.cpp.o.requires

src/common/mrcimg/CMakeFiles/mrcimg.dir/GraphUtils.cpp.o.provides: src/common/mrcimg/CMakeFiles/mrcimg.dir/GraphUtils.cpp.o.requires
	$(MAKE) -f src/common/mrcimg/CMakeFiles/mrcimg.dir/build.make src/common/mrcimg/CMakeFiles/mrcimg.dir/GraphUtils.cpp.o.provides.build
.PHONY : src/common/mrcimg/CMakeFiles/mrcimg.dir/GraphUtils.cpp.o.provides

src/common/mrcimg/CMakeFiles/mrcimg.dir/GraphUtils.cpp.o.provides.build: src/common/mrcimg/CMakeFiles/mrcimg.dir/GraphUtils.cpp.o


src/common/mrcimg/CMakeFiles/mrcimg.dir/mrc2img.cpp.o: src/common/mrcimg/CMakeFiles/mrcimg.dir/flags.make
src/common/mrcimg/CMakeFiles/mrcimg.dir/mrc2img.cpp.o: ../src/common/mrcimg/mrc2img.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/common/mrcimg/CMakeFiles/mrcimg.dir/mrc2img.cpp.o"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/mrcimg && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mrcimg.dir/mrc2img.cpp.o -c /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/common/mrcimg/mrc2img.cpp

src/common/mrcimg/CMakeFiles/mrcimg.dir/mrc2img.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mrcimg.dir/mrc2img.cpp.i"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/mrcimg && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/common/mrcimg/mrc2img.cpp > CMakeFiles/mrcimg.dir/mrc2img.cpp.i

src/common/mrcimg/CMakeFiles/mrcimg.dir/mrc2img.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mrcimg.dir/mrc2img.cpp.s"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/mrcimg && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/common/mrcimg/mrc2img.cpp -o CMakeFiles/mrcimg.dir/mrc2img.cpp.s

src/common/mrcimg/CMakeFiles/mrcimg.dir/mrc2img.cpp.o.requires:

.PHONY : src/common/mrcimg/CMakeFiles/mrcimg.dir/mrc2img.cpp.o.requires

src/common/mrcimg/CMakeFiles/mrcimg.dir/mrc2img.cpp.o.provides: src/common/mrcimg/CMakeFiles/mrcimg.dir/mrc2img.cpp.o.requires
	$(MAKE) -f src/common/mrcimg/CMakeFiles/mrcimg.dir/build.make src/common/mrcimg/CMakeFiles/mrcimg.dir/mrc2img.cpp.o.provides.build
.PHONY : src/common/mrcimg/CMakeFiles/mrcimg.dir/mrc2img.cpp.o.provides

src/common/mrcimg/CMakeFiles/mrcimg.dir/mrc2img.cpp.o.provides.build: src/common/mrcimg/CMakeFiles/mrcimg.dir/mrc2img.cpp.o


src/common/mrcimg/CMakeFiles/mrcimg.dir/mrcheader.cpp.o: src/common/mrcimg/CMakeFiles/mrcimg.dir/flags.make
src/common/mrcimg/CMakeFiles/mrcimg.dir/mrcheader.cpp.o: ../src/common/mrcimg/mrcheader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/common/mrcimg/CMakeFiles/mrcimg.dir/mrcheader.cpp.o"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/mrcimg && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mrcimg.dir/mrcheader.cpp.o -c /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/common/mrcimg/mrcheader.cpp

src/common/mrcimg/CMakeFiles/mrcimg.dir/mrcheader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mrcimg.dir/mrcheader.cpp.i"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/mrcimg && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/common/mrcimg/mrcheader.cpp > CMakeFiles/mrcimg.dir/mrcheader.cpp.i

src/common/mrcimg/CMakeFiles/mrcimg.dir/mrcheader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mrcimg.dir/mrcheader.cpp.s"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/mrcimg && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/common/mrcimg/mrcheader.cpp -o CMakeFiles/mrcimg.dir/mrcheader.cpp.s

src/common/mrcimg/CMakeFiles/mrcimg.dir/mrcheader.cpp.o.requires:

.PHONY : src/common/mrcimg/CMakeFiles/mrcimg.dir/mrcheader.cpp.o.requires

src/common/mrcimg/CMakeFiles/mrcimg.dir/mrcheader.cpp.o.provides: src/common/mrcimg/CMakeFiles/mrcimg.dir/mrcheader.cpp.o.requires
	$(MAKE) -f src/common/mrcimg/CMakeFiles/mrcimg.dir/build.make src/common/mrcimg/CMakeFiles/mrcimg.dir/mrcheader.cpp.o.provides.build
.PHONY : src/common/mrcimg/CMakeFiles/mrcimg.dir/mrcheader.cpp.o.provides

src/common/mrcimg/CMakeFiles/mrcimg.dir/mrcheader.cpp.o.provides.build: src/common/mrcimg/CMakeFiles/mrcimg.dir/mrcheader.cpp.o


# Object files for target mrcimg
mrcimg_OBJECTS = \
"CMakeFiles/mrcimg.dir/GraphUtils.cpp.o" \
"CMakeFiles/mrcimg.dir/mrc2img.cpp.o" \
"CMakeFiles/mrcimg.dir/mrcheader.cpp.o"

# External object files for target mrcimg
mrcimg_EXTERNAL_OBJECTS =

lib/libmrcimg.a: src/common/mrcimg/CMakeFiles/mrcimg.dir/GraphUtils.cpp.o
lib/libmrcimg.a: src/common/mrcimg/CMakeFiles/mrcimg.dir/mrc2img.cpp.o
lib/libmrcimg.a: src/common/mrcimg/CMakeFiles/mrcimg.dir/mrcheader.cpp.o
lib/libmrcimg.a: src/common/mrcimg/CMakeFiles/mrcimg.dir/build.make
lib/libmrcimg.a: src/common/mrcimg/CMakeFiles/mrcimg.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX static library ../../../lib/libmrcimg.a"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/mrcimg && $(CMAKE_COMMAND) -P CMakeFiles/mrcimg.dir/cmake_clean_target.cmake
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/mrcimg && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mrcimg.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/common/mrcimg/CMakeFiles/mrcimg.dir/build: lib/libmrcimg.a

.PHONY : src/common/mrcimg/CMakeFiles/mrcimg.dir/build

src/common/mrcimg/CMakeFiles/mrcimg.dir/requires: src/common/mrcimg/CMakeFiles/mrcimg.dir/GraphUtils.cpp.o.requires
src/common/mrcimg/CMakeFiles/mrcimg.dir/requires: src/common/mrcimg/CMakeFiles/mrcimg.dir/mrc2img.cpp.o.requires
src/common/mrcimg/CMakeFiles/mrcimg.dir/requires: src/common/mrcimg/CMakeFiles/mrcimg.dir/mrcheader.cpp.o.requires

.PHONY : src/common/mrcimg/CMakeFiles/mrcimg.dir/requires

src/common/mrcimg/CMakeFiles/mrcimg.dir/clean:
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/mrcimg && $(CMAKE_COMMAND) -P CMakeFiles/mrcimg.dir/cmake_clean.cmake
.PHONY : src/common/mrcimg/CMakeFiles/mrcimg.dir/clean

src/common/mrcimg/CMakeFiles/mrcimg.dir/depend:
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yangzhidong/haoyu/AuTom_20181119_withmrcview /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/common/mrcimg /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/mrcimg /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/mrcimg/CMakeFiles/mrcimg.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/common/mrcimg/CMakeFiles/mrcimg.dir/depend
