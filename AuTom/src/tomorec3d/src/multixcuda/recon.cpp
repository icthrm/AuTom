#include "recon.h"
#include "SARTmult.cuh"
#define MILLION 1000000


/**********************************************************************************/
int read_coef(double *data, FILE *f_coef, int id) {
	char *lstr;
	lstr = (char *) malloc(TEXT_LINE_MAX);

	int i = 0;
	while (fgets(lstr, TEXT_LINE_MAX, f_coef) != NULL) {
		data[i] = atof(lstr);

		i++;
	}
	return TRUE;
}

/**********************************************************************************/
int cal_coef(double *x_coef, double *y_coef, double *tilt_angle,
		double *pitch_angle, float offset, float xaxistilt, int angle)

		{

	int i;
	for (i = 0; i < angle; i++) {
		x_coef[3 * i] = cos(D2R(tilt_angle[i]+offset));
		x_coef[3 * i + 1] = sin(D2R(pitch_angle[i]-xaxistilt))
				* sin(D2R(tilt_angle[i]+offset));
		x_coef[3 * i + 2] = -1 * cos(D2R(pitch_angle[i]-xaxistilt))
				* sin(D2R(tilt_angle[i]+offset));
		y_coef[3 * i] = 0;
		y_coef[3 * i + 1] = cos(D2R(pitch_angle[i]-xaxistilt));
		y_coef[3 * i + 2] = sin(D2R(pitch_angle[i]-xaxistilt));
	}

	return TRUE;

}

/**********************************************************************************/
int val_coef(Pixel0 *pixel0, Pixel pixel, int angle, float zshift,
		double *x_coef, double *y_coef, Weight *comp_prj)

		{
	double x, y;
	/* int x_lef,y_low;
	 double x_del,y_del;*/
	int index;
	index = 3 * angle;  //order=1,straight model

	/*  x=x_coef[index]*(pixel.X-pixel0->X)+x_coef[index+1]*(pixel.Y-pixel0->Y)+x_coef[index+2]*(pixel.Z-pixel0->Z)+pixel0->X;
	 y=y_coef[index]*(pixel.X-pixel0->X)+y_coef[index+1]*(pixel.Y-pixel0->Y)+y_coef[index+2]*(pixel.Z-pixel0->Z)+pixel0->Y;*/

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

	return TRUE;
}

/******************************************backprojection****************************************/
int Cu_Slice_backproj_bilinear_BPT(MPI_File fin, MrcHeader *inhead, Pixel0 *pixel0,
		Projection prj, Volume vol, Weight *comp_prj, double *x_coef,
		double *y_coef, float zshift, float *slc_data, int Z_start, int Z_end,
		int id, int pixel_num_per, double *dev_x_coef, double *dev_y_coef,
		long int globalMem) {
	//for input file
	int line_num = prj.X * prj.Y;

	float *prj_real;
	float *dev_prj_real;
	if ((prj_real = (float *) malloc(sizeof(float) * line_num)) == NULL) {
		printf("Can't malloc memery for 'prj_real'!");
		return FALSE;
	}
	memset(prj_real, 0, sizeof(float) * line_num);

	double s;
//  double c;

	//filter to prj
	int filtlength = 251;
	//char *filter="RamLak";


	int row_pad = 2;  //x-padded
	int col_pad = 2;  //y-padded

	int symmetrize_2D_flag = 1;

	int angle;
	long long n, index;

	Pixel pixel;
	int total_vol = pixel_num_per * sizeof(float);

	int N_vol_num = (total_vol - 1) / (globalMem / 2) + 1; //half mem delieve to vol
	total_vol = pixel_num_per / line_num; //total slice num
	int size_vol; //each sub_vol size
	int N_prj_num;
	int pre_size_prj, size_prj;

	int start_vol, start_prj = 0;
	int offset_vol = 0;


	for (int i = 0; i < N_vol_num; i++) {

		if (i < total_vol % N_vol_num)
			size_vol = total_vol / N_vol_num + 1; // num of volume
		else
			size_vol = total_vol / N_vol_num;

		int zindexstart = offset_vol;
	//	printf("id=%d N_vol_num:%d size_vol=%d \n", id, N_vol_num, size_vol);
		int offset_prj=0;
		///////////////////////////////////init dev_slc_data
		float *dev_slc_data;
		initSlcMalloc(dev_slc_data, size_vol * line_num);
		initSlcDevMem(dev_slc_data, size_vol * line_num);

		/////////////////////////////////
		//printf(" ---Tset   globalMem %d %d \n",globalMem,(globalMem - size_vol * line_num* sizeof(float)));

		pre_size_prj = (globalMem - size_vol * line_num* sizeof(float)) / (prj.X * prj.Y*sizeof(float));

		N_prj_num = (prj.AngN - 1) / pre_size_prj + 1;  //divide n prj
		for (int j = 0; j < N_prj_num; j++) {  //(3) size=5,4,4

			if (j < prj.AngN % N_prj_num)
				size_prj = prj.AngN / N_prj_num + 1;    //num of prj
			else
				size_prj = prj.AngN / N_prj_num;
//			printf("id=%d j=%d N_prj_num=%d size_prj=%d \n", id, j, N_prj_num,
//					size_prj);
			//init and deliver prj

			initProjectionMalloc(dev_prj_real, line_num * size_prj);
			for (int k = 0; k < size_prj; k++) {
				int index_prj = k + offset_prj;
				mrc_read_slice(fin, inhead, index_prj, 'Z', prj_real);

				transferProjectionToDev(dev_prj_real + (line_num * k), prj_real,
						line_num);    //the kth  image
			}    //end transfor (int k = 0; k < size_prj; k++)
			Sart_Slice_backproj_bilinear(size_vol * line_num,
					zindexstart + Z_start, dev_prj_real, dev_slc_data, zshift,
					dev_x_coef, dev_y_coef, size_prj, offset_prj,
					zindexstart * line_num); //all pixel;init z start;dev_prj_real;dev_slc_data;empty-del;zshift(input)

			offset_prj += size_prj;
		} //end prj

		transferToHost_FBP(dev_slc_data, slc_data + (zindexstart * line_num),
				size_vol * line_num);

		freeDevSlc(dev_slc_data);

		offset_vol += size_vol;
	}
	/*
	 for (angle = 0; angle < prj.AngN; angle++) {
	 printf("begin to read %d prjection data for %d Zstart %d line_num,dev_%p  ,host_%p\n", angle,
	 Z_start,line_num,dev_prj_real,prj_real);
	 mrc_read_slice(fin, inhead, angle, 'Z', prj_real);

	 filter_prj_sym(prj_real, filter, filtlength, prj, row_pad, col_pad,
	 symmetrize_2D_flag, id);
	 transferProjectionToDev(dev_prj_real, prj_real, line_num);
	 //
	 Sart_Slice_backproj_bilinear(pixel_num_per, Z_start,  dev_prj_real, dev_slc_data, angle, zshift, dev_x_coef,
	 dev_y_coef);

	 }  //end angle
	 */

	free(prj_real);
	freeDevProjection(dev_prj_real);
	freeDevMem(dev_x_coef, dev_y_coef);
	return TRUE;

}
int Slice_backproj_BPT(MPI_File fin, float *prj_real, MrcHeader *inhead,
		Pixel0 *pixel0, Projection prj, Volume vol, Weight *comp_prj,
		double *x_coef, double *y_coef, float zshift, float *slc_data,
		int Z_start, int Z_end, int id)

		{

	double s;
//  double c;
	int angle;
	int n;
	long index;

	Pixel pixel;

	for (angle = 0; angle < prj.AngN; angle++) {
		printf("begin to read %d prjection data for %d Zstart\n", angle,
				Z_start);
		mrc_read_slice(fin, inhead, angle, 'Z', prj_real);

		for (pixel.Z = Z_start; pixel.Z < Z_end; pixel.Z++) {
			//  printf("slice %f is beginning.\n",pixel.Z);

			for (pixel.Y = vol.Ystart; pixel.Y < vol.Yend; pixel.Y++)
				for (pixel.X = vol.Xstart; pixel.X < vol.Xend; pixel.X++) {
					index = (long long) ((pixel.X - vol.Xstart)
							+ (pixel.Y - vol.Ystart) * vol.X
							+ (pixel.Z - Z_start) * vol.X * vol.Y); //pixel index
					s = 0;
					//   c=0;

					// val_coef(pixel0,pixel, angle,x_coef, y_coef, comp_prj);
					val_coef(pixel0, pixel, angle, zshift, x_coef, y_coef,
							comp_prj);

					if (comp_prj->x_min >= 0 && comp_prj->x_min < prj.X
							&& comp_prj->y_min >= 0 && comp_prj->y_min < prj.Y) //(x_min, y_min)
									{
						n = comp_prj->x_min + comp_prj->y_min * prj.X; //prj index
						s += (1 - comp_prj->x_min_del)
								* (1 - comp_prj->y_min_del) * prj_real[n];
						//  c+=(1-comp_prj->x_min_del)*(1-comp_prj->y_min_del);
					}
					if ((comp_prj->x_min + 1) >= 0
							&& (comp_prj->x_min + 1) < prj.X
							&& comp_prj->y_min >= 0 && comp_prj->y_min < prj.Y) //(x_min+1, y_min)
									{
						n = comp_prj->x_min + 1 + comp_prj->y_min * prj.X; //prj index
						s += comp_prj->x_min_del * (1 - comp_prj->y_min_del)
								* prj_real[n];
						//  c+=comp_prj->x_min_del*(1-comp_prj->y_min_del);
					}
					if (comp_prj->x_min >= 0 && comp_prj->x_min < prj.X
							&& (comp_prj->y_min + 1) >= 0
							&& (comp_prj->y_min + 1) < prj.Y) //(x_min, y_min+1)
									{
						n = comp_prj->x_min + (comp_prj->y_min + 1) * prj.X; //prj index
						s += (1 - comp_prj->x_min_del) * comp_prj->y_min_del
								* prj_real[n];
						//  c+=(1-comp_prj->x_min_del)*comp_prj->y_min_del;
					}
					if ((comp_prj->x_min + 1) >= 0
							&& (comp_prj->x_min + 1) < prj.X
							&& (comp_prj->y_min + 1) >= 0
							&& (comp_prj->y_min + 1) < prj.Y) //(x_min+1, y_min+1)
									{
						n = comp_prj->x_min + 1 + (comp_prj->y_min + 1) * prj.X; //prj index
						s += comp_prj->x_min_del * comp_prj->y_min_del
								* prj_real[n];
						// c+=comp_prj->x_min_del*comp_prj->y_min_del;
					}
					// printf("hello world in angle %d (%d,%d,%d)\n",angle,pixel.X,pixel.Y,pixel.Z);

					if (s != 0.0f)
						//  slc_data[index]+=(float)(s/c);
						slc_data[index] += (float) (s / prj.AngN);

				} //end pixel XY
				//   printf("slice %d is finished to backprojection.\n",pixel.Z);
				// slcN_per++;

		}      // end pixel.Z

	}      //end angle

	/*  MPI_Reduce(&slcN_per,&global_slcN,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

	 if(!myid) printf("%d slice is reconstructed by BPT!\n",global_slcN);*/

	return TRUE;

}


