#include "img_projs.h"
#include <cmath>
#include "dataf/quat_vec.h"
#include "sba.h"

using namespace bundle;

void calcImgProjFullR(double a[5],double qr0[4],double t[3],double M[3],
double n[2])
{
  double t1;
  double t11;
  double t13;
  double t17;
  double t2;
  double t22;
  double t27;
  double t3;
  double t38;
  double t46;
  double t49;
  double t5;
  double t6;
  double t8;
  double t9;
  {
    t1 = a[0];
    t2 = qr0[1];
    t3 = M[0];
    t5 = qr0[2];
    t6 = M[1];
    t8 = qr0[3];
    t9 = M[2];
    t11 = -t3*t2-t5*t6-t8*t9;
    t13 = qr0[0];
    t17 = t13*t3+t5*t9-t8*t6;
    t22 = t6*t13+t8*t3-t9*t2;
    t27 = t13*t9+t6*t2-t5*t3;
    t38 = -t5*t11+t13*t22-t27*t2+t8*t17+t[1];
    t46 = -t11*t8+t13*t27-t5*t17+t2*t22+t[2];
    t49 = 1/t46;
    n[0] = (t1*(-t2*t11+t13*t17-t22*t8+t5*t27+t[0])+a[4]*t38+a[1]*t46)*t49;
    n[1] = (t1*a[3]*t38+a[2]*t46)*t49;
    return;
  }
}

