cmake_minimum_required(VERSION 2.6)
FILE(GLOB SOURCES "*.cu" "*.cpp" "*.c" "*.h" "*.cuh")

if(ENABLE_OMP)
       find_package(OpenMP REQUIRED)
       set(CMAKE_C_FLAGS "${OpenMP_C_FLAGS} ${CMAKE_C_FLAGS}")
       set(CMAKE_CXX_FLAGS "${OpenMP_CXX_FLAGS} ${CMAKE_C_FLAGS}")
endif(ENABLE_OMP)

find_package(CUDA QUIET REQUIRED)
find_package(MPI REQUIRED)
#find_package(OpenCV REQUIRED)
MESSAGE(STATUS "building singlecuda")

include_directories(SYSTEM ${MPI_INCLUDE_PATH})
 
# Pass options to NVCC
set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};-G -g -O0 )
# list(APPEND CMAKE_CXX_FLAGS "-std=c++0x -O3 -ffast-math -Wall")
#list(APPEND CUDA_NVCC_FLAGS -gencode arch=compute_30,code=sm_30)
#list(APPEND CUDA_NVCC_FLAGS -gencode arch=compute_35,code=sm_35)
#list(APPEND CUDA_NVCC_FLAGS -gencode arch=compute_37,code=sm_37)
#list(APPEND CUDA_NVCC_FLAGS -gencode arch=compute_50,code=sm_50)
#list(APPEND CUDA_NVCC_FLAGS -gencode arch=compute_52,code=sm_52)
#-G -g -O0 -gencode arch=compute_30,code=sm_30 -gencode arch=compute_35,code=sm_35 -c  -o
# -gencode arch=compute_30,code=sm_30


cuda_add_library(singlecuda STATIC ${SOURCES} )
			
			
set_target_properties(singlecuda PROPERTIES OUTPUT_NAME "singlecuda")
set_target_properties(singlecuda PROPERTIES CLEAN_DIRECT_OUTPUT 1)
