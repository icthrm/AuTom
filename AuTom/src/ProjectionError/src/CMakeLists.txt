project(ProjectionError)

cmake_minimum_required(VERSION 2.6)
FILE(GLOB SOURCES "*.cu" "*.cpp" "*.c" "*.h")


find_package(CUDA QUIET REQUIRED)
set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};-G -g -O0)
#list(APPEND CUDA_NVCC_FLAGS -gencode arch=compute_30,code=sm_30)
#list(APPEND CUDA_NVCC_FLAGS -gencode arch=compute_35,code=sm_35)
#list(APPEND CUDA_NVCC_FLAGS -gencode arch=compute_37,code=sm_37)
#list(APPEND CUDA_NVCC_FLAGS -gencode arch=compute_50,code=sm_50)
#list(APPEND CUDA_NVCC_FLAGS -gencode arch=compute_52,code=sm_52)-gencode arch=compute_20,code=sm_20


cuda_add_executable(ProjectionErrorStraight ${SOURCES})

