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
include src/tomomain/paramfile/CMakeFiles/paramfile.dir/depend.make

# Include the progress variables for this target.
include src/tomomain/paramfile/CMakeFiles/paramfile.dir/progress.make

# Include the compile flags for this target's objects.
include src/tomomain/paramfile/CMakeFiles/paramfile.dir/flags.make

src/tomomain/paramfile/CMakeFiles/paramfile.dir/main.cpp.o: src/tomomain/paramfile/CMakeFiles/paramfile.dir/flags.make
src/tomomain/paramfile/CMakeFiles/paramfile.dir/main.cpp.o: ../src/tomomain/paramfile/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/tomomain/paramfile/CMakeFiles/paramfile.dir/main.cpp.o"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/tomomain/paramfile && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/paramfile.dir/main.cpp.o -c /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/tomomain/paramfile/main.cpp

src/tomomain/paramfile/CMakeFiles/paramfile.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/paramfile.dir/main.cpp.i"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/tomomain/paramfile && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/tomomain/paramfile/main.cpp > CMakeFiles/paramfile.dir/main.cpp.i

src/tomomain/paramfile/CMakeFiles/paramfile.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/paramfile.dir/main.cpp.s"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/tomomain/paramfile && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/tomomain/paramfile/main.cpp -o CMakeFiles/paramfile.dir/main.cpp.s

src/tomomain/paramfile/CMakeFiles/paramfile.dir/main.cpp.o.requires:

.PHONY : src/tomomain/paramfile/CMakeFiles/paramfile.dir/main.cpp.o.requires

src/tomomain/paramfile/CMakeFiles/paramfile.dir/main.cpp.o.provides: src/tomomain/paramfile/CMakeFiles/paramfile.dir/main.cpp.o.requires
	$(MAKE) -f src/tomomain/paramfile/CMakeFiles/paramfile.dir/build.make src/tomomain/paramfile/CMakeFiles/paramfile.dir/main.cpp.o.provides.build
.PHONY : src/tomomain/paramfile/CMakeFiles/paramfile.dir/main.cpp.o.provides

src/tomomain/paramfile/CMakeFiles/paramfile.dir/main.cpp.o.provides.build: src/tomomain/paramfile/CMakeFiles/paramfile.dir/main.cpp.o


# Object files for target paramfile
paramfile_OBJECTS = \
"CMakeFiles/paramfile.dir/main.cpp.o"

# External object files for target paramfile
paramfile_EXTERNAL_OBJECTS =

bin/paramfile: src/tomomain/paramfile/CMakeFiles/paramfile.dir/main.cpp.o
bin/paramfile: src/tomomain/paramfile/CMakeFiles/paramfile.dir/build.make
bin/paramfile: src/tomomain/paramfile/CMakeFiles/paramfile.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../../bin/paramfile"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/tomomain/paramfile && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/paramfile.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/tomomain/paramfile/CMakeFiles/paramfile.dir/build: bin/paramfile

.PHONY : src/tomomain/paramfile/CMakeFiles/paramfile.dir/build

src/tomomain/paramfile/CMakeFiles/paramfile.dir/requires: src/tomomain/paramfile/CMakeFiles/paramfile.dir/main.cpp.o.requires

.PHONY : src/tomomain/paramfile/CMakeFiles/paramfile.dir/requires

src/tomomain/paramfile/CMakeFiles/paramfile.dir/clean:
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/tomomain/paramfile && $(CMAKE_COMMAND) -P CMakeFiles/paramfile.dir/cmake_clean.cmake
.PHONY : src/tomomain/paramfile/CMakeFiles/paramfile.dir/clean

src/tomomain/paramfile/CMakeFiles/paramfile.dir/depend:
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yangzhidong/haoyu/AuTom_20181119_withmrcview /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/tomomain/paramfile /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/tomomain/paramfile /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/tomomain/paramfile/CMakeFiles/paramfile.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/tomomain/paramfile/CMakeFiles/paramfile.dir/depend
