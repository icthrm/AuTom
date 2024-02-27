#ifndef _CR_LIBFUNC_H
#define _CR_LIBFUNC_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <getopt.h>
#include "fftw3.h"

#include "mrcfile.h"
#ifndef __IN
#define __IN
#endif
#ifndef __OUT
#define __OUT
#endif

typedef double           double_comlex[2];      //the same as fftw_complex
typedef float            float_comlex[2];       //like fftw_complex, but in float form;
typedef unsigned long    uslong_complex[2];
typedef long             long_complex[2];
typedef int              int_complex[2];
typedef unsigned int     usint_complex[2];
typedef short            short_complex[2];
typedef unsigned short   usshort_complex[2];

typedef float            Vector2f[2];
void align(__IN char * inf,__IN char * inf_shift,__OUT char * outf, __IN char * inf_rtt);
int mrc_read_slice_double(FILE *fin, MrcHeader *head, int slcN,char axis,double *slcdata);
void cr_project(__IN char* in_st,__IN char* out_Prj/*,float * image_proj_st */);
int correlation_2d(double *Image1, double *Image2, double *Corr_out, int Nx, int Ny);
double max_pixel_2d(__IN double *Image,__OUT long *Max_x,__OUT long *Max_y,__IN int Nx,__IN int Ny);
void cr_xyshift_step(__IN double * image1,__IN double * image2,__IN MrcHeader * inhead,__OUT long * shift_x,__IN long z,__OUT long * shift_y,__IN MrcHeader * outhead );
void cr_pos_para(__IN const int nz,__IN __OUT long * shift_x,__IN __OUT long * shift_y,__IN const long dgr_rtt,__IN const char* filepath_in_shift,__IN long smp_dgr,__IN const char* filepath_outf_shift );
int taper_rotate_plus_45degree(__IN double *Image,__OUT float *Image_out,__IN int Out_nx,__IN int Out_ny,__IN int In_nx,__IN int In_ny);
int img_pad(__IN float *Image,__OUT float *Image_out,__IN const int In_nx,__IN const int In_ny,__IN const int Out_nx,__IN const int Out_ny,__IN const float Mean,__IN const long Pad_x,__IN const long Pad_y);
int img_fft(__IN double *Pre_fft_image,__IN float *Image_out,__IN int Nx,__IN int Ny);
int img_smooth(__IN __OUT float *Image_out,__IN int Nx,__IN int Ny);
int cr_rotation_para(__IN float *Image_fft,__IN int Nx,__IN int Ny,__IN long Smp_dgr,__IN long Ddct_dgr,__OUT long *Max_n);
int img_rotate(__IN char * inf,__OUT char * outf, __IN char * inf_rtt);
int read_rotation(__IN const char * rotation_filepath,__OUT long * rtt);
int write_rotation(__IN const char * rotation_filepath,__IN const long  rtt);
int read_shift(__IN const char * shift_filepath,__OUT float * shift_x,__OUT float * shift_y,__IN const int shiftnum);
int write_shift(__IN const char * shift_filepath,__IN float * shift_x,__IN float * shift_y,__IN const int shiftnum);
#endif
