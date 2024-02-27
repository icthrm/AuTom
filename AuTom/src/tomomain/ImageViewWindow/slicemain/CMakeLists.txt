set(ImageViewWindow_SRCS slicemainwindow.cpp slicemainwindow.h)

set(CMAKE_AUTOMOC ON)##对‘vtable for xxxxx未定义的引用

set(IMAGEVW_DIR ${PROJECT_SOURCE_DIR}/src/tomomain/ImageViewWindow)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR}  ${SRCDIR}/ui  ${IMAGEVW_DIR}/widgets ${IMAGEVW_DIR}/model ${IMAGEVW_DIR}/3drender ${IMAGEVW_DIR}/algorithm  ${IMAGEVW_DIR}/abstract  ${IMAGEVW_DIR} )

add_library(ImageViewWindow ${ImageViewWindow_SRCS})# ${ImageViewWindow_MOC}
target_link_libraries(ImageViewWindow Qt5::Widgets  model widgets resource)
set_target_properties(ImageViewWindow PROPERTIES OUTPUT_NAME "ImageViewWindow")
set_target_properties(ImageViewWindow PROPERTIES CLEAN_DIRECT_OUTPUT 1)
#set_target_properties(ImageViewWindow PROPERTIES AUTOMOC ON)

