project(msg_map)

cmake_minimum_required(VERSION 2.6)

include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/core ${PROJECT_SOURCE_DIR}/custom)
link_directories (${PROJECT_BINARY_DIR}/lib)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_subdirectory(core)	#definmation of msg map 
add_subdirectory(custom)