//FBP
/****************************FBP*****************************************************/
int Cu_Slice_backproj_bilinear(MPI_File fin, MrcHeader *inhead, Pixel0 *pixel0,
		Projection prj, Volume vol, Weight *comp_prj, double *x_coef,
		double *y_coef, float zshift, float *slc_data, int Z_start, int Z_end,
		int id, int pixel_num_per, double *dev_x_coef, double *dev_y_coef,
		long int globalMem,const char *filter) {
	//for input file
	int line_num = prj.X * prj.Y;

	float *prj_real;
	float *dev_prj_real;
	if ((prj_real = (float *) malloc(sizeof(float) * line_num)) == NULL) {
		printf("Can't malloc memery for 'prj_real'!");
		return FALSE;
	}
	memset(prj_real, 0, sizeof(float) * line_num);

	double s;
//  double c;

	//filter to prj
	int filtlength = 251;
	//char *filter="RamLak";


	int row_pad = 2;  //x-padded
	int col_pad = 2;  //y-padded

	int symmetrize_2D_flag = 1;

	int angle;
	long long n, index;

	Pixel pixel;
	int total_vol = pixel_num_per * sizeof(float);

	int N_vol_num = (total_vol - 1) / (globalMem / 2) + 1; //half mem delieve to vol
	total_vol = pixel_num_per / line_num; //total slice num
	int size_vol; //each sub_vol size
	int N_prj_num;
	int pre_size_prj, size_prj;

	int start_vol, start_prj = 0;
	int offset_vol = 0;


	for (int i = 0; i < N_vol_num; i++) {

		if (i < total_vol % N_vol_num)
			size_vol = total_vol / N_vol_num + 1; // num of volume
		else
			size_vol = total_vol / N_vol_num;

		int zindexstart = offset_vol;
	//	printf("id=%d N_vol_num:%d size_vol=%d \n", id, N_vol_num, size_vol);
		int offset_prj=0;
		///////////////////////////////////init dev_slc_data
		float *dev_slc_data;
		initSlcMalloc(dev_slc_data, size_vol * line_num);
		initSlcDevMem(dev_slc_data, size_vol * line_num);

		/////////////////////////////////
		//printf(" ---Tset   globalMem %d %d \n",globalMem,(globalMem - size_vol * line_num* sizeof(float)));

		pre_size_prj = (globalMem - size_vol * line_num* sizeof(float)) / (prj.X * prj.Y*sizeof(float));

		N_prj_num = (prj.AngN - 1) / pre_size_prj + 1;  //divide n prj
		for (int j = 0; j < N_prj_num; j++) {  //(3) size=5,4,4

			if (j < prj.AngN % N_prj_num)
				size_prj = prj.AngN / N_prj_num + 1;    //num of prj
			else
				size_prj = prj.AngN / N_prj_num;
//			printf("id=%d j=%d N_prj_num=%d size_prj=%d \n", id, j, N_prj_num,
//					size_prj);
			//init and deliver prj

			initProjectionMalloc(dev_prj_real, line_num * size_prj);
			for (int k = 0; k < size_prj; k++) {
				int index_prj = k + offset_prj;
				mrc_read_slice(fin, inhead, index_prj, 'Z', prj_real);
				filter_prj_sym(prj_real, filter, filtlength, prj, row_pad,
						col_pad, symmetrize_2D_flag, id);
				transferProjectionToDev(dev_prj_real + (line_num * k), prj_real,
						line_num);    //the kth  image
			}    //end transfor (int k = 0; k < size_prj; k++)
			Sart_Slice_backproj_bilinear(size_vol * line_num,
					zindexstart + Z_start, dev_prj_real, dev_slc_data, zshift,
					dev_x_coef, dev_y_coef, size_prj, offset_prj,
					zindexstart * line_num); //all pixel;init z start;dev_prj_real;dev_slc_data;empty-del;zshift(input)

			offset_prj += size_prj;
		} //end prj

		transferToHost_FBP(dev_slc_data, slc_data + (zindexstart * line_num),
				size_vol * line_num);

		freeDevSlc(dev_slc_data);

		offset_vol += size_vol;
	}
	/*
	 for (angle = 0; angle < prj.AngN; angle++) {
	 printf("begin to read %d prjection data for %d Zstart %d line_num,dev_%p  ,host_%p\n", angle,
	 Z_start,line_num,dev_prj_real,prj_real);
	 mrc_read_slice(fin, inhead, angle, 'Z', prj_real);

	 filter_prj_sym(prj_real, filter, filtlength, prj, row_pad, col_pad,
	 symmetrize_2D_flag, id);
	 transferProjectionToDev(dev_prj_real, prj_real, line_num);
	 //
	 Sart_Slice_backproj_bilinear(pixel_num_per, Z_start,  dev_prj_real, dev_slc_data, angle, zshift, dev_x_coef,
	 dev_y_coef);

	 }  //end angle
	 */

	free(prj_real);
	freeDevProjection(dev_prj_real);
	freeDevMem(dev_x_coef, dev_y_coef);
	return TRUE;

}
int Slice_backproj_FBP(MPI_File fin, float *prj_real, MrcHeader *inhead,
		Pixel0 *pixel0, Projection prj, Volume vol, Weight *comp_prj,
		double *x_coef, double *y_coef, float zshift, float *slc_data,
		int Z_start, int Z_end, int id, const char *filter)

		{

	double s;
//  double c;

	//filter to prj
	int filtlength = 251;
	int row_pad = 2;  //x-padded
	int col_pad = 2;  //y-padded
	int symmetrize_2D_flag = 1;

	int angle;
	int n;
	long index;

	Pixel pixel;

	for (angle = 0; angle < prj.AngN; angle++) {
		printf("begin to read %d prjection data for %d Zstart\n", angle,
				Z_start);
		mrc_read_slice(fin, inhead, angle, 'Z', prj_real);

		filter_prj_sym(prj_real, filter, filtlength, prj, row_pad, col_pad,
				symmetrize_2D_flag, id);

		for (pixel.Z = Z_start; pixel.Z < Z_end; pixel.Z++) {
			//  printf("slice %f is beginning.\n",pixel.Z);

			for (pixel.Y = vol.Ystart; pixel.Y < vol.Yend; pixel.Y++)
				for (pixel.X = vol.Xstart; pixel.X < vol.Xend; pixel.X++) {
					index = (long long) ((pixel.X - vol.Xstart)
							+ (pixel.Y - vol.Ystart) * vol.X
							+ (pixel.Z - Z_start) * vol.X * vol.Y); //pixel index
					s = 0;
					//   c=0;

					// val_coef(pixel0,pixel, angle,x_coef, y_coef, comp_prj);
					val_coef(pixel0, pixel, angle, zshift, x_coef, y_coef,
							comp_prj);

					if (comp_prj->x_min >= 0 && comp_prj->x_min < prj.X
							&& comp_prj->y_min >= 0 && comp_prj->y_min < prj.Y) //(x_min, y_min)
									{
						n = comp_prj->x_min + comp_prj->y_min * prj.X; //prj index
						s += (1 - comp_prj->x_min_del)
								* (1 - comp_prj->y_min_del) * prj_real[n];
						//  c+=(1-comp_prj->x_min_del)*(1-comp_prj->y_min_del);
					}
					if ((comp_prj->x_min + 1) >= 0
							&& (comp_prj->x_min + 1) < prj.X
							&& comp_prj->y_min >= 0 && comp_prj->y_min < prj.Y) //(x_min+1, y_min)
									{
						n = comp_prj->x_min + 1 + comp_prj->y_min * prj.X; //prj index
						s += comp_prj->x_min_del * (1 - comp_prj->y_min_del)
								* prj_real[n];
						//  c+=comp_prj->x_min_del*(1-comp_prj->y_min_del);
					}
					if (comp_prj->x_min >= 0 && comp_prj->x_min < prj.X
							&& (comp_prj->y_min + 1) >= 0
							&& (comp_prj->y_min + 1) < prj.Y) //(x_min, y_min+1)
									{
						n = comp_prj->x_min + (comp_prj->y_min + 1) * prj.X; //prj index
						s += (1 - comp_prj->x_min_del) * comp_prj->y_min_del
								* prj_real[n];
						//  c+=(1-comp_prj->x_min_del)*comp_prj->y_min_del;
					}
					if ((comp_prj->x_min + 1) >= 0
							&& (comp_prj->x_min + 1) < prj.X
							&& (comp_prj->y_min + 1) >= 0
							&& (comp_prj->y_min + 1) < prj.Y) //(x_min+1, y_min+1)
									{
						n = comp_prj->x_min + 1 + (comp_prj->y_min + 1) * prj.X; //prj index
						s += comp_prj->x_min_del * comp_prj->y_min_del
								* prj_real[n];
						// c+=comp_prj->x_min_del*comp_prj->y_min_del;
					}
					// printf("hello world in angle %d (%d,%d,%d)\n",angle,pixel.X,pixel.Y,pixel.Z);

					if (s != 0.0f)
						//  slc_data[index]+=(float)(s/c);
						slc_data[index] += (float) (s / prj.AngN);

				} //end pixel XY
				//   printf("slice %d is finished to backprojection.\n",pixel.Z);
				// slcN_per++;

		}      // end pixel.Z

	}      //end angle

	/*  MPI_Reduce(&slcN_per,&global_slcN,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

	 if(!myid) printf("%d slice is reconstructed by BPT!\n",global_slcN);*/

	return TRUE;

}

