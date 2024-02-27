/**
 * Copyright 1993-2012 NVIDIA Corporation.  All rights reserved.
 *
 * Please refer to the NVIDIA end user license agreement (EULA) associated
 * with this source code for terms and conditions that govern your use of
 * this software. Any use, reproduction, disclosure, or distribution of
 * this software and related documentation outside the terms of the EULA
 * is strictly prohibited.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "atom.h"
#include "mrc_file.h"
#define PI_180 0.01745329252f

using namespace std;
#ifndef PI
#define     PI  3.14159265358979323846
#endif

#define LOG_BLOCKDIM 6 //2^8=256
#define BLOCKDIM 64

float ANG[ANG_MAX];
float BNG[ANG_MAX];

__constant__ float p_ang[ANG_MAX];
__constant__ int c_Zmin[4096];
/**
 * This macro checks return value of the CUDA runtime call and exits
 * the application if the call failed.
 */
#define CUDA_CHECK_RETURN(value) {											\
	cudaError_t _m_cudaStat = value;										\
	if (_m_cudaStat != cudaSuccess) {										\
		fprintf(stderr, "Error %s at line %d in file %s\n",					\
				cudaGetErrorString(_m_cudaStat), __LINE__, __FILE__);		\
		exit(1);															\
	} }

__device__ float lineindex_value(const float &ang, const int &x, const int &z,
		const int &z0, const int &x0) {
	return ((x - x0 + 0.5f) * __cosf(ang) + (z - z0 + 0.5f) * __sinf(ang) + x0);
//return ((x-x0+0.5f)*__cosf(ang)+(z-z0+0.5f)*__sinf(ang));
}
__device__ inline void atomicAddF(float* address, float value) {
	while (value)
		value = atomicExch(address, value + atomicExch(address, 0.0f));

}

/**
 * CUDA kernel function that reverses the order of bits in each element of the array.
 */
