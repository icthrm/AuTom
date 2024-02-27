#include "recon.h"
#define MILLION 1000000
//#define GPU

/**********************************************************************************/
int read_coef(double *data,  FILE *f_coef, int id)
{
    char *lstr;
    lstr=(char *)malloc(TEXT_LINE_MAX);

    int i=0;
    while(fgets(lstr,TEXT_LINE_MAX,f_coef) != NULL)
    {
        data[i]=atof(lstr);

        i++;
    }
	return TRUE;
}


/******************************************backprojection****************************************/
int Slice_backproj( Projection prj, float *prj_real, Slice *slc, Slide *slid, double *tilt_angle, Volume vol, int j, int Y_start, int volY_per)
{
	int x0, z0;
	int n, m, z, x, angle, r, row_index, pixel_index;
	double weight, c;
	double s;
	double ang, rf;
//    #pragma offload target (mic:0)
    #pragma omp parallel for num_threads(256)
	for (z0 = 0; z0 < vol.Z; z0++)
		for (x0 = 0; x0 < vol.X; x0++) {
			n = x0 + z0 * vol.X;
			pixel_index = x0 + (j - Y_start) * vol.X + z0 * vol.X * volY_per;
			c = 0;
			s = 0;
			m = n + slc->min;
			z = (int) m / slid->X;
			x = m - z * slid->X;

			for (angle = 0; angle < prj.AngN; angle++)
			//     for(angle=0;angle<1;angle++)
					{
				ang = D2R(-tilt_angle[angle]);

				rf = (x - slid->x0 + 0.5) * cos(ang)
						+ (z - slid->z0 + 0.5) * sin(ang) + slid->x0;
				r = floor(rf - 0.5);
				weight = 1.5 - (rf - r);

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
		} // end for n

	return TRUE;

}


//SIRT
/****************************SIRT_Slice_reproj_bilinear******************************************************/
int SIRT_Slice_reproj_bilinear(Projection prj, float *prj_calc, float *prj_calc_num, Slice *slc, Slide *slid, double *tilt_angle, Volume vol, int j, int Y_start, int volY_per)
{
    int x0,z0;
    int n,m,z,x,angle,r,row_index,pixel_index;
    double ang,rf,weight;

     for(z0=0;z0<vol.Z;z0++)
       for(x0=0;x0<vol.X;x0++)
    {
      n=x0+z0*vol.X;
  //  for(n=0;n<pixel_num;n++)
      pixel_index=x0+(j-Y_start)*vol.X+z0*vol.X*volY_per;
      pixel_index=n;
      
      m=n+slc->min;
      z=(int)m/slid->X;
      x=m-z*slid->X;
    //  for(angle=0;angle<1;angle++)
      for(angle=0;angle<prj.AngN;angle++)
       {
        ang = D2R(-tilt_angle[angle]);

        rf = (x-slid->x0+0.5)*cos(ang)+(z-slid->z0+0.5)*sin(ang)+slid->x0;
        r = floor(rf-0.5);
        weight=1.5-(rf-r);
      //  r+=slid->x0;
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
       } //end for angle


     } // end for n

   return TRUE;
}
/****************************SIRT_one_update_bilinear******************************************************/
int SIRT_one_update_bilinear( Projection prj, float *prj_calc, Slice *slc, Slide *slid, double *tilt_angle, Volume vol, int j, int Y_start, int volY_per, float SIRT_ITR_STEP)

{
   int x0,z0;
   int n,m,z,x,angle,r,row_index,pixel_index;
   double ang,rf,weight,c;
   double s;

  // for(n=0;n<pixel_num;n++)
   for(z0=0;z0<vol.Z;z0++)
       for(x0=0;x0<vol.X;x0++)
    {
      n=x0+z0*vol.X;
      pixel_index=x0+(j-Y_start)*vol.X+z0*vol.X*volY_per;
      pixel_index=n;
      c=0;
      s=0;
      m=n+slc->min;
      z=(int)m/slid->X;
      x=m-z*slid->X;
    //for(angle=0;angle<1;angle++)
      for(angle=0;angle<prj.AngN;angle++)
       {

        ang = D2R(-tilt_angle[angle]);

        rf = (x-slid->x0+0.5)*cos(ang)+(z-slid->z0+0.5)*sin(ang)+slid->x0;
        r = floor(rf-0.5);
        weight=1.5-(rf-r);
        if(r>=0 && r<slid->X-1)
        {
         c++;
         row_index=angle*slid->X+r;
         s+=weight*prj_calc[row_index]+(1-weight)*prj_calc[row_index+1];
         }
         else if(r==slid->X-1)
            {
              c+=weight;
              row_index=angle*slid->X+r;
              s+=weight*prj_calc[row_index];
            }
              else if (r==-1)
                 {
                   c+=1-weight;
                   row_index=angle*slid->X;
                   s+=(1-weight)*prj_calc[row_index];
                 }
       } //end for angle

if(c!=0)    slc->data[pixel_index] += s/c*SIRT_ITR_STEP;

     } // end for n

   return TRUE;

}

/****************************SIRT_update_slice******************************************************/
int SIRT_update_slice(Projection prj, float *prj_real, float *prj_calc, float *prj_calc_num, Slice *slc, Slide *slid, double *tilt_angle, Volume vol, int j, int Y_start, int volY_per, float SIRT_ITR_STEP)
{

   int n;
   int line_num=prj.X*prj.AngN;

   SIRT_Slice_reproj_bilinear(prj, prj_calc, prj_calc_num, slc, slid, tilt_angle, vol, j, Y_start, volY_per);

   for(n=0;n<line_num;n++)
   {
      if(prj_calc_num[n] != 0){
    	  double c=prj_calc_num[n];
    	  double s=prj_calc[n];
    	  prj_calc[n]=s/c;
      }

      prj_calc[n]=prj_real[n]-prj_calc[n];
     }

   SIRT_one_update_bilinear(prj, prj_calc, slc, slid, tilt_angle, vol, j, Y_start, volY_per, SIRT_ITR_STEP);

   return TRUE;

}

//ASIRT
/****************************ASIRT_Slice_reproj_bilinear******************************************************/
int ASIRT_Slice_reproj_bilinear(Projection prj, float *prj_calc, float *prj_calc_abs, float *prj_calc_num, Slice *slc, Slide *slid, double *tilt_angle, Volume vol, int j, int Y_start, int volY_per)
{
    int x0,z0;
    int n,m,z,x,angle,r,row_index,pixel_index;
    double ang,rf,weight;

  //  for(n=0;n<pixel_num;n++)
    for(z0=0;z0<vol.Z;z0++)
       for(x0=0;x0<vol.X;x0++)
    {
      n=x0+z0*vol.X;
      pixel_index=x0+(j-Y_start)*vol.X+z0*vol.X*volY_per;      
        
      m=n+slc->min;
      z=(int)m/slid->X;
      x=m-z*slid->X;
      for(angle=0;angle<prj.AngN;angle++)
       {
        ang = D2R(-tilt_angle[angle]);

        rf = (x-slid->x0+0.5)*cos(ang)+(z-slid->z0+0.5)*sin(ang)+slid->x0;
        r = floor(rf-0.5);
        weight=1.5-(rf-r);
      //  r+=slid->x0;

        if(r>=0 && r<slid->X-1)
        {
         row_index=angle*slid->X+r;  //prj_index
         prj_calc[row_index]+=weight*slc->data[pixel_index];
         prj_calc[row_index+1]+=(1-weight)*slc->data[pixel_index];
         prj_calc_abs[row_index]+=weight*abs(slc->data[pixel_index]);
         prj_calc_abs[row_index+1]+=(1-weight)*abs(slc->data[pixel_index]);
         prj_calc_num[row_index]+=weight;
         prj_calc_num[row_index+1]+=1-weight;
         
        }
        else if(r==slid->X-1) //left
        {
              row_index=angle*slid->X+r; //prj_index
              prj_calc[row_index]+=weight*slc->data[pixel_index];         
              prj_calc_abs[row_index]+=weight*abs(slc->data[pixel_index]);
              
              prj_calc_num[row_index]+=weight;
        }
             else if (r==-1)  //right
                 {
                   row_index=angle*slid->X;
                   prj_calc[row_index+1]+=(1-weight)*slc->data[pixel_index];
                   prj_calc_abs[row_index]+=(1-weight)*abs(slc->data[pixel_index]);
                   
                   prj_calc_num[row_index+1]+=1-weight;
                 }
       } //end for j

     } // end for n

   return TRUE;
}
/****************************ASIRT_one_update_bilinear******************************************************/
int ASIRT_one_update_bilinear( Projection prj, float *prj_calc, float *prj_calc_abs, Slice *slc, Slide *slid, double *tilt_angle,  Volume vol, int j, int Y_start, int volY_per, float ASIRT_ITR_STEP)

{
   int x0,z0;    
   int n,m,z,x,angle,r,row_index,pixel_index;
   double ang,rf,weight,c;
   double t,s;

   for(z0=0;z0<vol.Z;z0++)
       for(x0=0;x0<vol.X;x0++)
    {
      n=x0+z0*vol.X;
      pixel_index=x0+(j-Y_start)*vol.X+z0*vol.X*volY_per; 
    
      c=0;
      s=0;
      t=0;
      m=n+slc->min;
      z=(int)m/slid->X;
      x=m-z*slid->X;
      for(angle=0;angle<prj.AngN;angle++)
       {
        ang = D2R(-tilt_angle[angle]);

        rf = (x-slid->x0+0.5)*cos(ang)+(z-slid->z0+0.5)*sin(ang)+slid->x0;
        r = floor(rf-0.5);
        weight=1.5-(rf-r);

        if(r>=0 && r<slid->X-1)
        {
         c++;
         row_index=angle*slid->X+r;
         s+=weight*prj_calc[row_index]+(1-weight)*prj_calc[row_index+1];
         t+=weight*prj_calc_abs[row_index]+(1-weight)*prj_calc_abs[row_index+1];

         }
         else if(r==slid->X-1)
            {
              c+=weight;
              row_index=angle*slid->X+r;
              s+=weight*prj_calc[row_index];
              t+=weight*prj_calc_abs[row_index];
            }
              else if (r==-1)
                 {
                   c+=1-weight;
                   row_index=angle*slid->X;
                   s+=(1-weight)*prj_calc[row_index];
                   t+=(1-weight)*prj_calc_abs[row_index];
                   
                 }
       } //end for j

if(t!=0)    slc->data[pixel_index] += abs(slc->data[pixel_index])*(float)s/t*ASIRT_ITR_STEP;
     } // end for n

   return TRUE;

}

/****************************ASIRT_update_slice******************************************************/
int ASIRT_update_slice(Projection prj, float *prj_real, float *prj_calc, float *prj_calc_abs, float *prj_calc_num, Slice *slc, Slide *slid, double *tilt_angle,  Volume vol, int j, int Y_start, int volY_per, float ASIRT_ITR_STEP)
{

   int n;
   int line_num=prj.X*prj.AngN;

   ASIRT_Slice_reproj_bilinear(prj, prj_calc, prj_calc_abs, prj_calc_num, slc, slid, tilt_angle, vol, j, Y_start, volY_per);
    
   for(n=0;n<line_num;n++)
     {
      if(prj_calc_num[n] != 0)
      {
          prj_calc[n]/=prj_calc_num[n];
          prj_calc_abs[n]/=prj_calc_num[n];
      }          
      prj_calc[n]=prj_real[n]-prj_calc[n];
     }

   ASIRT_one_update_bilinear(prj, prj_calc, prj_calc_abs, slc, slid, tilt_angle, vol, j, Y_start, volY_per, ASIRT_ITR_STEP);

   return TRUE;

}


//SART
/****************************SIRT_Slice_reproj_bilinear******************************************************/
int SART_Slice_reproj_bilinear(Projection prj, float *prj_calc, float *prj_calc_num, Slice *slc, Slide *slid, double *tilt_angle, Volume vol, int j, int Y_start, int volY_per,int angle)
{
    int x0,z0;
    int n,m,z,x,r,row_index,pixel_index;
    double ang,rf,weight;

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

        rf = (x-slid->x0+0.5)*cos(ang)+(z-slid->z0+0.5)*sin(ang)+slid->x0;
        r = floor(rf-0.5);
        weight=1.5-(rf-r);
      //  r+=slid->x0;

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

   return TRUE;
}
/****************************SIRT_one_update_bilinear******************************************************/
int SART_one_update_bilinear( Projection prj, float *prj_calc, Slice *slc, Slide *slid, double *tilt_angle, Volume vol, int j, int Y_start, int volY_per, float SART_ITR_STEP, int angle)

{
   int x0,z0;
   int n,m,z,x,r,row_index,pixel_index;
   double ang,rf,weight,c;
   double s;

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

			if (c != 0)
				slc->data[pixel_index] += s / c * SART_ITR_STEP;
		} // end for n

   return TRUE;

}

