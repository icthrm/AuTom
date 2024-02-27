
#set(UTIL_SRCS exception.cpp dataf.cpp)
aux_source_directory(. GEOMETRY_SRCS)

MESSAGE(STATUS "building geometry")

include_directories(${PROJECT_SOURCE_DIR})

add_library (geometry STATIC ${GEOMETRY_SRCS})

target_link_libraries(geometry)

set_target_properties(geometry PROPERTIES OUTPUT_NAME "geometry")
set_target_properties(geometry PROPERTIES CLEAN_DIRECT_OUTPUT 1)
