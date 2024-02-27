find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
#include(${QT_USE_FILE})  #qt4
include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} ${SRCDIR}/ui)

#set(MainWindow_UIS ui_mainWindow.h)
set(MainWindow_SRCS MainWindow.cpp .)

#qt4_automoc(${MainWindow_SRCS})
qt5_wrap_cpp(MainWindow_MOCS MainWindow.h)
#qt4_wrap_ui(MainWindow_UIS mainWindow.ui)

add_executable(AuTom main.cpp ${MainWindow_SRCS} ${MainWindow_MOCS}  ${MainWindow_UIS})
MESSAGE(STATUS "building main")
MESSAGE(STATUS ${RESOURCES_RCC})
target_link_libraries(AuTom Qt5::Widgets Qt5::Core Qt5::Gui profunc frame mrcimg util core custom env widget init promanage utils)

