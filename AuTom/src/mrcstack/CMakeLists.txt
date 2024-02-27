



add_executable(mrcstack main.cpp)
include_directories(${CMAKE_CURRENT_BINARY_DIR})


find_package(OpenCV REQUIRED)

target_link_libraries(mrcstack mrcimg ${OPENCV_STATIC_LIBS})
