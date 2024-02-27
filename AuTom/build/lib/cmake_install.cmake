# Install script for directory: /home/yangzhidong/haoyu/AuTom_20181119_withmrcview/lib

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
  include("/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/lib/opencv_2.4.1/cmake_install.cmake")
  include("/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/lib/clapack_3.1/cmake_install.cmake")
  include("/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/lib/ann_1.2/cmake_install.cmake")
  include("/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/lib/matrix/cmake_install.cmake")
  include("/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/lib/sba_1.6/cmake_install.cmake")
  include("/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/lib/cminpack/cmake_install.cmake")
  include("/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/lib/cblas/cmake_install.cmake")
  include("/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/lib/levmar_2.5/cmake_install.cmake")
  include("/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/lib/msg_map/cmake_install.cmake")
  include("/home/yangzhidong/haoyu/AuTom_20181119_withmrcview/build/lib/vis/cmake_install.cmake")

endif()

