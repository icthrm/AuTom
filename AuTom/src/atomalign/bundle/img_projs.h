#ifndef IMG_PROJS_H__
#define IMG_PROJS_H__

#include "sba.h"
#include "sfm_driver.h"

#define FULLQUATSZ	4

void calcImgProjFullR(double a[5], double qr0[4], double t[3], double M[3], double n[2]);
void calcImgProjJacKRTS(double a[5], double qr0[4], double v[3], double t[3], double M[3], double jacmKRT[2][11], double jacmS[2][3]);

void img_projsKRTS_x(double *p, struct sba_crsm *idxij, int *rcidxs, int *rcsubs, double *hx, void *adata);
void img_projsKRTS_jac_x(double *p, struct sba_crsm *idxij, int *rcidxs, int *rcsubs, double *jac, void *adata);


#endif