
MESSAGE(STATUS "building main")

add_executable(normcc main.cpp)

target_link_libraries(normcc mrcimg)
