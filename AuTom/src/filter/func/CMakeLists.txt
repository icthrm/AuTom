aux_source_directory(. FUNC_SRCS)

MESSAGE(STATUS "building func")

# find_package(OpenCV REQUIRED)
 find_package(OpenMP REQUIRED)
  if(ENABLE_OMP)
        find_package(OpenMP REQUIRED)
        set(CMAKE_C_FLAGS "${OpenMP_C_FLAGS} ${CMAKE_C_FLAGS}")
        set(CMAKE_CXX_FLAGS "${OpenMP_CXX_FLAGS} ${CMAKE_C_FLAGS}")
  endif(ENABLE_OMP)

add_library(func ${FUNC_SRCS})

target_link_libraries(func ${OPENCV_STATIC_LIBS})