/****************************SIRT_update_slice******************************************************/
int SART_update_slice(Projection prj, float *prj_real, float *prj_calc, float *prj_calc_num, Slice *slc, Slide *slid, double *tilt_angle, Volume vol, int j, int Y_start, int volY_per, float SART_ITR_STEP)
{

   int angle, n,prj_index;
 //  int line_num=prj.X*prj.AngN;

   for(angle=0;angle<prj.AngN;angle++)
//   for(angle=0;angle<3;angle++)
   {
   
     SART_Slice_reproj_bilinear(prj, prj_calc, prj_calc_num, slc, slid, tilt_angle, vol, j, Y_start, volY_per,angle);
   
     for(n=0;n<prj.X;n++)
     {
      prj_index=n+angle*prj.X;
      if(prj_calc_num[prj_index] != 0)
       
          prj_calc[prj_index]/=prj_calc_num[prj_index];
       
      prj_calc[prj_index]=prj_real[prj_index]-prj_calc[prj_index];
     }

     SART_one_update_bilinear(prj, prj_calc, slc, slid, tilt_angle, vol, j, Y_start, volY_per, SART_ITR_STEP,angle);
   }// end for angle

   return TRUE;

}


//ASART
/****************************ASART_Slice_reproj_bilinear******************************************************/
int ASART_Slice_reproj_bilinear(Projection prj, float *prj_calc, float *prj_calc_abs, float *prj_calc_num, Slice *slc, Slide *slid, double *tilt_angle, Volume vol, int j, int Y_start, int volY_per, int angle)
{
    int x0,z0;
    int n,m,z,x,r,row_index,pixel_index;
    double ang,rf,weight;

  //  for(n=0;n<pixel_num;n++)
    for(z0=0;z0<vol.Z;z0++)
       for(x0=0;x0<vol.X;x0++)
    {
      n=x0+z0*vol.X;
      pixel_index=x0+(j-Y_start)*vol.X+z0*vol.X*volY_per;      
        
      m=n+slc->min;
      z=(int)m/slid->X;
      x=m-z*slid->X;
    /*  for(angle=0;angle<prj.AngN;angle++)
       {*/
        ang = D2R(-tilt_angle[angle]);

        rf = (x-slid->x0+0.5)*cos(ang)+(z-slid->z0+0.5)*sin(ang)+slid->x0;
        r = floor(rf-0.5);
        weight=1.5-(rf-r);
      //  r+=slid->x0;

        if(r>=0 && r<slid->X-1)
        {
         row_index=angle*slid->X+r;  //prj_index
         prj_calc[row_index]+=weight*slc->data[pixel_index];
         prj_calc[row_index+1]+=(1-weight)*slc->data[pixel_index];
         prj_calc_abs[row_index]+=weight*abs(slc->data[pixel_index]);
         prj_calc_abs[row_index+1]+=(1-weight)*abs(slc->data[pixel_index]);
         prj_calc_num[row_index]+=weight;
         prj_calc_num[row_index+1]+=1-weight;
         
        }
        else if(r==slid->X-1) //left
        {
              row_index=angle*slid->X+r; //prj_index
              prj_calc[row_index]+=weight*slc->data[pixel_index];         
              prj_calc_abs[row_index]+=weight*abs(slc->data[pixel_index]);
              
              prj_calc_num[row_index]+=weight;
        }
             else if (r==-1)  //right
                 {
                   row_index=angle*slid->X;
                   prj_calc[row_index+1]+=(1-weight)*slc->data[pixel_index];
                   prj_calc_abs[row_index]+=(1-weight)*abs(slc->data[pixel_index]);
                   
                   prj_calc_num[row_index+1]+=1-weight;
                 }
    //   } //end for j

     } // end for n

   return TRUE;
}
/****************************ASART_one_update_bilinear******************************************************/
int ASART_one_update_bilinear( Projection prj, float *prj_calc, float *prj_calc_abs, Slice *slc, Slide *slid, double *tilt_angle,  Volume vol, int j, int Y_start, int volY_per, float ASART_ITR_STEP, int angle)

