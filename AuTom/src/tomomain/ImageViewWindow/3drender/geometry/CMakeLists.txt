#aux_source_directory(. GEO_SRCS)
set(CMAKE_AUTOMOC ON)
set(GEO_SRCS camera.h mesh.h geometry.h volume.h slicevolume.h renderobject.h
    mesh.cpp volume.cpp slicevolume.cpp renderobject.cpp)
#qt5_wrap_cpp(GEO_MOC  mesh.h renderobject.h  slicevolume.h  volume.h camera.h geometry.h)
add_library (geo STATIC ${GEO_SRCS} )#${GEO_MOC}
include_directories( ${PROJECT_SOURCE_DIR}/src/tomomain/ImageViewWindow)
target_link_libraries(geo  Qt5::Widgets abstract shader widgets algorithm)

set_target_properties(geo PROPERTIES OUTPUT_NAME "geo")
set_target_properties(geo PROPERTIES CLEAN_DIRECT_OUTPUT 1)
