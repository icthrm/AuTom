#include <iostream>
#include <climits>
#include <vector>
#include "sift/sift_core.h"
#include "dataf/dataf.h"
#include "dataf/calibration.h"
#include "mrcimg/mrc2img.h"
#include "keymatch/match_core.h"
#include "ransac/ransac_core.h"
#include "bundle/bundle_core.h"
#include "opts.h"
#include "ransac/xform.h"
#include "interpret/interpret.h"

using namespace std;

int main(int argc, char **argv) {
    opts param;
	param.rotation_angle = 0;
    if(GetOpts(argc, argv, &param) != 1) {
        return 0;
    }

    util::MrcStack mrcs;

    util::FeatureStack featsk;
    if(!mrcs.Open(param.input)) {
        std::cout<<"can't open mrc file."<<endl;
        return -1;
    }

    EX_TIME_BEGIN("\n%sDo SiftMain", _WAVE)
    Sift::SiftMain(mrcs, &featsk);
    EX_TIME_END("Do SiftMain")

//     featsk.ReadStackByFolder("featsv");
//     featsk.WriteStackByFolder("featsv");
//     Sift::Test(mrcs, featsk);

    util::ImgMatchVector imvector;
    EX_TIME_BEGIN("\n%sDo MatchMain", _WAVE)
    KeyMatch::MatchMain(featsk, &imvector);
    EX_TIME_END("Do MatchMain")
    featsk.Release();

    imvector.WriteVectorByFolder("imvector");
    imvector.Clear();
    imvector.ReadVectorByFolder("imvector");
    imvector.CoordinateTransform(featsk.Width(), featsk.Height());		//move axis to image centre
	if(param.rotation_angle < -0.01 || param.rotation_angle > 0.01){
		EX_TRACE("Do pre-rotation of series...\n")
		imvector.PreRotate(-DEG2RAD(param.rotation_angle));
	}
	
//     KeyMatch::Test(mrcs, imvector, 0.5);
    util::ImgMatchVector inlier_vec;
    util::ImgMatchVector outlier_vec;

    EX_TIME_BEGIN("\n%sDo RANSAC", _WAVE)
    Ransac::RansacMain(imvector, &inlier_vec, &outlier_vec);
    EX_TIME_END("Do RANSAC")

    inlier_vec.WriteVectorByFolder("inlier_vec");
    outlier_vec.WriteVectorByFolder("outlier_vec");
//     inlier_vec.ReadVectorByFolder("inlier_vec");
//     outlier_vec.ReadVectorByFolder("outlier_vec");

    imvector.Release();
    outlier_vec.Release();


    vector<float> angles;
    if(!util::ReadAnglesByName(param.inputangle, &angles)) {
        std::cout<<"can't open tilt angle file."<<endl;
        return -1;
    }

    util::TrackSpace trackspace;
    trackspace.Create(inlier_vec, angles);
    inlier_vec.Release();
//     trackspace.Create(imvector, angles);
    imvector.Release();

    std::vector<mx::camera_params> cameras;
    std::vector<v3_t> points;
    EX_TIME_BEGIN("\n%sDo BundleMain", _WAVE)
    BundleApp::BundleMain(trackspace, 0/*1024*//*featsk.Width()*/, 0/*1024*//*featsk.Height()*/, &cameras, &points);
    EX_TIME_END("Do BundleMain")

    std::vector<util::calibrated> calib_v;
	util::globalcalib gcalib;
    Interpreter::InterpretMain(cameras, points, angles, &calib_v, &gcalib);
    /** newstack -xform test.xf test.mrc newtest.mrc */
    util::PrintCalibrationAsImod(calib_v, gcalib, -DEG2RAD(param.rotation_angle), 1/featsk.Ratio(), param.outputxf, param.outputangle, "invalid.txt");
//     PrintCalibrationAsATOM(calib_v, 2, "calib.xf");
    mrcs.Close();

    return 0;
}

bool ReadAnglesByName(const char* name, std::vector<float>* angles)
{
    std::ifstream in(name);
    if(!in.good()) {
        return false;
    }

    while(in.good()) {
        float angle;
        in>>angle;
        if(in.fail()) {
            break;
        }
        angles->push_back(angle);
    }
    in.close();
    return true;
}
