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
include src/common/dataf/CMakeFiles/dataf.dir/depend.make

# Include the progress variables for this target.
include src/common/dataf/CMakeFiles/dataf.dir/progress.make

# Include the compile flags for this target's objects.
include src/common/dataf/CMakeFiles/dataf.dir/flags.make

src/common/dataf/CMakeFiles/dataf.dir/camera.cpp.o: src/common/dataf/CMakeFiles/dataf.dir/flags.make
src/common/dataf/CMakeFiles/dataf.dir/camera.cpp.o: ../src/common/dataf/camera.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/common/dataf/CMakeFiles/dataf.dir/camera.cpp.o"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/dataf && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/dataf.dir/camera.cpp.o -c /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/common/dataf/camera.cpp

src/common/dataf/CMakeFiles/dataf.dir/camera.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/dataf.dir/camera.cpp.i"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/dataf && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/common/dataf/camera.cpp > CMakeFiles/dataf.dir/camera.cpp.i

src/common/dataf/CMakeFiles/dataf.dir/camera.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/dataf.dir/camera.cpp.s"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/dataf && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/common/dataf/camera.cpp -o CMakeFiles/dataf.dir/camera.cpp.s

src/common/dataf/CMakeFiles/dataf.dir/camera.cpp.o.requires:

.PHONY : src/common/dataf/CMakeFiles/dataf.dir/camera.cpp.o.requires

src/common/dataf/CMakeFiles/dataf.dir/camera.cpp.o.provides: src/common/dataf/CMakeFiles/dataf.dir/camera.cpp.o.requires
	$(MAKE) -f src/common/dataf/CMakeFiles/dataf.dir/build.make src/common/dataf/CMakeFiles/dataf.dir/camera.cpp.o.provides.build
.PHONY : src/common/dataf/CMakeFiles/dataf.dir/camera.cpp.o.provides

src/common/dataf/CMakeFiles/dataf.dir/camera.cpp.o.provides.build: src/common/dataf/CMakeFiles/dataf.dir/camera.cpp.o


src/common/dataf/CMakeFiles/dataf.dir/dataf.cpp.o: src/common/dataf/CMakeFiles/dataf.dir/flags.make
src/common/dataf/CMakeFiles/dataf.dir/dataf.cpp.o: ../src/common/dataf/dataf.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/common/dataf/CMakeFiles/dataf.dir/dataf.cpp.o"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/dataf && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/dataf.dir/dataf.cpp.o -c /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/common/dataf/dataf.cpp

src/common/dataf/CMakeFiles/dataf.dir/dataf.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/dataf.dir/dataf.cpp.i"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/dataf && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/common/dataf/dataf.cpp > CMakeFiles/dataf.dir/dataf.cpp.i

src/common/dataf/CMakeFiles/dataf.dir/dataf.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/dataf.dir/dataf.cpp.s"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/dataf && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/common/dataf/dataf.cpp -o CMakeFiles/dataf.dir/dataf.cpp.s

src/common/dataf/CMakeFiles/dataf.dir/dataf.cpp.o.requires:

.PHONY : src/common/dataf/CMakeFiles/dataf.dir/dataf.cpp.o.requires

src/common/dataf/CMakeFiles/dataf.dir/dataf.cpp.o.provides: src/common/dataf/CMakeFiles/dataf.dir/dataf.cpp.o.requires
	$(MAKE) -f src/common/dataf/CMakeFiles/dataf.dir/build.make src/common/dataf/CMakeFiles/dataf.dir/dataf.cpp.o.provides.build
.PHONY : src/common/dataf/CMakeFiles/dataf.dir/dataf.cpp.o.provides

src/common/dataf/CMakeFiles/dataf.dir/dataf.cpp.o.provides.build: src/common/dataf/CMakeFiles/dataf.dir/dataf.cpp.o


src/common/dataf/CMakeFiles/dataf.dir/frame.cpp.o: src/common/dataf/CMakeFiles/dataf.dir/flags.make
src/common/dataf/CMakeFiles/dataf.dir/frame.cpp.o: ../src/common/dataf/frame.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/common/dataf/CMakeFiles/dataf.dir/frame.cpp.o"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/dataf && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/dataf.dir/frame.cpp.o -c /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/common/dataf/frame.cpp

src/common/dataf/CMakeFiles/dataf.dir/frame.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/dataf.dir/frame.cpp.i"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/dataf && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/common/dataf/frame.cpp > CMakeFiles/dataf.dir/frame.cpp.i

src/common/dataf/CMakeFiles/dataf.dir/frame.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/dataf.dir/frame.cpp.s"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/dataf && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/common/dataf/frame.cpp -o CMakeFiles/dataf.dir/frame.cpp.s

src/common/dataf/CMakeFiles/dataf.dir/frame.cpp.o.requires:

.PHONY : src/common/dataf/CMakeFiles/dataf.dir/frame.cpp.o.requires

