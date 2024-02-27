set(Utils_SRCS InputSaver.cpp SysInfo.cpp .)

find_package(Qt5Widgets REQUIRED)
 
include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} )

add_library(utils ${Utils_SRCS})

target_link_libraries(utils promanage)
