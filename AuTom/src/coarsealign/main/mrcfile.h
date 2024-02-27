#ifndef MRCFILES_H
#define MRCFILES_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#ifndef FALSE
#define FALSE       0           /*false for boolean*/
#endif
#ifndef TRUE
#define TRUE        1           /*true for boolean*/
#endif

#ifndef False
#define False       0
#endif
#ifndef True
#define True        1
#endif

#ifndef SEEK_SET
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#endif



#define MRC_MODE_BYTE            0
#define MRC_MODE_SHORT           1
#define MRC_MODE_FLOAT           2
#define MRC_MODE_COMPLEX_SHORT   3
#define MRC_MODE_COMPLEX_FLOAT   4
#define MRC_MODE_USHORT          6
#define MRC_MODE_RGB             16

#define MRC_CFLOAT_REAL_IMAG     20            /* COMPLEX FLOAT mode */
#define MRC_CFLOAT_AMP_RAD       21           /* COMPLEX FLOAT mode, but in amplitude and phase(  ) form */
#define MRC_CFLOAT_AMP_DEG       22           /* COMPLEX FLOAT mode, but in amplitude and phase( degree ) form */


#define MRC_LABEL_SIZE         80
#define MRC_NEXTRA             16
#define MRC_NLABELS            10
#define MRC_HEADER_SIZE        1024   /* Length of Header is 1024 Bytes. */
#define MRC_MAXCSIZE           3

#ifndef EPS
#define EPS 0.001
#endif

/* The header structure for MRC files */
#pragma pack(1)
typedef struct MRCheader
{
  int   nx;         /*  # of Columns                  */
  int   ny;         /*  # of Rows                     */
  int   nz;         /*  # of Sections.                */
  int   mode;       /*  given by #define MRC_MODE...  */

  int   nxstart;    /*  Starting point of sub image.  */
  int   nystart;
  int   nzstart;

  int   mx;         /* Grid size in x, y, and z       */
  int   my;
  int   mz;

  float   xlen;       /* length of x element in um.     */
  float   ylen;       /* get scale = xlen/nx ...        */
  float   zlen;

  float   alpha;      /* cell angles, ignore */
  float   beta;
  float   gamma;

  int   mapc;       /* map coloumn 1=x,2=y,3=z.       */
  int   mapr;       /* map row     1=x,2=y,3=z.       */
  int   maps;       /* map section 1=x,2=y,3=z.       */

  float   amin;
  float   amax;
  float   amean;
  
  short   ispg;       /* image type */
  short   nsymbt;     /* space group number */


  /* 64 bytes */

  int   next;
  short   creatid;  /* Creator id, hvem = 1000, DeltaVision = -16224 */

  
  char    blank[30];
  
  short   nint;
  short   nreal;
  short   sub;
  short   zfac;

  float   min2;
  float   max2;
  float   min3;
  float   max3;
  float   min4;
  float   max4;


  short   idtype;
  short   lens;
  short   nd1;     /* Devide by 100 to get float value. */
  short   nd2;
  short   vd1;
  short   vd2;
  float   tiltangles[6];  /* 0,1,2 = original:  3,4,5 = current */


  float   xorg;
  float   yorg;
  float   zorg;
  char    cmap[4];
  char    stamp[4];
  float   rms;

  int nlabl;
  char  labels[10][80];


} MrcHeader;
#pragma pack()
/* END OF HEAD CODE */


/******************************** Header functions **************************/

#ifdef __cplusplus
extern "C" 
{
#endif


//read head of file fin to hdata;
int mrc_read_head (FILE *fin,  MrcHeader *head);
int mrc_read_pixel(void *pix_gray, FILE *fin, MrcHeader  *head, int x, int y, int z );
int mrc_read_slice(FILE *fin, MrcHeader  *head, int slcN,char axis,float *slcdata);   //
int mrc_read_all(FILE *fin, MrcHeader  *head, void *mrc_data_all);

int mrc_cut_one_slice(FILE *fin, FILE *fout, int slcN, char axis );

//initiate the head of a mrc file
int mrc_init_head(MrcHeader *head);

//write head of file fout from hdata;
int mrc_write_head(FILE *fout, MrcHeader *head);

//replace the head of file outf by head
int mrc_replace_head(char *outf,MrcHeader *head);

//update the origin head by calculate the true value of the each pixel
int mrc_update_head(char *inoutf);

//set the length of each pixel of a mrc file;
int mrc_set_plen(char *inoutf,float plen);
int mrc_set_plen_3d(char *inoutf,float plenx, float pleny, float plenz);

//set the full length of the volume data

//create a new file.
void mrc_new_file(char *newf,  MrcHeader  *outhead);


//add the  slice data to file fout befor slice N, the slice  counted from 0,if slcN equals N then it mean add the slice is after N-1;
int mrc_add_sliceN(FILE *fout, MrcHeader  *headout, float *slcdata, int slcN);
int mrc_add_slice(FILE *fout , MrcHeader  *headout, float *slcdata);

//reverse the image by the function amax-gray,so the original max will be 0
void mrc_reverse(char *inoutf);

//write the slice data to file fout;
int mrc_write_slice(char *outf,float *slcdata);


//update  slice data of file fout
int mrc_update_slice(char *outf ,int slcN,void *slcdata);

#ifdef __cplusplus
}
#endif

#endif

