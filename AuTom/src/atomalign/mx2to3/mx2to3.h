#ifndef MX2TO3_H__
#define MX2TO3_H__

#include "dataf/camera.h"
#include "util/matrix.h"
#include "matrix/matrix.h"
#include "interpret/object_estimate.h"
#include <math.h>

// abc[3] is the vector of tilt axis

namespace mx2to3{

void RotCameraCoordinate(double angle, mx::camera_params* camparam);

void GlobalTiltAxisEstimate(const mx::camera_params* camparams, int num, double abc[3], int* inlier_size);

void GlobalTiltAxisEstimate2(const mx::camera_params* camparams, int num, double abc[3], int* inlier_size);

/** @brief ensure the x-axis direction unchanged*/
void DirectDotEstimateRot(const mx::camera_params& camparam, const double abc[3], double* angle);

/** @brief ensure the y-axis direction unchanged*/
void DirectDotEstimateRot2(const mx::camera_params& camparam, const double abc[3], double* angle);

void TiltVectorEstimate(const mx::camera_params& camparam, const double abc[3], double dir[3]);

float CompensativeScale(const mx::camera_params& camparam, const double abc[3]);

void HorizonEstimateRot(const mx::camera_params& camparam, double* angle);
    
void DirectionVerticalEstimateCentre(const mx::camera_params* camparams, int num, v3_t* centre);

}

#endif