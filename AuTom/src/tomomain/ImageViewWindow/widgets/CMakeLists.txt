#aux_source_directory(. WIDGETS_SRCS)
set(WIDGETS_SRCS  doubleslider.cpp tf1deditor.cpp tf1dmappingcanvas.cpp tf1dtexturecanvas.cpp slicecontrolwidget.cpp   colorlisteditor.cpp histogramwidget.cpp sliceeditorwidget.cpp renderwidget.cpp zoomwidget.cpp renderoptionwidget.cpp categorycontrolwidget.cpp mainwindow.cpp markcategray.cpp markinfowidget.cpp marktreeviewwidget.cpp fileinfowidget.cpp pixelwidget.cpp profilewidget.cpp slicewidget.cpp titledsliderwithspinbox.cpp
     globals.h doubleslider.h tf1deditor.h tf1dmappingcanvas.h tf1dtexturecanvas.h histogramwidget.h sliceeditorwidget.h markcategorydialog.h markinfowidget.h fileinfowidget.h pixelwidget.h profilewidget.h slicewidget.h titledsliderwithspinbox.h zoomwidget.h tf1dmappingkey.h marktreeviewwidget.h renderwidget.h renderoptionwidget.h categorycontrolwidget.h slicecontrolwidget.h  colorlisteditor.h mainwindow.h)
set(CMAKE_AUTOMOC ON)

add_library (widgets STATIC ${WIDGETS_SRCS} )
include_directories( ${PROJECT_SOURCE_DIR}/src/tomomain/ImageViewWindow )

target_link_libraries(widgets resource model geo abstract Qt5::Widgets Qt5::Core Qt5::Gui Qt5::OpenGL VolumeWindow)

set_target_properties(widgets PROPERTIES OUTPUT_NAME "widgets")
set_target_properties(widgets PROPERTIES CLEAN_DIRECT_OUTPUT 1)

