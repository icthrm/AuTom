#include "atomrecon.cuh"

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
__constant__ int slcmin[16];
//#define ICT_NORMAL
long int initGPU(int mpi_rank_id) {

	int deviceCount;
	cudaGetDeviceCount(&deviceCount);
	int gpu_id;
// #ifdef ICT_NORMAL
// 	gpu_id = mpi_rank_id % deviceCount;
// #else
// 	if (mpi_rank_id == 0) {
// 		gpu_id = 0;
// 	} else {
// 		gpu_id = 2;
// 	}
// 
// #endif
#ifdef ICT_NORMAL
	if (mpi_rank_id == 0) {
		gpu_id = 0;
		printf("Using GPU 0\n");
	} else {
		gpu_id = 2;
		printf("Using GPU 2\n");
	}
#else
	gpu_id = mpi_rank_id % deviceCount;
#endif
	cudaSetDevice(gpu_id);
	//cudaDeviceReset();
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
	return 0;
}
int initMalloc(ImageStruct & dev_img, int line_num, int prj_AngN,
		int pixel_num,int slicenum) {

	//CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img_ptr, sizeof(ImageStruct)));

	int total_used_mem=0;
	CUDA_CHECK_RETURN(
			cudaMalloc((void ** )&dev_img.prj_real, sizeof(float) * line_num*slicenum));


	//total_used_mem += sizeof(float) * prj_AngN * 6;

	CUDA_CHECK_RETURN(
			cudaMalloc((void ** )&dev_img.cosang,
					sizeof(float) * prj_AngN));
	CUDA_CHECK_RETURN(
				cudaMalloc((void ** )&dev_img.sinang,
						sizeof(float) * prj_AngN));
	CUDA_CHECK_RETURN(
			cudaMalloc((void ** )&dev_img.slc.data, sizeof(float) * pixel_num*slicenum));
	CUDA_CHECK_RETURN(
			cudaMalloc((void ** )&dev_img.slc.c, sizeof(float) * pixel_num*slicenum));
	CUDA_CHECK_RETURN(
			cudaMalloc((void ** )&dev_img.slc.s, sizeof(float) * pixel_num*slicenum));
	cudaMalloc((void ** )&dev_img.slc.min, sizeof(int) *slicenum);
	cudaMalloc((void ** )&dev_img.slc.max, sizeof(int) *slicenum);

//	total_used_mem+=sizeof(float) * line_num *4*slicenum;
//	total_used_mem+=sizeof(float) * prj_AngN ;
	//printf("total_used_mem %d \n",total_used_mem/1024/1024);
	cudaMemcpyToSymbol((const void*) &dev_const_img, (void*) &dev_img,sizeof(ImageStruct));
	return 0;
}
void BPT_hostmalloc(float **data,int prjsize,float **slcdata,int slcsize)
{
	cudaHostAlloc( (void**)&(*data),prjsize,cudaHostAllocDefault);
	cudaHostAlloc( (void**)&(*slcdata),slcsize,cudaHostAllocDefault);
}
int SIRT_initMalloc(ImageStruct & dev_img,  int line_num,int prj_AngN,int pixel_num,int slicenum
		) {


	//CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img_ptr, sizeof(ImageStruct)));

	CUDA_CHECK_RETURN(
			cudaMalloc((void ** )&dev_img.prj_real, sizeof(float) * line_num*slicenum));

	//total_used_mem += sizeof(float) * prj_AngN * 6;

	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.cosang,sizeof(float) * prj_AngN));
	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.sinang,sizeof(float) * prj_AngN));

	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.slc.data, sizeof(float)*pixel_num*slicenum));
	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.slc.c, sizeof(float)*pixel_num*slicenum));
	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.slc.s, sizeof(float)*pixel_num*slicenum));
	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.prj_calc, sizeof(float)*line_num*slicenum));
	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.prj_calc_num, sizeof(float)*line_num*slicenum));
	cudaMalloc((void ** )&dev_img.slc.min, sizeof(int) *slicenum);
	cudaMemcpyToSymbol((const void*) &dev_const_img, (void*) &dev_img, sizeof(ImageStruct));
	return 0;
}