//SIRT
/**********************************************************************************/
__global__ void Slice_reproj(float *d_prj_calc, float *d_prj_num,
		float *d_slc_data, Slice slc, Slide slid, int AngN) //A*X
		{

	const unsigned int n = (blockIdx.x << LOG_BLOCKDIM) + threadIdx.x; //pixel index and thread index
	unsigned int m;
	int j, z, x, row_index;
	float ang, rf, weight;
	int r;
	m = n + slid.min;
	int pixel_allnum = slc.X * slc.Z;

	if (m < pixel_allnum) {
		z = (int) __fdividef(m, slc.X);
		x = m - z * slc.X;

		for (j = 0; j < AngN; j++) {
			ang = PI_180 * p_ang[j];
			rf = lineindex_value(ang, x, z, slc.z0, slc.x0);
			r = floor(rf - 0.5);
			weight = 1.5 - (rf - r);
			//r += slc.x0;

			if (r >= 0 && r < slc.X - 1) {
				row_index = j * slc.X + r;
				atomicAddF(&d_prj_num[row_index], weight);
				atomicAddF(&d_prj_num[row_index + 1], 1 - weight);
				atomicAddF(&d_prj_calc[row_index], weight * d_slc_data[n]);
				atomicAddF(&d_prj_calc[row_index + 1],
						(1 - weight) * d_slc_data[n]);
			} //end if weight
			else if (r == slc.X - 1) //left
					{
				row_index = j * slc.X + r;
				atomicAddF(&d_prj_num[row_index], weight);
				atomicAddF(&d_prj_calc[row_index], weight * d_slc_data[n]);
			} else if (r == -1) //right
					{
				row_index = j * slc.X;
				atomicAddF(&d_prj_num[row_index], 1 - weight);
				atomicAddF(&d_prj_calc[row_index],
						(1 - weight) * d_slc_data[n]);
			}
		} //end for j
	} //end if m
}
/**********************************************************************************/
__global__ void proj_calc_sub(float* d_prj_real, float *d_prj_calc,
		float *d_prj_num, int line_allnum) {
	const unsigned int r = (blockIdx.x << LOG_BLOCKDIM) + threadIdx.x; //prj index
	float s, c;
	s = d_prj_calc[r];
	c = d_prj_num[r];
	if ((r < line_allnum) && (c != 0)) {
		atomicAddF(&d_prj_calc[r], (-1) * d_prj_real[r]);
	}
}
__global__ void proj_calc_div(float *d_prj_calc, float *d_prj_num,
		int line_allnum) {
	const unsigned int r = (blockIdx.x << LOG_BLOCKDIM) + threadIdx.x; //prj index
	float s, c;
	s = d_prj_calc[r];
	c = d_prj_num[r];
	if ((r < line_allnum) && (c != 0)) {
		d_prj_calc[r] = __fdividef(s, c);
	}
}
/**********************************************************************************/
__global__ void SIRT_one_update(float *d_prj_real, float *d_prj_calc,
		float *d_slc_data, int AngN, Slice slc, Slide slid,
		float SIRT_ITR_STEP) {
	const unsigned int n = (blockIdx.x << LOG_BLOCKDIM) + threadIdx.x; //pixel index and thread index
	unsigned int m;
	int j, z, x, row_index;
	float c = 0.0f;
	float s = 0.0f;
	float ang, rf, weight;
	int r;
	m = n + slid.min;
	int pixel_allnum = slc.X * slc.Z;

	if (m < pixel_allnum) {
		z = (int) __fdividef(m, slc.X);
		x = m - z * slc.X;
		for (j = 0; j < AngN; j++) {
			ang = PI_180 * p_ang[j];
			rf = lineindex_value(ang, x, z, slc.z0, slc.x0);
			r = floor(rf - 0.5);
			weight = 1.5 - (rf - r);
			r += slc.x0;
			if (r >= 0 && r < slc.X - 1) {
				c++;
				row_index = j * slc.X + r;
				s += weight * (d_prj_real[row_index] - d_prj_calc[row_index]);
				s +=
						(1 - weight)
								* (d_prj_real[row_index + 1]
										- d_prj_calc[row_index + 1]);
			} //end if
			else if (r == slc.X - 1) {
				c += weight;
				row_index = j * slc.X + r;
				s += weight * (d_prj_real[row_index] - d_prj_calc[row_index]);
			} else if (r == -1) {
				c += 1 - weight;
				row_index = j * slc.X;
				s += (1 - weight)
						* (d_prj_real[row_index] - d_prj_calc[row_index]);
			}
		} //end for j

		if (c != 0)
			d_slc_data[n] += __fdividef(s, c) * SIRT_ITR_STEP;

	} //end if m
}
/**********************************************************************************/
//update the pixel value of slc by comparing the value of real_gray and calc_gray of prj;
__host__ int SIRT_update_slice(float *d_prj_real, float *d_prj_calc,
		float *d_prj_num, float *d_slc_data, int AngN, Slice slc, Slide slid,
		int pixel_num, float SIRT_ITR_STEP)

		{
	int line_allnum = slc.X * AngN;
	int num_blocks;

	unsigned int timer = 0;

	num_blocks = pixel_num >> LOG_BLOCKDIM;

	Slice_reproj<<<dim3(num_blocks), dim3(BLOCKDIM)>>>(d_prj_calc, d_prj_num,
			d_slc_data, slc, slid, AngN);

	num_blocks = line_allnum >> LOG_BLOCKDIM;
	proj_calc_div<<<dim3(num_blocks), dim3(BLOCKDIM)>>>(d_prj_calc, d_prj_num,
			line_allnum);

	cudaThreadSynchronize();
	num_blocks = pixel_num >> LOG_BLOCKDIM;

	SIRT_one_update<<<dim3(num_blocks), dim3(BLOCKDIM)>>>(d_prj_real,
			d_prj_calc, d_slc_data, AngN, slc, slid, SIRT_ITR_STEP);
	cudaThreadSynchronize();
	//CUT_SAFE_CALL( cutStopTimer( timer));
//   printf("Processing time: %f (ms) in Slice_one_update\n", cutGetTimerValue( timer));

	return TRUE;
}

