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
int initMalloc(ImageStruct & dev_img,  int line_num,int prj_AngN,int pixel_num,
		long int total_mem) {
	long int total_used_mem = 0;

	//CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img_ptr, sizeof(ImageStruct)));
	total_used_mem += sizeof(ImageStruct);
	CUDA_CHECK_RETURN(
			cudaMalloc((void ** )&dev_img.prj_real, sizeof(float) * line_num));

	//total_used_mem += sizeof(double) * prj_AngN * 6;


	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.tilt_angle, sizeof(double)*prj_AngN));
	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.slc.data, sizeof(float)*pixel_num));
	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.slc.c, sizeof(double)*pixel_num));
	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.slc.s, sizeof(double)*pixel_num));
	cudaMemcpyToSymbol((const void*) &dev_const_img, (void*) &dev_img, sizeof(ImageStruct));
	return 0;
}
int SIRT_initMalloc(ImageStruct & dev_img,  int line_num,int prj_AngN,int pixel_num,
		long int total_mem) {
	long int total_used_mem = 0;

	//CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img_ptr, sizeof(ImageStruct)));
	total_used_mem += sizeof(ImageStruct);
	CUDA_CHECK_RETURN(
			cudaMalloc((void ** )&dev_img.prj_real, sizeof(float) * line_num));

	//total_used_mem += sizeof(double) * prj_AngN * 6;
	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.tilt_angle, sizeof(double)*prj_AngN));

	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.slc.data, sizeof(float)*pixel_num));
	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.slc.c, sizeof(double)*pixel_num));
	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.slc.s, sizeof(double)*pixel_num));
	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.prj_calc, sizeof(float)*line_num));
	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.prj_calc_num, sizeof(double)*line_num));
	cudaMemcpyToSymbol((const void*) &dev_const_img, (void*) &dev_img, sizeof(ImageStruct));
	return 0;
}
int ASIRT_initMalloc(ImageStruct & dev_img,  int line_num,int prj_AngN,int pixel_num,
		long int total_mem) {
	long int total_used_mem = 0;

	total_used_mem += sizeof(ImageStruct);
	CUDA_CHECK_RETURN(
			cudaMalloc((void ** )&dev_img.prj_real, sizeof(float) * line_num));

	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.tilt_angle, sizeof(double)*prj_AngN));

	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.slc.data, sizeof(float)*pixel_num));
	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.slc.c, sizeof(double)*pixel_num));
	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.slc.s, sizeof(double)*pixel_num));
	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.prj_calc, sizeof(float)*line_num));
	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.prj_calc_num, sizeof(double)*line_num));
	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.prj_calc_num_abs, sizeof(double)*line_num));
	cudaMemcpyToSymbol((const void*) &dev_const_img, (void*) &dev_img, sizeof(ImageStruct));
	return 0;
}
int SART_initMalloc(ImageStruct & dev_img, int line_num, int prj_AngN,
		int pixel_num) {

	CUDA_CHECK_RETURN(
			cudaMalloc((void ** )&dev_img.prj_real, sizeof(float) * line_num));
	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.tilt_angle, sizeof(double)*prj_AngN));

	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.slc.data, sizeof(float)*pixel_num));
	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.slc.c, sizeof(double)*pixel_num));
	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.slc.s, sizeof(double)*pixel_num));
	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.prj_calc, sizeof(float)*line_num));
	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.prj_calc_num, sizeof(double)*line_num));
	cudaMemcpyToSymbol((const void*) &dev_const_img, (void*) &dev_img, sizeof(ImageStruct));
	return 0;
}
int ASART_initMalloc(ImageStruct & dev_img, int line_num, int prj_AngN,
		int pixel_num) {

	CUDA_CHECK_RETURN(
			cudaMalloc((void ** )&dev_img.prj_real, sizeof(float) * line_num));
	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.tilt_angle, sizeof(double)*prj_AngN));

	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.slc.data, sizeof(float)*pixel_num));
	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.slc.c, sizeof(double)*pixel_num));
	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.slc.s, sizeof(double)*pixel_num));
	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.prj_calc, sizeof(float)*line_num));
	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.prj_calc_num, sizeof(double)*line_num));
	CUDA_CHECK_RETURN(cudaMalloc((void ** )&dev_img.prj_calc_num_abs, sizeof(double)*line_num));
	cudaMemcpyToSymbol((const void*) &dev_const_img, (void*) &dev_img, sizeof(ImageStruct));
	return 0;
}

