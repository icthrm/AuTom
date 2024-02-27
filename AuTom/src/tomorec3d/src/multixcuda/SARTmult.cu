#include "SARTmult.cuh"

static void CheckCudaErrorAux(const char *, unsigned, const char *,
		cudaError_t);
#define CUDA_CHECK_RETURN(value) CheckCudaErrorAux(__FILE__,__LINE__, #value, value)

static void CheckCudaErrorAux(const char *file, unsigned line,
		const char *statement, cudaError_t err) {
	if (err == cudaSuccess)
		return;
	std::cerr << statement << " returned " << cudaGetErrorString(err) << "("
			<< err << ") at " << file << ":" << line << std::endl;
	exit(1);
}
__constant__ ImageStruct dev_const_img;
__constant__ Volume dev_const_vol;
__constant__ Projection dev_const_prj;
__constant__ Pixel0 dev_const_Pixel0;

long int initGPU(int mpi_rank_id) {

	int deviceCount;
	cudaGetDeviceCount(&deviceCount);
	int gpu_id;
#ifdef ICT_NORMAL
	gpu_id = mpi_rank_id % deviceCount;
#else
	if (mpi_rank_id == 0) {
		gpu_id = 0;
	} else {
		gpu_id = 2;
	}

#endif
	cudaSetDevice(gpu_id);
	cudaDeviceReset();
	cudaDeviceProp device_prop;
	cudaGetDeviceProperties(&device_prop, gpu_id);

	printf("mpi=%d GPU=%d device properties is :\n"
			"\t device name is %s\n"
			"\t totalGlobalMem is %d MB\n"
			"\t sharedMemPerBlock is %d\n"
			"\t regsPerBlock is %d\n"
			"\t memPitch is %d\n"
			"\t maxThreadsPerBlock is %d\n"
			"\t maxThreadsDim [3] is %d X %d X %d\n"
			"\t maxGridSize [3] is %d X %d X %d\n"
			"\t totalConstMem is %d\n"
			"\t device version is major %d ,minor %d\n"
			"\t clockRate is %d\n"
			"\t textureAlignment is %d\n"
			"\t deviceOverlap is %d\n"
			"\t multiProcessorCount is %d\n", mpi_rank_id, gpu_id,
			device_prop.name, device_prop.totalGlobalMem / 1024 / 1024,
			device_prop.sharedMemPerBlock, device_prop.regsPerBlock,
			device_prop.memPitch, device_prop.maxThreadsPerBlock,
			device_prop.maxThreadsDim[0], device_prop.maxThreadsDim[1],
			device_prop.maxThreadsDim[2], device_prop.maxGridSize[0],
			device_prop.maxGridSize[1], device_prop.maxGridSize[2],
			device_prop.totalConstMem, device_prop.major, device_prop.minor,
			device_prop.clockRate, device_prop.textureAlignment,
			device_prop.deviceOverlap, device_prop.multiProcessorCount);
	cudaDeviceReset();
	return device_prop.totalGlobalMem;
}

