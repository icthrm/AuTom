
aux_source_directory(. WBP_SRCS)

MESSAGE(STATUS "building wbp")

include_directories(${PROJECT_SOURCE_DIR})

add_library (wbp STATIC ${WBP_SRCS})

target_link_libraries(wbp mrcimg)

set_target_properties(wbp PROPERTIES OUTPUT_NAME "wbp")
set_target_properties(wbp PROPERTIES CLEAN_DIRECT_OUTPUT 1)

