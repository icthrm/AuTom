#ifndef INTERPRET_H__
#define INTERPRET_H__

#include <iostream>
#include <climits>
#include <vector>
#include <sstream>
#include <sys/stat.h>
#include <algorithm>
#include <float.h>
#include <fstream>
#include "matrix/vector.h"
#include "dataf/camera.h"
#include "dataf/calibration.h"
#include "object_estimate.h"

extern "C"{
#include <clapack.h>
#include <fblaswr.h>
#include <f2c.h>
#include <math.h>
}

#ifndef ABS
#define ABS(x) ( ( (x) < 0 )? -(x) : (x) )
#endif

class Interpreter{
private:
    int ank_idx;

private:
    void BodyPlaneEstimate(const std::vector<v3_t>& pt_v, double abcd[4]);
    
    v3_t BodyCentreEstimate(double x, double y, const mx::camera_params& camparam, const double abcd[4]);

    /** @c x @c y are the location of centre point in picture*/
    v3_t BodyCentreEstimate(double x, double y, const mx::camera_params* camparams, int num, const double abcd[4]);

    /** @brief P_c = R*P_w+t*/
    void MotPointToTiltPlanePoint(const mx::camera_params* camparams, int num, const v3_t& point, std::vector<v3_t>& pt_v);

    /** @brief R'*P_c +C_~ = R'*(P_c-t) = P_w*/
    void TiltPlanePointToMotPoint(const mx::camera_params& camparam, const v3_t& point, v3_t* r);

    /** @brief R_T*P_c +C_~ = P-->ax+by+cz+d=0-->a`x'+b`y'+c`z'+d`=0 */
    void MotCameraToTiltPlaneABCD(const mx::camera_params* camparams, int num, const double abcd[4], std::vector<mx::Matrix<4,1,double> >& plane_v);

    int AnchorIndex();
    void SetAnchorIndex(int idx);

    void RotCameraParams(double rot_angle, mx::camera_params* camparam);

    void RotCameraParams(double rot_angle[], mx::camera_params* camparams, int num);

    void RotMxPlane(double rot_angle, mx::Matrix<4, 1, double>& plane);

    void RotMxPlane(double rot_angle[], std::vector<mx::Matrix<4, 1, double> >& plane_v);

    void StretchMxPlane(const double A[4], mx::Matrix<4, 1, double>& plane);
    
    void PrintPoints(const std::vector<v3_t>& pt_v, const char* filename);

    /** @brief ax+by+cz+d=0 -> z=ax+by+c print the abc*/
    void PrintPlaneABCDAsABC(const double abcd[4], std::ostream& o);

    void PrintMxPlaneVecAsABC(const std::vector<mx::Matrix<4, 1, double> >& plane_v, const char* filename);
    
    void InterpretAndCalibrate(const mx::camera_params* result, int num, double abcd[4], std::vector<util::calibrated>* calib_v, util::globalcalib* gcalib);
    
public:
    static void InterpretMain(const std::vector<mx::camera_params>& cameras, const std::vector<v3_t>& points, 
			      const std::vector<float>& angles, std::vector<util::calibrated>* calib_v, util::globalcalib* gcalib);
};

#endif
