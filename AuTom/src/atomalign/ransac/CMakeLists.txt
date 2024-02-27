
aux_source_directory(. RANSAC_SRCS)

MESSAGE(STATUS "building ransac_aa")

add_library (ransac_aa STATIC ${RANSAC_SRCS})

#find_package(OpenCV REQUIRED)

target_link_libraries(ransac_aa ${OPENCV_STATIC_LIBS} -lpng) #${OpenCV_LIBS})

set_target_properties(ransac_aa PROPERTIES OUTPUT_NAME "ransac_aa")
set_target_properties(ransac_aa PROPERTIES CLEAN_DIRECT_OUTPUT 1)

