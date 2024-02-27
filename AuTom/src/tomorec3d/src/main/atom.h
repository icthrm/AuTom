#ifndef ATOM_H
#define ATOM_H


#ifndef TRUE
#define TRUE  0
#endif
#ifndef FALSE
#define FALSE 1
#endif

#ifndef TEXT_LINE_MAX
#define TEXT_LINE_MAX 500
#endif

#ifndef Tilt_MAX
#define Tilt_MAX 200
#endif


#include "mrcfile_atom.h"
#include "opt.h"
// #include "function.h"
// using namespace std;


/*Volume stores the parameter(not the data) of a volum data*/
typedef struct
{

  int Xstart;
  int Xend;
  int Ystart;
  int Yend;
  int Zstart;
  int Zend;

  int X;                       //X,Y,Z are the pixel size of the three dimesions in a volum data;
  int Y;
  int Z;                               //it also equals the thickness of a volum data;

  long long volY_per;
  int Ycount;
  int Ystart_per;
  int Yend_per;
} Volume;

/*Proj stores the parameter(not the data) of a projection data*/
typedef struct
{
  int X;                       //X,Y,Z are the pixel size of the three dimesions in a volum data;
  int Y;
 /* float angle_start;   //tilt angle
  float angle_increment;
  float angle_end;*/

  int AngN;                               //it also equals the thickness of a volum data;

} Projection;

/*a Slice means a reconstructed section on the XOZ plane;*/
typedef struct
{
  int x0;
  int z0;
  int X;
  int Z;
}Slide;

typedef struct
{
  //int x0;                       //(x0,z0) is the center of a Slice
  //int z0;
  //int X;                        //X and Z are the pixel size of each dimesion
  //int Z;

  int *min;                      //pixels whose index number is above max or below min will be ignored in the iteraction
  int *max;
  // need array
  float *data;                  //data stores every pixel value of a Slice, pixel_allnum
  float *s;
  float *c;
} Slice;


typedef struct  //rotation angle
{
	float start;
	float increment;
	float end;
	int rotN;

}Rot;


/*Pixel is the coordinate number of a 3d map*/
typedef struct
{
  int     X;
  int    Y;
  int    Z;

} Pixel;

typedef struct
{
    float X;
    float Y;
    float Z;
}Pixel0;
/*computing proj by the coordinate of a 3D pixel*/
typedef struct
{
 /* int    x;//x coordinate of the proj
  int    y;//y coordinate of the proj
  float del; //weight of the proj*/

  int    x_min;//x coordinate of the proj
  int    y_min;//y coordinate of the proj
  
  float x_min_del;
  float y_min_del; //weight of the proj


} Weight;

//for cuda data transfer
typedef struct {
	Projection prj;
	Volume vol;
	Pixel0 pixel0;
	Weight comp_prj;

	Slice slc;
	Slide slid;
	float* x_coef;
	float* y_coef;
	float *tilt_angle;
	float *prj_real;
	float *slc_data;

	float *prj_calc;
	float *prj_calc_num;
	float *prj_calc_num_abs;

	float *global_prj_calc;
	float *global_prj_calc_num;

} ImageStruct;




#endif
