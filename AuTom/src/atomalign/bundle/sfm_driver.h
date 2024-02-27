#ifndef SFM_DRIVER_H__
#define SFM_DRIVER_H__

#include "img_projs.h"
#include "dataf/quat_vec.h"
#include "params.h"
#include "dataf/camera.h"
#include "geometry_data.h"
#include "matrix/vector.h"
#include "sba.h"

#define CNP		6+5
#define PNP		3   
#define MNP		2
#define FULLQUATSZ	4

namespace bundle{

/** pointers to additional data, used for computed image projections and their jacobians */
struct globs_{
    double *initial_rot_params; /* initial rotation parameters, combined with a local rotation parameterization */
    double *intrinsic_calib; /* the 5 intrinsic calibration parameters in the order [fu, u0, v0, ar, skew],
                      * where ar is the aspect ratio fv/fu.
                      * Used only when calibration is fixed for all cameras;
                      * otherwise, it is null and the intrinsic parameters are
                      * included in the set of motion parameters for each camera
                      */
    int nccalib; /* number of calibration parameters that must be kept constant.
                * 0: all parameters are free
                * 1: skew is fixed to its initial value, all other parameters vary (i.e. fu, u0, v0, ar)
                * 2: skew and aspect ratio are fixed to their initial values, all other parameters vary (i.e. fu, u0, v0)
                * 3: meaningless
                * 4: skew, aspect ratio and principal point are fixed to their initial values, only the focal length varies (i.e. fu)
                * 5: all intrinsics are kept fixed to their initial values
                * >5: meaningless
                * Used only when calibration varies among cameras
                */
    int cnp, pnp, mnp; /* dimensions */
};

class SFMDriver{
private:
    //defination of parameters used in sba API
    static const int cnp = 11; 					///< 3 rot params + 3 trans params;6+5(4 fixed)
    static const int pnp = PNP; 				///< euclidean 3D points 
    static const int mnp = MNP; 				///< image points are 2D
    double opts[SBA_OPTSSZ], info[SBA_INFOSZ+1];
    static const int verbose = 0;
    constexpr static const double min_proj_error_threshold = 1.2;//1.5;
   constexpr  static const double max_proj_error_threshold = 3.2;//2.5;//4.5;
    
#define WEIGHT_SIZE  		360
    float weight[WEIGHT_SIZE];
private:
    globs_ globs;
    
private:
    void Initialize();
    void Project(const double P[12], const double X[4], double r[3]);
    
    int Run(double* motstruct, double* imgpts, char* vmask, 
		    const int n3Dpts, const int ncams, const int nconcam, const int ncon3Dpts);
    
public:
    SFMDriver();
    ~SFMDriver();
    
    double Run(int* added_order, mx::camera_params* camparams, const int start_camera, const int ncams, const int nconcam,
				 v3_t* init_pts, const int n3Dpts, const int ncon3Dpts, std::vector<ImageKeyVector>& pt_views, GeometryData& data, bool remove_outliers = true);
    
    void RefineCameraParameters(v3_t* points, v2_t* projs, int num_points, mx::camera_params* camera);
    
    v2_t Project(const mx::camera_params& params, v3_t pt);
    
    int Cnp() const {return cnp;}
    int Mnp() const {return mnp;}
    int Pnp() const {return pnp;}
    const double* Info() const {return info;}
};

}

#endif