src/common/dataf/CMakeFiles/dataf.dir/frame.cpp.o.provides: src/common/dataf/CMakeFiles/dataf.dir/frame.cpp.o.requires
	$(MAKE) -f src/common/dataf/CMakeFiles/dataf.dir/build.make src/common/dataf/CMakeFiles/dataf.dir/frame.cpp.o.provides.build
.PHONY : src/common/dataf/CMakeFiles/dataf.dir/frame.cpp.o.provides

src/common/dataf/CMakeFiles/dataf.dir/frame.cpp.o.provides.build: src/common/dataf/CMakeFiles/dataf.dir/frame.cpp.o


src/common/dataf/CMakeFiles/dataf.dir/quat_vec.cpp.o: src/common/dataf/CMakeFiles/dataf.dir/flags.make
src/common/dataf/CMakeFiles/dataf.dir/quat_vec.cpp.o: ../src/common/dataf/quat_vec.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/common/dataf/CMakeFiles/dataf.dir/quat_vec.cpp.o"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/dataf && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/dataf.dir/quat_vec.cpp.o -c /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/common/dataf/quat_vec.cpp

src/common/dataf/CMakeFiles/dataf.dir/quat_vec.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/dataf.dir/quat_vec.cpp.i"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/dataf && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/common/dataf/quat_vec.cpp > CMakeFiles/dataf.dir/quat_vec.cpp.i

src/common/dataf/CMakeFiles/dataf.dir/quat_vec.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/dataf.dir/quat_vec.cpp.s"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/dataf && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/common/dataf/quat_vec.cpp -o CMakeFiles/dataf.dir/quat_vec.cpp.s

src/common/dataf/CMakeFiles/dataf.dir/quat_vec.cpp.o.requires:

.PHONY : src/common/dataf/CMakeFiles/dataf.dir/quat_vec.cpp.o.requires

src/common/dataf/CMakeFiles/dataf.dir/quat_vec.cpp.o.provides: src/common/dataf/CMakeFiles/dataf.dir/quat_vec.cpp.o.requires
	$(MAKE) -f src/common/dataf/CMakeFiles/dataf.dir/build.make src/common/dataf/CMakeFiles/dataf.dir/quat_vec.cpp.o.provides.build
.PHONY : src/common/dataf/CMakeFiles/dataf.dir/quat_vec.cpp.o.provides

src/common/dataf/CMakeFiles/dataf.dir/quat_vec.cpp.o.provides.build: src/common/dataf/CMakeFiles/dataf.dir/quat_vec.cpp.o


# Object files for target dataf
dataf_OBJECTS = \
"CMakeFiles/dataf.dir/camera.cpp.o" \
"CMakeFiles/dataf.dir/dataf.cpp.o" \
"CMakeFiles/dataf.dir/frame.cpp.o" \
"CMakeFiles/dataf.dir/quat_vec.cpp.o"

# External object files for target dataf
dataf_EXTERNAL_OBJECTS =

lib/libdataf.a: src/common/dataf/CMakeFiles/dataf.dir/camera.cpp.o
lib/libdataf.a: src/common/dataf/CMakeFiles/dataf.dir/dataf.cpp.o
lib/libdataf.a: src/common/dataf/CMakeFiles/dataf.dir/frame.cpp.o
lib/libdataf.a: src/common/dataf/CMakeFiles/dataf.dir/quat_vec.cpp.o
lib/libdataf.a: src/common/dataf/CMakeFiles/dataf.dir/build.make
lib/libdataf.a: src/common/dataf/CMakeFiles/dataf.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX static library ../../../lib/libdataf.a"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/dataf && $(CMAKE_COMMAND) -P CMakeFiles/dataf.dir/cmake_clean_target.cmake
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/dataf && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/dataf.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/common/dataf/CMakeFiles/dataf.dir/build: lib/libdataf.a

.PHONY : src/common/dataf/CMakeFiles/dataf.dir/build

src/common/dataf/CMakeFiles/dataf.dir/requires: src/common/dataf/CMakeFiles/dataf.dir/camera.cpp.o.requires
src/common/dataf/CMakeFiles/dataf.dir/requires: src/common/dataf/CMakeFiles/dataf.dir/dataf.cpp.o.requires
src/common/dataf/CMakeFiles/dataf.dir/requires: src/common/dataf/CMakeFiles/dataf.dir/frame.cpp.o.requires
src/common/dataf/CMakeFiles/dataf.dir/requires: src/common/dataf/CMakeFiles/dataf.dir/quat_vec.cpp.o.requires

.PHONY : src/common/dataf/CMakeFiles/dataf.dir/requires

src/common/dataf/CMakeFiles/dataf.dir/clean:
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/dataf && $(CMAKE_COMMAND) -P CMakeFiles/dataf.dir/cmake_clean.cmake
.PHONY : src/common/dataf/CMakeFiles/dataf.dir/clean

src/common/dataf/CMakeFiles/dataf.dir/depend:
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yangzhidong/haoyu/AuTom_20181119_withmrcview /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/common/dataf /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/dataf /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/dataf/CMakeFiles/dataf.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/common/dataf/CMakeFiles/dataf.dir/depend

