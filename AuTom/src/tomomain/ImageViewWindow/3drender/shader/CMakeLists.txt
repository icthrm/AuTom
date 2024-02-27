#aux_source_directory(. SHADER_SRCS)
#set(CMAKE_AUTOMOC ON)
set(SHADER_SRCS shaderdatainterface.h  shaderprogram.h raycastingshader.h
    shaderprogram.cpp raycastingshader.cpp )

add_library (shader STATIC ${SHADER_SRCS})
include_directories(${PROJECT_SOURCE_DIR}/src/tomomain/ImageViewWindow)
target_link_libraries(shader  Qt5::Widgets abstract)

set_target_properties(shader PROPERTIES OUTPUT_NAME "shader")
set_target_properties(shader PROPERTIES CLEAN_DIRECT_OUTPUT 1)

