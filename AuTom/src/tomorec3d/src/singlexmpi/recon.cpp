#include "recon.h"
#include "../main/function.h"
#include "atommpi.h"
#include "filter_prj.h"
#include <mpi.h>
#include <omp.h>

// #ifndef FALSE
// #define FALSE       1           /*false for boolean*/
// #endif
// #ifndef TRUE
// #define TRUE        0           /*true for boolean*/
// #endif
//#define TYPE float
//#define TYPE float

//#define TYPE float
//#define TYPE short
#define TYPE float
#define MILLION 1000000
//#define GPU

/**********************************************************************************/
/******************************************backprojection****************************************/
int Slice_backproj(Projection prj, float *prj_real, Slice *slc, Slide *slid,
		float *tilt_angle, Volume vol, int j, int Y_start, long long volY_per,
		float *cosang, float *sinang) {
	int x0, z0;
	int n, m, z, x, angle, r, row_index;
	long long pixel_index;
	float weight, c;
	float s;
	float rf;

	// #pragma omp parallel for
	int volxmultvolYper = vol.X * volY_per;
	int slicenum = (j - Y_start) * vol.X;
	int volxz = vol.X * vol.Z;
	for (n = 0; n < volxz; n++) {
		//for (n= volxz ; n--;) {
		x0 = n % vol.X; //1
		z0 = n / vol.X; //2
		pixel_index = x0 + slicenum + z0 * volxmultvolYper;
		c = 0;
		s = 0;
		m = n + slc->min;
		z = (int) m / slid->X; //3
		x = m - z * slid->X; //4

		for (angle = 0; angle < prj.AngN; angle++)
		//for (angle = prj.AngN; angle --; )
				{

			rf = (x - slid->x0 + 0.5) * cosang[angle]
					+ (z - slid->z0 + 0.5) * sinang[angle] + slid->x0;
			r = floor(rf - 0.5); //5
			weight = 1.5 - (rf - r); //6

			if (r >= 0 && r < slid->X - 1) {
				c++;
				row_index = angle * slid->X + r;
				s += weight * prj_real[row_index]
						+ (1 - weight) * prj_real[row_index + 1];

			} else if (r == slid->X - 1) {
				c += weight;
				row_index = angle * slid->X + r;
				s += weight * prj_real[row_index];

			} else if (r == -1) {
				c += 1 - weight;
				row_index = angle * slid->X;
				s += (1 - weight) * prj_real[row_index];
			}
		} //end for angle
		if (c != 0)
			slc->data[pixel_index] = (float) s / c;
		/*	if(n<10)
		 printf("%f ",slc->data[pixel_index]);*/

	} // end for n

	return TRUE;

}
int Slice_backproj3(Projection prj, TYPE *prj_real, Slice *slc, Slide *slid,
		float *tilt_angle, Volume vol, int j, int Y_start, long long volY_per,
		float* cosang, float* sinang, int rotate) {
	int x0, z0;
	int n, z, x, angle, r, row_index;
	long long pixel_index;
	int volxz = vol.X * vol.Z;
	float rf, rf_x, rf_z, rf_0;
	int row_base = 0;
	float weight;
	float tmp;
	float c[volxz];
	float s[volxz];
	// #pragma omp parallel for
	int volxmultvolYper = vol.X * volY_per;
	//int slicenum = (j - Y_start) * vol.X;

	int zStop = (int) (volxz + slc->min) / slid->X;
	int zStart = (int) slc->min / slid->X;
	int xStart = slc->min % slid->X;

	memset(c, 0, sizeof(float) * volxz);
	memset(s, 0, sizeof(float) * volxz);
	//printf("test1\n");
	int i = 0;
	for (angle = 0; angle < prj.AngN; angle++) {
		//printf("testang=%d zStart=%d zStop=%d slc->min=%d slid->X=%d\n",angle,zStart,zStop,slc->min%slid->X,slid->X);
		i = 0;
		rf_z = (zStart + 0.5 - slid->z0) * sinang[angle];
		rf_x = (xStart + 0.5 - slid->x0) * cosang[angle];
		rf_0 = slid->x0;
		for (z = zStart; z < zStop; z++) {
			rf = rf_x + rf_z + rf_0;
			for (x = xStart; x < slid->X; x++) {
				r = floor(rf - 0.5); //5
				weight = (double) 1.5 - (rf - r); //6
				//printf("z=%d x=%d i=%d rf=%e r=%d weight=%e \n", z,x,i,rf, r, weight);
				if (r >= 0 && r < slid->X - 1) {
					row_index = row_base + r;
					c[i] += 1;
					tmp = weight * prj_real[row_index]
							+ (1 - weight) * prj_real[row_index + 1];
					s[i] += tmp;

				} else if (r == slid->X - 1) {
					row_index = row_base + r;
					c[i] += weight;
					tmp = weight * prj_real[row_index];
					s[i] += tmp;

				} else if (r == -1) {
					row_index = row_base;
					c[i] += 1 - weight;
					tmp = (1 - weight) * prj_real[row_index];
					s[i] += tmp;
				}
				rf += cosang[angle];
				i++;
			} //end x
			rf_z += sinang[angle];
		} //end z
		row_base += slid->X;
	} //end angle

	i = 0;

	if (rotate == 1) {
		int slicenum = (j - Y_start) * vol.X * vol.Z;
		for (i = 0; i < volxz; i++) {
			pixel_index = i + slicenum;
			if (c[i] != 0) {
				slc->data[pixel_index] = (float) (s[i] / c[i]);
			} //end if
		} //end for
	} else {
		int slicenum = (j - Y_start) * vol.X;
		for (i = 0; i < volxz; i++) {
			x0 = i % vol.X;
			z0 = i / vol.X;
			pixel_index = x0 + slicenum + z0 * volxmultvolYper;

			if (c[i] != 0) {
				slc->data[pixel_index] = (float) (s[i] / c[i]);
			} //end if
		} //end for
	}

	return TRUE;

}
int Slice_backproj4(Projection prj, TYPE *prj_real, int slcmin, Slide *slid,
		float *tilt_angle, Volume vol, int j, int Y_start, long long volY_per,
		float* cosang, float* sinang, int rotate,float *slcdata) {
	int x0, z0;
	int  z, x, angle, r, row_index;
	long long pixel_index;
	int volxz = vol.X * vol.Z;
	float rf, rf_x, rf_z, rf_0;
	int row_base = 0;
	float weight;
	float tmp;
	float c[volxz];
	float s[volxz];
	// #pragma omp parallel for
	int volxmultvolYper = vol.X * volY_per;
	//int slicenum = (j - Y_start) * vol.X;

	int zStop = (int) (volxz + slcmin) / slid->X;
	int zStart = (int) slcmin / slid->X;
	int xStart = slcmin % slid->X;

	memset(c, 0, sizeof(float) * volxz);
	memset(s, 0, sizeof(float) * volxz);
	//printf("test1\n");
	int i = 0;
	for (angle = 0; angle < prj.AngN; angle++) {
		//printf("testang=%d zStart=%d zStop=%d slc->min=%d slid->X=%d\n",angle,zStart,zStop,slc->min%slid->X,slid->X);
		i = 0;
		rf_z = (zStart + 0.5 - slid->z0) * sinang[angle];
		rf_x = (xStart + 0.5 - slid->x0) * cosang[angle];
		rf_0 = slid->x0;
		for (z = zStart; z < zStop; z++) {
			rf = rf_x + rf_z + rf_0;
			for (x = xStart; x < slid->X; x++) {
				r = floor(rf - 0.5); //5
				weight = (double) 1.5 - (rf - r); //6
				//printf("z=%d x=%d i=%d rf=%e r=%d weight=%e \n", z,x,i,rf, r, weight);
				if (r >= 0 && r < slid->X - 1) {
					row_index = row_base + r;
					c[i] += 1;
					tmp = weight * prj_real[row_index]
							+ (1 - weight) * prj_real[row_index + 1];
					s[i] += tmp;

				} else if (r == slid->X - 1) {
					row_index = row_base + r;
					c[i] += weight;
					tmp = weight * prj_real[row_index];
					s[i] += tmp;

				} else if (r == -1) {
					row_index = row_base;
					c[i] += 1 - weight;
					tmp = (1 - weight) * prj_real[row_index];
					s[i] += tmp;
				}
				rf += cosang[angle];
				//if(i<=1&&j<=1)
				//			printf("%f %f %f %d %f %d %d\n",s[i],c[i],rf,r,weight,i,j);
				i++;
			} //end x
			rf_z += sinang[angle];
		} //end z
		row_base += slid->X;
	} //end angle

	i = 0;


		int slicenum = (j - Y_start) * vol.X;
		for (i = 0; i < volxz; i++) {
			x0 = i % vol.X;
			z0 = i / vol.X;
			pixel_index = x0 + slicenum + z0 * volxmultvolYper;

			if (c[i] != 0) {
				slcdata[pixel_index] = (float) (s[i] / c[i]);
			} //end if
//			if(x0==0&&z0==0&&j<=1)
//				printf("%f %d \n",slcdata[pixel_index],j);

	} //end for



	return TRUE;

}

