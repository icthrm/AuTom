
#set(UTIL_SRCS exception.cpp dataf.cpp)
aux_source_directory(. MX2TO3_SRCS)

MESSAGE(STATUS "building mx2to3")

include_directories(${PROJECT_SOURCE_DIR})

add_library (mx2to3 STATIC ${MX2TO3_SRCS})

target_link_libraries(mx2to3 matrix util dataf)

set_target_properties(mx2to3 PROPERTIES OUTPUT_NAME "mx2to3")
set_target_properties(mx2to3 PROPERTIES CLEAN_DIRECT_OUTPUT 1)