int SART_initMalloc(ImageStruct & dev_img, int line_num, int prj_AngN,
		int pixel_num,int slicenum) {

	CUDA_CHECK_RETURN(
			cudaMalloc((void ** )&dev_img.prj_real, sizeof(float) * line_num*slicenum));
	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.cosang,sizeof(float) * prj_AngN));
	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.sinang,sizeof(float) * prj_AngN));

	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.slc.data, sizeof(float)*pixel_num*slicenum));
	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.slc.c, sizeof(float)*pixel_num*slicenum));
	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.slc.s, sizeof(float)*pixel_num*slicenum));
	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.prj_calc, sizeof(float)*line_num*slicenum));
	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.prj_calc_num, sizeof(float)*line_num*slicenum));
	cudaMalloc((void ** )&dev_img.slc.min, sizeof(int) *slicenum);
	cudaMemcpyToSymbol((const void*) &dev_const_img, (void*) &dev_img, sizeof(ImageStruct));
	return 0;
}


void initDevMem(float *dev_data, int size) {

	CUDA_CHECK_RETURN(cudaMemset(dev_data, 0, size));
}
void initDevVol(float *dev_data, int size) {

	CUDA_CHECK_RETURN(cudaMemset(dev_data, 0, size));
}
void initDevfloat(float *dev_data, int size) {

	CUDA_CHECK_RETURN(cudaMemset(dev_data, 0, size));
}
void transferToDevtilt(float *dev_data, float *host_data, int size) {
	CUDA_CHECK_RETURN(
			cudaMemcpy(dev_data, host_data, size, cudaMemcpyHostToDevice));
}

void transferToDevPro(float *dev_prj_real, float *prj_real, int size) {

/*	cudaEvent_t start1;
		cudaEventCreate(&start1);
		cudaEvent_t stop1;
		cudaEventCreate(&stop1);
		cudaEventRecord(start1, NULL);*/
	CUDA_CHECK_RETURN(
			cudaMemcpy(dev_prj_real, prj_real, size, cudaMemcpyHostToDevice));
/*	 cudaEventRecord(stop1, NULL);
		cudaEventSynchronize(stop1);
		float msecTotal1 = 0.0f;
		cudaEventElapsedTime(&msecTotal1, start1, stop1);
	        printf("copy 1 : %f\n ",msecTotal1);*/
}
void transferIntToDev(int *dev_data, int *host_data, int size) {
	CUDA_CHECK_RETURN(cudaMemcpy(dev_data, host_data, size, cudaMemcpyHostToDevice));

}
void transferConst(int *host_slcmin,int size)
{

	cudaMemcpyToSymbol(slcmin, host_slcmin, size);

	//cudaMemcpyToSymbol((const void*) &slcmin, (void*) &host_slcmin, size);
}
void transferToDevVol(float *dev_slc_data, float *slc_data, int size) {
/*	cudaEvent_t start1;
			cudaEventCreate(&start1);
			cudaEvent_t stop1;
			cudaEventCreate(&stop1);
			cudaEventRecord(start1, NULL);*/
	CUDA_CHECK_RETURN(
			cudaMemcpy(dev_slc_data, slc_data, size, cudaMemcpyHostToDevice));
	/*	 cudaEventRecord(stop1, NULL);
			cudaEventSynchronize(stop1);
			float msecTotal1 = 0.0f;
			cudaEventElapsedTime(&msecTotal1, start1, stop1);
		        printf("copy back : %f\n ",msecTotal1);*/
}
__device__ float lineindex_value(const float &cosang, const float &sinang ,const int &x, const int &z,
		const int &z0, const int &x0) {
	return ((x - x0 + 0.5f) * cosang + (z - z0 + 0.5f) * sinang + x0);
}