//SIRT
/****************************SIRT_Slice_reproj_bilinear******************************************************/
/*int SIRT_Slice_reproj_bilinear(Projection prj, float *prj_calc, float *prj_calc_num, Slice *slc, Slide *slid, double *tilt_angle, int pixel_num)
 {
 int n,m,z,x,j,r,row_index;
 float ang,rf,weight;

 // int pixel_allnum=slid->X*slid->Z;
 for(n=0;n<pixel_num;n++)
 {
 m=n+slc->min;
 z=(int)m/slid->X;
 x=m-z*slid->X;
 for(j=0;j<prj.AngN;j++)
 {
 ang = D2R(-tilt_angle[j]);

 rf = (x-slid->x0+0.5)*cos(ang)+(z-slid->z0+0.5)*sin(ang)+slid->x0;
 r = floor(rf-0.5);
 weight=1.5-(rf-r);
 //  r+=slid->x0;

 if(r>=0 && r<slid->X-1)
 {
 row_index=j*slid->X+r;  //prj_index
 prj_calc[row_index]+=weight*slc->data[n];
 prj_calc[row_index+1]+=(1-weight)*slc->data[n];
 prj_calc_num[row_index]+=weight;
 prj_calc_num[row_index+1]+=1-weight;

 }
 else if(r==slid->X-1) //left
 {
 row_index=j*slid->X+r; //prj_index
 prj_calc[row_index]+=weight*slc->data[n];
 prj_calc_num[row_index]+=weight;
 }
 else if (r==-1)  //right
 {
 row_index=j*slid->X;
 prj_calc[row_index+1]+=(1-weight)*slc->data[n];
 prj_calc_num[row_index+1]+=1-weight;
 }
 } //end for j

 } // end for n

 return TRUE;
 }*/
/****************************SIRT_one_update_bilinear******************************************************/
/*int SIRT_one_update_bilinear( Projection prj, float *prj_calc, Slice *slc, Slide *slid, double *tilt_angle, int pixel_num, float SIRT_ITR_STEP)

 {
 int n,m,z,x,j,r,row_index;
 float ang,rf,weight,c;
 double s;

 // int pixel_allnum=slid->X*slid->Z;
 for(n=0;n<pixel_num;n++)
 {
 c=0;
 s=0;
 m=n+slc->min;
 z=(int)m/slid->X;
 x=m-z*slid->X;
 for(j=0;j<prj.AngN;j++)
 {
 ang = D2R(-tilt_angle[j]);

 rf = (x-slid->x0+0.5)*cos(ang)+(z-slid->z0+0.5)*sin(ang)+slid->x0;
 r = floor(rf-0.5);
 weight=1.5-(rf-r);

 if(r>=0 && r<slid->X-1)
 {
 c++;
 row_index=j*slid->X+r;
 s+=weight*prj_calc[row_index]+(1-weight)*prj_calc[row_index+1];
 }
 else if(r==slid->X-1)
 {
 c+=weight;
 row_index=j*slid->X+r;
 s+=weight*prj_calc[row_index];
 }
 else if (r==-1)
 {
 c+=1-weight;
 row_index=j*slid->X;
 s+=(1-weight)*prj_calc[row_index];
 }
 } //end for j

 if(c!=0)    slc->data[n] += (float)s/c*SIRT_ITR_STEP;
 } // end for n

 return TRUE;

 }
 */
/****************************SIRT_update_slice******************************************************/
/*int SIRT_update_slice(Projection prj, float *prj_real, float *prj_calc, float *prj_calc_num, Slice *slc, Slide *slid, double *tilt_angle, int pixel_num, float SIRT_ITR_STEP)
 {

 int n;
 int line_num=prj.X*prj.AngN;

 SIRT_Slice_reproj_bilinear(prj, prj_calc, prj_calc_num, slc, slid, tilt_angle, pixel_num);

 for(n=0;n<line_num;n++)
 {
 if(prj_calc_num[n] != 0)
 prj_calc[n]/=prj_calc_num[n];
 prj_calc[n]=prj_real[n]-prj_calc[n];
 }

 SIRT_one_update_bilinear(prj, prj_calc, slc, slid, tilt_angle, pixel_num, SIRT_ITR_STEP);

 return TRUE;

 }
 */
//ASIRT
/****************************ASIRT_Slice_reproj_bilinear******************************************************/
/*int ASIRT_Slice_reproj_bilinear(Projection prj, float *prj_calc, float *prj_calc_abs, float *prj_calc_num, Slice *slc, Slide *slid, double *tilt_angle, int pixel_num)
 {
 int n,m,z,x,j,r,row_index;
 float ang,rf,weight;

 // int pixel_allnum=slid->X*slid->Z;
 for(n=0;n<pixel_num;n++)
 {
 m=n+slc->min;
 z=(int)m/slid->X;
 x=m-z*slid->X;
 for(j=0;j<prj.AngN;j++)
 {
 ang = D2R(-tilt_angle[j]);

 rf = (x-slid->x0+0.5)*cos(ang)+(z-slid->z0+0.5)*sin(ang)+slid->x0;
 r = floor(rf-0.5);
 weight=1.5-(rf-r);
 //  r+=slid->x0;

 if(r>=0 && r<slid->X-1)
 {
 row_index=j*slid->X+r;  //prj_index
 prj_calc[row_index]+=weight*slc->data[n];
 prj_calc[row_index+1]+=(1-weight)*slc->data[n];
 prj_calc_abs[row_index]+=weight*abs(slc->data[n]);
 prj_calc_abs[row_index+1]+=(1-weight)*abs(slc->data[n]);
 prj_calc_num[row_index]+=weight;
 prj_calc_num[row_index+1]+=1-weight;

 }
 else if(r==slid->X-1) //left
 {
 row_index=j*slid->X+r; //prj_index
 prj_calc[row_index]+=weight*slc->data[n];
 prj_calc_abs[row_index]+=weight*abs(slc->data[n]);

 prj_calc_num[row_index]+=weight;
 }
 else if (r==-1)  //right
 {
 row_index=j*slid->X;
 prj_calc[row_index+1]+=(1-weight)*slc->data[n];
 prj_calc_abs[row_index]+=(1-weight)*abs(slc->data[n]);

 prj_calc_num[row_index+1]+=1-weight;
 }
 } //end for j

 } // end for n

 return TRUE;
 }
 */
/****************************ASIRT_one_update_bilinear******************************************************/
/*int ASIRT_one_update_bilinear( Projection prj, float *prj_calc, float *prj_calc_abs, Slice *slc, Slide *slid, double *tilt_angle, int pixel_num, float ASIRT_ITR_STEP)

 {
 int n,m,z,x,j,r,row_index;
 float ang,rf,weight,c;
 double t,s;

 // int pixel_allnum=slid->X*slid->Z;
 for(n=0;n<pixel_num;n++)
 {
 c=0;
 s=0;
 m=n+slc->min;
 z=(int)m/slid->X;
 x=m-z*slid->X;
 for(j=0;j<prj.AngN;j++)
 {
 ang = D2R(-tilt_angle[j]);

 rf = (x-slid->x0+0.5)*cos(ang)+(z-slid->z0+0.5)*sin(ang)+slid->x0;
 r = floor(rf-0.5);
 weight=1.5-(rf-r);

 if(r>=0 && r<slid->X-1)
 {
 c++;
 row_index=j*slid->X+r;
 s+=weight*prj_calc[row_index]+(1-weight)*prj_calc[row_index+1];
 t+=weight*prj_calc_abs[row_index]+(1-weight)*prj_calc_abs[row_index+1];

 }
 else if(r==slid->X-1)
 {
 c+=weight;
 row_index=j*slid->X+r;
 s+=weight*prj_calc[row_index];
 t+=weight*prj_calc_abs[row_index];
 }
 else if (r==-1)
 {
 c+=1-weight;
 row_index=j*slid->X;
 s+=(1-weight)*prj_calc[row_index];
 t+=(1-weight)*prj_calc_abs[row_index];
 }
 } //end for j

 if(t!=0)   // slc->data[n] += abs(slc->data[n])*(float)s/t*ASIRT_ITR_STEP;
 slc->data[n] += slc->data[n]*(float)s/t*ASIRT_ITR_STEP;

 } // end for n

 return TRUE;

 }
 */
