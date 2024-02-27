set(TESTNORM_SRCS testnorm.cpp)

 if(ENABLE_OMP)
       find_package(OpenMP REQUIRED)
       set(CMAKE_C_FLAGS "${OpenMP_C_FLAGS} ${CMAKE_C_FLAGS}")
       set(CMAKE_CXX_FLAGS "${OpenMP_CXX_FLAGS} ${CMAKE_C_FLAGS}")
 endif(ENABLE_OMP)

add_executable(norm2d ${TESTNORM_SRCS})

target_link_libraries(norm2d nad func)