{
   int x0,z0;    
   int n,m,z,x,r,row_index,pixel_index;
   double ang,rf,weight,c;
   double t,s;

   for(z0=0;z0<vol.Z;z0++)
       for(x0=0;x0<vol.X;x0++)
    {
      n=x0+z0*vol.X;
      pixel_index=x0+(j-Y_start)*vol.X+z0*vol.X*volY_per; 
    
      c=0;
      s=0;
      t=0;
      m=n+slc->min;
      z=(int)m/slid->X;
      x=m-z*slid->X;
   /*   for(angle=0;angle<prj.AngN;angle++)
       {*/
        ang = D2R(-tilt_angle[angle]);

        rf = (x-slid->x0+0.5)*cos(ang)+(z-slid->z0+0.5)*sin(ang)+slid->x0;
        r = floor(rf-0.5);
        weight=1.5-(rf-r);

        if(r>=0 && r<slid->X-1)
        {
         c++;
         row_index=angle*slid->X+r;
         s+=weight*prj_calc[row_index]+(1-weight)*prj_calc[row_index+1];
         t+=weight*prj_calc_abs[row_index]+(1-weight)*prj_calc_abs[row_index+1];

         }
         else if(r==slid->X-1)
            {
              c+=weight;
              row_index=angle*slid->X+r;
              s+=weight*prj_calc[row_index];
              t+=weight*prj_calc_abs[row_index];
            }
              else if (r==-1)
                 {
                   c+=1-weight;
                   row_index=angle*slid->X;
                   s+=(1-weight)*prj_calc[row_index];
                   t+=(1-weight)*prj_calc_abs[row_index];
                   
                 }
     //  } //end for j

if(t!=0)    slc->data[pixel_index] += abs(slc->data[pixel_index])*(s/t)*ASART_ITR_STEP;
//if(c!=0)    slc->data[pixel_index] += (float)s/c*ASART_ITR_STEP;

     } // end for n

   return TRUE;

}

