

option(ENABLE_OMP "Enables OpenMP, and has additional dependencies" ON)
add_subdirectory(func)
add_subdirectory(nad)
add_subdirectory(testnad)
add_subdirectory(norm2d)

