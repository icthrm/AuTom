#aux_source_directory(. ABSTRACT_SRCS)
set(CMAKE_AUTOMOC ON)
set(ABSTRACT_SRCS abstractslicedatamodel.h widgetplugininterface.h abstractsliceeditorplugin.h
    widgetplugininterface.cpp abstractsliceeditorplugin.cpp abstractslicedatamodel.cpp)
add_library (abstract STATIC ${ABSTRACT_SRCS})
include_directories( ${PROJECT_SOURCE_DIR}/src/tomomain/ImageViewWindow)
target_link_libraries(abstract Qt5::Widgets model widgets)

set_target_properties(abstract PROPERTIES OUTPUT_NAME "abstract")
set_target_properties(abstract PROPERTIES CLEAN_DIRECT_OUTPUT 1)