__global__ void Slice_backproj(float *d_prj_real, float *d_slc_data, int AngN,
		Slice slc, Slide slid) {
	const unsigned int n = (blockIdx.x << LOG_BLOCKDIM) + threadIdx.x; //pixel index and thread index
	unsigned int m;
	int j, z, x, row_index;
	float c = 0.0f;
	float s = 0.0f;
	float ang, rf, weight;
	int r;
	m = n + slid.min;
	int pixel_allnum = slc.X * slc.Z;

	if (m < pixel_allnum) {
		z = (int) __fdividef(m, slc.X);
		x = m - z * slc.X;
		for (j = 0; j < AngN; j++) {
			ang = PI_180 * p_ang[j];
			rf = lineindex_value(ang, x, z, slc.z0, slc.x0);
			r = floor(rf - 0.5);
			weight = 1.5 - (rf - r);
			if (r >= 0 && r < slc.X - 1) {
				c++;
				row_index = j * slc.X + r;
				s += weight * d_prj_real[row_index]
						+ (1 - weight) * d_prj_real[row_index + 1];
			} else if (r == slc.X - 1) {
				c += weight;
				row_index = j * slc.X + r;
				s += weight * d_prj_real[row_index];
			} else if (r == -1) {
				c += (1 - weight);
				row_index = j * slc.X;
				s += (1 - weight) * d_prj_real[row_index];
			}

		} //end for j

		if (c != 0)
			d_slc_data[n] = __fdividef(s, c);

	} //end if m

}
void help() {
	printf("\n usage:\n");
	printf(" atom <inputfile> <outputfile> [options]\n");
	printf(
			"     art/ART or sirt/SIRT must be entered so the program can decide wich method to be used \n");
	printf(
			"     [-n]: the interative number,default is 10\n           if n is 0,this program will just do a simple backprojection\n");
	printf(
			"     [-t]: the step size for refinement, suggested value is about 0.3,which is also the default\n");
	printf(
			"     example: atom caveolae.mrc caveolae_rec.mrc SIRT -n 50 -t 0.2 \n\n");
}
int read_proj(FILE *fin, MrcHeader *head, int slcN, float *prj_real) {
	float *slcdata;
	int dataN, n;
	dataN = head->nx * head->nz;

	if ((slcdata = (float *) malloc(sizeof(float) * dataN)) == NULL) {
		printf(
				"Error with function 'read_prj()'! Can not malloc memeroy for slcdata!");
		return FALSE;
	}

	mrc_read_slice(fin, head, slcN, 'y', slcdata);
	for (n = 0; n < dataN; n++)
		prj_real[n] = slcdata[n];

	free(slcdata);

	return TRUE;
}
void totalCount(char* filename, float*a, int num) {
	FILE *result_file;
/*	
	string ss=filename;
	ss="result_"+ss+".txt";*/
	
	if ((result_file = fopen("result.txt", "at+")) == NULL) {
		printf("Cannot open file strike any key exit!\n");
	}
	//fprintf(result_file,
	//			"filename	mean	absolute mean	standard deviation	\n\n");
	double meanf = 0.0;
	double absf = 0.0;
	double stdf = 0.0;
	double subf = 0.0;
	for (int i = 0; i < num; i++) {
		subf = a[i];
		meanf += subf;
		absf += fabs(subf);
		stdf += subf * subf;
	}
	meanf = meanf / num;
	absf = absf / num;
	stdf = sqrt(stdf) / num;
	fprintf(result_file,
			"filename %s, mean %f,absolute mean %f ,standard deviation %f\n\n",
			filename, meanf, absf, stdf);
}
void totalCount(char* filename, float*a, float *b, int num) {
	FILE *result_file;
	if ((result_file = fopen("result.txt", "at+")) == NULL) {
		printf("Cannot open file strike any key exit!\n");
	}
	double meanf = 0.0;
	double absf = 0.0;
	double stdf = 0.0;
	double subf = 0.0;
	for (int i = 0; i < num; i++) {
		subf = a[i] - b[i];
		meanf += subf;
		absf += fabs(subf);
		stdf += subf * subf;
	}
	meanf = meanf / num;
	absf = absf / num;
	stdf = sqrt(stdf) / num;
	fprintf(result_file,
			"filename %s, mean %f,absolute mean %f ,standard deviation %f\n\n",
			filename, meanf, absf, stdf);
}
/**********************************************************************************/
//int  ATOM(char *inf,char *outf, float ATOM_ITR_STEP, int ATOM_ITR_NUM,char *Method) //Method ="SIRT" means sirt, Method = "ART" means ART;Method ="SART" means sart
//count the projection error
/**'/home/ict/Public/ATOM-test/20130608experiment/mitochodria_ASART102.mrc' mitochondria_fin.mrc mitochondria_new.tlt  -t 286 -z -17.6 -f 0.87  -p 3.16
 */