/****************************ASIRT_update_slice******************************************************/
/*int ASIRT_update_slice(Projection prj, float *prj_real, float *prj_calc, float *prj_calc_abs, float *prj_calc_num, Slice *slc, Slide *slid, double *tilt_angle, int pixel_num, float ASIRT_ITR_STEP)
 {

 int n;
 int line_num=prj.X*prj.AngN;

 ASIRT_Slice_reproj_bilinear(prj, prj_calc, prj_calc_abs, prj_calc_num, slc, slid, tilt_angle, pixel_num);

 for(n=0;n<line_num;n++)
 {
 if(prj_calc_num[n] != 0)
 prj_calc[n]/=prj_calc_num[n];
 prj_calc[n]=prj_real[n]-prj_calc[n];
 }

 ASIRT_one_update_bilinear(prj, prj_calc, prj_calc_abs, slc, slid, tilt_angle, pixel_num, ASIRT_ITR_STEP);

 return TRUE;

 }
 */

//SART
/********************************SART_Slice_reproj_bilinear**************************************************/
int SART_Slice_reproj_bilinear(Pixel0 *pixel0, Projection prj, Volume vol,
		Weight *comp_prj, double *x_coef, double *y_coef, float zshift,
		float *prj_calc, float *prj_calc_num, float *slc_data, int Z_start,
		int Z_end, int angle) {

	int n, index;

	Pixel pixel;

	for (pixel.Z = Z_start; pixel.Z < Z_end; pixel.Z++) {

		for (pixel.Y = vol.Ystart; pixel.Y < vol.Yend; pixel.Y++)
			for (pixel.X = vol.Xstart; pixel.X < vol.Xend; pixel.X++) {
				index = (pixel.X - vol.Xstart) + (pixel.Y - vol.Ystart) * vol.X
						+ (pixel.Z - Z_start) * vol.X * vol.Y;   //pixel index

				val_coef(pixel0, pixel, angle, zshift, x_coef, y_coef,
						comp_prj);

				if (comp_prj->x_min >= 0 && comp_prj->x_min < prj.X
						&& comp_prj->y_min >= 0 && comp_prj->y_min < prj.Y) //(x_min, y_min)
								{

					n = comp_prj->x_min + comp_prj->y_min * prj.X; //prj index
					prj_calc[n] += (1 - comp_prj->x_min_del)
							* (1 - comp_prj->y_min_del) * slc_data[index];
					prj_calc_num[n] += (1 - comp_prj->x_min_del)
							* (1 - comp_prj->y_min_del);

				} //end if
				if ((comp_prj->x_min + 1) >= 0 && (comp_prj->x_min + 1) < prj.X
						&& comp_prj->y_min >= 0 && comp_prj->y_min < prj.Y) //(x_min+1, y_min)
								{

					n = comp_prj->x_min + 1 + comp_prj->y_min * prj.X; //prj index
					prj_calc[n] += comp_prj->x_min_del
							* (1 - comp_prj->y_min_del) * slc_data[index];
					prj_calc_num[n] += comp_prj->x_min_del
							* (1 - comp_prj->y_min_del);

				} //end if
				if (comp_prj->x_min >= 0 && comp_prj->x_min < prj.X
						&& (comp_prj->y_min + 1) >= 0
						&& (comp_prj->y_min + 1) < prj.Y) //(x_min, y_min+1)
								{
					n = comp_prj->x_min + (comp_prj->y_min + 1) * prj.X; //prj index
					prj_calc[n] += (1 - comp_prj->x_min_del)
							* comp_prj->y_min_del * slc_data[index];
					prj_calc_num[n] += (1 - comp_prj->x_min_del)
							* comp_prj->y_min_del;
				}
				if ((comp_prj->x_min + 1) >= 0 && (comp_prj->x_min + 1) < prj.X
						&& (comp_prj->y_min + 1) >= 0
						&& (comp_prj->y_min + 1) < prj.Y) //(x_min+1, y_min+1)
								{
					n = (comp_prj->x_min + 1) + (comp_prj->y_min + 1) * prj.X; //prj index
					prj_calc[n] += comp_prj->x_min_del * comp_prj->y_min_del
							* slc_data[index];
					prj_calc_num[n] += comp_prj->x_min_del
							* comp_prj->y_min_del;
				}

			} //end for pixelXY

			//  printf("slice %lf is finished to reproj.\n",pixel.Z);
	} //end for pixel.Z

	return TRUE;

}

/**********************************************************************************/
int SART_one_update_bilinear(Pixel0 *pixel0, Projection prj, Volume vol,
		Weight *comp_prj, double *x_coef, double *y_coef, float zshift,
		float *prj_calc, float *slc_data, float SART_ITR_STEP, int Z_start,
		int Z_end, int angle) {

	double s;
	double c;

	int n, index;

	Pixel pixel;

	for (pixel.Z = Z_start; pixel.Z < Z_end; pixel.Z++) {

		for (pixel.Y = vol.Ystart; pixel.Y < vol.Yend; pixel.Y++)
			for (pixel.X = vol.Xstart; pixel.X < vol.Xend; pixel.X++) {
				index = (pixel.X - vol.Xstart) + (pixel.Y - vol.Ystart) * vol.X
						+ (pixel.Z - Z_start) * vol.X * vol.Y;   //pixel index
				s = 0;
				c = 0;

				val_coef(pixel0, pixel, angle, zshift, x_coef, y_coef,
						comp_prj);
				//  val_coef_recur(n, pixel, vol,j, x_coef, y_coef, comp_prj,x_val,qx_pre_val,qx_val,y_val,qy_pre_val,qy_val);

				/*   if (pixel.X==-62 && pixel.Y==82 && pixel.Z==-12)
				 printf("comp_prj->x is %d, comp_prj->y is %d in angle=%d\n", comp_prj->x_min, comp_prj->y_min,j);*/

				if (comp_prj->x_min >= 0 && comp_prj->x_min < prj.X
						&& comp_prj->y_min >= 0 && comp_prj->y_min < prj.Y) //(x_min, y_min)
								{
					n = comp_prj->x_min + comp_prj->y_min * prj.X; //prj_calc index
					//  prj_real_index=prj_cal_index+j*prj.X*prj.Y;  //prj_real index

					s += (1 - comp_prj->x_min_del) * (1 - comp_prj->y_min_del)
							* prj_calc[n];
					c += (1 - comp_prj->x_min_del) * (1 - comp_prj->y_min_del);
				} //end if

				if ((comp_prj->x_min + 1) >= 0 && (comp_prj->x_min + 1) < prj.X
						&& comp_prj->y_min >= 0 && comp_prj->y_min < prj.Y) //(x_min+1, y_min)
								{
					n = comp_prj->x_min + 1 + comp_prj->y_min * prj.X; //prj_calc index
					//  prj_real_index=prj_cal_index+j*prj.X*prj.Y;  //prj_real index

					s += comp_prj->x_min_del * (1 - comp_prj->y_min_del)
							* prj_calc[n];
					c += comp_prj->x_min_del * (1 - comp_prj->y_min_del);
				} //end if
				if (comp_prj->x_min >= 0 && comp_prj->x_min < prj.X
						&& (comp_prj->y_min + 1) >= 0
						&& (comp_prj->y_min + 1) < prj.Y) //(x_min, y_min+1)
								{
					n = comp_prj->x_min + (1 + comp_prj->y_min) * prj.X; //prj_calc index
					//    prj_real_index=prj_cal_index+j*prj.X*prj.Y;  //prj_real index

					s += (1 - comp_prj->x_min_del) * comp_prj->y_min_del
							* prj_calc[n];
					c += (1 - comp_prj->x_min_del) * comp_prj->y_min_del;
				} //end if
				if ((comp_prj->x_min + 1) >= 0 && (comp_prj->x_min + 1) < prj.X
						&& (comp_prj->y_min + 1) >= 0
						&& (comp_prj->y_min + 1) < prj.Y)   //(x_min+1, y_min+1)
								{
					n = comp_prj->x_min + 1 + (comp_prj->y_min + 1) * prj.X; //prj_calc index
					//  prj_real_index=prj_cal_index+j*prj.X*prj.Y;  //prj_real index

					s += comp_prj->x_min_del * comp_prj->y_min_del
							* prj_calc[n];
					c += comp_prj->x_min_del * comp_prj->y_min_del;
				} //end if

				if (c != 0)
					slc_data[index] += (float) (s / c) * SART_ITR_STEP;

			} // end pixel
			//  printf("slice %d is completed to update.\n",pixel.Z);

	}     // end pixel.Z

		  // printf("block in Z_start %d is completed\n",Z_start);

	return TRUE;

}

