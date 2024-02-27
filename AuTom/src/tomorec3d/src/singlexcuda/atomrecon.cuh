#ifndef GPU_H
#define GPU_H
#include "atomgpu.h"

#define LOG_BLOCKDIM 9 //2^8=256
#define BLOCKDIM 512

long int initGPU(int mpi_rank_id);
void initDevMem(float *dev_data, int size);
void initDevVol(float *dev_data, int size);
void initDevfloat(float *dev_data, int size);
void transferToDevtilt(float *dev_data, float *host_data, int size);
int initMalloc(ImageStruct & host_img, int prj_AngN, int line_num,
		int pixel_num,int slicenum);

void BPT_hostmalloc(float **data,int prjsize,float **slcdata,int slcsize);

int SIRT_initMalloc(ImageStruct & dev_img, int line_num, int prj_AngN,
		int pixel_num, int slicenum);
int SART_initMalloc(ImageStruct & dev_img, int line_num, int prj_AngN,
		int pixel_num,int slicenum);
int ASIRT_initMalloc(ImageStruct & dev_img,  int line_num,int prj_AngN,int pixel_num);
int ASART_initMalloc(ImageStruct & dev_img, int line_num, int prj_AngN,
		int pixel_num) ;
void transferToDevPro(float *dev_prj_real, float *prj_real, int size);
void transferIntToDev(int *dev_data, int *host_data, int size) ;
void transferConst(int *host_slcmin,int size);
void transferToDevVol(float *dev_slc_data, float *slc_data, int size);
void transferToHost(float *dev_slc_data, float *slc_data, int size);
void Bpt_Slice_backproj(int pixel_num, int prj_angN, long long  Y_per, float *prj_real,
		Slice slc, Slide slid,int slicenum,int line_num) ;
void Bpt_Slice_divide(int total_num_slc,Slice slc,int slicenum) ;
int Cu_SIRT_update_slice(float *d_prj_real, float *d_prj_calc, float *d_prj_num,
		float *d_slc_data, int AngN, Slice slc, Slide slid, int pixel_num,int prjX,
		float SIRT_ITR_STEP,int slicenum);
int Cu_ASIRT_update_slice(float *d_prj_real, float *d_prj_calc, float *d_prj_num,
		float *d_slc_data, int AngN, Slice slc, Slide slid, int pixel_num,int prjX,
		float SIRT_ITR_STEP, int jslide,float *d_prj_calc_abs) ;
void Cu_SART_random_prj();
int Cu_SART_update_slice(float *d_prj_real, float *d_prj_calc, float *d_prj_num,
		float *d_slc_data, int AngN, Slice slc, Slide slid, int pixel_num, int prjX,
		float SART_ITR_STEP, int slicenum,int slice);
int Cu_ASART_update_slice(float *d_prj_real, float *d_prj_calc, float *d_prj_num,
		float *d_slc_data, int AngN, Slice slc, Slide slid, int pixel_num,int prjX,
		float ASART_ITR_STEP, int jslide,float *d_prj_calc_abs) ;
void freeDevMem_float(float *dev_data);
void freeDevProjection(float *prj);
void freeDevSlc(float *slc_data);
void freeDevMem_int(int *dev_data);
#endif
