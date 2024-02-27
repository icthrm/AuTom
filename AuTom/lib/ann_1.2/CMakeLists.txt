
set(ANN_INCLUDE_DIR ./include)
set(ANN_SRC_DIR ./src)

aux_source_directory(./src ANN_SRCS)


MESSAGE(STATUS "building ann_1.2c")		#coordinate char

include_directories(${ANN_INCLUDE_DIR} ${ANN_SRC_DIR})

add_library (ann_1.2c STATIC ${ANN_SRCS})

target_compile_definitions(ann_1.2c PRIVATE ANN_CHAR_COORD=1)

set_target_properties(ann_1.2c PROPERTIES OUTPUT_NAME "ann_1.2c")
set_target_properties(ann_1.2c PROPERTIES CLEAN_DIRECT_OUTPUT 1)


MESSAGE(STATUS "building ann_1.2d")		#coordinate double

include_directories(${ANN_INCLUDE_DIR} ${ANN_SRC_DIR})

# add_definitions(-DANN_CHAR_COORD=0)

add_library (ann_1.2d STATIC ${ANN_SRCS})

target_compile_definitions(ann_1.2d PRIVATE ANN_CHAR_COORD=0)

set_target_properties(ann_1.2d PROPERTIES OUTPUT_NAME "ann_1.2d")
set_target_properties(ann_1.2d PROPERTIES CLEAN_DIRECT_OUTPUT 1)
