

set(PROFUNC_SRCS profunc.cpp)
include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})
#qt4_automoc(${CUSTOM_SRCS})
set(CMAKE_INCLUDE_CURRENT_DIR ON)    
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)
add_library(profunc ${PROFUNC_SRCS})
target_link_libraries(profunc core  Qt5::Widgets )

set_target_properties(profunc PROPERTIES OUTPUT_NAME "profunc")
set_target_properties(profunc PROPERTIES CLEAN_DIRECT_OUTPUT 1)