int main(int argc, char *argv[]) {
//example: BBa_ASART102.mrc BBa_fin.mrc BBa_err.mrc BBa_rePro.mrc BBa_new.tlt  -t 256 -z 0.0 -f 0.29 -p 1.16
//input: 3D reconstruction result
//input: 2D original projection
//input: angle file
//input: offset e.g. -f 0.032
//output : reprojection mrc file  and projection error mrc file
//output : the mean error | the absolute mean error| the standard deviation
	char *prof;
	prof = (char *) malloc(TEXT_LINE_MAX);
	char *reconf;
	reconf = (char *) malloc(TEXT_LINE_MAX);
	char *angf;
	angf = (char *) malloc(TEXT_LINE_MAX);
// output mrc
	char * newprof;
	newprof = (char *) malloc(TEXT_LINE_MAX);
	char * errf;
	errf = (char *) malloc(TEXT_LINE_MAX);

	float offset, zshift, pitch;
	int thickness;
	reconf = argv[1];
	prof = argv[2];
	angf = argv[3];
	newprof = argv[4];
	errf = argv[5];

	for (int i = 6; i < argc; i = i + 2) {
		if (argv[i][0] != '-') {
			printf("\"%s\" error! No such option!\n", argv[i]);
			return 0;
		}

		switch (argv[i][1]) {
		case 'f':
			offset = atof(argv[i + 1]);
			break;
		case 't':
			thickness = atoi(argv[i + 1]);
			break;
		case 'z':
			zshift = atof(argv[i + 1]);
			break;
		case 'p':
			pitch = atof(argv[i + 1]);
			break;
		}
	}
	printf("start: %s %s %s: f:%f t:%d z:%f p:%f\n", reconf, prof, angf, offset,
			thickness, zshift, pitch);
// read in the projection &reconstruction result
	// open file
	FILE *pro_file, *recon_file, *ang_file, *newpro_file, *err_file;

	if ((pro_file = fopen(prof, "r")) == NULL) {
		printf("pro_file:Cannot open file strike any key exit!\n");
		return FALSE;
	}
	if ((recon_file = fopen(reconf, "r")) == NULL) {
		printf("recon_file:Cannot open file strike any key exit!\n");
		return FALSE;
	}
	if ((ang_file = fopen(angf, "r")) == NULL) {
		printf("ang_file:Cannot open file strike any key exit!\n");
		return FALSE;
	}
	if ((newpro_file = fopen(newprof, "w+")) == NULL) {
		printf("newpro_file:Cannot open file strike any key exit!\n");
	}
	if ((err_file = fopen(errf, "w+")) == NULL) {
		printf("err_file:Cannot open file strike any key exit!\n");
	}
	//get file information

	MrcHeader *pro_head, *recon_head, *newpro_head, *err_head;
	pro_head = (MrcHeader *) malloc(sizeof(MrcHeader));
	newpro_head = (MrcHeader *) malloc(sizeof(MrcHeader));
	err_head = (MrcHeader *) malloc(sizeof(MrcHeader));
	recon_head = (MrcHeader *) malloc(sizeof(MrcHeader));
	mrc_init_head(newpro_head);
	mrc_init_head(err_head);
	mrc_read_head(pro_file, pro_head);
	mrc_read_head(recon_file, recon_head);
	int pro_num = pro_head->nx * pro_head->ny * pro_head->nz;
	err_head->mode = newpro_head->mode = MRC_MODE_FLOAT;
	err_head->nx = newpro_head->nx = pro_head->nx;
	err_head->ny = newpro_head->ny = pro_head->ny;
	err_head->nz = newpro_head->nz = pro_head->nz;
	mrc_write_head(newpro_file, newpro_head);
	mrc_write_head(err_file, err_head);

	// need to release after debug
	float *newpro_data, *pro_data;
	//locate the new projection
	if ((newpro_data = (float *) malloc(sizeof(float) * pro_num)) == NULL) {
		printf(
				"Error with Function 'ATOM()'!Can't malloc memery for 'prj_real'!");
		return 0;
	}
	if ((pro_data = (float *) malloc(sizeof(float) * pro_num)) == NULL) {
		printf(
				"Error with Function 'ATOM()'!Can't malloc memery for 'prj_real'!");
		return 0;
	}
	mrc_read_all(pro_file, pro_head, pro_data);

	//handle the ang file
	int ang_num = 0;
	char* lstr, *tmp;
	lstr = (char *) malloc(TEXT_LINE_MAX);

	float *thita;
	thita = (float *) malloc(ANG_MAX * sizeof(float));

	while (NULL != (fgets(lstr, TEXT_LINE_MAX, ang_file))) {
		tmp = strtok(lstr, "\n");
		thita[ang_num++] = atof(tmp) + offset;
	}
	for (int i = 0; i < ang_num; i++) {
		ANG[i] = thita[i];
	}

	//for (int i = 0; i < ang_num; i++) {
	//	BNG[i] = -ANG[i];
	//}
	//count the coefficient
	Volume vol;
	vol.X = pro_head->nx;
	vol.Y = pro_head->ny;
	vol.Z = thickness;
	vol.zshift = zshift;
	vol.offset = offset;
	vol.pitch = pitch;

	Slice slc;
	Slide slid;
	slc.X = vol.X;
	slc.x0 = vol.X / 2;
	if (vol.pitch >= 0)
		slc.Z = (int) (vol.Z + tan(rad(vol.pitch)) * vol.Y);
	else
		slc.Z = (int) (vol.Z + tan(rad(-vol.pitch)) * vol.Y);
	slc.z0 = slc.Z / 2 + vol.zshift;

	int Z_minS, Z_maxS, Z_minE, Z_maxE;
	if (vol.pitch >= 0) {
		Z_minS = 0;
		Z_maxS = vol.Z;
		Z_minE = (int) (tan(rad(vol.pitch)) * vol.Y);
		Z_maxE = Z_minE + vol.Z;
	}

	if (vol.pitch < 0) {
		Z_minS = (int) (tan(rad(-vol.pitch)) * vol.Y);
		Z_maxS = vol.Z + Z_minS;
		Z_minE = 0;
		Z_maxE = vol.Z;
	}

	int line_allnum = vol.X * ang_num;
	int pixel_num = slc.X * vol.Z; //write pixels in fact per slice
	int num_blocks = pixel_num >> LOG_BLOCKDIM;
	//printf("ang %d line_allnum %d pixel_num %d num_blocks %d \n",ang_num,line_allnum,pixel_num,num_blocks);

	float *prj_real, *prj_calc, *slc_data;
	prj_real = (float *) malloc(sizeof(float) * line_allnum); //original projection
	memset(prj_real, 0, sizeof(float) * line_allnum);
	prj_calc = (float *) malloc(sizeof(float) * line_allnum); //counted projection
	memset(prj_calc, 0, sizeof(float) * line_allnum);
	slc_data = (float *) malloc(sizeof(float) * pixel_num); // the 3D slice for a y
	memset(slc_data, 0, sizeof(float) * pixel_num);

	float *d_prj_real;
	CUDA_CHECK_RETURN(
			cudaMalloc((void**) &d_prj_real, sizeof(float) * line_allnum));
	float *d_prj_calc;
	CUDA_CHECK_RETURN(
			cudaMalloc((void**) &d_prj_calc, sizeof(float) * line_allnum));
	CUDA_CHECK_RETURN(cudaMemset(d_prj_calc, 0, sizeof(float) * line_allnum));
	float *d_prj_num;
	CUDA_CHECK_RETURN(
			cudaMalloc((void**) &d_prj_num, sizeof(float) * line_allnum));
	CUDA_CHECK_RETURN(cudaMemset(d_prj_num, 0, sizeof(float) * line_allnum));
	float *d_slc_data;
	CUDA_CHECK_RETURN(
			cudaMalloc((void**) &d_slc_data, sizeof(float) * pixel_num));
	CUDA_CHECK_RETURN(cudaMemset(d_slc_data, 0, sizeof(float) * pixel_num));

// test: output the 3Dresult
	/*FILE* re_file;
	 char * ref = "newrecon.mrc";
	 if ((re_file = fopen(ref, "w+")) == NULL) {
	 printf("Cannot open file strike any key exit!\n");
	 }
	 MrcHeader *re_head;
	 re_head = (MrcHeader *) malloc(sizeof(MrcHeader));
	 mrc_init_head(re_head);
	 re_head->mode = MRC_MODE_FLOAT;
	 re_head->nx = recon_head->nx;
	 re_head->ny = recon_head->ny;
	 re_head->nz = recon_head->nz;
	 mrc_write_head(re_file, re_head);
	 float * recon_data;
	 int total_num = vol.X * vol.Z * vol.Y;
	 recon_data = (float *) malloc(sizeof(float) * total_num); //original projection
	 */

	cudaMemcpyToSymbol(p_ang, ANG, sizeof(float) * ang_num);
	//cudaMemcpyToSymbol(p_ang, ANG, sizeof(float) * ang_num);//????? why we need to use this negitive angle
	// allocate device memory for result
	CUDA_CHECK_RETURN(cudaGetLastError());
	for (int j = 0; j < vol.Y; j++) {
		//printf("j=%d\n", j);
		mrc_read_slice_y(pro_file, pro_head, j, prj_real);
		//mrc_write_slice(newpro_file, newpro_head, j, 'y', prj_real);
		cudaMemset(d_prj_calc, 0, sizeof(float) * line_allnum);
		cudaMemset(d_prj_num, 0, sizeof(float) * line_allnum);
		CUDA_CHECK_RETURN(
				cudaMemcpy(d_prj_real, prj_real, sizeof(float) * line_allnum, cudaMemcpyHostToDevice));

		mrc_read_slice_y(recon_file, recon_head, j, slc_data); //read the reconstruction data
		CUDA_CHECK_RETURN(
				cudaMemcpy(d_slc_data, slc_data, sizeof(float) * pixel_num, cudaMemcpyHostToDevice));
		slid.min = slc.X * ((j * (Z_minE - Z_minS)) / vol.Y + Z_minS);
		slid.max = slc.X * ((j * (Z_maxE - Z_maxS)) / vol.Y + Z_maxS);
		num_blocks = pixel_num >> LOG_BLOCKDIM;
		Slice_reproj<<<dim3(num_blocks), dim3(BLOCKDIM)>>>(d_prj_calc,
				d_prj_num, d_slc_data, slc, slid, ang_num);
		CUDA_CHECK_RETURN(cudaThreadSynchronize());
		num_blocks = line_allnum >> LOG_BLOCKDIM;
		proj_calc_div<<<dim3(num_blocks), dim3(BLOCKDIM)>>>(d_prj_calc,
				d_prj_num, line_allnum);
		CUDA_CHECK_RETURN(cudaThreadSynchronize());
		CUDA_CHECK_RETURN(cudaGetLastError());
		CUDA_CHECK_RETURN(
				cudaMemcpy((void*) prj_calc, (void*) d_prj_calc, sizeof(float) * line_allnum, cudaMemcpyDeviceToHost));
		mrc_write_slice(newpro_file, newpro_head, j, 'y', prj_calc);
		proj_calc_sub<<<dim3(num_blocks), dim3(BLOCKDIM)>>>(d_prj_real,
				d_prj_calc, d_prj_num, line_allnum);
		CUDA_CHECK_RETURN(cudaThreadSynchronize());
		CUDA_CHECK_RETURN(cudaGetLastError());
		//copy this part to the array
		CUDA_CHECK_RETURN(
				cudaMemcpy((void*) prj_calc, (void*) d_prj_calc, sizeof(float) * line_allnum, cudaMemcpyDeviceToHost));
		mrc_write_slice(err_file, err_head, j, 'y', prj_calc);
	}
	mrc_update_head(newprof);
	mrc_update_head(errf);
	//mrc_read_all(newpro_file, newpro_head, newpro_data);
	mrc_read_all(err_file, err_head, newpro_data);
	//totalCount(reconf, newpro_data, pro_data, pro_num);
	totalCount(reconf, newpro_data,pro_num);
	cudaFree(d_prj_real);
	cudaFree(d_prj_calc);
	cudaFree(d_prj_num);
	cudaFree(d_slc_data);

	free(slc_data);
	free(prj_calc);
	free(prj_real);
	free(pro_data);
	free(newpro_data);

	//fclose(re_file);
	fclose(pro_file);
	fclose(recon_file);
	fclose(ang_file);
	return 0;
}
