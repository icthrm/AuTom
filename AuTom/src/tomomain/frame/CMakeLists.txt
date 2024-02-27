set(FRAME_SRCS CMrcViewer.cpp CCtrlPanel.cpp CSectTool.cpp CSectView.cpp CSliceView.cpp .)

find_package(Qt5Widgets REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

#qt4_automoc(${FRAME_SRCS})
set(CMAKE_AUTOMOC ON)

add_library(frame ${FRAME_SRCS})
target_link_libraries(frame ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${OPENCV_STATIC_LIBS} widget resource  geometry promanage )

set_target_properties(frame PROPERTIES OUTPUT_NAME "frame")
set_target_properties(frame PROPERTIES CLEAN_DIRECT_OUTPUT 1)