/****************************SART_update_slice******************************************************/
int SART_update_slice(MPI_File fin, float *prj_real, MrcHeader *inhead,
		Pixel0 *pixel0, Projection prj, Volume vol, Weight *comp_prj,
		double *x_coef, double *y_coef, float zshift, float *slc_data,
		float *prj_calc, float *global_prj_calc, float *prj_calc_num,
		float *global_prj_calc_num, float SART_ITR_STEP, int Z_start,
		int Z_end) {

	int line_num;
	line_num = prj.X * prj.Y;
	int angle, n;
	for (angle = 0; angle < prj.AngN; angle++) {
		SART_Slice_reproj_bilinear(pixel0, prj, vol, comp_prj, x_coef, y_coef,
				zshift, prj_calc, prj_calc_num, slc_data, Z_start, Z_end,
				angle);
		MPI_Allreduce(prj_calc, global_prj_calc, line_num, MPI_FLOAT, MPI_SUM,
				MPI_COMM_WORLD );
		MPI_Allreduce(prj_calc_num, global_prj_calc_num, line_num, MPI_FLOAT,
				MPI_SUM, MPI_COMM_WORLD );

		memset(prj_calc, 0, sizeof(float) * line_num);
		memset(prj_calc_num, 0, sizeof(float) * line_num);

		mrc_read_slice(fin, inhead, angle, 'Z', prj_real);

		for (n = 0; n < line_num; n++) {
			if (global_prj_calc_num[n] != 0)
				global_prj_calc[n] /= global_prj_calc_num[n];
			global_prj_calc[n] = prj_real[n] - global_prj_calc[n]; //projection error
		}

		SART_one_update_bilinear(pixel0, prj, vol, comp_prj, x_coef, y_coef,
				zshift, global_prj_calc, slc_data, SART_ITR_STEP, Z_start,
				Z_end, angle);

		memset(global_prj_calc, 0, sizeof(float) * line_num);
		memset(global_prj_calc_num, 0, sizeof(float) * line_num);

		PRINT_DEBUG_MSG("angle %d has done for Z_start %d\n", angle, Z_start);

	}

	return TRUE;
}
int Cu_SART_update_slice(MPI_File fin,float *prj_real, MrcHeader *inhead,
		double *x_coef, double *y_coef, float zshift, float *slc_data,
		float *prj_calc, float *global_prj_calc, float *prj_calc_num,
		float *global_prj_calc_num, float SART_ITR_STEP, int Z_start,
		int Z_end,ImageStruct dev_img,ImageStruct host_img) {

	int line_num;
	line_num = host_img.prj.X * host_img.prj.Y;
	int angle;

	for (angle = 0; angle < host_img.prj.AngN; angle++) {
		initDevMem(dev_img, line_num);
		//read prj
		mrc_read_slice(fin, inhead, angle, 'Z', prj_real);

		transferToDevPro(dev_img.prj_real, prj_real,line_num * sizeof(float));
		float* slc_start = host_img.slc_data;
		int start_vol = 0;
		int total_z_per = (host_img.vol.volZ_per - 1) / host_img.vol.Zcount
				+ 1;
		int size_vol_per = total_z_per * host_img.vol.X * host_img.vol.Y;

		for (int i_vol = 0; i_vol < host_img.vol.Zcount; i_vol++) {
			//printf("i_vol=%d \n ", i_vol);
			if (i_vol == host_img.vol.Zcount - 1) {
				if (host_img.vol.volZ_per % total_z_per != 0) {
					total_z_per = host_img.vol.volZ_per % total_z_per;
				}
				size_vol_per = total_z_per * host_img.vol.X* host_img.vol.Y;
			}
			//Transfer vol to GPU

			transferToDevVol(dev_img.slc_data, slc_start, size_vol_per * sizeof(float));

			//Calculate Projection in GPU
			Sart_Slice_reproj(start_vol, size_vol_per, angle, zshift);

			slc_start = slc_start + size_vol_per;
			start_vol = start_vol + total_z_per;
		}   // end for vol
		transferToHostPro(dev_img.prj_calc, host_img.prj_calc,
				line_num * sizeof(float));
		transferToHostPro(dev_img.prj_calc_num, host_img.prj_calc_num,
				line_num * sizeof(float));


		MPI_Allreduce(host_img.prj_calc, host_img.global_prj_calc, line_num, MPI_FLOAT, MPI_SUM,
				MPI_COMM_WORLD );
		MPI_Allreduce(host_img.prj_calc_num, host_img.global_prj_calc_num, line_num, MPI_FLOAT,
				MPI_SUM, MPI_COMM_WORLD );

#ifdef wzh

    			PRINT_DEBUG_MSG("MPI_Allreduce 1047534 :%f\n",host_img.global_prj_calc[1047534]);
#endif
		transferToDevPro(dev_img.prj_calc, host_img.global_prj_calc,line_num * sizeof(float));
		transferToDevPro(dev_img.prj_calc_num, host_img.global_prj_calc_num,line_num * sizeof(float));


		calPrj(dev_img.prj_real, dev_img.prj_calc, dev_img.prj_calc_num, line_num);

		slc_start = host_img.slc_data;
		start_vol = 0;
		total_z_per = (host_img.vol.volZ_per - 1) / host_img.vol.Zcount + 1;
		size_vol_per = total_z_per * host_img.vol.X * host_img.vol.Y;

		for (int i_vol = 0; i_vol < host_img.vol.Zcount; i_vol++) {
			if (i_vol == host_img.vol.Zcount - 1) {
				if (host_img.vol.volZ_per % total_z_per != 0) {
					total_z_per = host_img.vol.volZ_per % total_z_per;
				}
				size_vol_per = total_z_per * host_img.vol.X
						* host_img.vol.Y;
			}
			//Transfer vol to GPU

			transferToDevVol(dev_img.slc_data, slc_start,
					size_vol_per * sizeof(float));

			//Calculate the ASART in GPU
			updateSlc(angle, zshift, start_vol, size_vol_per, SART_ITR_STEP);
			//Transfer vol back to Host
			transferToHost(slc_start, dev_img.slc_data,
					size_vol_per * sizeof(float));
			//transferToHost(slc_start, dev_img.prj_calc,
			//		line_num * sizeof(float));

			slc_start = slc_start + size_vol_per;
			start_vol = start_vol + total_z_per;
		}   // end for vol


		memset(host_img.global_prj_calc, 0, sizeof(float) * line_num);
		memset(host_img.global_prj_calc_num, 0, sizeof(float) * line_num);
//		initprj_calcMem(dev_img.prj_calc,line_num);
//		initprj_calcMem(dev_img.prj_calc_num,line_num);
//		initprj_calcMem(dev_img.prj_calc,line_num);
//		initprj_calcMem(dev_img.prj_calc_num,line_num);
#ifdef wzh

    	PRINT_DEBUG_MSG("slc_data 1047534 :%f\n",host_img.slc_data[1047534]);
#endif
		PRINT_DEBUG_MSG("angle %d has done for Z_start %d\n", angle, Z_start);

	}

	return TRUE;
}

//ASART
int Cu_ASART_update_slice(MPI_File fin,float *prj_real, MrcHeader *inhead,
		double *x_coef, double *y_coef, float zshift, float *slc_data,
		float *prj_calc, float *global_prj_calc, float *prj_calc_num,
		float *global_prj_calc_num, float *prj_calc_abs,
		float *global_prj_calc_abs, float ASART_ITR_STEP, int Z_start,
		int Z_end,ImageStruct dev_img,ImageStruct host_img) {

	int line_num;
	line_num = host_img.prj.X * host_img.prj.Y;
	int angle;

	//for (angle = 0; angle < host_img.prj.AngN; angle++) {
		for (angle = 0; angle < 3; angle++) {
		initDevMem(dev_img, line_num);
		//read prj
		mrc_read_slice(fin, inhead, angle, 'Z', prj_real);

		transferToDevPro(dev_img.prj_real, prj_real,line_num * sizeof(float));
		float* slc_start = host_img.slc_data;
		int start_vol = 0;
		int total_z_per = (host_img.vol.volZ_per - 1) / host_img.vol.Zcount
				+ 1;
		int size_vol_per = total_z_per * host_img.vol.X * host_img.vol.Y;

		for (int i_vol = 0; i_vol < host_img.vol.Zcount; i_vol++) {
			//printf("i_vol=%d \n ", i_vol);
			if (i_vol == host_img.vol.Zcount - 1) {
				if (host_img.vol.volZ_per % total_z_per != 0) {
					total_z_per = host_img.vol.volZ_per % total_z_per;
				}
				size_vol_per = total_z_per * host_img.vol.X* host_img.vol.Y;
			}
			//Transfer vol to GPU

			transferToDevVol(dev_img.slc_data, slc_start, size_vol_per * sizeof(float));

			//Calculate Projection in GPU
			Asart_Slice_reproj(start_vol, size_vol_per, angle, zshift);

			slc_start = slc_start + size_vol_per;
			start_vol = start_vol + total_z_per;
		}   // end for vol
		transferToHostPro(dev_img.prj_calc, host_img.prj_calc,
				line_num * sizeof(float));
		transferToHostPro(dev_img.prj_calc_num, host_img.prj_calc_num,
				line_num * sizeof(float));
		transferToHostPro(dev_img.prj_calc_abs, host_img.prj_calc_abs,
				line_num * sizeof(float));


		MPI_Allreduce(host_img.prj_calc, host_img.global_prj_calc, line_num, MPI_FLOAT, MPI_SUM,
				MPI_COMM_WORLD );
		MPI_Allreduce(host_img.prj_calc_num, host_img.global_prj_calc_num, line_num, MPI_FLOAT,
				MPI_SUM, MPI_COMM_WORLD );
		MPI_Allreduce(host_img.prj_calc_abs, host_img.global_prj_calc_abs, line_num, MPI_FLOAT,
				MPI_SUM, MPI_COMM_WORLD );

#ifdef wzh

    			PRINT_DEBUG_MSG("MPI_Allreduce 1047534 :%f\n",host_img.global_prj_calc[1047534]);
#endif
		transferToDevPro(dev_img.prj_calc, host_img.global_prj_calc,line_num * sizeof(float));
		transferToDevPro(dev_img.prj_calc_num, host_img.global_prj_calc_num,line_num * sizeof(float));
		transferToDevPro(dev_img.prj_calc_abs, host_img.global_prj_calc_abs,line_num * sizeof(float));
//		memset(prj_calc, 0, sizeof(float) * line_num);
//		memset(prj_calc_num, 0, sizeof(float) * line_num);


		calPrj(dev_img.prj_real, dev_img.prj_calc, dev_img.prj_calc_num, line_num);

		slc_start = host_img.slc_data;
		start_vol = 0;
		total_z_per = (host_img.vol.volZ_per - 1) / host_img.vol.Zcount + 1;
		size_vol_per = total_z_per * host_img.vol.X * host_img.vol.Y;

		for (int i_vol = 0; i_vol < host_img.vol.Zcount; i_vol++) {
			if (i_vol == host_img.vol.Zcount - 1) {
				if (host_img.vol.volZ_per % total_z_per != 0) {
					total_z_per = host_img.vol.volZ_per % total_z_per;
				}
				size_vol_per = total_z_per * host_img.vol.X
						* host_img.vol.Y;
			}
			//Transfer vol to GPU

			transferToDevVol(dev_img.slc_data, slc_start,
					size_vol_per * sizeof(float));

			//Calculate the ASART in GPU
			Asart_updateSlc(angle, zshift, start_vol, size_vol_per, ASART_ITR_STEP);
			//Transfer vol back to Host
			transferToHost(slc_start, dev_img.slc_data,
					size_vol_per * sizeof(float));
			//transferToHost(slc_start, dev_img.prj_calc,
			//		line_num * sizeof(float));

			slc_start = slc_start + size_vol_per;
			start_vol = start_vol + total_z_per;
		}   // end for vol


		memset(host_img.global_prj_calc, 0, sizeof(float) * line_num);
		memset(host_img.global_prj_calc_num, 0, sizeof(float) * line_num);
		memset(host_img.global_prj_calc_abs, 0, sizeof(float) * line_num);
//		initprj_calcMem(dev_img.prj_calc,line_num);
//		initprj_calcMem(dev_img.prj_calc_num,line_num);
		initprj_calcMem(dev_img.prj_calc,line_num);
		initprj_calcMem(dev_img.prj_calc_abs,line_num);
		initprj_calcMem(dev_img.prj_calc_num,line_num);
#ifdef wzh

    			PRINT_DEBUG_MSG("slc_data 1047534 :%f\n",host_img.slc_data[1047534]);
#endif
		PRINT_DEBUG_MSG("angle %d has done for Z_start %d\n", angle, Z_start);

	}

	return TRUE;
}

