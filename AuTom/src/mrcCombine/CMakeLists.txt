include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(mrcCombine main.cpp)
target_link_libraries(mrcCombine mrcimg )

