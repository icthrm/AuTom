set(XYZVIEW_SRCS  XyzWindow.cpp XyzGL.cpp b3dgfx.cpp func.cpp ImgInfo.cpp)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
#qt4_automoc(${XYZVIEW_SRCS})
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
FIND_PACKAGE(OpenGL)

if (OPENGL_FOUND)
   MESSAGE("OpenGL Correctly Found:
            OPENGL_LIBRARIES:${OPENGL_LIBRARIES}
            OPENGL_gl_LIBRARY:${OPENGL_gl_LIBRARY}
            OPENGL_glu_LIBRARY:${OPENGL_glu_LIBRARY}
            OPENGL_glaux_LIBRARY:${OPENGL_glaux_LIBRARY} ")
endif (OPENGL_FOUND)

add_definitions(${QT_DEFINITIONS})
include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} ${OPENGL_INCLUDE_DIRS})

SET(QT_USE_QTOPENGL TRUE)
#INCLUDE (${QT_USE_FILE})

qt5_wrap_cpp(XYZVIEW_MOC XyzGL.h XyzWindow.h ImgInfo.h)

LINK_DIRECTORIES(${OPENGL_LIBRARIES})
add_library(XyzView ${XYZVIEW_SRCS} ${XYZVIEW_MOC})
#set(CMAKE_EXE_LINKER_FLAGS "-static")
target_link_libraries(XyzView  Qt5::Widgets Qt5::Core Qt5::Gui ${QT_LIBRARIES} ${OPENGL_LIBRARIES}
                     ${OPENGL_glu_LIBRARY} ${OPENGL_gl_LIBRARY} ${OPENGL_glaux_LIBRARY} ${OPENCV_STATIC_LIBS} widget resource mrcimg)
set_target_properties(XyzView PROPERTIES OUTPUT_NAME "XyzView")
set_target_properties(XyzView PROPERTIES CLEAN_DIRECT_OUTPUT 1)