__device__ inline void atomicAddF(float* address, float value) {
	while (value)
		value = atomicExch(address, value + atomicExch(address, 0.0f));

}


__global__ void Bpt_Slice_backprj_kernel(int pixel_num, float *prj_real,
		Slice slc, Slide slid,int slicenum,int line_num) {

	const long long n = (blockIdx.x << LOG_BLOCKDIM) + threadIdx.x;

	int realslicenum=n/pixel_num;
/*	if(n==0){
	printf("%d %d %d %d \n",slcmin[0],slcmin[1],slcmin[2],slcmin[3]);
	}*/
	int angindex = blockIdx.y;
	//printf("GPU info --%d  total :%d \n",n,total_num_slc);
	if (n < pixel_num*slicenum) {

		float rf,ang,weight;
		int row_index;
		long long imageindex=n;

	//	pixel.Z = (n) / (dev_const_img.vol.X );
	//	pixel.Y = (n) % (dev_const_img.vol.X * Y_per) / dev_const_img.vol.X;
	//	pixel.X = (n) % dev_const_img.vol.X;

		//imageindex=pixel.X+(j-Y_start)*dev_const_img.vol.X+pixel.Z*dev_const_img.vol.X*Y_per;
		int realindex=n-pixel_num*realslicenum;
		int m = realindex + slcmin[realslicenum];
		//int m = realindex + slc.min[realslicenum];
		int z = m / slid.X;
		int x = m - z * slid.X;
		float temp;

		//ang = D2R(-dev_const_img.tilt_angle[angindex]);
		rf = (x - slid.x0 + 0.5) * dev_const_img.cosang[angindex] + (z - slid.z0 + 0.5) * dev_const_img.sinang[angindex]+ slid.x0;
		int r = floor(rf - 0.5);
		weight = 1.5 - (rf - r);
		//  r+=slid->x0;

		if (r >= 0 && r < slid.X - 1) {
			atomicAdd(&slc.c[imageindex],1.0);
			row_index = angindex * slid.X + r+line_num*realslicenum;
			temp=weight * prj_real[row_index]+ (1 - weight) * prj_real[row_index + 1];
			atomicAdd(&slc.s[imageindex],temp);

		} else if (r == slid.X - 1) {
			atomicAdd(&slc.c[imageindex],weight);
			row_index = angindex * slid.X + r+line_num*realslicenum;
			temp=weight * prj_real[row_index];
			atomicAdd(&slc.s[imageindex],temp);

		} else if (r == -1) {
			atomicAdd(&slc.c[imageindex], 1 - weight);
			row_index = angindex * slid.X+line_num*realslicenum;
			temp= (1 - weight) * prj_real[row_index];
			atomicAdd(&slc.s[imageindex],temp);

		}
	}

}
__global__ void Bpt_Slice_divide_kernel(int pixel_num,Slice slc,int slicenum) {

	long long  n = (blockIdx.x << LOG_BLOCKDIM) + threadIdx.x;

	if (n < pixel_num*slicenum) {

		float tempc=slc.c[n];
		if (tempc != 0){
			slc.data[n] = slc.s[n]/tempc;
			//slc.data[imageindex] = (float)slc.c[imageindex];
		}
			//slc.data[imageindex] = slc.s[imageindex] / slc.c[imageindex];
	}
    /*if (n < pixel_num*slicenum) {
	slc.data[n]=10;
	}*/
}
void transferToHost(float *dev_slc_data,float *slc_data,int size)
{
	CUDA_CHECK_RETURN(
			cudaMemcpy(slc_data, dev_slc_data, size ,cudaMemcpyDeviceToHost));
}
void Bpt_Slice_backproj(int pixel_num, int prj_angN, long long  Y_per, float *prj_real,
		Slice slc, Slide slid,int slicenum,int line_num) {
/*	cudaEvent_t start1;
		cudaEventCreate(&start1);
		cudaEvent_t stop1;
		cudaEventCreate(&stop1);
		cudaEventRecord(start1, NULL);*/


	int num_blocks = (pixel_num - 1) / BLOCKDIM + 1;
	dim3 dim2block(num_blocks*slicenum, prj_angN);
	//printf("pixel_num=%d prjnum=%d \n ", pixel_num, prjnum);
	Bpt_Slice_backprj_kernel<<<dim2block, BLOCKDIM>>>(pixel_num, prj_real, slc,
			slid, slicenum,line_num);
	CUDA_CHECK_RETURN(cudaDeviceSynchronize());
	CUDA_CHECK_RETURN(cudaGetLastError());/*
    cudaEventRecord(stop1, NULL);
	cudaEventSynchronize(stop1);
	float msecTotal1 = 0.0f;
	cudaEventElapsedTime(&msecTotal1, start1, stop1);
        printf("kernel 1 : %f\n ",msecTotal1);*/
}
void Bpt_Slice_divide(int total_num_slc,Slice slc,int slicenum) {
	int num_blocks = (total_num_slc - 1) / BLOCKDIM + 1;
/*	cudaEvent_t start1;
			cudaEventCreate(&start1);
			cudaEvent_t stop1;
			cudaEventCreate(&stop1);
			cudaEventRecord(start1, NULL);
	//printf("pixel_num=%d prjnum=%d \n ", pixel_num, prjnum);*/
	Bpt_Slice_divide_kernel<<<num_blocks*slicenum, BLOCKDIM>>>( total_num_slc,slc,slicenum);
	CUDA_CHECK_RETURN(cudaDeviceSynchronize());
	CUDA_CHECK_RETURN(cudaGetLastError());

/*	 cudaEventRecord(stop1, NULL);
		cudaEventSynchronize(stop1);
		float msecTotal1 = 0.0f;
		cudaEventElapsedTime(&msecTotal1, start1, stop1);
	        printf("kernel 2 : %f\n ",msecTotal1);*/

}
__global__ void SIRT_Slice_reproj(float *d_prj_calc, float *d_prj_num,
		float *d_slc_data, Slice slc, Slide slid, int AngN,int slicenum,int linenum) //A*X
		{
	int pixel_allnum = dev_const_img.vol.X*dev_const_img.vol.Z;
	const long long  n = (blockIdx.x << LOG_BLOCKDIM) + threadIdx.x; //pixel index and thread index
	int realslice=n/pixel_allnum;
	long long m;
	int j, z, x, row_index;
	float ang, rf, weight;
	int r;
	//j= blockIdx.y;

//	int pixel_allnum = slc.X * slc.Z;

	if (n < pixel_allnum*slicenum) {
		long long  realindex=n-(realslice*pixel_allnum);
		m = realindex + slcmin[realslice];
		//float *prjreal=prj_real+line_num*realslicenum;

		z = (int) __fdividef(m, slid.X);
		x = m - z * slid.X;
		for (j = 0; j < AngN; j++) {
			//ang = PI_180 * c_ang[j];


			rf = lineindex_value(dev_const_img.cosang[j],dev_const_img.sinang[j], x, z, slid.z0, slid.x0);
			r = floor(rf - 0.5);
			weight = 1.5 - (rf - r);
			//r += slc.x0;
			if (r >= 0 && r < slid.X - 1) {
				row_index = j *  slid.X + r+linenum*realslice;
				atomicAdd(&d_prj_num[row_index], weight);
				atomicAdd(&d_prj_num[row_index + 1], 1 - weight);
				atomicAdd(&d_prj_calc[row_index], weight * d_slc_data[n]);
				atomicAdd(&d_prj_calc[row_index + 1],(1 - weight) * d_slc_data[n]);
			} //end if weight
			else if (r == slid.X - 1) //left
					{
				row_index = j * slid.X + r+linenum*realslice;
				atomicAdd(&d_prj_num[row_index], weight);
				atomicAdd(&d_prj_calc[row_index], weight * d_slc_data[n]);
			} else if (r == -1) //right
					{
				row_index = j * slid.X+linenum*realslice;
				atomicAdd(&d_prj_num[row_index+1], 1 - weight);
				atomicAdd(&d_prj_calc[row_index+1],(1 - weight) * d_slc_data[n]);
			}

		} //end for j
	} //end if m

}