int initMalloc(ImageStruct & host_img, int prj_AngN, int line_num,
		long int total_mem) {
	long int total_used_mem = 0;

	//CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img_ptr, sizeof(ImageStruct)));
	total_used_mem += sizeof(ImageStruct);

	CUDA_CHECK_RETURN(
			cudaMalloc((void ** )&host_img.x_coef,
					sizeof(double) * prj_AngN * 3));
	CUDA_CHECK_RETURN(
			cudaMalloc((void ** )&host_img.y_coef,
					sizeof(double) * prj_AngN * 3));
	total_used_mem += sizeof(double) * prj_AngN * 6;

	CUDA_CHECK_RETURN(
			cudaMalloc((void ** )&host_img.prj_calc, sizeof(float) * line_num));

	CUDA_CHECK_RETURN(
			cudaMalloc((void ** )&host_img.prj_real, sizeof(float) * line_num));

	CUDA_CHECK_RETURN(
			cudaMalloc((void ** )&host_img.prj_calc_num,
					sizeof(float) * line_num));


	total_used_mem += (sizeof(float) * 5 + sizeof(double) * 2) * line_num;


	long int mem = total_mem - total_used_mem;
	int total_z = mem / (host_img.vol.X * host_img.vol.Y * sizeof(float));
	printf("total_z %d\n",total_z);
	int count = 0;
	if (total_z > host_img.vol.volZ_per) {
		total_z = host_img.vol.volZ_per;
		count = 1;
	} else {
		count = (host_img.vol.volZ_per - 1) / total_z + 1;
		total_z = (host_img.vol.volZ_per - 1) / count + 1;
	}
	host_img.vol.Zcount = count;
	CUDA_CHECK_RETURN(
			cudaMalloc((void ** )&host_img.slc_data,
					host_img.vol.X * host_img.vol.Y * sizeof(float) * total_z));

	cudaMemcpyToSymbol((const void*) &dev_const_img, (void*) &host_img, sizeof(ImageStruct));
	return count;

}
int ASARTinitMalloc(ImageStruct & host_img, int prj_AngN, int line_num,
		long int total_mem) {
	long int total_used_mem = 0;

	//CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img_ptr, sizeof(ImageStruct)));
	total_used_mem += sizeof(ImageStruct);

	CUDA_CHECK_RETURN(
			cudaMalloc((void ** )&host_img.x_coef,
					sizeof(double) * prj_AngN * 3));
	CUDA_CHECK_RETURN(
			cudaMalloc((void ** )&host_img.y_coef,
					sizeof(double) * prj_AngN * 3));
	total_used_mem += sizeof(double) * prj_AngN * 6;

	CUDA_CHECK_RETURN(
			cudaMalloc((void ** )&host_img.prj_calc, sizeof(float) * line_num));

	CUDA_CHECK_RETURN(
			cudaMalloc((void ** )&host_img.prj_real, sizeof(float) * line_num));

	CUDA_CHECK_RETURN(
			cudaMalloc((void ** )&host_img.prj_calc_num,
					sizeof(float) * line_num));

	CUDA_CHECK_RETURN(
			cudaMalloc((void ** )&host_img.prj_calc_abs,
					sizeof(float) * line_num));


	total_used_mem += (sizeof(float) * 5 + sizeof(double) * 2) * line_num;


	long int mem = total_mem - total_used_mem;
	int total_z = mem / (host_img.vol.X * host_img.vol.Y * sizeof(float));
	printf("total_z %d\n",total_z);
	int count = 0;
	if (total_z > host_img.vol.volZ_per) {
		total_z = host_img.vol.volZ_per;
		count = 1;
	} else {
		count = (host_img.vol.volZ_per - 1) / total_z + 1;
		total_z = (host_img.vol.volZ_per - 1) / count + 1;
	}
	host_img.vol.Zcount = count;
	CUDA_CHECK_RETURN(
			cudaMalloc((void ** )&host_img.slc_data,
					host_img.vol.X * host_img.vol.Y * sizeof(float) * total_z));

	cudaMemcpyToSymbol((const void*) &dev_const_img, (void*) &host_img, sizeof(ImageStruct));
	return count;

}
void initDevMem(ImageStruct host_img, int line_num) {

	CUDA_CHECK_RETURN(
			cudaMemset(host_img.prj_calc, 0, sizeof(float) * line_num));
	CUDA_CHECK_RETURN(
			cudaMemset(host_img.prj_real, 0, sizeof(float) * line_num));
	CUDA_CHECK_RETURN(
			cudaMemset(host_img.prj_calc_num, 0, sizeof(float) * line_num));

}
//init prj_calc & prj_calc_num
void initprj_calcMem(float *prj_calc, int line_num) {

	CUDA_CHECK_RETURN(cudaMemset(prj_calc, 0, sizeof(float) * line_num));
}
void transferToDev(ImageStruct *dev_img_ptr, ImageStruct *host_img_ptr,
		int prj_AngN) {

	CUDA_CHECK_RETURN(
			cudaMemcpy(dev_img_ptr->x_coef, host_img_ptr->x_coef,
					sizeof(double) * prj_AngN * 3, cudaMemcpyHostToDevice));
	CUDA_CHECK_RETURN(
			cudaMemcpy(dev_img_ptr->y_coef, host_img_ptr->y_coef,
					sizeof(double) * prj_AngN * 3, cudaMemcpyHostToDevice));
}
void transferToDevPro(float *dev_prj_real, float *prj_real, int size) {
	CUDA_CHECK_RETURN(
			cudaMemcpy(dev_prj_real, prj_real, size, cudaMemcpyHostToDevice));
}
void transferToHostPro(float *dev_prj_real, float *prj_real, int size) {
	CUDA_CHECK_RETURN(
			cudaMemcpy(prj_real, dev_prj_real, size, cudaMemcpyDeviceToHost));
}
void transferToDevVol(float *dev_slc_data, float *slc_data, int size) {
	CUDA_CHECK_RETURN(
			cudaMemcpy(dev_slc_data, slc_data, size, cudaMemcpyHostToDevice));
}
void transferToHost(float *slc_data, float *dev_slc_data, int pixel_num_per) {
	CUDA_CHECK_RETURN(
			cudaMemcpy(slc_data, dev_slc_data, pixel_num_per,
					cudaMemcpyDeviceToHost));
}
//////FBP
void initSlcMalloc(float *&slc_data, int pixel_num) {
	CUDA_CHECK_RETURN(
			cudaMalloc((void ** )&slc_data, sizeof(float) * pixel_num));
}
void initSlcDevMem(float *slc_data, int pixel_num) {

	CUDA_CHECK_RETURN(cudaMemset(slc_data, 0, sizeof(float) * pixel_num));
}
void initProjectionMalloc(float *&prj, int line_num) {
	CUDA_CHECK_RETURN(cudaMalloc((void ** )&prj, sizeof(float) * line_num));
}
void transferProjectionToDev(float *dev_prj, float *host_prj, int line_num) {
	CUDA_CHECK_RETURN(
			cudaMemcpy(dev_prj, host_prj, sizeof(float) * line_num,
					cudaMemcpyHostToDevice));
}
void initMalloc_FBP(double *&x_coef, double *&y_coef,
		int prj_AngN) {
	//CUDA_CHECK_RETURN(cudaMalloc((void ** )&Pix10, sizeof(Pixel0)));
	CUDA_CHECK_RETURN(
			cudaMalloc((void ** )&x_coef, sizeof(double) * prj_AngN * 3));
	CUDA_CHECK_RETURN(
			cudaMalloc((void ** )&y_coef, sizeof(double) * prj_AngN * 3));
}
void initDevMem_FBP(double *x_coef, double *y_coef, int prj_AngN) {

	//CUDA_CHECK_RETURN(cudaMemset(Pix10, 0, sizeof(Pixel0)));
	CUDA_CHECK_RETURN(cudaMemset(x_coef, 0, sizeof(double) * prj_AngN * 3));
	CUDA_CHECK_RETURN(cudaMemset(y_coef, 0, sizeof(double) * prj_AngN * 3));
}

