# Install script for directory: /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/common/cmake_install.cmake")
  include("/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/atomalign/cmake_install.cmake")
  include("/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/markererase/cmake_install.cmake")
  include("/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/tomomain/cmake_install.cmake")
  include("/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/normcc/cmake_install.cmake")
  include("/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/tomosect/cmake_install.cmake")
  include("/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/coarsealign/cmake_install.cmake")
  include("/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/filter/cmake_install.cmake")
  include("/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/tomorec3d/cmake_install.cmake")
  include("/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/trajplot/cmake_install.cmake")
  include("/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/ProjectionError/cmake_install.cmake")
  include("/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/mrcstack/cmake_install.cmake")
  include("/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/mrcCombine/cmake_install.cmake")
  include("/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/src/xfproduct/cmake_install.cmake")

endif()