__global__ void proj_calc_div(float *d_prj_calc, float *d_prj_num,
		int line_allnum) {
	const long long  r = (blockIdx.x << LOG_BLOCKDIM) + threadIdx.x; //prj index
	float s, c;
	s = d_prj_calc[r];
	c = d_prj_num[r];
	if ((r < line_allnum) && (c != 0)) {
		d_prj_calc[r] = s/c;
	}
}
__global__ void SIRT_one_update(float *d_prj_real, float *d_prj_calc,
		float *d_slc_data, int AngN, Slice slc, Slide slid, float SIRT_ITR_STEP,
		int slicenum,int linenum) {
	const long long n = (blockIdx.x << LOG_BLOCKDIM) + threadIdx.x; //pixel index and thread index
	long long m;

	int j, z, x, row_index;
	float c = 0.0f;
	float s = 0.0f;
	float ang, rf, weight;
	int r;

	long long pixel_allnum = dev_const_img.vol.X * dev_const_img.vol.Z;
	int realslice=n/pixel_allnum;
	//j= blockIdx.y;
	if (n < pixel_allnum*slicenum) {
		long long  realindex=n-(realslice*pixel_allnum);
		m = realindex + slcmin[realslice];


		z = (int) __fdividef(m, slid.X);
		x = m - z * slid.X;
		for (j = 0; j < AngN; j++) {

			rf = lineindex_value(dev_const_img.cosang[j],dev_const_img.sinang[j], x, z, slid.z0, slid.x0);
			r = floor(rf - 0.5);
			weight = 1.5 - (rf - r);
			//	r += slc.x0;
			if (r >= 0 && r < slid.X - 1) {
				c++;
				row_index = j * slid.X + r+linenum*realslice;
				s += weight * (d_prj_real[row_index] - d_prj_calc[row_index]);
				s +=(1 - weight)
								* (d_prj_real[row_index + 1]
										- d_prj_calc[row_index + 1]);
			} //end if
			else if (r == slid.X - 1) {
				c += weight;
				row_index = j * slid.X + r+linenum*realslice;
				s += weight * (d_prj_real[row_index] - d_prj_calc[row_index]);
			} else if (r == -1) {
				c += 1 - weight;
				row_index = j * slid.X+linenum*realslice;
				s += (1 - weight)
						* (d_prj_real[row_index] - d_prj_calc[row_index]);
			}
		} //end for j

		if (c != 0)
			d_slc_data[n] += __fdividef(s, c) * SIRT_ITR_STEP;
	} //end if m
}
int Cu_SIRT_update_slice(float *d_prj_real, float *d_prj_calc,
		float *d_prj_num, float *d_slc_data, int AngN, Slice slc, Slide slid,
		int pixel_num, int prjX,float SIRT_ITR_STEP,int slicenum) {
	int line_allnum =prjX* AngN *slicenum;
	int num_blocks;
	int linenum=prjX* AngN ;
	//unsigned int timer = 0;
	num_blocks = ((pixel_num *slicenum)+ BLOCKDIM - 1) >> LOG_BLOCKDIM;

	SIRT_Slice_reproj<<<dim3(num_blocks), dim3(BLOCKDIM)>>>(d_prj_calc, d_prj_num,d_slc_data, slc, slid, AngN,slicenum,linenum);
	CUDA_CHECK_RETURN(cudaDeviceSynchronize());
	num_blocks = (line_allnum + BLOCKDIM - 1) >> LOG_BLOCKDIM;
	proj_calc_div<<<dim3(num_blocks), dim3(BLOCKDIM)>>>(d_prj_calc, d_prj_num,
			line_allnum);
	CUDA_CHECK_RETURN(cudaDeviceSynchronize());


	num_blocks = ((pixel_num *slicenum) + BLOCKDIM - 1) >> LOG_BLOCKDIM;
	SIRT_one_update<<<dim3(num_blocks), dim3(BLOCKDIM)>>>(d_prj_real,d_prj_calc, d_slc_data, AngN, slc, slid, SIRT_ITR_STEP,slicenum,linenum);
	CUDA_CHECK_RETURN(cudaDeviceSynchronize());
	return TRUE;
}

