#ifndef BUNDLE_CORE_H__
#define BUNDLE_CORE_H__

#include "dataf/dataf.h"
#include "params.h"
#include "sfm_driver.h"
#include "util/exception.h"
#include "geometry_data.h"
#include <vector>
#include "dataf/camera.h"

/** The implement of bundle adjustment.*/
class BundleApp{
private:
    bundle::SFMDriver driver;
    bundle::GeometryData data;		///< the used data
    int width, height;
    
    const static int m_min_max_matches = 16;           /* Minimum number of matches needed to register an image */
    
private:
    mx::camera_params* result;
    std::vector<bundle::PointData> m_point_data;   		///> Information about 3D points in the scene
    
private:
    BundleApp();
    ~BundleApp();

private:
    void InitGeometryData(util::TrackSpace& trackspace);
    
    void InitBoundary(int _width, int _height);
    
    int PickInitialPair(int* i_best, int* j_best);
    
    void InitCameraParam(float angle, mx::camera_params* camera) const;
    
    void InitMotParam(float angle, double* params) const;
    
    /** @brief Setup the initial camera pair for bundle adjustment */
    int SetupInitialCameraPair(int i_best, int j_best, mx::camera_params* cameras, v3_t* points, std::vector<bundle::ImageKeyVector>& pt_views);
    
    /** @brief Find the camera with the most matches to existing points */
    int FindCameraWithMostMatches(int* added_order, int num_cameras, int* max_matches, const std::vector<bundle::ImageKeyVector>& pt_views);
    
    /** @brief Find all cameras with at least N matches to existing points */
    std::vector<int> FindCamerasWithNMatches(int n, int* added_order, int num_cameras, const std::vector<bundle::ImageKeyVector> &pt_views);
    
    /** @brief Initialize an image for bundle adjustment (have not been used)
     *  @c image_idx refer to the order in origin(total)
     *  @c camera_idx refer to the add order
     */
    void BundleInitializeImage(int* added_order, mx::camera_params* cameras, int image_idx, int added_order_idx, 
			       v3_t* points, int num_points, std::vector<bundle::ImageKeyVector> &pt_views, bool* success_out);
    
    /** @brief Add new points(3d object point) to the bundle adjustment */
    int BundleAdjustAddAllNewPoints(int num_points, int num_cameras, int* added_order, mx::camera_params* cameras, v3_t* points,
                                       std::vector<bundle::ImageKeyVector> &pt_views, double max_reprojection_error, int min_views);
    
    /** @brief Triangulate a subtrack */
    v3_t TriangulateNViews(const bundle::ImageKeyVector& views, int* added_order, mx::camera_params* cameras, double& error);
    
    void Process();
    void PrintCameras(const char* filename = "cameras.params") const;
    void PrintPoints(const char* filename = "points.mot") const;
    void PrintTrackBin(const char* filename = "track.hist") const;
    void PrintRepErrorBin(mx::camera_params* camparams, int ncams, const int added_order[],
				 const v3_t* points, const char* foldername = "verbose") const;
    void DumpOutCameras(std::vector<mx::camera_params>* cameras) const;
    void DumpOutPoints(std::vector<v3_t>* points) const;
    
public:
    static void BundleMain(util::TrackSpace& tspace_, int width_, int height_, std::vector<mx::camera_params>* cameras, std::vector<v3_t>* points);

public:
    static void Test();
};

#endif