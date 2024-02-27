#ifndef MATCH_CORE_H__
#define MATCH_CORE_H__

#include "dataf/keypoint.h"
#include "dataf/dataf.h"
#include "keymatch/method.h"
#include "mrcimg/mrc2img.h"
#include "mrcimg/img_util.h"
#include <cassert>
#include <vector>

#include "cxcore.h"
#include "cv.h"

class KeyMatch{
public:
    static void DrawMatch(IplImage* canvas, const IplImage* img1, 
			 const IplImage* img2, const std::vector<mthd::pair>& vpair);
    
    static void MatchMain(util::FeatureStack& fstack, util::ImgMatchVector* imvector);
    
    static void Test(util::MrcStack& mrcr, const util::ImgMatchVector& imvector, float ratio, const char* folder = "matchs");
};

util::_point Mapping(const util::_point& ori, int width, int height, float angle);

#endif