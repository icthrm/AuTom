project(cos_align)

#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBFUNC_SRCS .)
add_executable(cr_align mrcfile.c cr_libfunc.c cr_align.c)
add_executable(cr_align_xyshift cr_align_xyshift.c cr_libfunc.c mrcfile.c)
add_executable(cr_rotation cr_rotation.c cr_libfunc.c mrcfile.c)
add_executable(cr_xyshift cr_xyshift.c cr_libfunc.c mrcfile.c)
add_executable(par2xf par2xf.c)
target_link_libraries(cr_align libfftw3.a m )#DO NOT change the order
target_link_libraries(cr_align_xyshift libfftw3.a m)
target_link_libraries(cr_rotation libfftw3.a m)
target_link_libraries(cr_xyshift libfftw3.a m)
target_link_libraries(par2xf libfftw3.a m)

