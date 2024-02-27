#aux_source_directory(. Model_SRCS)
set(CMAKE_AUTOMOC ON)
set(Model_SRCS  markitem.cpp markmodel.cpp sliceitem.cpp treeitem.cpp mrc.cpp mrcdatamodel.cpp instanceitem.cpp roottreeitem.cpp marktreeitem.cpp iteminfomodel.cpp strokeitem.cpp categorytreeitem.cpp
    mrc.h mrcdatamodel.h sliceitem.h treeitem.h categorytreeitem.h markmodel.h globals.h)
add_library (model STATIC ${Model_SRCS})
include_directories( ${PROJECT_SOURCE_DIR}/src/tomomain/ImageViewWindow  ${PROJECT_SOURCE_DIR}/src/tomomain/ImageViewWindow/model)
target_link_libraries(model abstract  Qt5::Widgets algorithm)

set_target_properties(model PROPERTIES OUTPUT_NAME "model" )
set_target_properties(model PROPERTIES CLEAN_DIRECT_OUTPUT 1)