#include <math.h>
void calcImgProjJacKRTS(double a[5],double qr0[4],double v[3],double t[3],
double M[3],double jacmKRT[2][11],double jacmS[2][3])
{
  double t1;
  double t102;
  double t107;
  double t109;
  double t11;
  double t114;
  double t116;
  double t120;
  double t129;
  double t13;
  double t131;
  double t140;
  double t148;
  double t149;
  double t15;
  double t150;
  double t152;
  double t154;
  double t161;
  double t164;
  double t167;
  double t17;
  double t170;
  double t172;
  double t174;
  double t177;
  double t18;
  double t182;
  double t187;
  double t189;
  double t194;
  double t196;
  double t2;
  double t208;
  double t218;
  double t229;
  double t232;
  double t235;
  double t237;
  double t239;
  double t24;
  double t242;
  double t247;
  double t25;
  double t252;
  double t254;
  double t259;
  double t261;
  double t273;
  double t283;
  double t295;
  double t296;
  double t298;
  double t3;
  double t301;
  double t304;
  double t305;
  double t307;
  double t308;
  double t31;
  double t311;
  double t32;
  double t326;
  double t327;
  double t329;
  double t332;
  double t333;
  double t34;
  double t349;
  double t35;
  double t352;
  double t4;
  double t41;
  double t45;
  double t5;
  double t50;
  double t51;
  double t56;
  double t57;
  double t6;
  double t60;
  double t66;
  double t67;
  double t68;
  double t74;
  double t76;
  double t78;
  double t79;
  double t8;
  double t81;
  double t83;
  double t85;
  double t87;
  double t89;
  double t9;
  double t91;
  double t93;
  double t95;
  double t97;
  {
    t1 = v[0];
    t2 = t1*t1;
    t3 = v[1];
    t4 = t3*t3;
    t5 = v[2];
    t6 = t5*t5;
    t8 = sqrt(1.0-t2-t4-t6);
    t9 = qr0[1];
    t11 = qr0[0];
    t13 = qr0[3];
    t15 = qr0[2];
    t17 = t8*t9+t11*t1+t13*t3-t5*t15;
    t18 = M[0];
    t24 = t8*t15+t3*t11+t5*t9-t13*t1;
    t25 = M[1];
    t31 = t8*t13+t5*t11+t1*t15-t3*t9;
    t32 = M[2];
    t34 = -t17*t18-t24*t25-t31*t32;
    t35 = -t17;
    t41 = t11*t8-t1*t9-t3*t15-t5*t13;
    t45 = t41*t18+t24*t32-t31*t25;
    t50 = t41*t25+t31*t18-t17*t32;
    t51 = -t31;
    t56 = t41*t32+t17*t25-t24*t18;
    t57 = -t24;
    t60 = t34*t35+t41*t45+t50*t51-t56*t57+t[0];
    t66 = t34*t51+t41*t56+t45*t57-t50*t35+t[2];
    t67 = 1/t66;
    jacmKRT[0][0] = t60*t67;
    t68 = a[3];
    t74 = t34*t57+t41*t50+t56*t35-t45*t51+t[1];
    jacmKRT[1][0] = t68*t74*t67;
    jacmKRT[0][1] = 1.0;
    jacmKRT[1][1] = 0.0;
    jacmKRT[0][2] = 0.0;
    jacmKRT[1][2] = 1.0;
    jacmKRT[0][3] = 0.0;
    t76 = a[0];
    jacmKRT[1][3] = t76*t74*t67;
    jacmKRT[0][4] = t74*t67;
    jacmKRT[1][4] = 0.0;
    t78 = 1/t8;
    t79 = t78*t9;
    t81 = -t79*t1+t11;
    t83 = t78*t15;
    t85 = -t83*t1-t13;
    t87 = t78*t13;
    t89 = -t87*t1+t15;
    t91 = -t81*t18-t85*t25-t89*t32;
    t93 = -t81;
    t95 = t78*t11;
    t97 = -t95*t1-t9;
    t102 = t97*t18+t85*t32-t89*t25;
    t107 = t97*t25+t89*t18-t81*t32;
    t109 = -t89;
    t114 = t97*t32+t81*t25-t85*t18;
    t116 = -t85;
    t120 = a[4];
    t129 = t91*t57+t34*t116+t97*t50+t41*t107+t114*t35+t56*t93-t102*t51-t45*t109
;
    t131 = a[1];
    t140 = t91*t51+t34*t109+t97*t56+t41*t114+t102*t57+t45*t116-t107*t35-t50*t93
;
    t148 = t66*t66;
    t149 = 1/t148;
    t150 = (t76*t60+t120*t74+t131*t66)*t149;
    jacmKRT[0][5] = (t76*(t91*t35+t34*t93+t97*t45+t41*t102+t107*t51+t50*t109-
t114*t57-t56*t116)+t129*t120+t131*t140)*t67-t150*t140;
    t152 = t76*t68;
    t154 = a[2];
    t161 = (t152*t74+t154*t66)*t149;
    jacmKRT[1][5] = (t152*t129+t154*t140)*t67-t161*t140;
    t164 = -t79*t3+t13;
    t167 = -t83*t3+t11;
    t170 = -t87*t3-t9;
    t172 = -t164*t18-t167*t25-t170*t32;
    t174 = -t164;
    t177 = -t95*t3-t15;
    t182 = t177*t18+t167*t32-t170*t25;
    t187 = t177*t25+t170*t18-t164*t32;
    t189 = -t170;
    t194 = t177*t32+t164*t25-t167*t18;
    t196 = -t167;
    t208 = t172*t57+t34*t196+t177*t50+t41*t187+t194*t35+t56*t174-t182*t51-t45*
t189;
    t218 = t172*t51+t34*t189+t177*t56+t41*t194+t182*t57+t45*t196-t187*t35-t50*
t174;
    jacmKRT[0][6] = (t76*(t172*t35+t34*t174+t177*t45+t41*t182+t187*t51+t50*t189
-t194*t57-t56*t196)+t120*t208+t131*t218)*t67-t150*t218;
    jacmKRT[1][6] = (t152*t208+t154*t218)*t67-t161*t218;
    t229 = -t79*t5-t15;
    t232 = -t83*t5+t9;
    t235 = -t87*t5+t11;
    t237 = -t229*t18-t232*t25-t235*t32;
    t239 = -t229;
    t242 = -t95*t5-t13;
    t247 = t242*t18+t232*t32-t235*t25;
    t252 = t242*t25+t235*t18-t229*t32;
    t254 = -t235;
    t259 = t242*t32+t229*t25-t232*t18;
    t261 = -t232;
    t273 = t237*t57+t261*t34+t242*t50+t41*t252+t259*t35+t56*t239-t247*t51-t45*
t254;
    t283 = t237*t51+t34*t254+t242*t56+t41*t259+t247*t57+t45*t261-t252*t35-t50*
t239;
    jacmKRT[0][7] = (t76*(t237*t35+t34*t239+t242*t45+t41*t247+t252*t51+t50*t254
-t259*t57-t56*t261)+t120*t273+t131*t283)*t67-t150*t283;
    jacmKRT[1][7] = (t152*t273+t154*t283)*t67-t161*t283;
    jacmKRT[0][8] = t76*t67;
    jacmKRT[1][8] = 0.0;
    jacmKRT[0][9] = t120*t67;
    jacmKRT[1][9] = t152*t67;
    jacmKRT[0][10] = t131*t67-t150;
    jacmKRT[1][10] = t154*t67-t161;
    t295 = t35*t35;
    t296 = t41*t41;
    t298 = t57*t57;
    t301 = t35*t57;
    t304 = t41*t51;
    t305 = 2.0*t301+t41*t31-t304;
    t307 = t35*t51;
    t308 = t41*t57;
    t311 = t307+2.0*t308-t31*t35;
    jacmS[0][0] = (t76*(t295+t296+t31*t51-t298)+t120*t305+t131*t311)*t67-t150*
t311;
    jacmS[1][0] = (t152*t305+t154*t311)*t67-t161*t311;
    t326 = t51*t51;
    t327 = t298+t296+t17*t35-t326;
    t329 = t57*t51;
    t332 = t41*t35;
    t333 = 2.0*t329+t41*t17-t332;
    jacmS[0][1] = (t76*(t301+2.0*t304-t17*t57)+t120*t327+t131*t333)*t67-t150*
t333;
    jacmS[1][1] = (t152*t327+t154*t333)*t67-t161*t333;
    t349 = t329+2.0*t332-t24*t51;
    t352 = t326+t296+t24*t57-t295;
    jacmS[0][2] = (t76*(2.0*t307+t41*t24-t308)+t120*t349+t131*t352)*t67-t150*
t352;
    jacmS[1][2] = (t152*t349+t154*t352)*t67-t161*t352;
    return;
  }
}


