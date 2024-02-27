#ifndef ATOM_H
#define ATOM_H


#include "mrcfile_atom.h"
#include "opt.h"

#ifndef TRUE
#define TRUE  0
#define FALSE 1
#endif

#ifndef TEXT_LINE_MAX
#define TEXT_LINE_MAX 500
#endif

#ifndef Tilt_MAX
#define Tilt_MAX 200
#endif


/*Volume stores the parameter(not the data) of a volum data*/
typedef struct {

	int Xstart;
	int Xend;
	int Ystart;
	int Yend;
	int Zstart;
	int Zend;

	int X;    //X,Y,Z are the pixel size of the three dimesions in a volum data;
	int Y;
	int Z;                       //it also equals the thickness of a volum data;

	int volZ_per;
	int Zcount;
	int Zstart_per;
	int Zend_per;
} Volume;

/*Proj stores the parameter(not the data) of a projection data*/
typedef struct
{
  int X;                       //X,Y,Z are the pixel size of the three dimesions in a volum data;
  int Y;
  float angle_start;   //tilt angle
  float angle_increment;
  float angle_end;

  int AngN;                               //it also equals the thickness of a volum data;

} Projection;


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
  double del; //weight of the proj*/

  int    x_min;//x coordinate of the proj
  int    y_min;//y coordinate of the proj

  double x_min_del;
  double y_min_del; //weight of the proj


} Weight;


typedef struct {
	Projection prj;
	Volume vol;
	Pixel0 pixel0;
	Weight comp_prj;


	double* x_coef;
	double* y_coef;

	float *prj_real;
	float *slc_data;

	float *prj_calc;
	float *prj_calc_num;
	float *prj_calc_abs;

	float *global_prj_calc;
	float *global_prj_calc_num;
	float *global_prj_calc_abs;


} ImageStruct;


#endif