void transferToDev_FBP(Pixel0 *host_Pixel0, double *dev_x_coef, double *host_x_coef,
		double *dev_y_coef, double *host_y_coef, int prj_AngN,
		Projection host_prj, Volume host_volume) {

	cudaMemcpyToSymbol((const void*) &dev_const_Pixel0, (void*) host_Pixel0,
			sizeof(Pixel0));
	cudaMemcpyToSymbol((const void*) &dev_const_prj, (void*) &host_prj,
			sizeof(Projection));
	cudaMemcpyToSymbol((const void*) &dev_const_vol, (void*) &host_volume,
			sizeof(Volume));

	CUDA_CHECK_RETURN(
			cudaMemcpy(dev_x_coef, host_x_coef, sizeof(double) * prj_AngN * 3,
					cudaMemcpyHostToDevice));
	CUDA_CHECK_RETURN(
			cudaMemcpy(dev_y_coef, host_y_coef, sizeof(double) * prj_AngN * 3,
					cudaMemcpyHostToDevice));
}
__device__ void FBP_Projection(int x, int y, float x_weight, float y_weight,
		int n, float *prj_real, int prjx, float *slc_data, int prjAngN) {
	int m = x + y * prjx; //prj index
	atomicAdd(&slc_data[n], (x_weight * y_weight * prj_real[m]) / prjAngN);
//	if(blockIdx.y==1&&blockIdx.x==0&&threadIdx.x==0){
//		printf("id =%d %d slc=%f prj=%f \n", blockIdx.x, threadIdx.x,slc_data[n],   prj_real[m]);
//	}
}
__device__ void dev_val_coef(Pixel0 *pixel0, Pixel pixel, int angle,
		float zshift, double *x_coef, double *y_coef, Weight *comp_prj)

		{
	double x, y;
	int index;
	index = 3 * angle;  //order=1,straight model

	x = x_coef[index] * (pixel.X - pixel0->X)
			+ x_coef[index + 1] * (pixel.Y - pixel0->Y)
			+ x_coef[index + 2] * (pixel.Z - pixel0->Z - zshift) + pixel0->X;
	y = y_coef[index] * (pixel.X - pixel0->X)
			+ y_coef[index + 1] * (pixel.Y - pixel0->Y)
			+ y_coef[index + 2] * (pixel.Z - pixel0->Z - zshift) + pixel0->Y;

	comp_prj->x_min = floor(x);
	comp_prj->y_min = floor(y);

	comp_prj->x_min_del = x - comp_prj->x_min;
	comp_prj->y_min_del = y - comp_prj->y_min;

}
__global__ void FBP_Slice_prj(int total_num_slc, int z_start, float *prj_real,
		float *slc_data, int zshift, double *dev_x_coef,
		double *dev_y_coef,int prjoffset,int voloffset) {

	const unsigned int n = (blockIdx.x << LOG_BLOCKDIM) + threadIdx.x;
	int angindex=blockIdx.y+prjoffset;
	//printf("GPU info --%d  total :%d \n",n,total_num_slc);
	if (n < total_num_slc) {
		//printf("GPU info --%d  total :%d \n",n,total_num_slc);
		//get the x y z of volume
		Pixel pixel;
		Weight comp_prj;

		pixel.Z = (n) / (dev_const_vol.X * dev_const_vol.Y)
				+ z_start; //+vol_ptr->Zstart_per
		pixel.Y = (n) % (dev_const_vol.X * dev_const_vol.Y)
				/ dev_const_vol.X + dev_const_vol.Ystart;
		pixel.X = (n) % (dev_const_vol.X * dev_const_vol.Y)
				% dev_const_vol.X + dev_const_vol.Xstart;
		prj_real+=blockIdx.y*(dev_const_prj.X*dev_const_prj.Y);
		//get the x y of projection
		dev_val_coef(&dev_const_Pixel0, pixel, angindex, zshift, dev_x_coef,
				dev_y_coef, &comp_prj); //&dev_Pixel0
		if (comp_prj.x_min >= 0 && comp_prj.x_min < dev_const_prj.X
				&& comp_prj.y_min >= 0 && comp_prj.y_min < dev_const_prj.Y) {

			FBP_Projection(comp_prj.x_min, comp_prj.y_min,
					1 - comp_prj.x_min_del, 1 - comp_prj.y_min_del, n, prj_real,
					dev_const_prj.X, slc_data, dev_const_prj.AngN);
		}
		if ((comp_prj.x_min + 1) >= 0 && (comp_prj.x_min + 1) < dev_const_prj.X
				&& comp_prj.y_min >= 0 && comp_prj.y_min < dev_const_prj.Y) //(x_min+1, y_min)
						{
			FBP_Projection(comp_prj.x_min + 1, comp_prj.y_min,
					comp_prj.x_min_del, 1 - comp_prj.y_min_del, n, prj_real,
					dev_const_prj.X, slc_data, dev_const_prj.AngN);
		}
		if (comp_prj.x_min >= 0 && comp_prj.x_min < dev_const_prj.X
				&& (comp_prj.y_min + 1) >= 0
				&& (comp_prj.y_min + 1) < dev_const_prj.Y) //(x_min, y_min+1)
						{
			FBP_Projection(comp_prj.x_min, comp_prj.y_min + 1,
					1 - comp_prj.x_min_del, comp_prj.y_min_del, n, prj_real,
					dev_const_prj.X, slc_data, dev_const_prj.AngN);
		}
		if ((comp_prj.x_min + 1) >= 0 && (comp_prj.x_min + 1) < dev_const_prj.X
				&& (comp_prj.y_min + 1) >= 0
				&& (comp_prj.y_min + 1) < dev_const_prj.Y) //(x_min+1, y_min+1)
						{
			FBP_Projection(comp_prj.x_min + 1, comp_prj.y_min + 1,
					comp_prj.x_min_del, comp_prj.y_min_del, n, prj_real,
					dev_const_prj.X, slc_data, dev_const_prj.AngN);
		}
	}
}
void Sart_Slice_backproj_bilinear(int pixel_num, int z_start, float *prj_real,
		float *slc_data, int zshift, double *dev_x_coef,
		double *dev_y_coef,int prjnum,int prjoffset,int voloffset) {
	int num_blocks = (pixel_num - 1) / BLOCKDIM + 1;
	dim3 dim2block(num_blocks,prjnum);
	//printf("pixel_num=%d prjnum=%d \n ", pixel_num, prjnum);
	FBP_Slice_prj<<<dim2block, BLOCKDIM>>>(pixel_num, z_start, prj_real,
			slc_data,  zshift, dev_x_coef, dev_y_coef,prjoffset,voloffset);
	CUDA_CHECK_RETURN(cudaThreadSynchronize());
	CUDA_CHECK_RETURN(cudaGetLastError());
}
void transferToHost_FBP(float *dev_slc_data, float *slc_data, int size) {
	CUDA_CHECK_RETURN(
			cudaMemcpy(slc_data, dev_slc_data, size * sizeof(float),
					cudaMemcpyDeviceToHost));
}
void freeDevSlc(float *slc_data) {
	cudaFree(slc_data);
}
void freeDevMem(double *x_coef,double *y_coef) {

	cudaFree(x_coef);
	cudaFree(y_coef);

}
void freeDevProjection(float *prj) {
	cudaFree(prj);
}
//end BPT
void freeDevMem(ImageStruct *dev_img_ptr) {

	cudaFree(dev_img_ptr->prj_calc);
	cudaFree(dev_img_ptr->global_prj_calc);
	cudaFree(dev_img_ptr->global_prj_calc_num);
	cudaFree(dev_img_ptr->prj_calc_num);
	cudaFree(dev_img_ptr->prj_real);
	cudaFree(dev_img_ptr->slc_data);
	cudaFree(dev_img_ptr->x_coef);
	cudaFree(dev_img_ptr->y_coef);
}
__device__ double atomicAdd(double* address, double val) {
	unsigned long long int* address_as_ull = (unsigned long long int*) address;
	unsigned long long int old = *address_as_ull, assumed;
	do {
		assumed = old;
		old = atomicCAS(address_as_ull, assumed,
				__double_as_longlong(val + __longlong_as_double(assumed)));
	} while (assumed != old);
	return __longlong_as_double(old);
}


