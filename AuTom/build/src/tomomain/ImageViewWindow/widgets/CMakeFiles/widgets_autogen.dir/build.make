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

# Utility rule file for widgets_autogen.

# Include the progress variables for this target.
include src/tomomain/ImageViewWindow/widgets/CMakeFiles/widgets_autogen.dir/progress.make

src/tomomain/ImageViewWindow/widgets/CMakeFiles/widgets_autogen:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC for target widgets"
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/tomomain/ImageViewWindow/widgets && /usr/bin/cmake -E cmake_autogen /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/tomomain/ImageViewWindow/widgets/CMakeFiles/widgets_autogen.dir ""

widgets_autogen: src/tomomain/ImageViewWindow/widgets/CMakeFiles/widgets_autogen
widgets_autogen: src/tomomain/ImageViewWindow/widgets/CMakeFiles/widgets_autogen.dir/build.make

.PHONY : widgets_autogen

# Rule to build all files generated by this target.
src/tomomain/ImageViewWindow/widgets/CMakeFiles/widgets_autogen.dir/build: widgets_autogen

.PHONY : src/tomomain/ImageViewWindow/widgets/CMakeFiles/widgets_autogen.dir/build

src/tomomain/ImageViewWindow/widgets/CMakeFiles/widgets_autogen.dir/clean:
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/tomomain/ImageViewWindow/widgets && $(CMAKE_COMMAND) -P CMakeFiles/widgets_autogen.dir/cmake_clean.cmake
.PHONY : src/tomomain/ImageViewWindow/widgets/CMakeFiles/widgets_autogen.dir/clean

src/tomomain/ImageViewWindow/widgets/CMakeFiles/widgets_autogen.dir/depend:
	cd /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yangzhidong/haoyu/AuTom_20181119_withmrcview /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src/tomomain/ImageViewWindow/widgets /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/tomomain/ImageViewWindow/widgets /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/tomomain/ImageViewWindow/widgets/CMakeFiles/widgets_autogen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/tomomain/ImageViewWindow/widgets/CMakeFiles/widgets_autogen.dir/depend