/********************************ASART_Slice_reproj_bilinear**************************************************/
int ASART_Slice_reproj_bilinear(Pixel0 *pixel0, Projection prj, Volume vol,
		Weight *comp_prj, double *x_coef, double *y_coef, float zshift,
		float *prj_calc, float *prj_calc_abs, float *prj_calc_num,
		float *slc_data, int Z_start, int Z_end, int angle) {

	int n, index;

	Pixel pixel;

	for (pixel.Z = Z_start; pixel.Z < Z_end; pixel.Z++) {

		for (pixel.Y = vol.Ystart; pixel.Y < vol.Yend; pixel.Y++)
			for (pixel.X = vol.Xstart; pixel.X < vol.Xend; pixel.X++) {
				index = (pixel.X - vol.Xstart) + (pixel.Y - vol.Ystart) * vol.X
						+ (pixel.Z - Z_start) * vol.X * vol.Y;   //pixel index

				val_coef(pixel0, pixel, angle, zshift, x_coef, y_coef,
						comp_prj);

				if (comp_prj->x_min >= 0 && comp_prj->x_min < prj.X
						&& comp_prj->y_min >= 0 && comp_prj->y_min < prj.Y) //(x_min, y_min)
								{

					n = comp_prj->x_min + comp_prj->y_min * prj.X; //prj index
					prj_calc[n] += (1 - comp_prj->x_min_del)
							* (1 - comp_prj->y_min_del) * slc_data[index];
					prj_calc_abs[n] += (1 - comp_prj->x_min_del)
							* (1 - comp_prj->y_min_del) * abs(slc_data[index]);
					prj_calc_num[n] += (1 - comp_prj->x_min_del)
							* (1 - comp_prj->y_min_del);

				} //end if
				if ((comp_prj->x_min + 1) >= 0 && (comp_prj->x_min + 1) < prj.X
						&& comp_prj->y_min >= 0 && comp_prj->y_min < prj.Y) //(x_min+1, y_min)
								{

					n = comp_prj->x_min + 1 + comp_prj->y_min * prj.X; //prj index
					prj_calc[n] += comp_prj->x_min_del
							* (1 - comp_prj->y_min_del) * slc_data[index];
					prj_calc_abs[n] += comp_prj->x_min_del
							* (1 - comp_prj->y_min_del) * abs(slc_data[index]);
					prj_calc_num[n] += comp_prj->x_min_del
							* (1 - comp_prj->y_min_del);

				} //end if
				if (comp_prj->x_min >= 0 && comp_prj->x_min < prj.X
						&& (comp_prj->y_min + 1) >= 0
						&& (comp_prj->y_min + 1) < prj.Y) //(x_min, y_min+1)
								{
					n = comp_prj->x_min + (comp_prj->y_min + 1) * prj.X; //prj index
					prj_calc[n] += (1 - comp_prj->x_min_del)
							* comp_prj->y_min_del * slc_data[index];
					prj_calc_abs[n] += (1 - comp_prj->x_min_del)
							* comp_prj->y_min_del * abs(slc_data[index]);
					prj_calc_num[n] += (1 - comp_prj->x_min_del)
							* comp_prj->y_min_del;
				}
				if ((comp_prj->x_min + 1) >= 0 && (comp_prj->x_min + 1) < prj.X
						&& (comp_prj->y_min + 1) >= 0
						&& (comp_prj->y_min + 1) < prj.Y) //(x_min+1, y_min+1)
								{
					n = (comp_prj->x_min + 1) + (comp_prj->y_min + 1) * prj.X; //prj index
					prj_calc[n] += comp_prj->x_min_del * comp_prj->y_min_del
							* slc_data[index];
					prj_calc_abs[n] += comp_prj->x_min_del * comp_prj->y_min_del
							* abs(slc_data[index]);
					prj_calc_num[n] += comp_prj->x_min_del
							* comp_prj->y_min_del;
				}

			} //end for pixelXY

			//  printf("slice %lf is finished to reproj.\n",pixel.Z);
	} //end for pixel.Z

	return TRUE;

}

/**********************************************************************************/
int ASART_one_update_bilinear(Pixel0 *pixel0, Projection prj, Volume vol,
		Weight *comp_prj, double *x_coef, double *y_coef, float zshift,
		float *prj_calc, float *prj_calc_abs, float *slc_data,
		float ASART_ITR_STEP, int Z_start, int Z_end, int angle) {

	double s, t;
	double c;

	int n, index;

	Pixel pixel;

	for (pixel.Z = Z_start; pixel.Z < Z_end; pixel.Z++) {

		for (pixel.Y = vol.Ystart; pixel.Y < vol.Yend; pixel.Y++)
			for (pixel.X = vol.Xstart; pixel.X < vol.Xend; pixel.X++) {
				index = (pixel.X - vol.Xstart) + (pixel.Y - vol.Ystart) * vol.X
						+ (pixel.Z - Z_start) * vol.X * vol.Y;   //pixel index
				s = 0;
				t = 0;
				c = 0;

				val_coef(pixel0, pixel, angle, zshift, x_coef, y_coef,
						comp_prj);

				if (comp_prj->x_min >= 0 && comp_prj->x_min < prj.X
						&& comp_prj->y_min >= 0 && comp_prj->y_min < prj.Y) //(x_min, y_min)
								{
					n = comp_prj->x_min + comp_prj->y_min * prj.X; //prj_calc index
					//  prj_real_index=prj_cal_index+j*prj.X*prj.Y;  //prj_real index

					s += (1 - comp_prj->x_min_del) * (1 - comp_prj->y_min_del)
							* prj_calc[n];
					t += (1 - comp_prj->x_min_del) * (1 - comp_prj->y_min_del)
							* prj_calc_abs[n];
					c += (1 - comp_prj->x_min_del) * (1 - comp_prj->y_min_del);
				} //end if

				if ((comp_prj->x_min + 1) >= 0 && (comp_prj->x_min + 1) < prj.X
						&& comp_prj->y_min >= 0 && comp_prj->y_min < prj.Y) //(x_min+1, y_min)
								{
					n = comp_prj->x_min + 1 + comp_prj->y_min * prj.X; //prj_calc index
					//  prj_real_index=prj_cal_index+j*prj.X*prj.Y;  //prj_real index

					s += comp_prj->x_min_del * (1 - comp_prj->y_min_del)
							* prj_calc[n];
					t += comp_prj->x_min_del * (1 - comp_prj->y_min_del)
							* prj_calc_abs[n];
					c += comp_prj->x_min_del * (1 - comp_prj->y_min_del);
				} //end if
				if (comp_prj->x_min >= 0 && comp_prj->x_min < prj.X
						&& (comp_prj->y_min + 1) >= 0
						&& (comp_prj->y_min + 1) < prj.Y) //(x_min, y_min+1)
								{
					n = comp_prj->x_min + (1 + comp_prj->y_min) * prj.X; //prj_calc index
					//    prj_real_index=prj_cal_index+j*prj.X*prj.Y;  //prj_real index

					s += (1 - comp_prj->x_min_del) * comp_prj->y_min_del
							* prj_calc[n];
					t += (1 - comp_prj->x_min_del) * comp_prj->y_min_del
							* prj_calc_abs[n];
					c += (1 - comp_prj->x_min_del) * comp_prj->y_min_del;
				} //end if
				if ((comp_prj->x_min + 1) >= 0 && (comp_prj->x_min + 1) < prj.X
						&& (comp_prj->y_min + 1) >= 0
						&& (comp_prj->y_min + 1) < prj.Y)   //(x_min+1, y_min+1)
								{
					n = comp_prj->x_min + 1 + (comp_prj->y_min + 1) * prj.X; //prj_calc index
					//  prj_real_index=prj_cal_index+j*prj.X*prj.Y;  //prj_real index

					s += comp_prj->x_min_del * comp_prj->y_min_del
							* prj_calc[n];
					t += comp_prj->x_min_del * comp_prj->y_min_del
							* prj_calc[n];
					c += comp_prj->x_min_del * comp_prj->y_min_del;
				} //end if

				if (t != 0)
					slc_data[index] += abs(slc_data[index]) * (float) s / t
							* ASART_ITR_STEP;
			} // end pixel
			//  printf("slice %d is completed to update.\n",pixel.Z);
	}     // end pixel.Z

		  // printf("block in Z_start %d is completed\n",Z_start);

	return TRUE;

}

