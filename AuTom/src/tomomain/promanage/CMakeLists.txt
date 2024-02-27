set(PROMANAGE_SRCS ProManage.cpp)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)
include_directories(${QT_INCLUDES} ${Qt5Core_LIBRARIES} ${CMAKE_CURRENT_BINARY_DIR})

#qt4_automoc(${PROMANAGE_SRCS})
set(CMAKE_INCLUDE_CURRENT_DIR ON) 
set(CMAKE_AUTOMOC ON)

add_library(promanage ${PROMANAGE_SRCS})
target_link_libraries(promanage env Qt5::Widgets Qt5::OpenGL)
 