/** MEASUREMENT VECTOR AND JACOBIAN COMPUTATION FOR THE EXPERT DRIVERS ***/

/** FULL BUNDLE ADJUSTMENT, I.E. SIMULTANEOUS ESTIMATION OF CAMERA AND STRUCTURE PARAMETERS */

/**Given a parameter vector p made up of the 3D coordinates of n points and the parameters of m cameras, compute in
 * hx the prediction of the measurements, i.e. the projections of 3D points in the m images. The measurements
 * are returned in the order (hx_11^T, .. hx_1m^T, ..., hx_n1^T, .. hx_nm^T)^T, where hx_ij is the predicted
 * projection of the i-th point on the j-th camera.
 * Notice that depending on idxij, some of the hx_ij might be missing
 *
 */
void img_projsKRTS_x(double *p, struct sba_crsm *idxij, int *rcidxs, int *rcsubs, double *hx, void *adata)
{
  register int i, j;
  int cnp, pnp, mnp;
  double *pa, *pb, *pqr, *pt, *ppt, *pmeas, *pcalib, *pr0, lrot[FULLQUATSZ], trot[FULLQUATSZ];
  //int n;
  int m, nnz;
  struct globs_ *gl;

  gl=(struct globs_ *)adata;
  cnp=gl->cnp; pnp=gl->pnp; mnp=gl->mnp;

  //n=idxij->nr;
  m=idxij->nc;
  pa=p; pb=p+m*cnp;

  for(j=0; j<m; ++j){
    /* j-th camera parameters */
    pcalib=pa+j*cnp;
    pqr=pcalib+5;
    pt=pqr+3; // quaternion vector part has 3 elements
    pr0=gl->initial_rot_params+j*FULLQUATSZ; // full quat for initial rotation estimate
    _MK_QUAT_FRM_VEC(lrot, pqr);
    QuatMultiply(lrot, pr0, trot); // trot=lrot*pr0

    nnz=sba_crsm_col_elmidxs(idxij, j, rcidxs, rcsubs); /* find nonzero hx_ij, i=0...n-1 */

    for(i=0; i<nnz; ++i){
      ppt=pb + rcsubs[i]*pnp;
      pmeas=hx + idxij->val[rcidxs[i]]*mnp; // set pmeas to point to hx_ij

      calcImgProjFullR(pcalib, trot, pt, ppt, pmeas); // evaluate Q in pmeas
      //calcImgProj(pcalib, pr0, pqr, pt, ppt, pmeas); // evaluate Q in pmeas
    }
  }
}

