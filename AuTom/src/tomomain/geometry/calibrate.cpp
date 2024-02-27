#include "calibrate.h"
#include <cmath>
#include <iostream>

void Calibrate(const float ly[], const float lz[], int thickness0, int width, int height,
	       float* z_shift, float* offset, float* thickness, float* pitch_angle)
{
    std::cout<<"calibrate input parameter"<<std::endl;
    std::cout<<"ly "<<ly[0]<<" "<<ly[1]<<" " <<ly[2]<<std::endl;
    std::cout<<"lz ";
    for(int i=0;i<12;i++){
        std::cout<<lz[i];
    }
    std::cout<<"thickness "<<thickness0<<"width "<<width<<"height "<<height<<std::endl;
    std::cout<<std::endl;
    
	*offset = 0;
	
	for(int i = 0; i < 12; i+= 4){
	*offset += tilt_offset(0, (lz[i]+lz[i+2])*.5*thickness0, width, (lz[i+1]+lz[i+3])*.5*thickness0);
    }
    *offset /= 3;
    
	float midy[6];
	for(int i = 0; i < 3; i++){
		midy[0+i*2] = line_point_y(0, lz[0+i*4], 1, lz[1+i*4], 0.5);
		midy[1+i*2] = line_point_y(0, lz[2+i*4], 1, lz[3+i*4], 0.5);
    }
    
//     for(int i = 0; i < 12; i++){
// 		std::cout<<lz[i]<<" ";
//     }
//     std::cout<<std::endl;
//     
//     for(int i = 0; i < 6; i++){
// 		std::cout<<midy[i]<<" ";
//     }
//     std::cout<<std::endl;
    
    *thickness = 0;
    for(int i = 0; i < 3; i++){
		*thickness += midy[i*2+1]-midy[i*2];
    }
    *thickness = *thickness/3.0*thickness0;
	
	*z_shift = 0;
    for(int i = 0; i < 6; i++){
		*z_shift += midy[i];
    }
    
    *z_shift = -(*z_shift/6*thickness0-thickness0/2.0);

	*pitch_angle = ls_pitch_angle(ly[0]*height, (midy[0]+midy[1])*.5*thickness0, ly[1]*height, (midy[2]+midy[3])*.5*thickness0, ly[2]*height, (midy[4]+midy[5])*.5*thickness0);
}

float line_point_y(float x1, float y1, float x2, float y2, float newx)
{
	return (y2-y1)/(x2-x1)*(newx-x1)+y1;
}

float tilt_offset(float x1, float y1, float x2, float y2)
{
    return atan((y2-y1)/(x2-x1));
}

float ls_pitch_angle(float x1, float y1, float x2, float y2, float x3, float y3)
{
// 	std::cout<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<" "<<x3<<" "<<y3<<std::endl;
	
    float sumx = (x1+x2+x3), sumy = (y1+y2+y3);
	float xx = (x1*x1+x2*x2+x3*x3), yy = (y1*y1+y2*y2+y3*y3), xy = (x1*y1+x2*y2+x3*y3);
	
	float a = sumx*yy-sumy*xy, b = -sumx*xy+xx*sumy;

    return atan(-a/b); 
}