void initDevMem(double *dev_data, int size) {

	CUDA_CHECK_RETURN(cudaMemset(dev_data, 0, size));
}
void initDevVol(float *dev_data, int size) {

	CUDA_CHECK_RETURN(cudaMemset(dev_data, 0, size));
}
void initDevfloat(float *dev_data, int size) {

	CUDA_CHECK_RETURN(cudaMemset(dev_data, 0, size));
}
void transferToDevtilt(double *dev_data, double *host_data, int size) {
	CUDA_CHECK_RETURN(
			cudaMemcpy(dev_data, host_data, size, cudaMemcpyHostToDevice));
}

void transferToDevPro(float *dev_prj_real, float *prj_real, int size) {
	CUDA_CHECK_RETURN(
			cudaMemcpy(dev_prj_real, prj_real, size, cudaMemcpyHostToDevice));
}
void transferToDevVol(float *dev_slc_data, float *slc_data, int size) {
	CUDA_CHECK_RETURN(
			cudaMemcpy(dev_slc_data, slc_data, size, cudaMemcpyHostToDevice));
}
__device__ double lineindex_value(const double &ang, const int &x, const int &z,
		const int &z0, const int &x0) {
	return ((x - x0 + 0.5f) * __cosf(ang) + (z - z0 + 0.5f) * __sinf(ang) + x0);
}
__device__ inline void atomicAddF(float* address, float value) {
	while (value)
		value = atomicExch(address, value + atomicExch(address, 0.0f));

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

__global__ void Bpt_Slice_backprj_kernel(int total_num_slc, int Y_per, float *prj_real,
		Slice slc, Slide slid,int Y_start,int j) {

	const unsigned int n = (blockIdx.x << LOG_BLOCKDIM) + threadIdx.x;
	int angindex = blockIdx.y;
	//printf("GPU info --%d  total :%d \n",n,total_num_slc);
	if (n < total_num_slc) {
		double rf,ang,weight;
		int row_index;
		int imageindex;

	//	pixel.Z = (n) / (dev_const_img.vol.X );
	//	pixel.Y = (n) % (dev_const_img.vol.X * Y_per) / dev_const_img.vol.X;
	//	pixel.X = (n) % dev_const_img.vol.X;

		//imageindex=pixel.X+(j-Y_start)*dev_const_img.vol.X+pixel.Z*dev_const_img.vol.X*Y_per;
		imageindex=n;
		int m = n + slc.min;
		int z = m / slid.X;
		int x = m - z * slid.X;
		double temp;
		ang = D2R(-dev_const_img.tilt_angle[angindex]);

		rf = (x - slid.x0 + 0.5) * cos(ang) + (z - slid.z0 + 0.5) * sin(ang)
				+ slid.x0;
		int r = floor(rf - 0.5);
		weight = 1.5 - (rf - r);
		//  r+=slid->x0;
		if(j==100&&n==0&&(angindex==6||angindex==7))
			printf("Test -weight-GPU-weight %f r %d %f %f\n",weight,r,rf,ang);
		if (r >= 0 && r < slid.X - 1) {
			atomicAdd(&slc.c[imageindex],1.0);
			row_index = angindex * slid.X + r;
			temp=weight * prj_real[row_index]+ (1 - weight) * prj_real[row_index + 1];
			atomicAdd(&slc.s[imageindex],temp);
			if(j==100&&n==0)
				 printf("Test -0-0-1-GPU- %d %f %f %f %f\n",row_index,prj_real[row_index],prj_real[row_index+1],slc.s[n],weight);
		} else if (r == slid.X - 1) {
			atomicAdd(&slc.c[imageindex],weight);
			row_index = angindex * slid.X + r;
			temp=weight * prj_real[row_index];
			atomicAdd(&slc.s[imageindex],temp);
			if(j==100&&n==0)
						 printf("Test -0-0-2-GPU- %d %f %f %f\n",row_index,prj_real[row_index],prj_real[row_index+1],slc.s[n]);
		} else if (r == -1) {
			atomicAdd(&slc.c[imageindex], 1 - weight);
			row_index = angindex * slid.X;
			temp= (1 - weight) * prj_real[row_index];
			atomicAdd(&slc.s[imageindex],temp);
			if(j==100&&n==0)
						 printf("Test -0-0-3-GPU- %d %f %f %f\n",row_index,prj_real[row_index],prj_real[row_index+1],slc.s[n]);
		}
	}

}
__global__ void Bpt_Slice_divide_kernel(int total_num_slc,Slice slc,int Y_start,int j,int Y_per) {

	const unsigned int n = (blockIdx.x << LOG_BLOCKDIM) + threadIdx.x;

	if (n < total_num_slc) {

		Pixel pixel;
		int imageindex;
		pixel.Z = (n) / (dev_const_img.vol.X);
		//	pixel.Y = (n) % (dev_const_img.vol.X * Y_per) / dev_const_img.vol.X;
		pixel.X = (n) % dev_const_img.vol.X;

		imageindex = pixel.X + (j - Y_start) * dev_const_img.vol.X
				+ pixel.Z * dev_const_img.vol.X * Y_per;
		imageindex=n;
		if (slc.c[imageindex] != 0){
			slc.data[imageindex] = slc.s[imageindex]/slc.c[imageindex];
			//slc.data[imageindex] = (float)slc.c[imageindex];
		}
			//slc.data[imageindex] = slc.s[imageindex] / slc.c[imageindex];
	}
}
void transferToHost(float *dev_slc_data,float *slc_data,int size)
{
	CUDA_CHECK_RETURN(
			cudaMemcpy(slc_data, dev_slc_data, size ,cudaMemcpyDeviceToHost));
}
void Bpt_Slice_backproj(int pixel_num, int prj_angN, int Y_per, float *prj_real,
		Slice slc, Slide slid,int Y_start,int j) {
	int num_blocks = (pixel_num - 1) / BLOCKDIM + 1;
	dim3 dim2block(num_blocks, prj_angN);
	//printf("pixel_num=%d prjnum=%d \n ", pixel_num, prjnum);
	Bpt_Slice_backprj_kernel<<<dim2block, BLOCKDIM>>>(pixel_num, Y_per, prj_real, slc,
			slid,Y_start,j);
	CUDA_CHECK_RETURN(cudaThreadSynchronize());
	CUDA_CHECK_RETURN(cudaGetLastError());
}
void Bpt_Slice_divide(int total_num_slc,Slice slc,int Y_start,int j,int Y_per) {
	int num_blocks = (total_num_slc - 1) / BLOCKDIM + 1;
	//printf("pixel_num=%d prjnum=%d \n ", pixel_num, prjnum);
	Bpt_Slice_divide_kernel<<<num_blocks, BLOCKDIM>>>( total_num_slc,slc,Y_start,j, Y_per);

}
__global__ void SIRT_Slice_reproj(float *d_prj_calc, float *d_prj_num,
		float *d_slc_data, Slice slc, Slide slid, int AngN,int jslide) //A*X
		{
	const unsigned int n = (blockIdx.x << LOG_BLOCKDIM) + threadIdx.x; //pixel index and thread index
	unsigned int m;
	int j, z, x, row_index;
	double ang, rf, weight;
	int r;
	m = n + slc.min;
//	int pixel_allnum = slc.X * slc.Z;
	int pixel_allnum = dev_const_img.vol.X*dev_const_img.vol.Z;
	if (n < pixel_allnum) {

		z = (int) __fdividef(m, slid.X);
		x = m - z * slid.X;
		for (j = 0; j < AngN; j++) {
			//ang = PI_180 * c_ang[j];
	        ang = D2R(-dev_const_img.tilt_angle[j]);
			rf = lineindex_value(ang, x, z, slid.z0, slid.x0);
			r = floor(rf - 0.5);
			weight = 1.5 - (rf - r);
			//r += slc.x0;
			if (r >= 0 && r < slid.X - 1) {
				row_index = j *  slid.X + r;
				atomicAdd(&d_prj_num[row_index], weight);
				atomicAdd(&d_prj_num[row_index + 1], 1 - weight);
				atomicAdd(&d_prj_calc[row_index], weight * d_slc_data[n]);
				atomicAdd(&d_prj_calc[row_index + 1],
						(1 - weight) * d_slc_data[n]);
			} //end if weight
			else if (r == slid.X - 1) //left
					{
				row_index = j * slid.X + r;
				atomicAdd(&d_prj_num[row_index], weight);
				atomicAdd(&d_prj_calc[row_index], weight * d_slc_data[n]);
			} else if (r == -1) //right
					{
				row_index = j * slid.X;
				atomicAdd(&d_prj_num[row_index+1], 1 - weight);
				atomicAdd(&d_prj_calc[row_index+1],
						(1 - weight) * d_slc_data[n]);
			}

		} //end for j
	} //end if m

}
__global__ void proj_calc_div(float *d_prj_calc, float *d_prj_num,
		int line_allnum) {
	const unsigned int r = (blockIdx.x << LOG_BLOCKDIM) + threadIdx.x; //prj index
	double s, c;
	s = d_prj_calc[r];
	c = d_prj_num[r];
	if ((r < line_allnum) && (c != 0)) {
		d_prj_calc[r] = s/c;
	}
}
__global__ void SIRT_one_update(float *d_prj_real, float *d_prj_calc,
		float *d_slc_data, int AngN, Slice slc, Slide slid, float SIRT_ITR_STEP,
		int jslide) {
	const unsigned int n = (blockIdx.x << LOG_BLOCKDIM) + threadIdx.x; //pixel index and thread index
	unsigned int m;
	int j, z, x, row_index;
	double c = 0.0f;
	double s = 0.0f;
	double ang, rf, weight;
	int r;
	m = n + slc.min;
	int pixel_allnum = dev_const_img.vol.X * dev_const_img.vol.Z;
	if (n < pixel_allnum) {
		z = (int) __fdividef(m, slid.X);
		x = m - z * slid.X;
		for (j = 0; j < AngN; j++) {
			ang = D2R(-dev_const_img.tilt_angle[j]);
			rf = lineindex_value(ang, x, z, slid.z0, slid.x0);
			r = floor(rf - 0.5);
			weight = 1.5 - (rf - r);
			//	r += slc.x0;
			if (r >= 0 && r < slid.X - 1) {
				c++;
				row_index = j * slid.X + r;
				s += weight * (d_prj_real[row_index] - d_prj_calc[row_index]);
				s +=
						(1 - weight)
								* (d_prj_real[row_index + 1]
										- d_prj_calc[row_index + 1]);
			} //end if
			else if (r == slid.X - 1) {
				c += weight;
				row_index = j * slid.X + r;
				s += weight * (d_prj_real[row_index] - d_prj_calc[row_index]);
			} else if (r == -1) {
				c += 1 - weight;
				row_index = j * slid.X;
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
		int pixel_num, float SIRT_ITR_STEP,ImageStruct host_img,int jslide) {
	int line_allnum =host_img.prj.X* AngN ;
	int num_blocks;
	//unsigned int timer = 0;
	num_blocks = (pixel_num + BLOCKDIM - 1) >> LOG_BLOCKDIM;
	SIRT_Slice_reproj<<<dim3(num_blocks), dim3(BLOCKDIM)>>>(d_prj_calc, d_prj_num,
			d_slc_data, slc, slid, AngN,jslide);
	CUDA_CHECK_RETURN(cudaThreadSynchronize());
	num_blocks = (line_allnum + BLOCKDIM - 1) >> LOG_BLOCKDIM;
	proj_calc_div<<<dim3(num_blocks), dim3(BLOCKDIM)>>>(d_prj_calc, d_prj_num,
			line_allnum);
	CUDA_CHECK_RETURN(cudaThreadSynchronize());
	num_blocks = (pixel_num + BLOCKDIM - 1) >> LOG_BLOCKDIM;
	SIRT_one_update<<<dim3(num_blocks), dim3(BLOCKDIM)>>>(d_prj_real,
			d_prj_calc, d_slc_data, AngN, slc, slid, SIRT_ITR_STEP,jslide);
	CUDA_CHECK_RETURN(cudaThreadSynchronize());
	return TRUE;
}
__global__ void ASIRT_Slice_reproj(float *d_prj_calc, float *d_prj_num,
		float *d_slc_data, Slice slc, Slide slid, int AngN, int jslide,
		float *d_prj_calc_abs) //A*X
		{
	const unsigned int n = (blockIdx.x << LOG_BLOCKDIM) + threadIdx.x; //pixel index and thread index
	unsigned int m;
	int j, z, x, row_index;
	float ang, rf, weight;
	int r;
	m = n + slc.min;
//	int pixel_allnum = slc.X * slc.Z;
	int pixel_allnum = dev_const_img.vol.X * dev_const_img.vol.Z;
	if (n < pixel_allnum) {

		z = (int) __fdividef(m, slid.X);
		x = m - z * slid.X;
		for (j = 0; j < AngN; j++) {
			//ang = PI_180 * c_ang[j];
			ang = D2R(-dev_const_img.tilt_angle[j]);
			rf = lineindex_value(ang, x, z, slid.z0, slid.x0);
			r = floor(rf - 0.5);
			weight = 1.5 - (rf - r);
			//r += slc.x0;
			if (r >= 0 && r < slid.X - 1) {
				row_index = j * slid.X + r;
				atomicAdd(&d_prj_num[row_index], weight);
				atomicAdd(&d_prj_num[row_index + 1], 1 - weight);
				atomicAdd(&d_prj_calc[row_index], weight * d_slc_data[n]);
				atomicAdd(&d_prj_calc[row_index + 1],
						(1 - weight) * d_slc_data[n]);
				atomicAdd(&d_prj_calc_abs[row_index],
						weight * abs(d_slc_data[n]));
				atomicAdd(&d_prj_calc_abs[row_index + 1],
						(1 - weight) * abs(d_slc_data[n]));

			} //end if weight
			else if (r == slid.X - 1) //left
					{
				row_index = j * slid.X + r;
				atomicAdd(&d_prj_num[row_index], weight);
				atomicAdd(&d_prj_calc[row_index], weight * d_slc_data[n]);
				atomicAdd(&d_prj_calc_abs[row_index],
						weight * abs(d_slc_data[n]));

			} else if (r == -1) //right
					{
				row_index = j * slid.X;
				atomicAdd(&d_prj_num[row_index + 1], 1 - weight);
				atomicAdd(&d_prj_calc[row_index + 1],
						(1 - weight) * d_slc_data[n]);
				atomicAdd(&d_prj_calc_abs[row_index],
						(1 - weight) * abs(d_slc_data[n]));
			}
		} //end for j
	} //end if m
}
__global__ void ASIRT_proj_calc_div(float *d_prj_calc, float *d_prj_num,float *d_prj_calc_abs,
		int line_allnum) {
	const unsigned int r = (blockIdx.x << LOG_BLOCKDIM) + threadIdx.x; //prj index
	float s, c,s2;
	s = d_prj_calc[r];
	c = d_prj_num[r];
	s2=d_prj_calc_abs[r];
	if ((r < line_allnum) && (c != 0)) {
		d_prj_calc[r] = __fdividef(s, c);
		d_prj_calc_abs[r] = __fdividef(s2, c);
	}
}
__global__ void ASIRT_one_update(float *d_prj_real, float *d_prj_calc,
		float *d_slc_data, int AngN, Slice slc, Slide slid, float ASIRT_ITR_STEP,
		int jslide,float *d_prj_calc_abs) {
	const unsigned int n = (blockIdx.x << LOG_BLOCKDIM) + threadIdx.x; //pixel index and thread index
	unsigned int m;
	int j, z, x, row_index;
	float c = 0.0f;
	float s = 0.0f;
	float t=0;
	float ang, rf, weight;
	int r;
	m = n + slc.min;
	int pixel_allnum = dev_const_img.vol.X * dev_const_img.vol.Z;
	if (n < pixel_allnum) {
		z = (int) __fdividef(m, slid.X);
		x = m - z * slid.X;
		for (j = 0; j < AngN; j++) {
			ang = D2R(-dev_const_img.tilt_angle[j]);
			rf = lineindex_value(ang, x, z, slid.z0, slid.x0);
			r = floor(rf - 0.5);
			weight = 1.5 - (rf - r);
			//	r += slc.x0;
			if (r >= 0 && r < slid.X - 1) {
				c++;
				row_index = j * slid.X + r;
				s += weight * (d_prj_real[row_index] - d_prj_calc[row_index]);
				s +=(1 - weight)* (d_prj_real[row_index + 1]- d_prj_calc[row_index + 1]);
				t +=weight*d_prj_calc_abs[row_index]+(1-weight)*d_prj_calc_abs[row_index+1];
			} //end if
			else if (r == slid.X - 1) {
				c += weight;
				row_index = j * slid.X + r;
				s += weight * (d_prj_real[row_index] - d_prj_calc[row_index]);
				t+=weight*d_prj_calc_abs[row_index];
			} else if (r == -1) {
				c += 1 - weight;
				row_index = j * slid.X;
				s += (1 - weight)
						* (d_prj_real[row_index] - d_prj_calc[row_index]);
		         t+=(1-weight)*d_prj_calc_abs[row_index];
			}
		} //end for j

		if (t != 0)
			d_slc_data[n] += abs(d_slc_data[n])*(float)s/t*ASIRT_ITR_STEP;
	} //end if m
}
int Cu_ASIRT_update_slice(float *d_prj_real, float *d_prj_calc, float *d_prj_num,
		float *d_slc_data, int AngN, Slice slc, Slide slid, int pixel_num,
		float ASIRT_ITR_STEP, ImageStruct host_img, int jslide,float *d_prj_calc_abs) {
	int line_allnum =host_img.prj.X* AngN ;
	int num_blocks;
	//unsigned int timer = 0;
	num_blocks = (pixel_num + BLOCKDIM - 1) >> LOG_BLOCKDIM;
	ASIRT_Slice_reproj<<<dim3(num_blocks), dim3(BLOCKDIM)>>>(d_prj_calc, d_prj_num,
			d_slc_data, slc, slid, AngN,jslide,d_prj_calc_abs);
	CUDA_CHECK_RETURN(cudaThreadSynchronize());
	num_blocks = (line_allnum + BLOCKDIM - 1) >> LOG_BLOCKDIM;
	ASIRT_proj_calc_div<<<dim3(num_blocks), dim3(BLOCKDIM)>>>(d_prj_calc, d_prj_num,d_prj_calc_abs,
			line_allnum);
	CUDA_CHECK_RETURN(cudaThreadSynchronize());
	num_blocks = (pixel_num + BLOCKDIM - 1) >> LOG_BLOCKDIM;
	ASIRT_one_update<<<dim3(num_blocks), dim3(BLOCKDIM)>>>(d_prj_real,d_prj_calc, d_slc_data, AngN, slc, slid, ASIRT_ITR_STEP,jslide,d_prj_calc_abs);
	CUDA_CHECK_RETURN(cudaThreadSynchronize());
	return TRUE;
}

__global__ void SART_Slice_reproj(float *d_prj_calc, float *d_prj_num,
		float *d_slc_data, Slice slc, Slide slid, int angleindex,int jslide) //A*X
		{
	const unsigned int n = (blockIdx.x << LOG_BLOCKDIM) + threadIdx.x; //pixel index and thread index
	unsigned int m;
	int z, x, row_index;
	double ang, rf, weight;
	int r;
	m = n + slc.min;
//	int pixel_allnum = slc.X * slc.Z;
	int pixel_allnum = dev_const_img.vol.X*dev_const_img.vol.Z;
	if (n < pixel_allnum) {
		z = (int) __fdividef(m, slid.X);
		x = m - z * slid.X;
		ang = D2R(-dev_const_img.tilt_angle[angleindex]);
		rf = lineindex_value(ang, x, z, slid.z0, slid.x0);
		r = floor(rf - 0.5);
		weight = 1.5 - (rf - r);
		//r += slc.x0;
		if (r >= 0 && r < slid.X - 1) {
			row_index = angleindex * slid.X + r;
			atomicAdd(&d_prj_num[row_index], weight);
			atomicAdd(&d_prj_num[row_index + 1], 1 - weight);
			atomicAdd(&d_prj_calc[row_index], weight * d_slc_data[n]);
			atomicAdd(&d_prj_calc[row_index + 1], (1 - weight) * d_slc_data[n]);
		} //end if weight
		else if (r == slid.X - 1) //left
				{
			row_index = angleindex * slid.X + r;
			atomicAdd(&d_prj_num[row_index], weight);
			atomicAdd(&d_prj_calc[row_index], weight * d_slc_data[n]);
		} else if (r == -1) //right
				{
			row_index = angleindex * slid.X;
			atomicAdd(&d_prj_num[row_index + 1], 1 - weight);
			atomicAdd(&d_prj_calc[row_index + 1], (1 - weight) * d_slc_data[n]);
		}
//		if(row_index==800&&jslide==3)
//			printf(" GPU The result: %d  %f\n",n,d_slc_data[n]);
	} //end if n
}
__global__ void SART_proj_calc_div(float *d_prj_calc, float *d_prj_num,
		int angleindex,float *d_prj_real,int jslide) {
	const unsigned int r = (blockIdx.x << LOG_BLOCKDIM) + threadIdx.x; //prj index
	int prj_index=r+angleindex*dev_const_img.prj.X;
	double s, c;
	s = d_prj_calc[prj_index];
	c = d_prj_num[prj_index];
	if ((r < dev_const_img.prj.X)) {
		if(c!=0){
			d_prj_calc[prj_index] = __fdividef(s, c);
		}
		d_prj_calc[prj_index]=d_prj_real[prj_index]-d_prj_calc[prj_index];
	}
}
__global__ void SART_one_update(float *d_prj_calc,
		float *d_slc_data, int angleindex, Slice slc, Slide slid, float SIRT_ITR_STEP,
		int jslide) {
	const unsigned int n = (blockIdx.x << LOG_BLOCKDIM) + threadIdx.x; //pixel index and thread index
	unsigned int m;
	int z, x, row_index;
	double c = 0.0f;
	double s = 0.0f;
	double ang, rf, weight;
	int r;
	m = n + slc.min;
	int pixel_allnum = dev_const_img.vol.X * dev_const_img.vol.Z;
	if (n < pixel_allnum) {
		z = (int) __fdividef(m, slid.X);
		x = m - z * slid.X;
		ang = D2R(-dev_const_img.tilt_angle[angleindex]);
		rf = lineindex_value(ang, x, z, slid.z0, slid.x0);
		r = floor(rf - 0.5);
		weight = 1.5 - (rf - r);
		//	r += slc.x0;

		if (r >= 0 && r < slid.X - 1) {
			c++;
			row_index = angleindex * slid.X + r;
			s += weight * (d_prj_calc[row_index]);
			s += (1 - weight)
					* (d_prj_calc[row_index + 1]);
		} //end if
		else if (r == slid.X - 1) {
			c += weight;
			row_index = angleindex * slid.X + r;
			s += weight * (d_prj_calc[row_index]);
		} else if (r == -1) {
			c += 1 - weight;
			row_index = angleindex * slid.X;
			s += (1 - weight) * (d_prj_calc[row_index]);
		}

		if (c != 0)
			d_slc_data[n] += __fdividef(s, c) * SIRT_ITR_STEP;
	} //end if m
}
int Cu_SART_update_slice(float *d_prj_real, float *d_prj_calc, float *d_prj_num,
		float *d_slc_data, int AngN, Slice slc, Slide slid, int pixel_num,
		float SIRT_ITR_STEP, ImageStruct host_img, int jslide) {
	int angle;
	for (angle = 0; angle < host_img.prj.AngN; angle++) {
		int num_blocks;
		//unsigned int timer = 0;
		num_blocks = (pixel_num + BLOCKDIM - 1) >> LOG_BLOCKDIM;
		SART_Slice_reproj<<<dim3(num_blocks), dim3(BLOCKDIM)>>>(d_prj_calc,
				d_prj_num, d_slc_data, slc, slid, angle, jslide);
		CUDA_CHECK_RETURN(cudaThreadSynchronize());
		num_blocks = (host_img.prj.X + BLOCKDIM - 1) >> LOG_BLOCKDIM;
		SART_proj_calc_div<<<dim3(num_blocks), dim3(BLOCKDIM)>>>(d_prj_calc,
				d_prj_num, angle, d_prj_real,jslide);
		CUDA_CHECK_RETURN(cudaThreadSynchronize());
		num_blocks = (pixel_num + BLOCKDIM - 1) >> LOG_BLOCKDIM;
		SART_one_update<<<dim3(num_blocks), dim3(BLOCKDIM)>>>(
				d_prj_calc, d_slc_data, angle, slc, slid, SIRT_ITR_STEP, jslide);
		CUDA_CHECK_RETURN(cudaThreadSynchronize());
	} // end for angle
	return 0;
}
__global__ void ASART_Slice_reproj(float *d_prj_calc, float *d_prj_num,
		float *d_slc_data, Slice slc, Slide slid, int angleindex,int jslide,float *d_prj_calc_abs) //A*X
		{
	const unsigned int n = (blockIdx.x << LOG_BLOCKDIM) + threadIdx.x; //pixel index and thread index
	unsigned int m;
	int z, x, row_index;
	double ang, rf, weight;
	int r;
	m = n + slc.min;
//	int pixel_allnum = slc.X * slc.Z;
	int pixel_allnum = dev_const_img.vol.X*dev_const_img.vol.Z;
	if (n < pixel_allnum) {
		z = (int) __fdividef(m, slid.X);
		x = m - z * slid.X;
		ang = D2R(-dev_const_img.tilt_angle[angleindex]);
		rf = lineindex_value(ang, x, z, slid.z0, slid.x0);
		r = floor(rf - 0.5);
		weight = 1.5 - (rf - r);
		//r += slc.x0;
		if (r >= 0 && r < slid.X - 1) {
			row_index = angleindex * slid.X + r;
			atomicAdd(&d_prj_num[row_index], weight);
			atomicAdd(&d_prj_num[row_index + 1], 1 - weight);
			atomicAdd(&d_prj_calc[row_index], weight * d_slc_data[n]);
			atomicAdd(&d_prj_calc[row_index + 1], (1 - weight) * d_slc_data[n]);
			atomicAdd(&d_prj_calc_abs[row_index],
					weight * abs(d_slc_data[n]));
			atomicAdd(&d_prj_calc_abs[row_index + 1],
					(1 - weight) * abs(d_slc_data[n]));
		} //end if weight
		else if (r == slid.X - 1) //left
				{
			row_index = angleindex * slid.X + r;
			atomicAdd(&d_prj_num[row_index], weight);
			atomicAdd(&d_prj_calc[row_index], weight * d_slc_data[n]);
			atomicAdd(&d_prj_calc_abs[row_index],
							weight * abs(d_slc_data[n]));
		} else if (r == -1) //right
				{
			row_index = angleindex * slid.X;
			atomicAdd(&d_prj_num[row_index + 1], 1 - weight);
			atomicAdd(&d_prj_calc[row_index + 1], (1 - weight) * d_slc_data[n]);
			atomicAdd(&d_prj_calc_abs[row_index],
								(1-weight) * abs(d_slc_data[n]));
		}
	} //end if n
}
__global__ void ASART_proj_calc_div(float *d_prj_calc, float *d_prj_num,
		int angleindex,float *d_prj_real,int jslide,float *d_prj_calc_abs) {
	const unsigned int r = (blockIdx.x << LOG_BLOCKDIM) + threadIdx.x; //prj index
	int prj_index=r+angleindex*dev_const_img.prj.X;
	double s, c,s2;
	s = d_prj_calc[prj_index];
	c = d_prj_num[prj_index];
	s2=d_prj_calc_abs[prj_index];
	if ((r < dev_const_img.prj.X)) {
		if(c!=0){
			d_prj_calc[prj_index] = __fdividef(s, c);
			d_prj_calc_abs[prj_index]= __fdividef(s2, c);
		}
		d_prj_calc[prj_index]=d_prj_real[prj_index]-d_prj_calc[prj_index];
	}
}
__global__ void ASART_one_update(float *d_prj_calc,
		float *d_slc_data, int angleindex, Slice slc, Slide slid, float ASART_ITR_STEP,
		int jslide,float *d_prj_calc_abs) {
	const unsigned int n = (blockIdx.x << LOG_BLOCKDIM) + threadIdx.x; //pixel index and thread index
	unsigned int m;
	int z, x, row_index;
	double c = 0.0f;
	double s = 0.0f;
	double t=0;
	double ang, rf, weight;
	int r;
	m = n + slc.min;
	int pixel_allnum = dev_const_img.vol.X * dev_const_img.vol.Z;
	if (n < pixel_allnum) {
		z = (int) __fdividef(m, slid.X);
		x = m - z * slid.X;
		ang = D2R(-dev_const_img.tilt_angle[angleindex]);
		rf = lineindex_value(ang, x, z, slid.z0, slid.x0);
		r = floor(rf - 0.5);
		weight = 1.5 - (rf - r);
		//	r += slc.x0;

		if (r >= 0 && r < slid.X - 1) {
			c++;
			row_index = angleindex * slid.X + r;
			s += weight * (d_prj_calc[row_index]);
			s += (1 - weight)
					* (d_prj_calc[row_index + 1]);
			t+=weight*d_prj_calc_abs[row_index]+(1-weight)*d_prj_calc_abs[row_index+1];
		} //end if
		else if (r == slid.X - 1) {
			c += weight;
			row_index = angleindex * slid.X + r;
			s += weight * (d_prj_calc[row_index]);
		     t+=weight*d_prj_calc_abs[row_index];
		} else if (r == -1) {
			c += 1 - weight;
			row_index = angleindex * slid.X;
			s += (1 - weight) * (d_prj_calc[row_index]);
		     t+=(1-weight)*d_prj_calc_abs[row_index];
		}
		if(t!=0)    d_slc_data[n] += abs(d_slc_data[n])*(float)s/t*ASART_ITR_STEP;
	} //end if m
}
int Cu_ASART_update_slice(float *d_prj_real, float *d_prj_calc, float *d_prj_num,
		float *d_slc_data, int AngN, Slice slc, Slide slid, int pixel_num,
		float ASART_ITR_STEP, ImageStruct host_img, int jslide,float *d_prj_calc_abs) {
	int angle;
	for (angle = 0; angle < host_img.prj.AngN; angle++) {
		int num_blocks;
		//unsigned int timer = 0;
		num_blocks = (pixel_num + BLOCKDIM - 1) >> LOG_BLOCKDIM;
		ASART_Slice_reproj<<<dim3(num_blocks), dim3(BLOCKDIM)>>>(d_prj_calc,
				d_prj_num, d_slc_data, slc, slid, angle, jslide,d_prj_calc_abs);
		CUDA_CHECK_RETURN(cudaThreadSynchronize());
		num_blocks = (host_img.prj.X + BLOCKDIM - 1) >> LOG_BLOCKDIM;
		ASART_proj_calc_div<<<dim3(num_blocks), dim3(BLOCKDIM)>>>(d_prj_calc,
				d_prj_num, angle, d_prj_real,jslide,d_prj_calc_abs);
		CUDA_CHECK_RETURN(cudaThreadSynchronize());
		num_blocks = (pixel_num + BLOCKDIM - 1) >> LOG_BLOCKDIM;
		ASART_one_update<<<dim3(num_blocks), dim3(BLOCKDIM)>>>(
				d_prj_calc, d_slc_data, angle, slc, slid, ASART_ITR_STEP, jslide,d_prj_calc_abs);
		CUDA_CHECK_RETURN(cudaThreadSynchronize());
	} // end for angle
	return 0;
}

void freeDevMem_double(double *dev_data) {
	cudaFree(dev_data);
}
void freeDevMem_float(double *dev_data) {

	cudaFree(dev_data);
}
void freeDevProjection(float *prj) {
	cudaFree(prj);
}
void freeDevSlc(float *slc_data) {
	cudaFree(slc_data);
}

