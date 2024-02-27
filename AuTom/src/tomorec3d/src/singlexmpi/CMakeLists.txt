cmake_minimum_required(VERSION 2.6)

aux_source_directory(. SingleMpi_SRCS)
FILE(GLOB SOURCES "*.cu" "*.cpp" "*.c" "*.h")

# if(ENABLE_OMP)
#        find_package(OpenMP REQUIRED)
#        set(CMAKE_C_FLAGS "${OpenMP_C_FLAGS} ${CMAKE_C_FLAGS}")
#        set(CMAKE_CXX_FLAGS "${OpenMP_CXX_FLAGS} ${CMAKE_C_FLAGS}")
MESSAGE(STATUS "building singlempi")

add_library (singlempi STATIC ${SingleMpi_SRCS})
find_package(OpenCV REQUIRED)
	
	
set_target_properties(singlempi PROPERTIES OUTPUT_NAME "singlempi")
set_target_properties(singlempi PROPERTIES CLEAN_DIRECT_OUTPUT 1)


find_package(MPI REQUIRED)
find_package(OpenCV REQUIRED)
target_link_libraries(singlempi ${MPI_LIBRARIES}  ${OPENCV_STATIC_LIBS})

include_directories(SYSTEM ${MPI_INCLUDE_PATH})



