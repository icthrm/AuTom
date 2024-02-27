#include <iostream>
#include <climits>
#include <vector>
#include "mrcimg/mrc2img.h"
#include "opts.h"
#include "env/env.h"

using namespace std;

float NCC(float * test , float * data , int width , int height);

int main(int argc, char **argv) {
    opts param;
    if(GetOpts(argc, argv, &param) != 1) {
        return 0;
    }
    
    util::MrcStack mrcs, mrcs2;
    mrcs.Open(param.input);
    mrcs2.Open(param.input2);

	float ncc_arr[mrcs.Size()];
	float average=0.0;
	for(int i = 0; i < mrcs.Size(); i++){
		EX_TRACE("[P%d] ", i)
		IplImage* img1 = mrcs.GetIplImage(i);
		IplImage* img2 = mrcs2.GetIplImage(i);
		float* start1 = (float*)img1->imageData;
		float* start2 = (float*)img2->imageData;
		ncc_arr[i] = NCC(start1, start2, mrcs.Width(), mrcs.Height());
		cvReleaseImage(&img1);
		cvReleaseImage(&img2);
		average+=ncc_arr[i];
	}
        average=average/mrcs.Size();
	EX_TRACE("\n")
	
	std::ofstream o(param.output);
	for(int i = 0; i < mrcs.Size(); i++){
		o<<ncc_arr[i]<<" ";
	}
	o<<std::endl<<average;
	EX_TRACE("AVERAGE %e\n", average);
	o.close();
	
    mrcs.Close();
    mrcs2.Close();

    return 0;
}

float NCC(float * test , float * data , int width , int height)
{
    float *tmp1,*tmp2,dataavg,testavg,t1,t2,t3;
	
    tmp1 = new float[width*height];
    tmp2 = new float[width*height];
	memset(tmp1, 0, sizeof(float)*width*height);
	memset(tmp2, 0, sizeof(float)*width*height);
	
    dataavg = testavg = 0;
	int count = 0;
	
	float blankv = 0;			

// #define IN_IMG(x)		x>=0&&x<width*height
// 	
// 	for (int i = 0 ; i < width*height; i++)
//     {
// 		if(IN_IMG(i-1)&&IN_IMG(i+1)&&IN_IMG(i+width)&&IN_IMG(i-width)
// 			&&IN_IMG(i-width-1)&&IN_IMG(i-width+1)&&IN_IMG(i+width-1)&&IN_IMG(i+width+1)
// 			&&data[i-1]==data[i]&&data[i+1]==data[i]&&data[i+width]==data[i]&&data[i-width]==data[i]
// 			&&data[i-width-1]==data[i]&&data[i-width+1]==data[i]&&data[i+width-1]==data[i]&&data[i+width+1]==data[i])
// 		{
// 			blankv = data[i];
// 			break;
// 		}
//     }
	
    for (int i = 0 ; i < width*height; i++)
    {
		if(data[i]==0||test[i]==blankv){
			continue;
		}
		dataavg += data[i];
		testavg += test[i];
		count++;
    }

    dataavg /= count;
    testavg /= count;
	
    for (int i = 0 ; i < width*height; i++)
    {
		if(data[i]==0||test[i]==blankv){
			continue;
		}
        tmp1[i] = data[i] - dataavg;
        tmp2[i] = test[i] - testavg;
    }
    
    t1 = t2 = t3 =0;
    for (int i = 0 ; i < width*height; i++)
    {
        t1+=tmp1[i]*tmp2[i];
        t2+=tmp1[i]*tmp1[i];
        t3+=tmp2[i]*tmp2[i];
    }
    
    delete [] tmp1;
	delete [] tmp2;
    
    return t1/sqrt(t2*t3);
}

