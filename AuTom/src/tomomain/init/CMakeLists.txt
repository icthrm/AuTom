
set(INIT_SRCS Cinit.cpp)

add_library(init ${INIT_SRCS})

target_link_libraries(init env )

