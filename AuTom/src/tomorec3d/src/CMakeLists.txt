option(ENABLE_OMP "Enables OpenMP, and has additional dependencies" ON)

add_subdirectory(singlexcuda)
#add_subdirectory(multixmpi)
#add_subdirectory(multixcuda)
add_subdirectory(singlexmpi)
add_subdirectory(main)
