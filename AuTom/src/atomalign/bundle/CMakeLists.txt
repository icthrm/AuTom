aux_source_directory(. BUNDLE_SRCS)

MESSAGE(STATUS "building bundle")

include_directories(${PROJECT_SOURCE_DIR})

add_library (bundle STATIC ${BUNDLE_SRCS})

target_link_libraries(bundle dataf sba util matrix m)

set_target_properties(bundle PROPERTIES OUTPUT_NAME "bundle")
set_target_properties(bundle PROPERTIES CLEAN_DIRECT_OUTPUT 1)
