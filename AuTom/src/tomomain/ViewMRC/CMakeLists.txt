find_package(Qt5Widgets REQUIRED)
#find_package(Qt5Core REQUIRED)
#find_package(Qt5Gui REQUIRED)
#include(${QT_USE_FILE})
include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} ${SRCDIR}/ui)

add_executable(ViewMRC ViewMRC.cpp)

target_link_libraries(ViewMRC  Qt5::Widgets XyzView  mrcimg ImageViewWindow )#Qt5::Gui  Qt5::Core