__global__ void SART_Slice_reproj(float *d_prj_calc, float *d_prj_num,
		float *d_slc_data, Slice slc, Slide slid, int angleindex,int slicenum,int linenum,int slice) //A*X
		{
	const long long n = (blockIdx.x << LOG_BLOCKDIM) + threadIdx.x; //pixel index and thread index
	int pixel_allnum = dev_const_img.vol.X*dev_const_img.vol.Z;
	long long m;
	long long z, x, row_index;
	float ang, rf, weight;
	long long r;
	int realslice=n/pixel_allnum;

//	int pixel_allnum = slc.X * slc.Z;

	if (n < pixel_allnum*slicenum) {
		long long  realindex=n-realslice*pixel_allnum;
		m = realindex + slcmin[realslice];
		float *d_prj_num_real=d_prj_num+linenum*realslice;
		float *d_prj_calc_real=d_prj_calc+linenum*realslice;
		z = (int) __fdividef(m, slid.X);
		x = m - z * slid.X;

		rf = lineindex_value(dev_const_img.cosang[angleindex],dev_const_img.sinang[angleindex], x, z, slid.z0, slid.x0);
		r = floor(rf - 0.5);
		weight = 1.5 - (rf - r);

		//r += slc.x0;
		if (r >= 0 && r < slid.X - 1) {
			row_index = angleindex * slid.X + r;

			atomicAdd(&d_prj_num_real[row_index], weight);
			atomicAdd(&d_prj_num_real[row_index + 1], 1 - weight);
			atomicAdd(&d_prj_calc_real[row_index], weight * d_slc_data[n]);
			atomicAdd(&d_prj_calc_real[row_index + 1], (1 - weight) * d_slc_data[n]);

		} //end if weight
		else if (r == slid.X - 1) //left
				{
			row_index = angleindex * slid.X + r;
			atomicAdd(&d_prj_num_real[row_index], weight);
			atomicAdd(&d_prj_calc_real[row_index], weight * d_slc_data[n]);

		} else if (r == -1) //right
				{
			row_index = angleindex * slid.X;
			atomicAdd(&d_prj_num_real[row_index + 1], 1 - weight);
			atomicAdd(&d_prj_calc_real[row_index + 1], (1 - weight) * d_slc_data[n]);

		}

//			printf(" GPU The result: %d  %f\n",n,d_slc_data[n]);
	} //end if n
}
__global__ void SART_proj_calc_div(float *d_prj_calc, float *d_prj_num,
		int angleindex,float *d_prj_real,int slicenum,int prjnum) {
	const long long r = (blockIdx.x << LOG_BLOCKDIM) + threadIdx.x; //prj index
	int realslice=r/dev_const_img.prj.X;
	long long realindex=r-realslice*dev_const_img.prj.X;
	long long prj_index=realindex+angleindex*dev_const_img.prj.X+realslice*prjnum;

	float s, c;
	s = d_prj_calc[prj_index];
	c = d_prj_num[prj_index];
	if ((r < dev_const_img.prj.X*slicenum)) {
		if(c!=0){
			d_prj_calc[prj_index] = __fdividef(s, c);
		}
		d_prj_calc[prj_index]=d_prj_real[prj_index]-d_prj_calc[prj_index];
	}
}
__global__ void SART_one_update(float *d_prj_calc,
		float *d_slc_data, int angleindex, Slice slc, Slide slid, float SART_ITR_STEP,
		int slicenum,int linenum) {
	const long long n = (blockIdx.x << LOG_BLOCKDIM) + threadIdx.x; //pixel index and thread index
	int pixel_allnum = dev_const_img.vol.X * dev_const_img.vol.Z;
	long long m;
	long long z, x, row_index;
	float c = 0.0f;
	float s = 0.0f;
	float ang, rf, weight;
	long long r;
	int realslice=n/pixel_allnum;
	int realindex=n-realslice*pixel_allnum;
	m = realindex + slcmin[realslice];

	if (n < pixel_allnum*slicenum) {


		float *d_prj_calc_real=d_prj_calc+linenum*realslice;


		z = (int) __fdividef(m, slid.X);
		x = m - z * slid.X;

		rf = lineindex_value(dev_const_img.cosang[angleindex],dev_const_img.sinang[angleindex], x, z, slid.z0, slid.x0);
		r = floor(rf - 0.5);
		weight = 1.5 - (rf - r);
		//	r += slc.x0;

		if (r >= 0 && r < slid.X - 1) {
			c++;
			row_index = angleindex * slid.X + r;
			s += weight * (d_prj_calc_real[row_index]);
			s += (1 - weight)
					* (d_prj_calc_real[row_index + 1]);
		} //end if
		else if (r == slid.X - 1) {
			c += weight;
			row_index = angleindex * slid.X + r;
			s += weight * (d_prj_calc_real[row_index]);
		} else if (r == -1) {
			c += 1 - weight;
			row_index = angleindex * slid.X;
			s += (1 - weight) * (d_prj_calc_real[row_index]);
		}

		if (c != 0)
			d_slc_data[n] += __fdividef(s, c) * SART_ITR_STEP;

	} //end if m
}

