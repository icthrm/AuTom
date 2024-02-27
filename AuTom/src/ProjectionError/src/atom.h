#ifndef ATOM_H
#define ATOM_H


#include "mrc_file.h"

#ifndef TRUE
#define TRUE  0
#define FALSE 1
#endif

#ifndef TEXT_LINE_MAX
#define TEXT_LINE_MAX 500
#endif


#define ART_MODE_ADD   0
#define ART_MODE_MULT  1
#define ANG_MAX        360
#define ART_MODE_COMMON       0
#define ART_MODE_RANDOM       1
#define ART_MODE_ORDERED      2

#define SART_MODE_COMMON      10
#define SART_MODE_RANDOM      11
#define SART_MODE_ORDERED     12

#define SIRT_MODE_COMMON      20

#define BP_MODE_SIMPLE        30
#define BP_MODE_WEIGHTED      31
#define BP_MODE_FILTER        32

#define ASART_MODE_COMMON      40

#define ATOM_MODEL_ZERO       0
#define ATOM_MODEL_RANDOM     1
#define ATOM_MODEL_BP         2
#define ATOM_MODEL_WBP        3
#define ATOM_MODEL_FBP        4
#define ATOM_MODEL_DFR        5
#define ATOM_MODEL_IFR        6
#define ATOM_MODEL_EXIST      10


#define PATH_MODE_I           1
#define PATH_MODE_II          2
#define PATH_MODE_III         3
#define PATH_MODE_IV          4
#define PATH_MODE_V           5



#define MAXMUM_PATH_PIXEL 10240

#define ANG2RAD(ang) ang*M_PI/180
static int PIXC_CORRECTION = TRUE;
static int TILT_CORRECTION = FALSE;

/*Volume stores the parameter(not the data) of a volum data*/
typedef struct
{
  int X;                       //X,Y,Z are the pixel size of the three dimesions in a volum data;
  int Y;
  int Z;                               //it also equals the thickness of a volum data;


  float zshift;
  float offset;                //offset angle of the tilt image,which has an offset effect on the volum data;
  float pitch;                 // pitch angle of the volum data;
} Volume;

/*Proj stores the parameter(not the data) of a projection data*/
typedef struct
{
  int X;                       //X,Y,Z are the pixel size of the three dimesions in a volum data;
  int Y;
  int AngN;                               //it also equals the thickness of a volum data;

} Projection;

/*Pixel is the coordinate number of a 3d map*/
typedef struct
{
  int    X;
  int    Y;
  int    Z;

} Pixel;

/*computing proj by the coordinate of a 3D pixel*/
typedef struct
{
  int    x;//x coordinate of the proj
  int    y;//y coordinate of the proj
  double del; //weight of the proj

} Weight;


/* Line refers to the parameter of a line:cos(ang)*(x-x0)-sin(ang)*(z-z0)-k=0;*/
/*for a serial of lines with the same tilt angle,the line function is similar,only k varies*/
typedef struct
{
  float  ang;
  int    x0;
  int    z0;
  int    k;
} Line;

typedef struct __align__(4)
{
  int x0;                       //(x0,z0) is the center of a Slice
  int z0;
  int X;                        //X and Z are the pixel size of each dimesion
  int Y;
  int Z;
  int Zmin;
  int Zmax;
}Slice;
typedef struct __align__(4)
{
  int min;                      //pixels whose index number is above max or below min will be ignored in the iteraction
  int max;
} Slide;


typedef struct
{
   int min;
   int max;

} SlcRng;
typedef struct
{

        int   mode;               //the program mode ART_MODE_ADD or ART_MODE_MULT,defual is ART_MODE_ADD
        int   itr;
        float step;

        char  StartModel;         //ATOM_RADM_MODEL,

        char   isweight;          //when this is True the program will use the exact weighted function,but the speed will be much slower;defalt is false
        char   INTP;
        int PathMode;
} AtomPara;

#endif

