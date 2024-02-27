
# set(SIFT_SRCS pyramid.cpp sift_core.cpp pgm.cpp)
aux_source_directory(. SIFT_SRCS)

MESSAGE(STATUS "building sift")

include_directories(${PROJECT_SOURCE_DIR})
#add_library(sift SHARED ${SIFT_SRCS})
add_library(sift STATIC ${SIFT_SRCS})

#find_package(OpenCV REQUIRED)

target_link_libraries(sift util dataf mrcimg ${OPENCV_STATIC_LIBS}) #${OpenCV_LIBS})

set_target_properties(sift PROPERTIES OUTPUT_NAME "sift")
set_target_properties(sift PROPERTIES CLEAN_DIRECT_OUTPUT 1)