int SIRT_Slice_reproj_bilinear(Projection prj, float *prj_calc,
		float *prj_calc_num, Slice *slc, Slide *slid, float *tilt_angle,
		Volume vol, int j, int Y_start, long long volY_per,float *cosang, float *sinang,int rotate) {
	int n, z, x, angle, r, row_index;
	float  rf, weight;
	float rf_x, rf_z, rf_0;

	int volxz = vol.X * vol.Z;
	int zStop = (int) (volxz + slc->min) / slid->X;
	int zStart = (int) slc->min / slid->X;
	int xStart = slc->min % slid->X;
	int row_base = 0;
	rf_0 = slid->x0;
	//printf("z=%d %d x=%d %d \n", zStart, zStop, xStart, slid->X);
	for (angle = 0; angle < prj.AngN; angle++) {
		n = 0;
		rf_z = (zStart + 0.5 - slid->z0) * sinang[angle];
		rf_x = (xStart + 0.5 - slid->x0) * cosang[angle];

		for (z = zStart; z < zStop; z++) {
			rf = rf_x + rf_z + rf_0;
			for (x = xStart; x < slid->X; x++) {
				r = floor(rf - 0.5);
				weight = 1.5 - (rf - r);
				if (r >= 0 && r < slid->X - 1) {
					row_index = row_base + r;  //prj_index
					prj_calc[row_index] += weight * slc->data[n];
					prj_calc[row_index + 1] += (1 - weight) * slc->data[n];
					prj_calc_num[row_index] += weight;
					prj_calc_num[row_index + 1] += 1 - weight;
				} else if (r == slid->X - 1) //left
						{
					row_index = row_base + r; //prj_index
					prj_calc[row_index] += weight * slc->data[n];
					prj_calc_num[row_index] += weight;
				} else if (r == -1)  //right
						{
					row_index = row_base;
					prj_calc[row_index + 1] += (1 - weight) * slc->data[n];
					prj_calc_num[row_index + 1] += 1 - weight;
				}

				n = n + 1;
				rf += cosang[angle];
			} //end for x
			rf_z += sinang[angle];
		} // end for z
		row_base += slid->X;
	} //end for angle

	return TRUE;
}
/****************************SIRT_one_update_bilinear******************************************************/
int SIRT_one_update_bilinear(Projection prj, float *prj_calc, Slice *slc,
		Slide *slid, float *tilt_angle, Volume vol, int j, int Y_start,
		int volY_per, float SIRT_ITR_STEP,float *cosang,float *sinang,int rotate)

		{

	int n, z, x, angle, r, row_index;

	float  rf, weight;
	float rf_x, rf_z, rf_0;

	int volxz = vol.X * vol.Z;
	int zStop = (int) (volxz + slc->min) / slid->X;
	int zStart = (int) slc->min / slid->X;
	int xStart = slc->min % slid->X;
	int row_base = 0;
	rf_0 = slid->x0;

	float c[volxz];
	float s[volxz];
	memset(c, 0, sizeof(float) * volxz);
	memset(s, 0, sizeof(float) * volxz);
	//printf("z=%d %d x=%d %d \n", zStart, zStop, xStart, slid->X);
	for (angle = 0; angle < prj.AngN; angle++) {

		n = 0;
		rf_z = (zStart + 0.5 - slid->z0) * sinang[angle];
		rf_x = (xStart + 0.5 - slid->x0) * cosang[angle];

		for (z = zStart; z < zStop; z++) {
			rf = rf_x + rf_z + rf_0;
			for (x = xStart; x < slid->X; x++) {
				r = floor(rf - 0.5);
				weight = 1.5 - (rf - r);

				if (r >= 0 && r < slid->X - 1) {
					c[n] += 1;
					row_index = row_base + r;
					s[n] += weight * prj_calc[row_index]
							+ (1 - weight) * prj_calc[row_index + 1];
				} else if (r == slid->X - 1) {
					c[n] += weight;
					row_index = row_base + r;
					s[n] += weight * prj_calc[row_index];
				} else if (r == -1) {
					c[n] += 1 - weight;
					row_index = row_base;
					s[n] += (1 - weight) * prj_calc[row_index];
				}

				n = n + 1;
				rf += cosang[angle];
			} //end for x
			rf_z += sinang[angle];
		} // end for z
		row_base += slid->X;
	} //end for angle

	for (n = 0; n < volxz; n++) {
		if (c[n] != 0) {
			slc->data[n] += s[n] / c[n] * SIRT_ITR_STEP;
		} //end if
	} //end for n

	return TRUE;

}

