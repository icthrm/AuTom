set(VolumeWindow_SRCS volumemainwindow.cpp volumemainwindow.h)
set(CMAKE_AUTOMOC ON)##qt signalslot-----对‘vtable for xxxxx未定义的引用
include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR}  ${SRCDIR}/ui  . ${PROJECT_SOURCE_DIR}/src/tomomain/ImageViewWindow)
add_library(VolumeWindow ${VolumeWindow_SRCS})# ${ImageViewWindow_MOC}
target_link_libraries(VolumeWindow Qt5::Widgets Qt5::Widgets  Qt5::Core widgets model resource)
set_target_properties(VolumeWindow PROPERTIES OUTPUT_NAME "VolumeWindow")
set_target_properties(VolumeWindow PROPERTIES CLEAN_DIRECT_OUTPUT 1)
#set_target_properties(ImageViewWindow PROPERTIES AUTOMOC ON)