__device__ void atomProjection(int x, int y, double x_weight, double y_weight,
		int n) {
	int prj_index;
	prj_index = x + y * dev_const_img.prj.X; //prj index
	double tmp = x_weight * y_weight * dev_const_img.slc_data[n];

	atomicAdd(&dev_const_img.prj_calc[prj_index], tmp);
	//dev_const_img.prj_calc[prj_index]+=tmp;
	atomicAdd(&dev_const_img.prj_calc_num[prj_index], x_weight * y_weight);

}
__device__ void ASARTatomProjection(int x, int y, double x_weight, double y_weight,
		int n) {
	int prj_index;
	prj_index = x + y * dev_const_img.prj.X; //prj index
	double tmp = x_weight * y_weight * dev_const_img.slc_data[n];
	double tmp2 = x_weight * y_weight * abs(dev_const_img.slc_data[n]);
	atomicAdd(&dev_const_img.prj_calc[prj_index], tmp);
	//dev_const_img.prj_calc[prj_index]+=tmp;
	atomicAdd(&dev_const_img.prj_calc_num[prj_index], x_weight * y_weight);
	atomicAdd(&dev_const_img.prj_calc_abs[prj_index], tmp2);

}
__global__ void SART_Slice_reproj_kernel(int ang, float zshift, int z_start,
		int total_num_slc) {
	const unsigned int n = (blockIdx.x << LOG_BLOCKDIM) + threadIdx.x;

	if (n < total_num_slc) {
		//get the x y z of volume
		Pixel pixel;
		Weight comp_prj;
		Volume *vol_ptr = &(dev_const_img.vol);
		pixel.Z = n / (vol_ptr->X * vol_ptr->Y) + vol_ptr->Zstart_per + z_start;
		pixel.Y = n % (vol_ptr->X * vol_ptr->Y) / vol_ptr->X + vol_ptr->Ystart;
		pixel.X = n % (vol_ptr->X * vol_ptr->Y) % vol_ptr->X + vol_ptr->Xstart;

		//get the x y of projection
		dev_val_coef(&dev_const_img.pixel0, pixel, ang, zshift,
				dev_const_img.x_coef, dev_const_img.y_coef, &comp_prj);

		//calculate the projection
		if (comp_prj.x_min >= 0 && comp_prj.x_min < dev_const_img.prj.X
				&& comp_prj.y_min >= 0
				&& comp_prj.y_min < dev_const_img.prj.Y) {
			atomProjection(comp_prj.x_min, comp_prj.y_min,
					1 - comp_prj.x_min_del, 1 - comp_prj.y_min_del, n);
		}
		if ((comp_prj.x_min + 1) >= 0
				&& (comp_prj.x_min + 1) < dev_const_img.prj.X
				&& comp_prj.y_min >= 0 && comp_prj.y_min < dev_const_img.prj.Y) //(x_min+1, y_min)
						{
			atomProjection(comp_prj.x_min + 1, comp_prj.y_min,
					comp_prj.x_min_del, 1 - comp_prj.y_min_del, n);
		}
		if (comp_prj.x_min >= 0 && comp_prj.x_min < dev_const_img.prj.X
				&& (comp_prj.y_min + 1) >= 0
				&& (comp_prj.y_min + 1) < dev_const_img.prj.Y) //(x_min, y_min+1)
						{
			atomProjection(comp_prj.x_min, comp_prj.y_min+1,
					1 - comp_prj.x_min_del, comp_prj.y_min_del, n);
		}
		if ((comp_prj.x_min + 1) >= 0
				&& (comp_prj.x_min + 1) < dev_const_img.prj.X
				&& (comp_prj.y_min + 1) >= 0
				&& (comp_prj.y_min + 1) < dev_const_img.prj.Y) //(x_min+1, y_min+1)
						{
			atomProjection(comp_prj.x_min + 1, comp_prj.y_min + 1,
					comp_prj.x_min_del, comp_prj.y_min_del, n);
		}

	} //end if(n<total_num_slc)
}
__global__ void ASART_Slice_reproj_kernel(int ang, float zshift, int z_start,
		int total_num_slc) {
	const unsigned int n = (blockIdx.x << LOG_BLOCKDIM) + threadIdx.x;

	if (n < total_num_slc) {
		//get the x y z of volume
		Pixel pixel;
		Weight comp_prj;
		Volume *vol_ptr = &(dev_const_img.vol);
		pixel.Z = n / (vol_ptr->X * vol_ptr->Y) + vol_ptr->Zstart_per + z_start;
		pixel.Y = n % (vol_ptr->X * vol_ptr->Y) / vol_ptr->X + vol_ptr->Ystart;
		pixel.X = n % (vol_ptr->X * vol_ptr->Y) % vol_ptr->X + vol_ptr->Xstart;

		//get the x y of projection
		dev_val_coef(&dev_const_img.pixel0, pixel, ang, zshift,
				dev_const_img.x_coef, dev_const_img.y_coef, &comp_prj);

		//calculate the projection
		if (comp_prj.x_min >= 0 && comp_prj.x_min < dev_const_img.prj.X
				&& comp_prj.y_min >= 0
				&& comp_prj.y_min < dev_const_img.prj.Y) {
			ASARTatomProjection(comp_prj.x_min, comp_prj.y_min,
					1 - comp_prj.x_min_del, 1 - comp_prj.y_min_del, n);
		}
		if ((comp_prj.x_min + 1) >= 0
				&& (comp_prj.x_min + 1) < dev_const_img.prj.X
				&& comp_prj.y_min >= 0 && comp_prj.y_min < dev_const_img.prj.Y) //(x_min+1, y_min)
						{
			ASARTatomProjection(comp_prj.x_min + 1, comp_prj.y_min,
					comp_prj.x_min_del, 1 - comp_prj.y_min_del, n);
		}
		if (comp_prj.x_min >= 0 && comp_prj.x_min < dev_const_img.prj.X
				&& (comp_prj.y_min + 1) >= 0
				&& (comp_prj.y_min + 1) < dev_const_img.prj.Y) //(x_min, y_min+1)
						{
			ASARTatomProjection(comp_prj.x_min, comp_prj.y_min+1,
					1 - comp_prj.x_min_del, comp_prj.y_min_del, n);
		}
		if ((comp_prj.x_min + 1) >= 0
				&& (comp_prj.x_min + 1) < dev_const_img.prj.X
				&& (comp_prj.y_min + 1) >= 0
				&& (comp_prj.y_min + 1) < dev_const_img.prj.Y) //(x_min+1, y_min+1)
						{
			ASARTatomProjection(comp_prj.x_min + 1, comp_prj.y_min + 1,
					comp_prj.x_min_del, comp_prj.y_min_del, n);
		}

	} //end if(n<total_num_slc)
}
__device__ void atomBackProjection(int x, int y, double x_weight,
		double y_weight, int n, float ATOM_ITR_STEP,double &c,double &s) {
	int prj_index;
	prj_index = x + y * dev_const_img.prj.X; //prj index
	double tmp = y_weight * x_weight ;
	double del = dev_const_img.prj_real[prj_index]- dev_const_img.prj_calc[prj_index];
	s+=  tmp * del;
	c+= tmp;
}
__device__ void ASARTatomBackProjection(int x, int y, double x_weight,
		double y_weight, int n, float ATOM_ITR_STEP,double &c,double &s,double &t) {
	int prj_index;
	prj_index = x + y * dev_const_img.prj.X; //prj index
	double tmp = y_weight * x_weight ;
	double del = dev_const_img.prj_real[prj_index]- dev_const_img.prj_calc[prj_index];
	s+=  tmp * del;
	c+= tmp;
	t+=tmp*dev_const_img.prj_calc_abs[prj_index];
}
__global__ void SART_one_update(int ang, float zshift, int z_start,
		int total_num_slc, float ATOM_ITR_STEP) {
	const unsigned int n = (blockIdx.x << LOG_BLOCKDIM) + threadIdx.x;
	if (n < total_num_slc) {
		//get the x y z of volume
		Pixel pixel;
		Weight comp_prj;
		Volume *vol_ptr = &(dev_const_img.vol);

		pixel.Z = n / (vol_ptr->X * vol_ptr->Y) + vol_ptr->Zstart_per + z_start;
		pixel.Y = n % (vol_ptr->X * vol_ptr->Y) / vol_ptr->X + vol_ptr->Ystart;
		pixel.X = n % (vol_ptr->X * vol_ptr->Y) % vol_ptr->X + vol_ptr->Xstart;

		//get the x y of projection
		dev_val_coef(&dev_const_img.pixel0, pixel, ang, zshift,
				dev_const_img.x_coef, dev_const_img.y_coef, &comp_prj);

		double c=0;
		double s=0;

		//calculate the projection
		if (comp_prj.x_min >= 0 && comp_prj.x_min < dev_const_img.prj.X
				&& comp_prj.y_min >= 0
				&& comp_prj.y_min < dev_const_img.prj.Y) {
			atomBackProjection(comp_prj.x_min, comp_prj.y_min,
					1 - comp_prj.x_min_del, 1 - comp_prj.y_min_del, n,
					ATOM_ITR_STEP,c,s);
		}
		if ((comp_prj.x_min + 1) >= 0
				&& (comp_prj.x_min + 1) < dev_const_img.prj.X
				&& comp_prj.y_min >= 0 && comp_prj.y_min < dev_const_img.prj.Y) //(x_min+1, y_min)
						{
			atomBackProjection(comp_prj.x_min + 1, comp_prj.y_min,
					comp_prj.x_min_del, 1 - comp_prj.y_min_del, n,
					ATOM_ITR_STEP,c,s);
		}
		if (comp_prj.x_min >= 0 && comp_prj.x_min < dev_const_img.prj.X
				&& (comp_prj.y_min + 1) >= 0
				&& (comp_prj.y_min + 1) < dev_const_img.prj.Y) //(x_min, y_min+1)
						{
			atomBackProjection(comp_prj.x_min, comp_prj.y_min+1,
					1 - comp_prj.x_min_del, comp_prj.y_min_del, n,
					ATOM_ITR_STEP,c,s);
		}
		if ((comp_prj.x_min + 1) >= 0
				&& (comp_prj.x_min + 1) < dev_const_img.prj.X
				&& (comp_prj.y_min + 1) >= 0
				&& (comp_prj.y_min + 1) < dev_const_img.prj.Y) //(x_min+1, y_min+1)
						{
			atomBackProjection(comp_prj.x_min + 1, comp_prj.y_min + 1,
					comp_prj.x_min_del, comp_prj.y_min_del, n, ATOM_ITR_STEP,c,s);
		}
        if(c!=0)
		dev_const_img.slc_data[n]+=(float)(s/c)*ATOM_ITR_STEP;
	} //end if(n<total_num_slc)
}
__global__ void ASART_one_update(int ang, float zshift, int z_start,
		int total_num_slc, float ATOM_ITR_STEP) {
	const unsigned int n = (blockIdx.x << LOG_BLOCKDIM) + threadIdx.x;
	if (n < total_num_slc) {
		//get the x y z of volume
		Pixel pixel;
		Weight comp_prj;
		Volume *vol_ptr = &(dev_const_img.vol);

		pixel.Z = n / (vol_ptr->X * vol_ptr->Y) + vol_ptr->Zstart_per + z_start;
		pixel.Y = n % (vol_ptr->X * vol_ptr->Y) / vol_ptr->X + vol_ptr->Ystart;
		pixel.X = n % (vol_ptr->X * vol_ptr->Y) % vol_ptr->X + vol_ptr->Xstart;

		//get the x y of projection
		dev_val_coef(&dev_const_img.pixel0, pixel, ang, zshift,
				dev_const_img.x_coef, dev_const_img.y_coef, &comp_prj);

		double c=0;
		double s=0;
		double t=0;
		//calculate the projection
		if (comp_prj.x_min >= 0 && comp_prj.x_min < dev_const_img.prj.X
				&& comp_prj.y_min >= 0
				&& comp_prj.y_min < dev_const_img.prj.Y) {
			ASARTatomBackProjection(comp_prj.x_min, comp_prj.y_min,
					1 - comp_prj.x_min_del, 1 - comp_prj.y_min_del, n,
					ATOM_ITR_STEP,c,s,t);
		}
		if ((comp_prj.x_min + 1) >= 0
				&& (comp_prj.x_min + 1) < dev_const_img.prj.X
				&& comp_prj.y_min >= 0 && comp_prj.y_min < dev_const_img.prj.Y) //(x_min+1, y_min)
						{
			ASARTatomBackProjection(comp_prj.x_min + 1, comp_prj.y_min,
					comp_prj.x_min_del, 1 - comp_prj.y_min_del, n,
					ATOM_ITR_STEP,c,s,t);
		}
		if (comp_prj.x_min >= 0 && comp_prj.x_min < dev_const_img.prj.X
				&& (comp_prj.y_min + 1) >= 0
				&& (comp_prj.y_min + 1) < dev_const_img.prj.Y) //(x_min, y_min+1)
						{
			ASARTatomBackProjection(comp_prj.x_min, comp_prj.y_min+1,
					1 - comp_prj.x_min_del, comp_prj.y_min_del, n,
					ATOM_ITR_STEP,c,s,t);
		}
		if ((comp_prj.x_min + 1) >= 0
				&& (comp_prj.x_min + 1) < dev_const_img.prj.X
				&& (comp_prj.y_min + 1) >= 0
				&& (comp_prj.y_min + 1) < dev_const_img.prj.Y) //(x_min+1, y_min+1)
						{
			ASARTatomBackProjection(comp_prj.x_min + 1, comp_prj.y_min + 1,
					comp_prj.x_min_del, comp_prj.y_min_del, n, ATOM_ITR_STEP,c,s,t);
		}
        if(t!=0)
        	dev_const_img.slc_data[n]+= abs(dev_const_img.slc_data[n]) * (float) s / t * ATOM_ITR_STEP;


	} //end if(n<total_num_slc)
}
__global__ void fixPrjCal(float *prj_calc, float *prj_calc_num, int size_pro) {
	unsigned int i = blockIdx.x * blockDim.x + threadIdx.x;

	if (prj_calc_num[i] != 0 && i < size_pro) {
		prj_calc[i] = prj_calc[i] / prj_calc_num[i];
	}

//	prj_calc[i] = prj_real[i] - prj_calc[i];
}
void Sart_Slice_reproj(int z_start, int size_slc, int ang, float zshift) {
	int num_blocks = (size_slc - 1) / BLOCKDIM + 1;
	SART_Slice_reproj_kernel<<<num_blocks, BLOCKDIM>>>(ang, zshift, z_start,
			size_slc);
}
void calPrj(float* prj_real, float *prj_calc, float *prj_calc_num,
		int size_pro) {
	int num_blocks = (size_pro - 1) / BLOCKDIM + 1;
	fixPrjCal<<<num_blocks, BLOCKDIM>>>(prj_calc, prj_calc_num, size_pro);
}
void updateSlc(int ang, float zshift, int z_start, int size_slc,
		float ATOM_ITR_STEP) {
	int num_blocks = (size_slc - 1) / BLOCKDIM + 1;
	SART_one_update<<<num_blocks, BLOCKDIM>>>(ang, zshift, z_start, size_slc,
			ATOM_ITR_STEP);
}
void Asart_Slice_reproj(int z_start, int size_slc, int ang, float zshift) {
	int num_blocks = (size_slc - 1) / BLOCKDIM + 1;
	ASART_Slice_reproj_kernel<<<num_blocks, BLOCKDIM>>>(ang, zshift, z_start,
			size_slc);
}

void Asart_updateSlc(int ang, float zshift, int z_start, int size_slc,
		float ATOM_ITR_STEP) {
	int num_blocks = (size_slc - 1) / BLOCKDIM + 1;
	ASART_one_update<<<num_blocks, BLOCKDIM>>>(ang, zshift, z_start, size_slc,
			ATOM_ITR_STEP);
}