/****************************SIRT_update_slice******************************************************/
int SIRT_update_slice(Projection prj, float *prj_real, float *prj_calc,
		float *prj_calc_num, Slice *slc, Slide *slid, float *tilt_angle,
		Volume vol, int j, int Y_start, int volY_per, float SIRT_ITR_STEP,float *cosang, float *sinang,int rotate) {

	int n;
	int line_num = prj.X * prj.AngN;

	SIRT_Slice_reproj_bilinear(prj, prj_calc, prj_calc_num, slc, slid,
			tilt_angle, vol, j, Y_start, volY_per,cosang,sinang,rotate);

	for (n = 0; n < line_num; n++) {
		if (prj_calc_num[n] != 0) {
			float c = prj_calc_num[n];
			float s = prj_calc[n];
			prj_calc[n] = s / c;
		}

		prj_calc[n] = prj_real[n] - prj_calc[n];
	}

	SIRT_one_update_bilinear(prj, prj_calc, slc, slid, tilt_angle, vol, j,
			Y_start, volY_per, SIRT_ITR_STEP,cosang,sinang,rotate);

	return TRUE;

}

int SART_Slice_reproj_bilinear(Projection prj, float *prj_calc,
		float *prj_calc_num, Slice *slc, Slide *slid, float *tilt_angle,
		Volume vol, int j, int Y_start, long long volY_per, int angle ,float sin_ang,float cos_ang) {
	int n, z, x, r, row_index, pixel_index;
	float ang, rf, weight;
	float rf_x, rf_z, rf_0;
	int x0, z0;
	int volxz = vol.X * vol.Z;
	int zStop = (int) (volxz + slc->min) / slid->X;
	int zStart = (int) slc->min / slid->X;
	int xStart = slc->min % slid->X;
	int row_base = slid->X * angle;
	int volxmultvolYper = vol.X * volY_per;
	int slicenum = (j - Y_start) * vol.X;


	n = 0;
	rf_0 = slid->x0;
	rf_z = (zStart + 0.5 - slid->z0) *sin_ang;
	rf_x = (xStart + 0.5 - slid->x0) * cos_ang;
	for (z = zStart; z < zStop; z++) {
		rf = rf_x + rf_z + rf_0;
		for (x = xStart; x < slid->X; x++) {
			x0 = n % vol.X;
			z0 = n / vol.X;
			pixel_index = x0 + slicenum + z0 * volxmultvolYper;
			r = floor(rf - 0.5);
			weight = 1.5 - (rf - r);
			//if(x0==0&&z0==0){
			//printf("slc->min=%d angle=%d j=%d x=%d z=%d x0=%d z0=%d pixel_index=%d weight=%e slc=%e sin=%e c=%e rf=%e t=%e\n",
			//		slc->min,angle, j,x,z,x0,z0,pixel_index, weight,slc->data[pixel_index],sin_ang, cos_ang,rf,tilt_angle[angle]);
			//}
			if (r >= 0 && r < slid->X - 1) {
				row_index = row_base + r;  //prj_index
				prj_calc[row_index] += weight * slc->data[pixel_index];
				prj_calc[row_index + 1] += (1 - weight)
						* slc->data[pixel_index];
				prj_calc_num[row_index] += weight;
				prj_calc_num[row_index + 1] += 1 - weight;

			} else if (r == slid->X - 1) //left
					{
				row_index = row_base + r; //prj_index
				prj_calc[row_index] += weight * slc->data[pixel_index];
				prj_calc_num[row_index] += weight;
			} else if (r == -1)  //right
					{
				row_index = row_base;
				prj_calc[row_index + 1] += (1 - weight)
						* slc->data[pixel_index];
				prj_calc_num[row_index + 1] += 1 - weight;
			}
			n = n + 1;
			rf += cos_ang;
		} //end for x
		rf_z += sin_ang;

	} // end for z
	//while(1){
	//	;
	//}
	return TRUE;
}
int SART_one_update_bilinear(Projection prj, float *prj_calc, Slice *slc,
		Slide *slid, float *tilt_angle, Volume vol, int j, int Y_start,
		long long volY_per, float SART_ITR_STEP,int angle ,float sin_ang,float cos_ang)

		{
	int n, z, x, r, row_index,pixel_index;
	float ang, rf, weight;
	float rf_x, rf_z, rf_0;
    int x0,z0;
    float c=0;
    float s=0;
	int volxz = vol.X * vol.Z;
	int zStop = (int) (volxz + slc->min) / slid->X;
	int zStart = (int) slc->min / slid->X;
	int xStart = slc->min % slid->X;
	int row_base = angle * slid->X;
	int volxmultvolYper = vol.X * volY_per;
	int slicenum = (j - Y_start) * vol.X;


	n = 0;

	rf_0 = slid->x0;
	rf_z = (zStart + 0.5 - slid->z0) * sin_ang;
	rf_x = (xStart + 0.5 - slid->x0) * cos_ang;

	for (z = zStart; z < zStop; z++) {
		rf = rf_x + rf_z + rf_0;
		for (x = xStart; x < slid->X; x++) {
			x0 = n % vol.X;
			z0 = n / vol.X;
			pixel_index = x0 + slicenum + z0 * volxmultvolYper;

			r = floor(rf - 0.5);
			weight = 1.5 - (rf - r);


			if (r >= 0 && r < slid->X - 1) {
				c++;
				row_index = row_base + r;
				s += weight * prj_calc[row_index]
						+ (1 - weight) * prj_calc[row_index + 1];
			} else if (r == slid->X - 1) {
				c += weight;
				row_index = row_base + r;
				s += weight * prj_calc[row_index];
			} else if (r == -1) {
				c += 1 - weight;
				row_index = row_base;
				s += (1 - weight) * prj_calc[row_index];
			}

			if (c != 0)
				slc->data[pixel_index] += s / c * SART_ITR_STEP;

		/*	if(x0==10&&z0==0){
			    	printf("slc->min=%d angle=%d j=%d x=%d z=%d x0=%d z0=%d p=%d w=%e prj=%e s=%e c=%e \n",
			    			slc->min,angle, j,x,z,x0,z0,row_index, weight,prj_calc[row_index], s,c );
	     	}*/

			n = n + 1;
			rf += cos_ang;
		} //end for x
		rf_z += sin_ang;
	} // end for z
	/*while(1){
		;
	}*/
    return TRUE;
}
int SART_update_slice(Projection prj, float *prj_real, float *prj_calc,
	float *prj_calc_num, Slice *slc, Slide *slid, float *tilt_angle, Volume vol,
	int j, int Y_start, int volY_per, float SART_ITR_STEP,float *cosang, float *sinang) {

int angle, n, prj_index;
//  int line_num=prj.X*prj.AngN;

for (angle = 0; angle < prj.AngN; angle++){

	SART_Slice_reproj_bilinear(prj, prj_calc, prj_calc_num, slc, slid,
			tilt_angle, vol, j, Y_start, volY_per,angle,sinang[angle],cosang[angle]);

	    for (n = 0; n < prj.X; n++) {
		prj_index = n + angle * prj.X;
		if (prj_calc_num[prj_index] != 0)
          prj_calc[prj_index] /= prj_calc_num[prj_index];
          prj_calc[prj_index] = prj_real[prj_index] - prj_calc[prj_index];
	    }

	SART_one_update_bilinear(prj, prj_calc, slc, slid, tilt_angle, vol, j,
			Y_start, volY_per, SART_ITR_STEP,angle,sinang[angle],cosang[angle]);
} // end for angle

return TRUE;

}
int SART_Slice_reproj_bilinear2(Projection prj, float *prj_calc, float *prj_calc_num, Slice *slc, Slide *slid, float *tilt_angle, Volume vol, int j, int Y_start, long long  volY_per,int angle)
{
    int x0,z0;
    int n,m,z,x,r,row_index;
    long long pixel_index;
    float ang,rf,weight;

     for(z0=0;z0<vol.Z;z0++)
       for(x0=0;x0<vol.X;x0++)
    {
      n=x0+z0*vol.X;
  //  for(n=0;n<pixel_num;n++)
      pixel_index=x0+(j-Y_start)*vol.X+z0*vol.X*volY_per;

      m=n+slc->min;
      z=(int)m/slid->X;
      x=m-z*slid->X;
    /*  for(angle=0;angle<prj.AngN;angle++)
       {*/
        ang = D2R(-tilt_angle[angle]);
    	double sin_ang=sin(ang);
    	double cos_ang=cos(ang);
        rf = (x-slid->x0+0.5)*cos_ang+(z-slid->z0+0.5)*sin_ang+slid->x0;
        r = floor(rf-0.5);
        weight=1.5-(rf-r);
    	//if(x0==0&&z0==0){
    	//		printf("slc->min=%d angle=%d j=%d x=%d z=%d x0=%d z0=%d pixel_index=%d weight=%e slc=%e sin=%e c=%e rf=%e t=%e \n",
    	//				slc->min,angle, j,x,z,x0,z0,pixel_index, weight,slc->data[pixel_index],sin_ang, cos_ang,rf,tilt_angle[angle]);
    	//		}
        if(r>=0 && r<slid->X-1)
        {
         row_index=angle*slid->X+r;  //prj_index
         prj_calc[row_index]+=weight*slc->data[pixel_index];
         prj_calc[row_index+1]+=(1-weight)*slc->data[pixel_index];
         prj_calc_num[row_index]+=weight;
         prj_calc_num[row_index+1]+=1-weight;

        }
        else if(r==slid->X-1) //left
        {
              row_index=angle*slid->X+r; //prj_index
              prj_calc[row_index]+=weight*slc->data[pixel_index];
              prj_calc_num[row_index]+=weight;
        }
             else if (r==-1)  //right
                 {
                   row_index=angle*slid->X;
                   prj_calc[row_index+1]+=(1-weight)*slc->data[pixel_index];
                   prj_calc_num[row_index+1]+=1-weight;
                 }
     //  } //end for angle

     } // end for n
	//	while(1){
	//		;
	//	}
   return TRUE;
}
int SART_one_update_bilinear2( Projection prj, float *prj_calc, Slice *slc, Slide *slid, float *tilt_angle, Volume vol, int j, int Y_start, long long  volY_per, float SART_ITR_STEP, int angle)

