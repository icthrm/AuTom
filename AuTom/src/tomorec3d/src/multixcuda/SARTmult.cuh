#ifndef SART_H
#define SART_H
#include "atom.h"

#define LOG_BLOCKDIM 8 //2^8=256
#define BLOCKDIM 256

long int initGPU(int mpi_rank_id);
int initMalloc(ImageStruct & host_img, int prj_AngN, int line_num,
		long int total_mem) ;
int ASARTinitMalloc(ImageStruct & host_img, int prj_AngN, int line_num,
		long int total_mem) ;
void initDevMem(ImageStruct host_img, int line_num) ;
void transferToDev(ImageStruct *dev_img_ptr, ImageStruct *host_img_ptr,
		int prj_AngN) ;
void transferToDevPro(float *dev_prj_real, float *prj_real, int size) ;
void transferToHostPro(float *dev_prj_real, float *prj_real, int size) ;
void transferToDevVol(float *dev_slc_data, float *slc_data, int size) ;
void transferToHost(float *slc_data, float *dev_slc_data, int pixel_num_per);
void freeDevMem(ImageStruct *dev_img_ptr) ;
void initprj_calcMem(float *prj_calc, int line_num) ;
void Sart_Slice_reproj(int z_start, int size_slc, int ang, float zshift) ;
void Asart_Slice_reproj(int z_start, int size_slc, int ang, float zshift) ;

void calPrj(float* prj_real, float *prj_calc, float *prj_calc_num,
		int size_pro) ;
void updateSlc(int ang, float zshift, int z_start, int size_slc,
		float ATOM_ITR_STEP) ;
void Asart_updateSlc(int ang, float zshift, int z_start, int size_slc,
		float ATOM_ITR_STEP) ;
//FBP
void initSlcMalloc(float *&slc_data, int pixel_num) ;
void initSlcDevMem(float *slc_data, int pixel_num) ;
void initProjectionMalloc(float *&prj, int line_num) ;
void transferProjectionToDev(float *dev_prj, float *host_prj, int line_num) ;
void initMalloc_FBP(double *&x_coef, double *&y_coef,
		int prj_AngN) ;
void initDevMem_FBP(double *x_coef, double *y_coef, int prj_AngN) ;
void transferToDev_FBP(Pixel0 *host_Pixel0, double *dev_x_coef, double *host_x_coef,
		double *dev_y_coef, double *host_y_coef, int prj_AngN,
		Projection host_prj, Volume host_volume) ;
void Sart_Slice_backproj_bilinear(int pixel_num, int z_start, float *prj_real,
		float *slc_data, int zshift, double *dev_x_coef,
		double *dev_y_coef,int prjnum,int prjoffset,int voloffset) ;
void transferToHost_FBP(float *dev_slc_data, float *slc_data, int size) ;
void freeDevSlc(float *slc_data) ;
void freeDevMem(double *x_coef,double *y_coef) ;
void freeDevProjection(float *prj) ;
//endFBP
#endif
