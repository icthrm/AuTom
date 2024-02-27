find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)

#include(${QT_USE_FILE})
include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} ${SRCDIR}/ui)

add_executable(XyzTest main.cpp)

target_link_libraries(XyzTest XyzView  mrcimg Qt5::Widgets Qt5::Core Qt5::Gui )
