
aux_source_directory(. INTERPRET_SRCS)

MESSAGE(STATUS "building interpret")

include_directories(${PROJECT_SOURCE_DIR})

add_library (interpret STATIC ${INTERPRET_SRCS})

target_link_libraries(interpret util dataf cblas mx2to3)

set_target_properties(interpret PROPERTIES OUTPUT_NAME "interpret")
set_target_properties(interpret PROPERTIES CLEAN_DIRECT_OUTPUT 1)