{
   int x0,z0;
   int n,m,z,x,r,row_index;
   long long pixel_index;
   float ang,rf,weight,c;
   float s;

  // for(n=0;n<pixel_num;n++)
	for (z0 = 0; z0 < vol.Z; z0++)
		for (x0 = 0; x0 < vol.X; x0++) {
			n = x0 + z0 * vol.X;
			pixel_index = x0 + (j - Y_start) * vol.X + z0 * vol.X * volY_per;

			c = 0;
			s = 0;
			m = n + slc->min;
			z = (int) m / slid->X;
			x = m - z * slid->X;

			ang = D2R(-tilt_angle[angle]);

			rf = (x - slid->x0 + 0.5) * cos(ang)
					+ (z - slid->z0 + 0.5) * sin(ang) + slid->x0;
			r = floor(rf - 0.5);
			weight = 1.5 - (rf - r);

			if (r >= 0 && r < slid->X - 1) {
				c++;
				row_index = angle * slid->X + r;
				s += weight * prj_calc[row_index]
						+ (1 - weight) * prj_calc[row_index + 1];
			} else if (r == slid->X - 1) {
				c += weight;
				row_index = angle * slid->X + r;
				s += weight * prj_calc[row_index];
			} else if (r == -1) {
				c += 1 - weight;
				row_index = angle * slid->X;
				s += (1 - weight) * prj_calc[row_index];
			}
			//  } //end for angle
			/*if(x0==10&&z0==0){
			    	printf("slc->min=%d angle=%d j=%d p%d w=%e prj=%e s=%e c=%e \n",
			    			slc->min,angle, j,
			    			row_index,weight,prj_calc[row_index], s,c );
	     	}*/
			if (c != 0)
				slc->data[pixel_index] += s / c * SART_ITR_STEP;
		} // end for n
//	while(1){
//		;
//	}
   return TRUE;

}

