#aux_source_directory(. ALGORITHM_SRCS)
set(CMAKE_AUTOMOC ON)
set(ALGORITHM_SRCS bimap.cpp gradientcalculator.cpp triangulate.cpp subdivion.cpp
    gradientcalculator.h triangulate.h subdivion.h bimap.h CImg.h)

add_library (algorithm STATIC ${ALGORITHM_SRCS})
include_directories( ${PROJECT_SOURCE_DIR}/src/tomomain/ImageViewWindow)
target_link_libraries(algorithm  Qt5::Widgets abstract model)

set_target_properties(algorithm PROPERTIES OUTPUT_NAME "algorithm")
set_target_properties(algorithm PROPERTIES CLEAN_DIRECT_OUTPUT 1)