int Cu_SART_update_slice(float *d_prj_real, float *d_prj_calc, float *d_prj_num,
		float *d_slc_data, int AngN, Slice slc, Slide slid, int pixel_num, int prjX,
		float SART_ITR_STEP ,int slicenum,int slice) {
	int angle;
	int num_blocks;
	int linenum=prjX* AngN ;

	for (angle = 0; angle < AngN; angle++) {

		//unsigned int timer = 0;
		num_blocks = (pixel_num*slicenum + BLOCKDIM - 1) >> LOG_BLOCKDIM;
		SART_Slice_reproj<<<dim3(num_blocks), dim3(BLOCKDIM)>>>(d_prj_calc,
				d_prj_num, d_slc_data, slc, slid, angle, slicenum,linenum,slice);

		CUDA_CHECK_RETURN(cudaDeviceSynchronize());
		num_blocks = ((prjX *slicenum)+ BLOCKDIM - 1) >> LOG_BLOCKDIM;
		SART_proj_calc_div<<<dim3(num_blocks), dim3(BLOCKDIM)>>>(d_prj_calc,
				d_prj_num, angle, d_prj_real,slicenum,linenum);
		CUDA_CHECK_RETURN(cudaDeviceSynchronize());
		num_blocks = (pixel_num *slicenum+ BLOCKDIM - 1) >> LOG_BLOCKDIM;
		SART_one_update<<<dim3(num_blocks), dim3(BLOCKDIM)>>>(
				d_prj_calc, d_slc_data, angle, slc, slid, SART_ITR_STEP, slicenum,linenum);
		CUDA_CHECK_RETURN(cudaDeviceSynchronize());

	} // end for angle
	return 0;
}


void freeDevMem_float(float *dev_data) {
	cudaFree(dev_data);
}
void freeDevMem_int(int *dev_data) {
	cudaFree(dev_data);
}

void freeDevProjection(float *prj) {
	cudaFree(prj);
}
void freeDevSlc(float *slc_data) {
	cudaFree(slc_data);
}