int SART_update_slice2(Projection prj, float *prj_real, float *prj_calc,
	float *prj_calc_num, Slice *slc, Slide *slid, float *tilt_angle, Volume vol,
	int j, int Y_start, int volY_per, float SART_ITR_STEP,float *cosang, float *sinang) {

int angle, n, prj_index;
//  int line_num=prj.X*prj.AngN;

for (angle = 0; angle < prj.AngN; angle++){

	SART_Slice_reproj_bilinear2(prj, prj_calc, prj_calc_num, slc, slid,
			tilt_angle, vol, j, Y_start, volY_per,angle);

	    for (n = 0; n < prj.X; n++) {
		prj_index = n + angle * prj.X;
		if (prj_calc_num[prj_index] != 0)
          prj_calc[prj_index] /= prj_calc_num[prj_index];
          prj_calc[prj_index] = prj_real[prj_index] - prj_calc[prj_index];
	    }

	SART_one_update_bilinear2(prj, prj_calc, slc, slid, tilt_angle, vol, j,
			Y_start, volY_per, SART_ITR_STEP,angle);
} // end for angle

return TRUE;

}
//SIRT
/****************************SIRT_Slice_reproj_bilinear******************************************************/

/**********************************************************************************/

int ATOM_MPI(options *opt, int myid, int mypro) {

	const char *inf = opt->input.c_str();
	const char *tilt_f = opt->tiltangle.c_str();
	const char *outf = opt->output.c_str();

	MPI_File fin, fout;

	MPI_File_open(MPI_COMM_WORLD, const_cast<char*>(inf),
			MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &fin);
	MPI_File_open(MPI_COMM_WORLD, const_cast<char*>(outf),
			MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &fout);


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

	/*   mrc_init_head(prohead);
	 prohead->nx=inhead->nx;
	 prohead->ny=inhead->nz;
	 prohead->nz=inhead->ny;*/

	/*   if(myid==0)
	 {
	 mrc_write_head(fout,outhead);
	 mrc_write_head(fproj,prohead);
	 }*/

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
	vol.Ycount = 0; //init

	if (myid == 0)
		PRINT_DEBUG_MSG("vol.Xend is %d, vol.Yend is %d, vol.Zend is %d\n",
				vol.Xend, vol.Yend, vol.Zend);

	Projection prj;
	prj.X = vol.X;
	prj.Y = vol.Y;
	prj.AngN = inhead->nz;

	FILE *f_tilt;
	f_tilt = fopen(tilt_f, "r");

	float *tilt_angle;
	if ((tilt_angle = (float *) malloc(sizeof(float) * prj.AngN)) == NULL) {
		printf("Can't malloc memery for 'tilt_angle'!");
		return FALSE;
	}
	memset(tilt_angle, 0, sizeof(float) * prj.AngN);
	read_coef(tilt_angle, f_tilt, myid);

	int i;
	for (i = 0; i < prj.AngN; i++)
	{
		tilt_angle[i] += opt->offset;
	}

	/************************malloc memory for prj, slc, slc->data****************************************/
	Slide *slid;

	if ((slid = (Slide *) malloc(sizeof(Slide))) == NULL) {
		printf("Can't malloc memery for 'slid'!");
		return FALSE;
	}

	slid->X = vol.X;
	slid->x0 = vol.X / 2;

	if (opt->xaxistilt >= 0)
		slid->Z = (int) (vol.Z + tan(D2R(opt->xaxistilt)) * vol.Y);
	else
		slid->Z = (int) (vol.Z + tan(D2R(-opt->xaxistilt)) * vol.Y);
	slid->z0 = slid->Z / 2 + opt->zshift;

	int pixel_num, line_num;
	long long pixel_num_per;
	line_num = vol.X * prj.AngN;
	pixel_num = vol.X * vol.Z;

	if (PRINTINFO) {
		PRINT_DEBUG_MSG(
				"slid->X is %d, slid->Z is %d, pixel_num is %d, line_allnum is %d\n",
				slid->X, slid->Z, pixel_num, line_num);
		PRINT_DEBUG_MSG("slid->x0 is %d, slid->z0 is %d\n", slid->x0, slid->z0);
	}

//for input file
	//float

	TYPE *prj_real;
	if ((prj_real = (TYPE *) malloc(sizeof(TYPE) * line_num)) == NULL) {
		printf("Can't malloc memery for 'prj_real'!");
		return FALSE;
	}
	memset(prj_real, 0, sizeof(TYPE) * line_num);

//for Iterative method
	float *prj_calc, *prj_calc_num;
	prj_calc = (float *) malloc(sizeof(float) * line_num);
	memset(prj_calc, 0, sizeof(float) * line_num);

	prj_calc_num = (float *) malloc(sizeof(float) * line_num);
	memset(prj_calc_num, 0, sizeof(float) * line_num);

//for output file
	Slice *slc;
	slc = (Slice *) malloc(sizeof(Slice));

	/**********************decide Z_minS,Z_minE,Z_maxS,Z_maxE  ***********************************************************/
	int Z_minS, Z_minE, Z_maxS, Z_maxE;

	if (opt->xaxistilt >= 0) {
		Z_minS = 0;
		Z_maxS = vol.Z;
		Z_minE = (int) (tan(D2R(opt->xaxistilt)) * vol.Y);
		Z_maxE = Z_minE + vol.Z;
	}

	if (opt->xaxistilt < 0) {
		Z_minS = (int) (tan(D2R(-opt->xaxistilt)) * vol.Y);
		Z_maxS = vol.Z + Z_minS;
		Z_minE = 0;
		Z_maxE = vol.Z;
	}
	if (PRINTINFO)
		PRINT_DEBUG_MSG("Z_minS is %d,Z_maxS is %d,Z_minE is %d,Z_maxE is %d\n",
				Z_minS, Z_maxS, Z_minE, Z_maxE);

	/**********************reconstruction begin for each slice***********************************************************/
	long long volY_per;
	int volY_add = vol.Y % mypro;
	int Y_start;   //the start slice of reproject per process
	int Y_end;   //the end slice of reproject per process

	if (myid < volY_add) {
		volY_per = vol.Y / mypro + 1;
		Y_start = volY_per * myid + vol.Ystart;
		Y_end = volY_per * (myid + 1) + vol.Ystart;
	} else {
		volY_per = vol.Y / mypro;
		Y_start = volY_per * myid + volY_add + vol.Ystart;
		Y_end = volY_per * (myid + 1) + volY_add + vol.Ystart;
	}
	if (PRINTINFO)
		PRINT_DEBUG_MSG(
				"Y_start is %d, volY_per is %lld, Y_end is %d in process %d\n",
				Y_start, volY_per, Y_end, myid);

	vol.volY_per = volY_per;
	vol.Ystart_per = Y_start;
	vol.Yend_per = Y_end;

	pixel_num_per = vol.X * vol.Z * volY_per;

	slc->data = (float *) malloc(sizeof(float) * pixel_num_per);
	memset(slc->data, 0, sizeof(float) * pixel_num_per);

	int j;

	if (opt->rotate == 1) {
		outhead->ny = opt->thickness;
		outhead->nz = inhead->ny;
		if (myid == 0) {
			mrc_write_head(fout, outhead);
		}
	} else {
		if (myid == 0) {
			mrc_write_head(fout, outhead);
		}
	}

	float *ang = (float *) malloc(sizeof(float) * prj.AngN);
	float *cosang = (float *) malloc(sizeof(float) * prj.AngN);
	float *sinang = (float *) malloc(sizeof(float) * prj.AngN);
	for (int n = 0; n < prj.AngN; n++) {
		ang[n] = D2R(-tilt_angle[n]);
		cosang[n] = cos(ang[n]);
		sinang[n] = sin(ang[n]);
	}

	if (opt->method == "BPT") {

#define BPT
#define NOROTATE


		int zminEsubS = Z_minE - Z_minS;

        //#pragma omp parallel for num_threads(3) private(prj_real,slcmin) firstprivate(fin)
		for (j = Y_start; j < Y_end; j++) {

			//mrc_read_slice_short(fin, inhead, j, 'y', prj_real);
			   // mrc_read_slice(fin, inhead, j, 'y', prj_real);
			mrc_read_slice_y(fin, inhead, j, 'y', prj_real);
			//mrc_read_slice(fin, inhead, j, 'y', prj_real);
			//mrc_read_slice_short(fin, inhead, j, 'y', prj_real);
			slc->min = slid->X * ((j * zminEsubS) / vol.Y + Z_minS);
			Slice_backproj3(prj, prj_real, slc, slid, tilt_angle, vol, j,Y_start, volY_per, cosang, sinang, opt->rotate);
			//Slice_backproj4(prj, prj_real, slcmin, slid, tilt_angle, vol, j, Y_start, volY_per, cosang, sinang, opt->rotate,slcdata);

		}//end for j
		if (opt->rotate == 1)
			mrc_write_block(fout, outhead, Y_start, Y_end, 'Z', slc->data);
		else
			mrc_write_block(fout, outhead, Y_start, Y_end, 'Y', slc->data);

	}   // end BPT
	else if(opt->method == "FBP"){
		int filtlength=251;
		prj.Y=prj.AngN;
		const char *filter = opt->filter.c_str();
		int zminEsubS = Z_minE - Z_minS;
        int row_pad = 2;  //x-padded
        int col_pad = 2;  //y-padded
        int symmetrize_2D_flag=1;
		for (j = Y_start; j < Y_end; j++) {

			mrc_read_slice_y(fin, inhead, j, 'y', prj_real);
			slc->min = slid->X * ((j * zminEsubS) / vol.Y + Z_minS);
			filter_prj_sym(prj_real, filter, filtlength, prj, row_pad, col_pad,
					symmetrize_2D_flag, j, myid);

			Slice_backproj3(prj, prj_real, slc, slid, tilt_angle, vol, j,
					Y_start, volY_per, cosang, sinang, opt->rotate);

		}

		if (opt->rotate == 1)
			mrc_write_block(fout, outhead, Y_start, Y_end, 'Z', slc->data);
		else
			mrc_write_block(fout, outhead, Y_start, Y_end, 'Y', slc->data);

	}
	else if (opt->method == "SIRT") {
	#define SIRT
	//#define GPU

		int SIRT_ITR_STEP= opt->step;
		const char *initialf = opt->initial.c_str();
		float *temp = (float *) malloc(sizeof(float) * pixel_num); //special for SIRT
		if (strcmp(initialf, "initial") == 0) {

			printf("First use BPT to init\n");
			//mrc_read_block(fin, inhead, Y_start, Y_end, 'Y', slc->data);
			for (j = Y_start; j < Y_end; j++) {
				//mrc_read_slice(fin, inhead, j, 'y', prj_real);

				//mrc_read_slice_floatseek(fin, inhead, j, 'y', prj_real);
				mrc_read_slice_y(fin, inhead, j, 'y', prj_real);
				slc->min = slid->X * ((j * (Z_minE - Z_minS)) / vol.Y + Z_minS);
				Slice_backproj3(prj, prj_real, slc, slid, tilt_angle, vol, j,Y_start, volY_per, cosang, sinang, opt->rotate);

				memset(temp, 0, sizeof(float) * pixel_num);
				if (opt->rotate == 0) {
					for (int index = 0; index < pixel_num; index++) {
						int z = index / vol.X;
						int y = j - Y_start;
						int x = index % vol.X;
						temp[index] = slc->data[x + y * vol.X+ z * vol.X * (volY_per)];
					}
				} else {
					memcpy(temp, slc->data + (j - Y_start) * vol.X * vol.Z,sizeof(float) * pixel_num);
					//no need rotate
				}

				int i; //itertive number
				for (i = 0; i < opt->iter_num; i++) {

					Slice slctemp;
					slctemp = *slc;
					slctemp.data = temp;
					SIRT_update_slice(prj, prj_real, prj_calc, prj_calc_num,
							&slctemp, slid, tilt_angle, vol, j, Y_start,
							volY_per, SIRT_ITR_STEP, cosang, sinang,
							opt->rotate);
					memset(prj_calc, 0, sizeof(float) * line_num);
					memset(prj_calc_num, 0, sizeof(float) * line_num);

					//
				}
				if (opt->rotate == 0)
					mrc_write_slice(fout, outhead, j, 'y', temp);
				else
					mrc_write_slice(fout, outhead, j, 'z', temp);
			}

			free(temp);
		} //end if
		else {
			MPI_File f_initial;
			MPI_File_open(MPI_COMM_WORLD, const_cast<char*>(initialf),
			MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &f_initial);
			mrc_read_block(f_initial, outhead, Y_start, Y_end, 'Y', slc->data);
			for (j = Y_start; j < Y_end; j++) {

				memset(temp, 0, sizeof(float) * pixel_num);

				for (int index = 0; index < pixel_num; index++) {
					int z = index / vol.X;
					int y = j - Y_start;
					int x = index % vol.X;
					temp[index] = slc->data[x + y * vol.X + z * vol.X * (volY_per)];

				}
				mrc_read_slice_y(fin, inhead, j, 'y', prj_real);
				//mrc_read_slice(fin, inhead, j, 'y', prj_real);
				//mrc_read_slice_y(fin, inhead, j, 'y', prj_real);
				slc->min = slid->X * ((j * (Z_minE - Z_minS)) / vol.Y + Z_minS);


				int i; //itertive number
				for (i = 0; i < opt->iter_num; i++) {

					Slice slctemp;
					slctemp = *slc;
					slctemp.data = temp;
					SIRT_update_slice(prj, prj_real, prj_calc, prj_calc_num, &slctemp,
							slid, tilt_angle, vol, j, Y_start, volY_per, SIRT_ITR_STEP,cosang, sinang, opt->rotate);
					memset(prj_calc, 0, sizeof(float) * line_num);
					memset(prj_calc_num, 0, sizeof(float) * line_num);

					//
				}
				if(opt->rotate==0)
					mrc_write_slice(fout, outhead, j, 'y', temp);
				else
					mrc_write_slice(fout, outhead, j, 'z', temp);
				//printf("Iteration %d has done in Y_slice %d\n", i, j);

			} //end for

			free(temp);
		}


	} // end if SIRT

	else if (opt->method == "SART") {

	#define SART

		int *numsequence = (int *) malloc(sizeof(int) * prj.AngN);
		memset(numsequence, 0, sizeof(sizeof(int) * prj.AngN));
		random_angle(tilt_angle, prj.AngN, numsequence);
		float SART_ITR_STEP;
		SART_ITR_STEP = opt->step;
		const char *initialf = opt->initial.c_str();

		if (strcmp(initialf, "initial") == 0) {

			printf("First use BPT to init\n");

			for (j = Y_start; j < Y_end; j++) {
				mrc_read_slice_y(fin, inhead, j, 'y', prj_real);
				slc->min = slid->X * ((j * (Z_minE - Z_minS)) / vol.Y + Z_minS);
				Slice_backproj3(prj, prj_real, slc, slid, tilt_angle, vol, j, Y_start, volY_per, cosang, sinang, opt->rotate);

				//random
				float* prj_real_temp = (float *) malloc(sizeof(float) * line_num);
				//    Cu_SART_random_prj();
				for (int i = 0; i < prj.AngN; i++) {
					for (int j = 0; j < vol.X; j++)
						prj_real_temp[i * vol.X + j] = prj_real[numsequence[i] * vol.X+ j];
				}

				slc->min = slid->X * ((j * (Z_minE - Z_minS)) / vol.Y + Z_minS);


				int i; //itertive number
				for (i = 0; i < opt->iter_num; i++) {

					SART_update_slice2(prj, prj_real_temp, prj_calc,
							prj_calc_num, slc, slid, tilt_angle, vol, j,
							Y_start, volY_per, SART_ITR_STEP, cosang, sinang);
					memset(prj_calc, 0, sizeof(float) * line_num);
					memset(prj_calc_num, 0, sizeof(float) * line_num);


				}
			}
			mrc_write_block(fout, outhead, Y_start, Y_end, 'Y', slc->data);

		} else {
			MPI_File f_initial;
			MPI_File_open(MPI_COMM_WORLD, const_cast<char*>(initialf),
					MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &f_initial);
			mrc_read_block(f_initial, outhead, Y_start, Y_end, 'Y', slc->data);
			for (j = Y_start; j < Y_end; j++) {
				mrc_read_slice_y(fin, inhead, j, 'y', prj_real);

				//random
				float* prj_real_temp = (float *) malloc(
						sizeof(float) * line_num);
				//    Cu_SART_random_prj();
				for (int i = 0; i < prj.AngN; i++) {
					for (int j = 0; j < vol.X; j++)
						prj_real_temp[i * vol.X + j] = prj_real[numsequence[i]* vol.X + j];
				}

				slc->min = slid->X * ((j * (Z_minE - Z_minS)) / vol.Y + Z_minS);

				int i; //itertive number
				for (i = 0; i < opt->iter_num; i++) {

					SART_update_slice2(prj, prj_real_temp, prj_calc,
							prj_calc_num, slc, slid, tilt_angle, vol, j,
							Y_start, volY_per, SART_ITR_STEP, cosang, sinang);
					memset(prj_calc, 0, sizeof(float) * line_num);
					memset(prj_calc_num, 0, sizeof(float) * line_num);

				}
			}
			mrc_write_block(fout, outhead, Y_start, Y_end, 'Y', slc->data);
		}    				//end init

		free(numsequence);
	} // end if SART

	MPI_Barrier(MPI_COMM_WORLD );

	mrc_update_head_MPI(fout, myid, mypro);

	free(slc->data);
	free(slc);
	fclose(f_tilt);
	free(slid);
	free(inhead);
	inhead = NULL;

	free(outhead);
	outhead = NULL;

	free(prj_calc);
	free(prj_calc_num);
	free(tilt_angle);
	free(ang);
	free(cosang);
	free(sinang);

	MPI_File_close(&fin);
	MPI_File_close(&fout);
	//MPI_File_close(&fproj);

	return TRUE;
}

