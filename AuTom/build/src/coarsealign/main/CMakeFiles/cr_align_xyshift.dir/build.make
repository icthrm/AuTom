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
include src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/depend.make

# Include the progress variables for this target.
include src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/progress.make

# Include the compile flags for this target's objects.
include src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/flags.make

src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/cr_align_xyshift.c.o: src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/flags.make
src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/cr_align_xyshift.c.o: ../src/coarsealign/main/cr_align_xyshift.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/cr_align_xyshift.c.o"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/coarsealign/main && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/cr_align_xyshift.dir/cr_align_xyshift.c.o   -c /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/coarsealign/main/cr_align_xyshift.c

src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/cr_align_xyshift.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/cr_align_xyshift.dir/cr_align_xyshift.c.i"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/coarsealign/main && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/coarsealign/main/cr_align_xyshift.c > CMakeFiles/cr_align_xyshift.dir/cr_align_xyshift.c.i

src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/cr_align_xyshift.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/cr_align_xyshift.dir/cr_align_xyshift.c.s"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/coarsealign/main && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/coarsealign/main/cr_align_xyshift.c -o CMakeFiles/cr_align_xyshift.dir/cr_align_xyshift.c.s

src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/cr_align_xyshift.c.o.requires:

.PHONY : src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/cr_align_xyshift.c.o.requires

src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/cr_align_xyshift.c.o.provides: src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/cr_align_xyshift.c.o.requires
	$(MAKE) -f src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/build.make src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/cr_align_xyshift.c.o.provides.build
.PHONY : src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/cr_align_xyshift.c.o.provides

src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/cr_align_xyshift.c.o.provides.build: src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/cr_align_xyshift.c.o


src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/cr_libfunc.c.o: src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/flags.make
src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/cr_libfunc.c.o: ../src/coarsealign/main/cr_libfunc.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/cr_libfunc.c.o"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/coarsealign/main && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/cr_align_xyshift.dir/cr_libfunc.c.o   -c /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/coarsealign/main/cr_libfunc.c

src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/cr_libfunc.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/cr_align_xyshift.dir/cr_libfunc.c.i"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/coarsealign/main && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/coarsealign/main/cr_libfunc.c > CMakeFiles/cr_align_xyshift.dir/cr_libfunc.c.i

src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/cr_libfunc.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/cr_align_xyshift.dir/cr_libfunc.c.s"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/coarsealign/main && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/coarsealign/main/cr_libfunc.c -o CMakeFiles/cr_align_xyshift.dir/cr_libfunc.c.s

src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/cr_libfunc.c.o.requires:

.PHONY : src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/cr_libfunc.c.o.requires

src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/cr_libfunc.c.o.provides: src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/cr_libfunc.c.o.requires
	$(MAKE) -f src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/build.make src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/cr_libfunc.c.o.provides.build
.PHONY : src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/cr_libfunc.c.o.provides

src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/cr_libfunc.c.o.provides.build: src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/cr_libfunc.c.o


src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/mrcfile.c.o: src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/flags.make
src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/mrcfile.c.o: ../src/coarsealign/main/mrcfile.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/mrcfile.c.o"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/coarsealign/main && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/cr_align_xyshift.dir/mrcfile.c.o   -c /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/coarsealign/main/mrcfile.c

src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/mrcfile.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/cr_align_xyshift.dir/mrcfile.c.i"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/coarsealign/main && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/coarsealign/main/mrcfile.c > CMakeFiles/cr_align_xyshift.dir/mrcfile.c.i

src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/mrcfile.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/cr_align_xyshift.dir/mrcfile.c.s"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/coarsealign/main && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/coarsealign/main/mrcfile.c -o CMakeFiles/cr_align_xyshift.dir/mrcfile.c.s

src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/mrcfile.c.o.requires:

.PHONY : src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/mrcfile.c.o.requires

src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/mrcfile.c.o.provides: src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/mrcfile.c.o.requires
	$(MAKE) -f src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/build.make src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/mrcfile.c.o.provides.build
.PHONY : src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/mrcfile.c.o.provides

src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/mrcfile.c.o.provides.build: src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/mrcfile.c.o


# Object files for target cr_align_xyshift
cr_align_xyshift_OBJECTS = \
"CMakeFiles/cr_align_xyshift.dir/cr_align_xyshift.c.o" \
"CMakeFiles/cr_align_xyshift.dir/cr_libfunc.c.o" \
"CMakeFiles/cr_align_xyshift.dir/mrcfile.c.o"

# External object files for target cr_align_xyshift
cr_align_xyshift_EXTERNAL_OBJECTS =

bin/cr_align_xyshift: src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/cr_align_xyshift.c.o
bin/cr_align_xyshift: src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/cr_libfunc.c.o
bin/cr_align_xyshift: src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/mrcfile.c.o
bin/cr_align_xyshift: src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/build.make
bin/cr_align_xyshift: src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable ../../../bin/cr_align_xyshift"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/coarsealign/main && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cr_align_xyshift.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/build: bin/cr_align_xyshift

.PHONY : src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/build

src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/requires: src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/cr_align_xyshift.c.o.requires
src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/requires: src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/cr_libfunc.c.o.requires
src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/requires: src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/mrcfile.c.o.requires

.PHONY : src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/requires

src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/clean:
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/coarsealign/main && $(CMAKE_COMMAND) -P CMakeFiles/cr_align_xyshift.dir/cmake_clean.cmake
.PHONY : src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/clean

src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/depend:
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yangzhidong/haoyu/AuTom_20181119_withmrcview /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/coarsealign/main /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/coarsealign/main /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/coarsealign/main/CMakeFiles/cr_align_xyshift.dir/depend
