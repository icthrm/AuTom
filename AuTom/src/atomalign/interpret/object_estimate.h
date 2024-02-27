#ifndef OBJECT_ESTIMATE_H__
#define OBJECT_ESTIMATE_H__

#include <vector>

#include "util/matrix.h"
#include "matrix/vector.h"

namespace util{

/** @brief z = ax+by+c*/
bool LeastSquare3DPanel(const std::vector<v3_t>& pt_v, double abc[3]);

double PlanePointError(const v3_t& pt, double abc[3]);

/* RANSAC error tolerance in pixels */
#define RANSAC_ERR_TOL 3

/** pessimistic estimate of fraction of inlers for RANSAC */
#define RANSAC_INLIER_FRAC_EST 0.25

/** estimate of the probability that a correspondence supports a bad model */
#define RANSAC_PROB_BAD_SUPP 0.10


/**
  Calculates a best-fit 3d plane from 3d points correspondences using RANSAC.
  @param min_req minimum number of correspondences necessary to instantiate the model computed by lsq_homog
  @param p_badplane desired probability that the final transformation
    returned by RANSAC is corrupted by outliers (i.e. the probability that
    no samples of all inliers were drawn)
  @param err_tol correspondences within this distance of a computed model are
    considered as inliers
  @param inliers if not NULL, output as an array of pointers to the final
    set of inliers
*/
bool RansacPlane(const std::vector<v3_t>& pt_v, int min_req, double p_badplane, double err_tol, 
		 double abc[3], std::vector<v3_t>* inliers, std::vector<v3_t>* outliers, const std::vector<v3_t>* pts_const = NULL);

bool RansacVector(const std::vector<v3_t>& pt_v, int min_req, double p_bad, double err_tol, double abc[3], int* inlier_size);

}

#endif