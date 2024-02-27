
aux_source_directory(. CORE_SRCS)

MESSAGE(STATUS "building core")

include_directories(${PROJECT_SOURCE_DIR})

add_library(core STATIC ${CORE_SRCS} msg.h proc_state.h)

target_link_libraries(core util pthread)

set_target_properties(core PROPERTIES OUTPUT_NAME "core")
set_target_properties(core PROPERTIES CLEAN_DIRECT_OUTPUT 1)
