find_package(Qt5Widgets REQUIRED)

#include(${QT_USE_FILE} )

include_directories(Qt5::Widgets ${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} ${OPENGL_INCLUDE_DIRS} )

LINK_DIRECTORIES(${OPENGL_LIBRARIES})
