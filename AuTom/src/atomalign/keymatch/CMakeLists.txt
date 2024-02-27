
#set(KEYMATCH_SRCS match_core.cpp)
aux_source_directory(. KEYMATCH_SRCS)

MESSAGE(STATUS "building keymatch")

add_library (keymatch STATIC ${KEYMATCH_SRCS})

target_link_libraries(keymatch util sift ann_1.2c)

target_compile_definitions(keymatch PRIVATE ANN_CHAR_COORD=1)

set_target_properties(keymatch PROPERTIES OUTPUT_NAME "keymatch")
set_target_properties(keymatch PROPERTIES CLEAN_DIRECT_OUTPUT 1)
