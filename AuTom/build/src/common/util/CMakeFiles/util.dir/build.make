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
include src/common/util/CMakeFiles/util.dir/depend.make

# Include the progress variables for this target.
include src/common/util/CMakeFiles/util.dir/progress.make

# Include the compile flags for this target's objects.
include src/common/util/CMakeFiles/util.dir/flags.make

src/common/util/CMakeFiles/util.dir/exception.cpp.o: src/common/util/CMakeFiles/util.dir/flags.make
src/common/util/CMakeFiles/util.dir/exception.cpp.o: ../src/common/util/exception.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/common/util/CMakeFiles/util.dir/exception.cpp.o"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/util && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/util.dir/exception.cpp.o -c /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/common/util/exception.cpp

src/common/util/CMakeFiles/util.dir/exception.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/util.dir/exception.cpp.i"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/util && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/common/util/exception.cpp > CMakeFiles/util.dir/exception.cpp.i

src/common/util/CMakeFiles/util.dir/exception.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/util.dir/exception.cpp.s"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/util && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/common/util/exception.cpp -o CMakeFiles/util.dir/exception.cpp.s

src/common/util/CMakeFiles/util.dir/exception.cpp.o.requires:

.PHONY : src/common/util/CMakeFiles/util.dir/exception.cpp.o.requires

src/common/util/CMakeFiles/util.dir/exception.cpp.o.provides: src/common/util/CMakeFiles/util.dir/exception.cpp.o.requires
	$(MAKE) -f src/common/util/CMakeFiles/util.dir/build.make src/common/util/CMakeFiles/util.dir/exception.cpp.o.provides.build
.PHONY : src/common/util/CMakeFiles/util.dir/exception.cpp.o.provides

src/common/util/CMakeFiles/util.dir/exception.cpp.o.provides.build: src/common/util/CMakeFiles/util.dir/exception.cpp.o


src/common/util/CMakeFiles/util.dir/qsort.cpp.o: src/common/util/CMakeFiles/util.dir/flags.make
src/common/util/CMakeFiles/util.dir/qsort.cpp.o: ../src/common/util/qsort.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/common/util/CMakeFiles/util.dir/qsort.cpp.o"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/util && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/util.dir/qsort.cpp.o -c /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/common/util/qsort.cpp

src/common/util/CMakeFiles/util.dir/qsort.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/util.dir/qsort.cpp.i"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/util && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/common/util/qsort.cpp > CMakeFiles/util.dir/qsort.cpp.i

src/common/util/CMakeFiles/util.dir/qsort.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/util.dir/qsort.cpp.s"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/util && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/common/util/qsort.cpp -o CMakeFiles/util.dir/qsort.cpp.s

src/common/util/CMakeFiles/util.dir/qsort.cpp.o.requires:

.PHONY : src/common/util/CMakeFiles/util.dir/qsort.cpp.o.requires

src/common/util/CMakeFiles/util.dir/qsort.cpp.o.provides: src/common/util/CMakeFiles/util.dir/qsort.cpp.o.requires
	$(MAKE) -f src/common/util/CMakeFiles/util.dir/build.make src/common/util/CMakeFiles/util.dir/qsort.cpp.o.provides.build
.PHONY : src/common/util/CMakeFiles/util.dir/qsort.cpp.o.provides

src/common/util/CMakeFiles/util.dir/qsort.cpp.o.provides.build: src/common/util/CMakeFiles/util.dir/qsort.cpp.o


# Object files for target util
util_OBJECTS = \
"CMakeFiles/util.dir/exception.cpp.o" \
"CMakeFiles/util.dir/qsort.cpp.o"

# External object files for target util
util_EXTERNAL_OBJECTS =

lib/libutil.a: src/common/util/CMakeFiles/util.dir/exception.cpp.o
lib/libutil.a: src/common/util/CMakeFiles/util.dir/qsort.cpp.o
lib/libutil.a: src/common/util/CMakeFiles/util.dir/build.make
lib/libutil.a: src/common/util/CMakeFiles/util.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library ../../../lib/libutil.a"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/util && $(CMAKE_COMMAND) -P CMakeFiles/util.dir/cmake_clean_target.cmake
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/util && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/util.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/common/util/CMakeFiles/util.dir/build: lib/libutil.a

.PHONY : src/common/util/CMakeFiles/util.dir/build

src/common/util/CMakeFiles/util.dir/requires: src/common/util/CMakeFiles/util.dir/exception.cpp.o.requires
src/common/util/CMakeFiles/util.dir/requires: src/common/util/CMakeFiles/util.dir/qsort.cpp.o.requires

.PHONY : src/common/util/CMakeFiles/util.dir/requires

src/common/util/CMakeFiles/util.dir/clean:
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/util && $(CMAKE_COMMAND) -P CMakeFiles/util.dir/cmake_clean.cmake
.PHONY : src/common/util/CMakeFiles/util.dir/clean

src/common/util/CMakeFiles/util.dir/depend:
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yangzhidong/haoyu/AuTom_20181119_withmrcview /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/common/util /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/util /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/util/CMakeFiles/util.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/common/util/CMakeFiles/util.dir/depend