/****************************ASIRT_update_slice******************************************************/
int ASART_update_slice(Projection prj, float *prj_real, float *prj_calc, float *prj_calc_abs, float *prj_calc_num, Slice *slc, Slide *slid, double *tilt_angle,  Volume vol, int j, int Y_start, int volY_per, float ASART_ITR_STEP)
{

   int angle, n, prj_index;

   for(angle=0;angle<prj.AngN;angle++)
   {
     ASART_Slice_reproj_bilinear(prj, prj_calc, prj_calc_abs, prj_calc_num, slc, slid, tilt_angle, vol, j, Y_start, volY_per, angle);
    
     for(n=0;n<prj.X;n++)
     {
      prj_index=n+angle*prj.X;
         
      if(prj_calc_num[prj_index] != 0)
      {
          prj_calc[prj_index]/=prj_calc_num[prj_index];
          prj_calc_abs[prj_index]/=prj_calc_num[prj_index];
      }          
      prj_calc[prj_index]=prj_real[prj_index]-prj_calc[prj_index];
     }

   ASART_one_update_bilinear(prj, prj_calc, prj_calc_abs, slc, slid, tilt_angle, vol, j, Y_start, volY_per, ASART_ITR_STEP, angle);
   }//END FOR

   return TRUE;

}


/**********************************************************************************/