/****************************ASART_update_slice******************************************************/
int ASART_update_slice(MPI_File fin, float *prj_real, MrcHeader *inhead,
		Pixel0 *pixel0, Projection prj, Volume vol, Weight *comp_prj,
		double *x_coef, double *y_coef, float zshift, float *slc_data,
		float *prj_calc, float *global_prj_calc, float *prj_calc_abs,
		float *global_prj_calc_abs, float *prj_calc_num,
		float *global_prj_calc_num, float SART_ITR_STEP, int Z_start,
		int Z_end) {

	int line_num;
	line_num = prj.X * prj.Y;
	int angle, n;
	for (angle = 0; angle < prj.AngN; angle++) {
		ASART_Slice_reproj_bilinear(pixel0, prj, vol, comp_prj, x_coef, y_coef,
				zshift, prj_calc, prj_calc_abs, prj_calc_num, slc_data, Z_start,
				Z_end, angle);
		MPI_Allreduce(prj_calc, global_prj_calc, line_num, MPI_FLOAT, MPI_SUM,
				MPI_COMM_WORLD );
		MPI_Allreduce(prj_calc_abs, global_prj_calc_abs, line_num, MPI_FLOAT,
				MPI_SUM, MPI_COMM_WORLD );
		MPI_Allreduce(prj_calc_num, global_prj_calc_num, line_num, MPI_FLOAT,
				MPI_SUM, MPI_COMM_WORLD );

		memset(prj_calc, 0, sizeof(float) * line_num);
		memset(prj_calc_abs, 0, sizeof(float) * line_num);
		memset(prj_calc_num, 0, sizeof(float) * line_num);

		mrc_read_slice(fin, inhead, angle, 'Z', prj_real);

		for (n = 0; n < line_num; n++) {
			if (global_prj_calc_num[n] != 0)
				global_prj_calc[n] /= global_prj_calc_num[n];
			global_prj_calc[n] = prj_real[n] - global_prj_calc[n]; //projection error
		}

		ASART_one_update_bilinear(pixel0, prj, vol, comp_prj, x_coef, y_coef,
				zshift, global_prj_calc, global_prj_calc_abs, slc_data,
				SART_ITR_STEP, Z_start, Z_end, angle);

		memset(global_prj_calc, 0, sizeof(float) * line_num);
		memset(global_prj_calc_abs, 0, sizeof(float) * line_num);
		memset(global_prj_calc_num, 0, sizeof(float) * line_num);

		PRINT_DEBUG_MSG("angle %d has done for Z_start %d\n", angle, Z_start);

	}

	return TRUE;
}

/**********************************************************************************/

