#ifndef FUNC_H
#define FUNC_H

#include "mrcimg/mrc2img.h"

#define MRC_MODE_BYTE            0
#define MRC_MODE_SHORT           1
#define MRC_MODE_FLOAT           2
#define MRC_MODE_COMPLEX_SHORT   3
#define MRC_MODE_COMPLEX_FLOAT   4
#define MRC_MODE_USHORT          6
#define MRC_MODE_RGB             16

int ivwGetPixelBytes(int mode);
void GetDataFromIplimage(const IplImage* iplimage , unsigned char** buf);
int BrightContractAdjust(
  const unsigned char* srcImg,
  unsigned char* dstImg,
  int n,
  float bright,
  float contract);
#endif