int ATOM(options *opt, int myid, int mypro)
{

   const char *inf = opt->input.c_str();
   const char *tilt_f = opt->tiltangle.c_str();
   const char *outf = opt->output.c_str();


   MPI_File fin, fout;

   MPI_File_open(MPI_COMM_WORLD, const_cast<char*>(inf), MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &fin);
   MPI_File_open(MPI_COMM_WORLD, const_cast<char*>(outf), MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &fout);

   MrcHeader *inhead, *outhead;

   inhead=(MrcHeader *)malloc(sizeof(MrcHeader));
   outhead=(MrcHeader *)malloc(sizeof(MrcHeader));

   if (myid==0)
	   mrc_read_head_MPI(fin, inhead);
   MPI_Bcast(inhead, 1024, MPI_CHAR, 0, MPI_COMM_WORLD);

   mrc_init_head(outhead);
   outhead->nx=inhead->nx;
   outhead->ny=inhead->ny;
   outhead->nz=opt->thickness;

   if(myid==0)
	   mrc_write_head(fout,outhead);

   Volume vol;
   vol.X=outhead->nx;
   vol.Y=outhead->ny;
   vol.Z=outhead->nz;
   vol.Xstart=0;
   vol.Xend=vol.Xstart+vol.X;
   vol.Ystart=0;
   vol.Yend=vol.Ystart+vol.Y;
   vol.Zstart=0;
   vol.Zend=vol.Zstart+vol.Z;

   if (myid ==0)
	   printf("vol.Xend is %d, vol.Yend is %d, vol.Zend is %d\n",vol.Xend, vol.Yend, vol.Zend);

   Projection prj;
   prj.X=vol.X;
   prj.Y=vol.Y;
   prj.AngN=inhead->nz;

   FILE *f_tilt;
   f_tilt=fopen(tilt_f,"r");


   double *tilt_angle;
   if((tilt_angle=(double *)malloc(sizeof(double)*prj.AngN))==NULL)
   {
       printf("Can't malloc memery for 'tilt_angle'!");
       return FALSE;
   }
   memset(tilt_angle, 0 , sizeof(double)*prj.AngN);
   read_coef(tilt_angle, f_tilt, myid);

   int i;
   for (i=0;i<prj.AngN;i++)
       tilt_angle[i]+=opt->offset;

/************************malloc memory for prj, slc, slc->data****************************************/
   Slide *slid;

   if((slid=(Slide *)malloc(sizeof(Slide)))==NULL)
   {
       printf("Can't malloc memery for 'slid'!");
       return FALSE;
   }

   slid->X=vol.X;
   slid->x0=vol.X/2;

   if(opt->xaxistilt>=0)
       slid->Z=(int)(vol.Z+tan(D2R(opt->xaxistilt))*vol.Y);
   else slid->Z=(int)(vol.Z+tan(D2R(-opt->xaxistilt))*vol.Y);
   slid->z0=slid->Z/2+opt->zshift;

   int pixel_num_per, pixel_num, line_num;
   line_num=vol.X*prj.AngN;
   pixel_num=vol.X*vol.Z;

   printf("slid->X is %d, slid->Z is %d, pixel_num is %d, line_allnum is %d\n",slid->X,slid->Z,pixel_num, line_num);
   printf("slid->x0 is %d, slid->z0 is %d\n",slid->x0,slid->z0);


//for input file
   float *prj_real;
   if((prj_real=(float *)malloc(sizeof(float)*line_num))==NULL)
   {
       printf("Can't malloc memery for 'prj_real'!");
       return FALSE;
   }
   memset(prj_real, 0 , sizeof(float)*line_num);

//for Iterative method
   float *prj_calc, *prj_calc_num;
   prj_calc  = (float *)malloc(sizeof(float)*line_num); 
   memset(prj_calc, 0 , sizeof(float)*line_num);

   prj_calc_num  = (float *)malloc(sizeof(float)*line_num); 
   memset(prj_calc_num, 0 , sizeof(float)*line_num);

//for output file
   Slice *slc;
   slc=(Slice *)malloc(sizeof(Slice));


/**********************decide Z_minS,Z_minE,Z_maxS,Z_maxE  ***********************************************************/
   int Z_minS,Z_minE,Z_maxS,Z_maxE;

   if(opt->xaxistilt>=0)
   {
       Z_minS=0;
       Z_maxS=vol.Z;
       Z_minE=(int)(tan(D2R(opt->xaxistilt))*vol.Y);
       Z_maxE=Z_minE+vol.Z;
   }

   if(opt->xaxistilt<0)
   {
       Z_minS=(int)(tan(D2R(-opt->xaxistilt))*vol.Y);
       Z_maxS=vol.Z+Z_minS;
       Z_minE=0;
       Z_maxE=vol.Z;
   }

  printf("Z_minS is %d,Z_maxS is %d,Z_minE is %d,Z_maxE is %d\n",Z_minS,Z_maxS,Z_minE,Z_maxE);

  /**********************reconstruction begin for each slice***********************************************************/
        int volY_per;
        int volY_add=vol.Y%mypro;
        int Y_start;   //the start slice of reproject per process
        int Y_end  ;   //the end slice of reproject per process

        if(myid<volY_add)
        {
            volY_per=vol.Y/mypro+1;
            Y_start=volY_per*myid+vol.Ystart;
            Y_end=volY_per*(myid+1)+vol.Ystart;
        }
        else
        {
            volY_per=vol.Y/mypro;
            Y_start=volY_per*myid+volY_add+vol.Ystart;
            Y_end=volY_per*(myid+1)+volY_add+vol.Ystart;
        }
        printf("Y_start is %d, volY_per is %d, Y_end is %d in process %d\n",Y_start,volY_per,Y_end,myid);


        vol.volY_per=volY_per;
        vol.Ystart_per=Y_start;
        vol.Yend_per=Y_end;

        pixel_num_per=vol.X*vol.Z*volY_per;
        slc->data=(float *)malloc(sizeof(float)*pixel_num_per);
        memset(slc->data, 0 , sizeof(float)*pixel_num_per);

        int j;

        if(opt->method=="BPT")
        {
//#define GPU
#define BPT
#ifdef GPU

            long int total_mem;
            total_mem=initGPU(myid);
            ImageStruct host_img,dev_img;
            host_img.prj=prj;
            host_img.slid=*slid;
            host_img.tilt_angle=tilt_angle;
            host_img.vol=vol;
            host_img.slc=*slc;
            dev_img=host_img;
            initMalloc(dev_img,line_num,prj.AngN,pixel_num,total_mem);
            transferToDevtilt(dev_img.tilt_angle,tilt_angle,sizeof(double)*prj.AngN);

#endif
            mrc_read_block(fin, inhead, Y_start, Y_end, 'Y', slc->data);
           // int slcdata_offset=0;

            for(j=Y_start;j<Y_end;j++)
            {
#ifdef GPU
            	float *temp=(float *)malloc(sizeof(float)*pixel_num);
                memset(temp,0,sizeof(float)*pixel_num);
                for(int index=0;index<pixel_num;index++){
                	int z=index/vol.X;
                	int y=j-Y_start;
                	int x=index%vol.X;
                	temp[index]=slc->data[x+y*vol.X+z*vol.X*(volY_per)];
                }
#endif
                mrc_read_slice(fin, inhead, j, 'y', prj_real);
                prj.Y=prj.AngN; //one projection image along Y-axis is prj.X*prj.AngN
                slc->min=slid->X*((j*(Z_minE-Z_minS))/vol.Y+Z_minS);
                slc->max=slid->X*((j*(Z_maxE-Z_maxS))/vol.Y+Z_maxS);
#ifdef GPU
                dev_img.prj.Y=prj.Y;
                dev_img.slc.min=slc->min;
                dev_img.slc.max=slc->max;
                transferToDevVol(dev_img.slc.data,temp,pixel_num*sizeof(float));
                initDevVol(dev_img.slc.data,pixel_num*sizeof(float));
                initDevMem(dev_img.slc.c,pixel_num*sizeof(double));
                initDevMem(dev_img.slc.s,pixel_num*sizeof(double));

                transferToDevPro(dev_img.prj_real, prj_real,line_num*sizeof(float));
#else
               Slice_backproj( prj, prj_real, slc,slid, tilt_angle, vol, j, Y_start, volY_per);
#endif
#ifdef GPU

               Bpt_Slice_backproj(pixel_num, prj.AngN, volY_per, dev_img.prj_real,dev_img.slc, dev_img.slid,Y_start,j);
               Bpt_Slice_divide(pixel_num,dev_img.slc,Y_start,j,volY_per);
               transferToHost(dev_img.slc.data,slc->data,pixel_num*sizeof(float));
               mrc_write_slice(fout, outhead, j, 'y', slc->data);

#endif

               //printf("the slice %d has been done\n",j);
            }

#ifdef GPU
        freeDevMem_double(dev_img.tilt_angle) ;
        freeDevProjection(dev_img.prj_real) ;
        freeDevSlc(dev_img.slc.data) ;
        freeDevMem_double(dev_img.slc.c) ;
        freeDevMem_double(dev_img.slc.s) ;
#else
        mrc_write_block(fout, outhead, Y_start, Y_end, 'Y', slc->data);
#endif
        }// end BPT

        else if(opt->method=="FBP")
        {
#define FBP

#ifdef GPU

            long int total_mem;
            total_mem=initGPU(myid);
            ImageStruct host_img,dev_img;
            host_img.prj=prj;
            host_img.slid=*slid;
            host_img.tilt_angle=tilt_angle;
            host_img.vol=vol;
            host_img.slc=*slc;
            dev_img=host_img;
            initMalloc(dev_img,line_num,prj.AngN,pixel_num,total_mem);
            transferToDevtilt(dev_img.tilt_angle,tilt_angle,sizeof(double)*prj.AngN);
#endif
            mrc_read_block(fin, inhead, Y_start, Y_end, 'Y', slc->data);
            for(j=Y_start;j<Y_end;j++)
            {

#ifdef GPU
            	float *temp=(float *)malloc(sizeof(float)*pixel_num);
                memset(temp,0,sizeof(float)*pixel_num);
                for(int index=0;index<pixel_num;index++){
                	int z=index/vol.X;
                	int y=j-Y_start;
                	int x=index%vol.X;
                	temp[index]=slc->data[x+y*vol.X+z*vol.X*(volY_per)];
                }
#endif
                mrc_read_slice(fin, inhead, j, 'y', prj_real);
                prj.Y=prj.AngN; //one projection image along Y-axis is prj.X*prj.AngN
                slc->min=slid->X*((j*(Z_minE-Z_minS))/vol.Y+Z_minS);
                slc->max=slid->X*((j*(Z_maxE-Z_maxS))/vol.Y+Z_maxS);
                int filtlength=251;
	            const char *filter = opt->filter.c_str();
                int row_pad = 2;  //x-padded
                int col_pad = 2;  //y-padded
                int symmetrize_2D_flag=1;
                //in host
                filter_prj_sym(prj_real, filter, filtlength, prj, row_pad, col_pad, symmetrize_2D_flag,j, myid);
#ifdef GPU
                dev_img.prj.Y=prj.Y;
                dev_img.slc.min=slc->min;
                dev_img.slc.max=slc->max;
                transferToDevVol(dev_img.slc.data,temp,pixel_num*sizeof(float));
                initDevVol(dev_img.slc.data,pixel_num*sizeof(float));
                initDevMem(dev_img.slc.c,pixel_num*sizeof(double));
                initDevMem(dev_img.slc.s,pixel_num*sizeof(double));
                transferToDevPro(dev_img.prj_real, prj_real,line_num*sizeof(float));
#else
               Slice_backproj( prj, prj_real, slc,slid, tilt_angle, vol, j, Y_start, volY_per);
#endif
#ifdef GPU

               Bpt_Slice_backproj(pixel_num, prj.AngN, volY_per, dev_img.prj_real,dev_img.slc, dev_img.slid,Y_start,j);
               Bpt_Slice_divide(pixel_num,dev_img.slc,Y_start,j,volY_per);
               transferToHost(dev_img.slc.data,temp,pixel_num*sizeof(float));
               mrc_write_slice(fout, outhead, j, 'y', temp);


#else


#endif

              // mrc_write_slice(fout, outhead, j, 'y', slc->data);
               //mrc_write_block(fout, outhead, j, j+1, 'Y', slc->data);
             //  printf("the slice %d has been done\n",j);
            }


#ifdef GPU
        freeDevMem_double(dev_img.tilt_angle) ;
        freeDevProjection(dev_img.prj_real) ;
        freeDevSlc(dev_img.slc.data) ;
        freeDevMem_double(dev_img.slc.c) ;
        freeDevMem_double(dev_img.slc.s) ;
#else
            mrc_write_block(fout, outhead, Y_start, Y_end, 'Y', slc->data);
#endif
        }// end FBP
        
        else if (opt->method=="SIRT")
        {
#define SIRT

#ifdef GPU

            long int total_mem;
            total_mem=initGPU(myid);
            ImageStruct host_img,dev_img;
            host_img.prj=prj;
            host_img.slid=*slid;
            host_img.tilt_angle=tilt_angle;
            host_img.vol=vol;
            host_img.slc=*slc;
            dev_img=host_img;
            SIRT_initMalloc(dev_img,line_num,prj.AngN,pixel_num,total_mem);
            transferToDevtilt(dev_img.tilt_angle,tilt_angle,sizeof(double)*prj.AngN);
#endif
            const char *initialf = opt->initial.c_str();
            MPI_File  f_initial;

            MPI_File_open(MPI_COMM_WORLD, const_cast<char*>(initialf), MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &f_initial);

            float SIRT_ITR_STEP;
            SIRT_ITR_STEP=opt->step;

            mrc_read_block(f_initial, outhead, Y_start, Y_end, 'Y', slc->data);

            for(j=Y_start;j<Y_end;j++)
            {

            	float *temp=(float *)malloc(sizeof(float)*pixel_num);
                memset(temp,0,sizeof(float)*pixel_num);
                for(int index=0;index<pixel_num;index++){
                	int z=index/vol.X;
                	int y=j-Y_start;
                	int x=index%vol.X;
                	temp[index]=slc->data[x+y*vol.X+z*vol.X*(volY_per)];
                }
                mrc_read_slice(fin, inhead, j, 'y', prj_real);
               // mrc_read_slice(f_initial, outhead, j, 'Y', slc->data);
                slc->min=slid->X*((j*(Z_minE-Z_minS))/vol.Y+Z_minS);
                slc->max=slid->X*((j*(Z_maxE-Z_maxS))/vol.Y+Z_maxS);
#ifdef GPU
                transferToDevPro(dev_img.prj_real, prj_real,line_num*sizeof(float));
                transferToDevVol(dev_img.slc.data, temp,pixel_num*sizeof(float));
                dev_img.prj.Y=prj.Y;
                dev_img.slc.min=slc->min;
                dev_img.slc.max=slc->max;
#endif

                int i; //itertive number
                for(i=0;i<opt->iter_num;i++)
                {
#ifdef GPU
                	initDevfloat(dev_img.prj_calc, line_num*sizeof(float));
                	initDevfloat(dev_img.prj_calc_num,line_num*sizeof(float));
                	Cu_SIRT_update_slice(dev_img.prj_real, dev_img.prj_calc,
                			dev_img.prj_calc_num, dev_img.slc.data, prj.AngN, dev_img.slc, dev_img.slid,
                			pixel_num, SIRT_ITR_STEP,host_img,j);

#else
                   	Slice slctemp;
                   	slctemp=*slc;
                   	slctemp.data=temp;
                	SIRT_update_slice(prj, prj_real, prj_calc, prj_calc_num, &slctemp, slid, tilt_angle, vol, j, Y_start, volY_per, SIRT_ITR_STEP);
                    memset(prj_calc, 0 , sizeof(float)*line_num);
                    memset(prj_calc_num, 0 , sizeof(float)*line_num);
#endif

   //                 printf("Iteration %d has done in Y_slice %d\n",i,j);
                }
#ifdef GPU
            	transferToHost(dev_img.slc.data, temp,pixel_num*sizeof(float));
#endif
                mrc_write_slice(fout, outhead, j, 'y', temp);

            }
            printf("%d of %d slice finished  of process %d\n",j+1,inhead->ny,myid);

           // mrc_write_block(fout, outhead, Y_start, Y_end, 'Y', slc->data);
            
        }// end if SIRT

         else if (opt->method=="ASIRT")
        {
#define ASIRT
//#define GPU
#ifdef GPU

            long int total_mem;
            float *slcpart=(float *)malloc(sizeof(float)*pixel_num_per);
            total_mem=initGPU(myid);
            ImageStruct host_img,dev_img;
            host_img.prj=prj;
            host_img.slid=*slid;
            host_img.tilt_angle=tilt_angle;
            host_img.vol=vol;
            host_img.slc=*slc;
            dev_img=host_img;
            ASIRT_initMalloc(dev_img,line_num,prj.AngN,pixel_num,total_mem);
            transferToDevtilt(dev_img.tilt_angle,tilt_angle,sizeof(double)*prj.AngN);

#endif
            const char *initialf = opt->initial.c_str();
            MPI_File  f_initial;

            MPI_File_open(MPI_COMM_WORLD, const_cast<char*>(initialf), MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &f_initial);

            float ASIRT_ITR_STEP;
            ASIRT_ITR_STEP=opt->step;

            float *prj_calc_abs;
            prj_calc_abs  = (float *)malloc(sizeof(float)*line_num); 
            memset(prj_calc_abs, 0 , sizeof(float)*line_num);

            mrc_read_block(f_initial, outhead, Y_start, Y_end, 'Y', slc->data);

            for(j=Y_start;j<Y_end;j++)
            {
#ifdef GPU
            	float *temp=(float *)malloc(sizeof(float)*pixel_num);
                memset(temp,0,sizeof(float)*pixel_num);
                for(int index=0;index<pixel_num;index++){
                	int z=index/vol.X;
                	int y=j-Y_start;
                	int x=index%vol.X;
                	temp[index]=slc->data[x+y*vol.X+z*vol.X*(volY_per)];
                }
#endif
                mrc_read_slice(fin, inhead, j, 'y', prj_real);

                slc->min=slid->X*((j*(Z_minE-Z_minS))/vol.Y+Z_minS);
                slc->max=slid->X*((j*(Z_maxE-Z_maxS))/vol.Y+Z_maxS);
#ifdef GPU
                transferToDevPro(dev_img.prj_real, prj_real,line_num*sizeof(float));
                transferToDevVol(dev_img.slc.data, temp,pixel_num*sizeof(float));
                dev_img.prj.Y=prj.Y;
                dev_img.slc.min=slc->min;
                dev_img.slc.max=slc->max;
#endif
                int i; //itertive number
                for(i=0;i<opt->iter_num;i++)
                {
#ifdef GPU
                	initDevfloat(dev_img.prj_calc, line_num*sizeof(float));
                	initDevfloat(dev_img.prj_calc_num,line_num*sizeof(float));
                    initDevfloat(dev_img.prj_calc_num_abs, line_num*sizeof(float));
                	Cu_ASIRT_update_slice(dev_img.prj_real, dev_img.prj_calc,
                			dev_img.prj_calc_num, dev_img.slc.data, prj.AngN, dev_img.slc, dev_img.slid,
                			pixel_num, ASIRT_ITR_STEP,host_img,j,dev_img.prj_calc_num_abs);
#else
                    ASIRT_update_slice(prj, prj_real, prj_calc, prj_calc_abs, prj_calc_num, slc, slid, tilt_angle, vol, j, Y_start, volY_per, ASIRT_ITR_STEP);
                    printf("Iteration %d has done in Y_slice %d\n",i,j);
  
                    memset(prj_calc, 0 , sizeof(float)*line_num);
                    memset(prj_calc_abs, 0 , sizeof(float)*line_num);
                    memset(prj_calc_num, 0 , sizeof(float)*line_num);
#endif
                }
#ifdef GPU
            	transferToHost(dev_img.slc.data, temp,pixel_num*sizeof(float));
 //           	mrc_write_slice(fout, outhead, j, 'y', temp);
                for(int index=0;index<pixel_num;index++){
                	int z=index/vol.X;
                	int y=j-Y_start;
                	int x=index%vol.X;
                	slcpart[x+y*vol.X+z*vol.X*(volY_per)]=temp[index];
                }
#endif
             //   mrc_write_slice(fout, outhead, j, 'y', slc->data);
            }
          //  printf("%d of %d slice finished  of process %d\n",j+1,inhead->ny,myid);
#ifdef GPU
            mrc_write_block(fout, outhead, Y_start, Y_end, 'Y', slcpart);
#else
            mrc_write_block(fout, outhead, Y_start, Y_end, 'Y', slc->data);
#endif

        }// end if ASIRT

        else if (opt->method=="SART")
        {

#define SART
#ifdef GPU
            long int total_mem;
            float *slcpart=(float *)malloc(sizeof(float)*pixel_num_per);
            memset(slcpart, 0 , sizeof(float)*pixel_num_per);
            total_mem=initGPU(myid);
            ImageStruct host_img,dev_img;
            host_img.prj=prj;
            host_img.slid=*slid;
            host_img.tilt_angle=tilt_angle;
            host_img.vol=vol;
            host_img.slc=*slc;
            dev_img=host_img;
            SART_initMalloc(dev_img,line_num,prj.AngN,pixel_num);
            transferToDevtilt(dev_img.tilt_angle,tilt_angle,sizeof(double)*prj.AngN);
#endif
            const char *initialf = opt->initial.c_str();
            MPI_File  f_initial;

            MPI_File_open(MPI_COMM_WORLD, const_cast<char*>(initialf), MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &f_initial);

            float SART_ITR_STEP;
            SART_ITR_STEP=opt->step;

            mrc_read_block(f_initial, outhead, Y_start, Y_end, 'Y', slc->data);


            for(j=Y_start;j<Y_end;j++)
            {
#ifdef GPU
            	float *temp=(float *)malloc(sizeof(float)*pixel_num);
                memset(temp,0,sizeof(float)*pixel_num);
                for(int index=0;index<pixel_num;index++){
                	int z=index/vol.X;
                	int y=j-Y_start;
                	int x=index%vol.X;
                	temp[index]=slc->data[x+y*vol.X+z*vol.X*(volY_per)];
                }
#endif
                mrc_read_slice(fin, inhead, j, 'y', prj_real);
 //               mrc_read_slice(f_initial, outhead, j, 'Y', slc->data);
                slc->min=slid->X*((j*(Z_minE-Z_minS))/vol.Y+Z_minS);
                slc->max=slid->X*((j*(Z_maxE-Z_maxS))/vol.Y+Z_maxS);
#ifdef GPU
                transferToDevPro(dev_img.prj_real, prj_real,line_num*sizeof(float));
                transferToDevVol(dev_img.slc.data, temp,pixel_num*sizeof(float));
                dev_img.prj.Y=prj.Y;
                dev_img.slc.min=slc->min;
                dev_img.slc.max=slc->max;
#endif
                int i; //itertive number
                for(i=0;i<opt->iter_num;i++)
                {
#ifdef GPU
                	initDevfloat(dev_img.prj_calc, line_num*sizeof(float));
                	initDevfloat(dev_img.prj_calc_num,line_num*sizeof(float));
                	Cu_SART_update_slice(dev_img.prj_real, dev_img.prj_calc,
                			dev_img.prj_calc_num, dev_img.slc.data, prj.AngN, dev_img.slc, dev_img.slid,
                			pixel_num, SART_ITR_STEP,host_img,j);
#else
                    SART_update_slice(prj, prj_real, prj_calc, prj_calc_num, slc, slid, tilt_angle, vol, j, Y_start, volY_per, SART_ITR_STEP);
                    //printf("Iteration %d has done in Y_slice %d\n",i,j);
                    memset(prj_calc, 0 , sizeof(float)*line_num);
                    memset(prj_calc_num, 0 , sizeof(float)*line_num);
#endif
                }
#ifdef GPU
            	transferToHost(dev_img.slc.data, temp,pixel_num*sizeof(float));
 //           	mrc_write_slice(fout, outhead, j, 'y', temp);
                for(int index=0;index<pixel_num;index++){
                	int z=index/vol.X;
                	int y=j-Y_start;
                	int x=index%vol.X;
                	slcpart[x+y*vol.X+z*vol.X*(volY_per)]=temp[index];
                }
#endif

            }
            //printf("%d of %d slice finished  of process %d\n",j+1,inhead->ny,myid);
#ifdef GPU
            mrc_write_block(fout, outhead, Y_start, Y_end, 'Y', slcpart);
#else
            mrc_write_block(fout, outhead, Y_start, Y_end, 'Y', slc->data);
#endif
        }// end if SART
        else if (opt->method=="ASART")
        {
#define ASART
//#define GPU
#ifdef GPU
            long int total_mem;
            float *slcpart=(float *)malloc(sizeof(float)*pixel_num_per);
            memset(slcpart, 0 , sizeof(float)*pixel_num_per);
            total_mem=initGPU(myid);
            ImageStruct host_img,dev_img;
            host_img.prj=prj;
            host_img.slid=*slid;
            host_img.tilt_angle=tilt_angle;
            host_img.vol=vol;
            host_img.slc=*slc;
            dev_img=host_img;
            ASART_initMalloc(dev_img,line_num,prj.AngN,pixel_num);
            transferToDevtilt(dev_img.tilt_angle,tilt_angle,sizeof(double)*prj.AngN);
#endif
            const char *initialf = opt->initial.c_str();
            MPI_File  f_initial;
            MPI_File_open(MPI_COMM_WORLD, const_cast<char*>(initialf), MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &f_initial);
            float ASART_ITR_STEP;
            ASART_ITR_STEP=opt->step;
            float *prj_calc_abs;
            prj_calc_abs  = (float *)malloc(sizeof(float)*line_num); 
            memset(prj_calc_abs, 0 , sizeof(float)*line_num);


            mrc_read_block(f_initial, outhead, Y_start, Y_end, 'Y', slc->data);

            for(j=Y_start;j<Y_end;j++)
            {
#ifdef GPU
            	float *temp=(float *)malloc(sizeof(float)*pixel_num);
                memset(temp,0,sizeof(float)*pixel_num);
                for(int index=0;index<pixel_num;index++){
                	int z=index/vol.X;
                	int y=j-Y_start;
                	int x=index%vol.X;
                	temp[index]=slc->data[x+y*vol.X+z*vol.X*(volY_per)];
                }
#endif
                mrc_read_slice(fin, inhead, j, 'y', prj_real);

                slc->min=slid->X*((j*(Z_minE-Z_minS))/vol.Y+Z_minS);
                slc->max=slid->X*((j*(Z_maxE-Z_maxS))/vol.Y+Z_maxS);
#ifdef GPU
                transferToDevPro(dev_img.prj_real, prj_real,line_num*sizeof(float));
                transferToDevVol(dev_img.slc.data, temp,pixel_num*sizeof(float));
                dev_img.prj.Y=prj.Y;
                dev_img.slc.min=slc->min;
                dev_img.slc.max=slc->max;
#endif
                int i; //itertive number
                for(i=0;i<opt->iter_num;i++)
                {
#ifdef GPU
                	initDevfloat(dev_img.prj_calc, line_num*sizeof(float));
                	initDevfloat(dev_img.prj_calc_num,line_num*sizeof(float));
                	initDevfloat(dev_img.prj_calc_num_abs, line_num*sizeof(float));
                	Cu_ASART_update_slice(dev_img.prj_real, dev_img.prj_calc,
                			dev_img.prj_calc_num, dev_img.slc.data, prj.AngN, dev_img.slc, dev_img.slid,
                			pixel_num, ASART_ITR_STEP,host_img,j,dev_img.prj_calc_num_abs);
#else
                    ASART_update_slice(prj, prj_real, prj_calc, prj_calc_abs, prj_calc_num, slc, slid, tilt_angle, vol, j, Y_start, volY_per, ASART_ITR_STEP);
                    
                    printf("Iteration %d has done in Y_slice %d\n",i,j);
  
                    memset(prj_calc, 0 , sizeof(float)*line_num);
                    memset(prj_calc_abs, 0 , sizeof(float)*line_num);                    
                    memset(prj_calc_num, 0 , sizeof(float)*line_num);
#endif
                }
                
            //    mrc_write_slice(fout, outhead, j, 'y', slc->data);
#ifdef GPU
            	transferToHost(dev_img.slc.data, temp,pixel_num*sizeof(float));
 //           	mrc_write_slice(fout, outhead, j, 'y', temp);
                for(int index=0;index<pixel_num;index++){
                	int z=index/vol.X;
                	int y=j-Y_start;
                	int x=index%vol.X;
                	slcpart[x+y*vol.X+z*vol.X*(volY_per)]=temp[index];
                }
#endif

            }
            printf("%d of %d slice finished  of process %d\n",j+1,inhead->ny,myid);
#ifdef GPU
            mrc_write_block(fout, outhead, Y_start, Y_end, 'Y', slcpart);
#else
            mrc_write_block(fout, outhead, Y_start, Y_end, 'Y', slc->data);
#endif
            
        }// end if ASART



    MPI_Barrier(MPI_COMM_WORLD );

	mrc_update_head_MPI(fout, myid, mypro);

	free(slc->data);
	free(slc);

	free(slid);

	free(inhead);
	inhead = NULL;

	free(outhead);
	outhead = NULL;

	MPI_File_close(&fin);
	MPI_File_close(&fout);


   return TRUE;
}