int ATOM(options *opt, int myid, int mypro) {

	const char *inf = opt->input.c_str();
	const char *tilt_f = opt->tiltangle.c_str();
	const char *pitch_f = opt->xtiltangle.c_str();
	const char *outf = opt->output.c_str();
	float offset = opt->offset;
	float xaxistilt = opt->xaxistilt;
	float zshift = opt->zshift;
	const char *filter = opt->filter.c_str();

	MPI_File fin, fout;

	MPI_File_open(MPI_COMM_WORLD, const_cast<char*>(inf), MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &fin);
	MPI_File_open(MPI_COMM_WORLD, const_cast<char*>(outf), MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &fout);

	MrcHeader *inhead, *outhead;

	inhead = (MrcHeader *) malloc(sizeof(MrcHeader));
	outhead = (MrcHeader *) malloc(sizeof(MrcHeader));

	if (myid == 0)
		mrc_read_head_MPI(fin, inhead);
	MPI_Bcast(inhead, 1024, MPI_CHAR, 0, MPI_COMM_WORLD );

	mrc_init_head(outhead);
	outhead->nx = inhead->nx;
	outhead->ny = inhead->ny;
	outhead->nz = opt->thickness;

	if (myid == 0)
		mrc_write_head(fout, outhead);

	Volume vol;
	vol.X = outhead->nx;
	vol.Y = outhead->ny;
	vol.Z = outhead->nz;
	vol.Xstart = 0;
	vol.Xend = vol.Xstart + vol.X;
	vol.Ystart = 0;
	vol.Yend = vol.Ystart + vol.Y;
	vol.Zstart = 0;
	vol.Zend = vol.Zstart + vol.Z;

	if (myid == 0)
		PRINT_DEBUG_MSG("vol.Xend is %d, vol.Yend is %d, vol.Zend is %d\n", vol.Xend,
				vol.Yend, vol.Zend);

	Projection prj;
	prj.X = vol.X;
	prj.Y = vol.Y;
	prj.AngN = inhead->nz;

	FILE *f_tilt, *f_pitch;
	f_tilt = fopen(tilt_f, "r");
	f_pitch = fopen(pitch_f, "r");

	double *tilt_angle;
	if ((tilt_angle = (double *) malloc(sizeof(double) * prj.AngN)) == NULL) {
		printf("Can't malloc memery for 'tilt_angle'!");
		return FALSE;
	}
	memset(tilt_angle, 0, sizeof(double) * prj.AngN);
	read_coef(tilt_angle, f_tilt, myid);

	double *pitch_angle;
	if ((pitch_angle = (double *) malloc(sizeof(double) * prj.AngN)) == NULL) {
		printf(
				"Error with Function 'ATOM()'!Can't malloc memery for 'pitch_angle'!");
		return FALSE;
	}
	memset(pitch_angle, 0, sizeof(double) * prj.AngN);
	read_coef(pitch_angle, f_pitch, myid);

	double *x_coef;
	x_coef = (double *) malloc(sizeof(double) * prj.AngN * 3);
	double *y_coef;
	y_coef = (double *) malloc(sizeof(double) * prj.AngN * 3);

//  cal_coef(x_coef, y_coef, tilt_angle, pitch_angle, prj.AngN);
	cal_coef(x_coef, y_coef, tilt_angle, pitch_angle, offset, xaxistilt,
			prj.AngN);

	/************************malloc memory for prj, slc, slc->data****************************************/
	int line_num = prj.X * prj.Y;
	// int pixel_num=vol.X*vol.Y*vol.Z;
	int volZ_per;
	int volZ_add = vol.Z % mypro;
	int Z_start;   //the start slice of reproject per process
	int Z_end;   //the end slice of reproject per process

	if (myid < volZ_add) {
		volZ_per = vol.Z / mypro + 1;
		Z_start = volZ_per * myid + vol.Zstart;
		Z_end = volZ_per * (myid + 1) + vol.Zstart;
	} else {
		volZ_per = vol.Z / mypro;
		Z_start = volZ_per * myid + volZ_add + vol.Zstart;
		Z_end = volZ_per * (myid + 1) + volZ_add + vol.Zstart;
	}
	PRINT_DEBUG_MSG("Z_start is %d, volZ_per is %d, Z_end is %d in process %d\n",
			Z_start, volZ_per, Z_end, myid);

	int pixel_num_per = vol.X * vol.Y * volZ_per;

	vol.volZ_per=volZ_per;
	vol.Zstart_per=Z_start;
	vol.Zend_per=Z_end;

	// printf("pixel_num_per is %d in process %d\n",pixel_num_per,myid);

	// int slcN_per=0;
	// int global_slcN=0;

//for input file
	float *prj_real;
	if ((prj_real = (float *) malloc(sizeof(float) * line_num)) == NULL) {
		printf("Can't malloc memery for 'prj_real'!");
		return FALSE;
	}
	memset(prj_real, 0, sizeof(float) * line_num);

//for output file
	float *slc_data;
	if ((slc_data = (float *) malloc(sizeof(float) * pixel_num_per)) == NULL) {
		printf(
				"Error with Function 'ATOM()'!Can't malloc memery for 'slc_data'!");
		return FALSE;
	}
	memset(slc_data, 0, sizeof(float) * pixel_num_per);

	Pixel0 *pixel0;
	pixel0 = (Pixel0 *) malloc(sizeof(Pixel0));
	pixel0->X = ((float) (vol.Xend - vol.Xstart)) / 2;
	pixel0->Y = ((float) (vol.Yend - vol.Ystart)) / 2;
	pixel0->Z = ((float) (vol.Zend - vol.Zstart)) / 2;

	if (myid == 0)
		PRINT_DEBUG_MSG("pixel0->X is %f, pixel0->Y is %f, pixel0->Z is %f\n", pixel0->X,
				pixel0->Y, pixel0->Z);

	Weight *comp_prj;
	comp_prj = (Weight *) malloc(sizeof(Weight));
	if ((comp_prj = (Weight *) malloc(sizeof(Weight))) == NULL) {
		printf(
				"Error with Function 'ATOM()'!Can't malloc memery for 'comp_prj'!");
		return FALSE;
	}

	/**********************reconstruction begin along Z-axis***********************************************************/

	if (opt->method == "BPT") {

#ifdef GPU
		long int totalGlobalMem = initGPU(myid);
		double *dev_x_coef, *dev_y_coef;

		initMalloc_FBP(dev_x_coef, dev_y_coef, prj.AngN);
		initDevMem_FBP(dev_x_coef, dev_y_coef, prj.AngN);
		transferToDev_FBP(pixel0, dev_x_coef, x_coef, dev_y_coef, y_coef, prj.AngN, prj,
				vol);
		Cu_Slice_backproj_bilinear_BPT(fin, inhead, pixel0, prj, vol, comp_prj, x_coef,
				y_coef, zshift, slc_data, Z_start, Z_end, myid, pixel_num_per,
				dev_x_coef, dev_y_coef, totalGlobalMem);
#else
		Slice_backproj_BPT(fin, prj_real, inhead, pixel0, prj, vol, comp_prj,
				x_coef, y_coef, zshift, slc_data, Z_start, Z_end, myid);
#endif
	}  // end FBP

	else if (opt->method == "FBP") {


#ifdef GPU
		long int totalGlobalMem = initGPU(myid);
		double *dev_x_coef, *dev_y_coef;

		initMalloc_FBP(dev_x_coef, dev_y_coef, prj.AngN);
		initDevMem_FBP(dev_x_coef, dev_y_coef, prj.AngN);
		transferToDev_FBP(pixel0, dev_x_coef, x_coef, dev_y_coef, y_coef, prj.AngN, prj,
				vol);

		Cu_Slice_backproj_bilinear(fin, inhead, pixel0, prj, vol, comp_prj, x_coef,
				y_coef, zshift, slc_data, Z_start, Z_end, myid, pixel_num_per,
				dev_x_coef, dev_y_coef, totalGlobalMem, filter);
#else
		Slice_backproj_FBP(fin, prj_real, inhead, pixel0, prj, vol, comp_prj,
						x_coef, y_coef, zshift, slc_data, Z_start, Z_end, myid, filter);
#endif

	}  // end FBP

	else if (opt->method == "SART") {
		//for Iterative method
		float *prj_calc, *global_prj_calc, *prj_calc_num, *global_prj_calc_num;
		prj_calc = (float *) malloc(sizeof(float) * line_num);
		memset(prj_calc, 0, sizeof(float) * line_num);
		global_prj_calc = (float *) malloc(sizeof(float) * line_num);
		memset(global_prj_calc, 0, sizeof(float) * line_num);

		prj_calc_num = (float *) malloc(sizeof(float) * line_num);
		memset(prj_calc_num, 0, sizeof(float) * line_num);
		global_prj_calc_num = (float *) malloc(sizeof(float) * line_num);
		memset(global_prj_calc_num, 0, sizeof(float) * line_num);

		const char *initialf = opt->initial.c_str();
		MPI_File f_initial;

		MPI_File_open(MPI_COMM_WORLD, const_cast<char*>(initialf), MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &f_initial);
		mrc_read_block(f_initial, outhead, Z_start, Z_end, 'Z', slc_data);

		float SART_ITR_STEP;
		SART_ITR_STEP = opt->step;
#define SART
#define GPU
		ImageStruct host_img, dev_img;

		host_img.vol = vol;
		host_img.prj = prj;
		host_img.pixel0.X = pixel0->X;
		host_img.pixel0.Y = pixel0->Y;
		host_img.pixel0.Z = pixel0->Z;
		host_img.x_coef = x_coef;
		host_img.y_coef = y_coef;
		host_img.prj_real = prj_real;
		host_img.slc_data = slc_data;
		host_img.prj_calc = prj_calc;
		host_img.prj_calc_num = prj_calc_num;
		host_img.global_prj_calc = global_prj_calc;
		host_img.global_prj_calc_num=global_prj_calc_num;
		long int total_mem = initGPU(myid);

		dev_img = host_img;
		host_img.vol.Zcount = initMalloc(dev_img, host_img.prj.AngN, line_num,
				total_mem);
		transferToDev(&dev_img, &host_img, host_img.prj.AngN);

		int i; //itertive number
		for (i = 0; i < opt->iter_num; i++) {

			Cu_SART_update_slice(fin,prj_real, inhead, x_coef, y_coef, zshift, slc_data, prj_calc,
					global_prj_calc, prj_calc_num, global_prj_calc_num,
					SART_ITR_STEP, Z_start, Z_end,dev_img,host_img);

			 memset(prj_calc, 0 , sizeof(float)*line_num);
			 memset(prj_calc_num, 0 , sizeof(float)*line_num);
			printf("Iteration %d has done in Z_start %d\n", i, Z_start);
/*
  			SART_update_slice(MPI_File fin, float *prj_real, MrcHeader *inhead,
					Pixel0 *pixel0, Projection prj, Volume vol, Weight *comp_prj,
					double *x_coef, double *y_coef, float zshift, float *slc_data,
					float *prj_calc, float *global_prj_calc, float *prj_calc_num,
					float *global_prj_calc_num, float SART_ITR_STEP, int Z_start,
					int Z_end)
*/
		}
		freeDevMem(&dev_img);

	} // end if SART

	else if (opt->method == "ASART") {
		//for Iterative method
		float *prj_calc, *global_prj_calc, *prj_calc_abs, *global_prj_calc_abs,
				*prj_calc_num, *global_prj_calc_num;
		prj_calc = (float *) malloc(sizeof(float) * line_num);
		memset(prj_calc, 0, sizeof(float) * line_num);
		global_prj_calc = (float *) malloc(sizeof(float) * line_num);
		memset(global_prj_calc, 0, sizeof(float) * line_num);

		prj_calc_abs = (float *) malloc(sizeof(float) * line_num);
		memset(prj_calc_abs, 0, sizeof(float) * line_num);
		global_prj_calc_abs = (float *) malloc(sizeof(float) * line_num);
		memset(global_prj_calc_abs, 0, sizeof(float) * line_num);

		prj_calc_num = (float *) malloc(sizeof(float) * line_num);
		memset(prj_calc_num, 0, sizeof(float) * line_num);
		global_prj_calc_num = (float *) malloc(sizeof(float) * line_num);
		memset(global_prj_calc_num, 0, sizeof(float) * line_num);

		const char *initialf = opt->initial.c_str();
		MPI_File f_initial;

		MPI_File_open(MPI_COMM_WORLD, const_cast<char*>(initialf), MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &f_initial);
		mrc_read_block(f_initial, outhead, Z_start, Z_end, 'Z', slc_data);

		float ASART_ITR_STEP;
		ASART_ITR_STEP = opt->step;
#define ASART
		ImageStruct host_img, dev_img;
		host_img.vol = vol;
		host_img.prj = prj;
		host_img.pixel0.X = pixel0->X;
		host_img.pixel0.Y = pixel0->Y;
		host_img.pixel0.Z = pixel0->Z;
		host_img.x_coef = x_coef;
		host_img.y_coef = y_coef;
		host_img.prj_real = prj_real;
		host_img.slc_data = slc_data;
		host_img.prj_calc = prj_calc;
		host_img.prj_calc_num = prj_calc_num;
		host_img.prj_calc_abs = prj_calc_abs;
		host_img.global_prj_calc = global_prj_calc;
		host_img.global_prj_calc_num=global_prj_calc_num;
		host_img.global_prj_calc_abs = global_prj_calc_abs;

		long int total_mem = initGPU(myid);

		dev_img = host_img;
		host_img.vol.Zcount = ASARTinitMalloc(dev_img, host_img.prj.AngN, line_num,
				total_mem);
		transferToDev(&dev_img, &host_img, host_img.prj.AngN);



		int i; //itertive number
		for (i = 0; i < opt->iter_num; i++) {


			Cu_ASART_update_slice(fin,prj_real, inhead, x_coef, y_coef, zshift, slc_data, prj_calc,
					global_prj_calc, prj_calc_num, global_prj_calc_num,prj_calc_abs, global_prj_calc_abs,
					ASART_ITR_STEP, Z_start, Z_end,dev_img,host_img);

			 memset(prj_calc, 0 , sizeof(float)*line_num);
			 memset(prj_calc_abs, 0 , sizeof(float)*line_num);
			 memset(prj_calc_num, 0 , sizeof(float)*line_num);
#ifndef GPU
			ASART_update_slice(fin,prj_real, inhead, pixel0, prj, vol,
					comp_prj, x_coef, y_coef, zshift, slc_data, prj_calc,
					global_prj_calc, prj_calc_abs, global_prj_calc_abs,
					prj_calc_num, global_prj_calc_num, ASART_ITR_STEP, Z_start,
					Z_end);
#endif
			printf("Iteration %d has done in Z_start %d\n", i, Z_start);

		}


	} // end if ASART

	/*   else if (opt->method=="ASIRT")
	 {
	 const char *initialf = opt->initial.c_str();
	 MPI_File  f_initial;

	 MPI_File_open(MPI_COMM_WORLD, initialf, MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &f_initial);
	 mrc_read_block(f_initial, outhead, Z_start, Z_end, 'Z', slc_data);

	 float ASIRT_ITR_STEP;
	 ASIRT_ITR_STEP=opt->step;

	 float *prj_calc_abs;
	 prj_calc_abs  = (float *)malloc(sizeof(float)*line_num);
	 memset(prj_calc_abs, 0 , sizeof(float)*line_num);

	 for(j=Y_start;j<Y_end;j++)
	 {
	 mrc_read_slice(f_initial, outhead, j, 'Y', slc->data);
	 mrc_read_slice(fin, inhead, j, 'y', prj_real);

	 slc->min=slid->X*((j*(Z_minE-Z_minS))/vol.Y+Z_minS);
	 slc->max=slid->X*((j*(Z_maxE-Z_maxS))/vol.Y+Z_maxS);

	 int i; //itertive number
	 for(i=0;i<opt->iter_num;i++)
	 {
	 ASIRT_update_slice(prj, prj_real, prj_calc, prj_calc_abs, prj_calc_num, slc, slid, tilt_angle, pixel_num, ASIRT_ITR_STEP);
	 printf("Iteration %d has done in Y_slice %d\n",i,j);

	 memset(prj_calc, 0 , sizeof(float)*line_num);
	 memset(prj_calc_abs, 0 , sizeof(float)*line_num);
	 memset(prj_calc_num, 0 , sizeof(float)*line_num);
	 }
	 }
	 }// end if*/

	mrc_write_block(fout, outhead, Z_start, Z_end, 'Z', slc_data);

	MPI_Barrier(MPI_COMM_WORLD );

	mrc_update_head_MPI(fout, myid, mypro);

	free(slc_data);

	free(inhead);
	inhead = NULL;

	free(outhead);
	outhead = NULL;

	MPI_File_close(&fin);
	MPI_File_close(&fout);

	return TRUE;
}

