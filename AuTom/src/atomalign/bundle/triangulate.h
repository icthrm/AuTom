#ifndef TRIANGULATE_H__
#define TRIANGULATE_H__

#include "matrix/matrix.h"
#include "matrix/vector.h"
#include "util/exception.h"

namespace bundle{

    /* Find the point with the smallest squared projection error */
    v3_t triangulate_n(int num_points, const v2_t* pv, const double* Ps, double *error_out);
    
    /* Solve for a 3x4 projection matrix, given a set of 3D points and 2D projections */
    bool find_projection_3x4(int num_pts, const v3_t *points, const v2_t *projs, double *P);
    
    bool find_projection_3x4_ransac(int num_pts, const v3_t *points, const v2_t *projs, double *P);
    
    bool find_projection_3x4_ransac(int num_pts, const v3_t* points, const v2_t* projs, double* P, 
                                       int min_req, double p_bad, double err_tol, double* inl_ratio);
    
    bool pattern_search_projection(int num_pts, const v3_t* points, const v2_t* projs, double K[9], double euler_angle[3], double T[3], const double* steps, double tol = 0.000001);
    
    void euler_to_R(const double euler_angle[3], double R[9]);
}

#endif