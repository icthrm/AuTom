#include "wbp.h"
#include <cmath>
#include <cstring>
#include <iostream>

int DecideAllLightPath(int** all_light_path, int* all_pixc, 
		       const std::vector< float >& angles, int proj_width, const Slice& slc)
{
    double x_length, z_length, light_lenght, xstep, zstep, xcur, zcur;
    
    for(int n = 0, j = 0; j < angles.size(); j++){
	double sin_theta = sin(angles[j]);
	double cos_theta = cos(angles[j]);
	
	/*
        		if(sin_theta < PER_MILLION  && sin_theta >= 0.0)sin_theta =  PER_MILLION;
        		else if( sin_theta > -PER_MILLION  && sin_theta < 0.0)sin_theta = -PER_MILLION;

        		if(cos_theta < PER_MILLION  && cos_theta >= 0.0)cos_theta =  PER_MILLION;
        		else if( cos_theta > -PER_MILLION  && cos_theta < 0.0)cos_theta = -PER_MILLION;
        */
	
	double dist_from_center = -proj_width/2.0;

        double xstep = sin_theta;
        double zstep = cos_theta;
	
	for(int i = 0; i < proj_width; i++, n++){
            double Lter_LZstart = 0.0;
            double Lter_LXstart = (dist_from_center+(Lter_LZstart-slc.z0)*sin_theta)/cos_theta+slc.x0;

            double Lter_LZend = slc.nz;
            double Lter_LXend = (dist_from_center+(Lter_LZend-slc.z0)*sin_theta)/cos_theta+slc.x0;
	    
	    {
		x_length = Lter_LXend-Lter_LXstart;
                z_length = Lter_LZend-Lter_LZstart;

                light_lenght = sqrt (x_length*x_length+z_length*z_length);
		all_light_path[n] = new int[(int)(light_lenght+4)];
		memset(all_light_path[n], 0, ((int)(light_lenght+4))*sizeof(int));
		
                xcur = Lter_LXstart;
                zcur = Lter_LZstart;
		
		int r = 0;
		
                while(zcur < Lter_LZend){
                    if (xcur >= 0.0 && xcur < slc.nx){
                        all_light_path[n][r++] = (int)(xcur/*+.5*/)+slc.nx*(int)(zcur/*+.5*/);
                    }
                    xcur += xstep;
                    zcur += zstep;
                }

                all_pixc[n] = r;

                if(r==0){
                    delete [] all_light_path[n];
                    all_light_path[n] = NULL;
                }

            }

            dist_from_center += 1.0;		
	}
    }
    
    return 0;
}

int BackProjectAllLight(float* all_proj_gray, int ang_num, int proj_width, int** all_light_path, int* all_pixc, Slice* slc)
{

    int proj_num = ang_num*proj_width;

    for(int n = 0; n < proj_num; n++){
        for (int r =0 ; r < all_pixc[n]; r++){
            //if((all_light_path[n][r]>=slc_start)&&(all_light_path[n][r]<slc_end))
            {
                slc->data[all_light_path[n][r]] += all_proj_gray[n];
            }
        }
    }

    return 0;
}
