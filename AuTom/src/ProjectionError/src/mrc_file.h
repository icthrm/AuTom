/*
 * mrc_file.h
 *
 *  Created on: Apr 16, 2013
 *      Author: ict
 */

#ifndef MRC_FILE_H_
#define MRC_FILE_H_

#ifndef PI
#define     PI  3.14159265358979323846
#endif

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include<math.h>
#include<string.h>
#include <time.h>
#ifndef FALSE
#define FALSE       1           /*false for boolean*/
#endif
#ifndef TRUE
#define TRUE        0           /*true for boolean*/
#endif

#ifndef falsefloat
#define false       1
#endif
#ifndef true
#define true        0
#endif

#ifndef SEEK_SET
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#endif



#define MRC_MODE_BYTE          0
#define MRC_MODE_SHORT         1
#define MRC_MODE_FLOAT         2
#define MRC_MODE_COMPLEX_SHORT 3
#define MRC_MODE_COMPLEX_FLOAT 4
#define MRC_MODE_USHORT        6
#define MRC_MODE_RGB           16


#define MRC_LABEL_SIZE         80
#define MRC_NEXTRA             16
#define MRC_NLABELS            10
#define MRC_HEADER_SIZE        1024   /* Length of Header is 1024 Bytes. */
#define MRC_MAXCSIZE           3



/*typedef struct  //complex floating number
{
  float a;
  float b;

} ComplexFloat;

typedef struct  //complex short number
{
  short a;
  short b;

} ComplexShort;
*/

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
/* END_CODE */



/******************************** Header functions or useful io functions **************************/
float rad(float ang);

//read head of file fin to hdata;
int mrc_read_head (FILE *fin,  MrcHeader *head);
int mrc_write_head(FILE *fout, MrcHeader *head);
int mrc_init_head(MrcHeader *head);
int mrc_replace_head(char *outf,MrcHeader *head);
int mrc_update_head(char *inoutf);

//create a new mrc file using the head outhead;
int mrc_new_file(char *newf,  MrcHeader  *outhead);

int mrc_read_pixel( FILE *fin, MrcHeader  *head, int x, int y, int z, void *pix_gray );
int mrc_read_slice(FILE *fin, MrcHeader  *head, int slcN,char axis,float *slcdata);  //�˺���Ŀǰ���һ������Ϊfloat *, ׼����Ϊvoid *
void mrc_read_slice_y(FILE *fin, MrcHeader *head, int slcN, float *slcdata);
int mrc_read_block(FILE *fin, MrcHeader  *head, int start, int end, char axis,float *mrc_data_block);
int mrc_read_all(FILE *fin, MrcHeader  *head, float *mrc_data_all);

int mrc_write_slice(FILE *fout, MrcHeader  *head, int slcN,char axis,float *slcdata);
//add slcdata to fout ,slcN ranges from 0 to headout->nz
int mrc_add_sliceN(FILE *fout, MrcHeader  *headout, float *slcdata, int slcN);
int mrc_add_slice(FILE *fout , MrcHeader  *headout, float *slcdata);
int mrc_write_all(FILE *fout, MrcHeader  *head,  int Z_start,int Z_end, void *mrc_data_all);

int mrc_flipyz(char *inf,char *outf,int myid,int mypro);
int mrc_upsidedown(char *inf,char *outf);


//read sub area of the 3d image;
/*int mrc_read_sub(FILE *fin, MrcHeader  *head, int sx,int sy,int sz,int xsize,int ysize,int zsize, void *mrc_data_sub);

int mrc_check_head(FILE *fin);
int mrc_check_size(FILE *fin);
int mrc_check(FILE *fin);

//set the length of each pixel of a mrc file;
void mrc_set_plen(char *inoutf,float plen);

//set the length of each dimesion;
void mrc_set_flen(char *inoutf,float xlen,float ylen, float zlen);





//write the slice data to file fout;
int mrc_replace_sliceN(FILE *fout, MrcHeader  *headout, float *slcdata, int slcN);

//read the number slcN slice data to slc from file fin,counted from 0;
int mrc_read_slice(FILE *fin, MrcHeader  *head, int slcN,char axis,float *slcdata); //axis can be 'x','X','y','Y','z','Z';

//update  slice data of file fout
int mrc_update_slice(char *outf ,int slcN,void *slcdata);
*/
//float rad(float)




#endif /* MRC_FILE_H_ */
