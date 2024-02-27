#ifndef WBP_H__
#define WBP_H__

#include <vector>

/*Volume stores the parameter of a volum data*/
struct Volume
{
    int nx;                      //X,Y,Z are the pixel size of the three dimesions in a volum data;
    int ny;
    int nz;                      //it also equals the thickness of a volum data;

    int extnz;                   //the extended Z size as a result of pitch angle;

    float x0;                    //(x0,z0) is the center of a Volume;
    float y0;
    float z0;

    int *nzstart;                //Zstart for each slice, there are ny elements in total;
    int *nzend;                  //Zend for each slice, there are ny elements in total;

    float zshift;
    float offset;                //offset angle of the tilt image,which has an offset effect on the volum data;
    float pitch;                 // pitch angle of the volum data;
    float azimuth; 		    // tilt azimuth of volume;
};


/*a Slice means a reconstructed section on the XOZ plane;*/
struct Slice
{

    int nx;                        //X and Z are the pixel size of each dimesion
    int nz;

    float x0;                       //(x0,z0) is the center of a Slice
    float z0;

    int zstart;                      //pixels whose index number is above max or below min will be ignored in the iteraction
    int zend;

    float *data;                  //data stores every pixel value of a Slice no use for parallel program
};


/** PATH_MODE_I */
int DecideAllLightPath(int **all_light_path, int *all_pixc, const std::vector<float>& angles, int proj_width, const Slice& slc);

int BackProjectAllLight(float *all_proj_gray, int ang_num, int proj_width, int **all_light_path, int *all_pixc, Slice* slc);

#endif