/**Given a parameter vector p made up of the 3D coordinates of n points and the parameters of m cameras, compute in
 * jac the jacobian of the predicted measurements, i.e. the jacobian of the projections of 3D points in the m images.
 * The jacobian is returned in the order (A_11, ..., A_1m, ..., A_n1, ..., A_nm, B_11, ..., B_1m, ..., B_n1, ..., B_nm),
 * where A_ij=dx_ij/db_j and B_ij=dx_ij/db_i (see HZ).
 * Notice that depending on idxij, some of the A_ij, B_ij might be missing
 *
 */
void img_projsKRTS_jac_x(double *p, struct sba_crsm *idxij, int *rcidxs, int *rcsubs, double *jac, void *adata)
{
  register int i, j, ii, jj;
  int cnp, pnp, mnp, ncK;
  double *pa, *pb, *pqr, *pt, *ppt, *pA, *pB, *pcalib, *pr0;
  //int n;
  int m, nnz, Asz, Bsz, ABsz;
  struct globs_ *gl;
  
  gl=(struct globs_ *)adata;
  cnp=gl->cnp; pnp=gl->pnp; mnp=gl->mnp;
  ncK=gl->nccalib;

  //n=idxij->nr;
  m=idxij->nc;
  pa=p; pb=p+m*cnp;
  Asz=mnp*cnp; Bsz=mnp*pnp; ABsz=Asz+Bsz;

  for(j=0; j<m; ++j){
    /* j-th camera parameters */
    pcalib=pa+j*cnp;
    pqr=pcalib+5;
    pt=pqr+3; // quaternion vector part has 3 elements
    pr0=gl->initial_rot_params+j*FULLQUATSZ; // full quat for initial rotation estimate

    nnz=sba_crsm_col_elmidxs(idxij, j, rcidxs, rcsubs); /* find nonzero hx_ij, i=0...n-1 */

    for(i=0; i<nnz; ++i){
      ppt=pb + rcsubs[i]*pnp;
      pA=jac + idxij->val[rcidxs[i]]*ABsz; // set pA to point to A_ij
      pB=pA  + Asz; // set pB to point to B_ij

      calcImgProjJacKRTS(pcalib, pr0, pqr, pt, ppt, (double (*)[5+6])pA, (double (*)[3])pB); // evaluate dQ/da, dQ/db in pA, pB

      /* clear the columns of the Jacobian corresponding to fixed calibration parameters */
      if(ncK){
        int jj0=5-ncK;

        for(ii=0; ii<mnp; ++ii, pA+=cnp)
          for(jj=jj0; jj<5; ++jj)
            pA[jj]=0.0; // pA[ii*cnp+jj]=0.0;
      }
    }
  }
}

