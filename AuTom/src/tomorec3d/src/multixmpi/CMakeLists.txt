cmake_minimum_required(VERSION 2.6)

FILE(GLOB SOURCES "*.cu" "*.cpp" "*.c" "*.h")

# if(ENABLE_OMP)
#        find_package(OpenMP REQUIRED)
#        set(CMAKE_C_FLAGS "${OpenMP_C_FLAGS} ${CMAKE_C_FLAGS}")
#        set(CMAKE_CXX_FLAGS "${OpenMP_CXX_FLAGS} ${CMAKE_C_FLAGS}")
# endif(ENABLE_OMP)


find_package(MPI REQUIRED)
#find_package(OpenCV REQUIRED)


include_directories(SYSTEM ${MPI_INCLUDE_PATH})


add_executable(volrec_mltm ${SOURCES})

target_link_libraries(volrec_mltm ${MPI_LIBRARIES} ${OPENCV_STATIC_LIBS})
