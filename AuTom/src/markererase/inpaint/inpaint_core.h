#ifndef INPAINT_CORE_H__
#define INPAINT_CORE_H__

#include "opencv/cv.h"
#include "dataf/dataf.h"
#include "mrcimg/mrc2img.h"

/** inpaint for the fiduical markers; cope with image with 0~1*/
class InpaintApp{
	
protected:
	InpaintApp();
	~InpaintApp();
	
private:
	bool RegionGrow(IplImage* img, const util::point2d& seed, float diameter, std::vector<CvPoint>& region, double& bg_avg);
	
protected:
	void ConstInpaint(IplImage* img, const std::vector<util::point2d>& fids, float dia, IplImage* cpy);
	void ConstInpaintByRegionGrow(IplImage* img, const std::vector<util::point2d>& fids, float dia, IplImage* cpy);
	
public:
	static void InpaintingMain(util::MrcStack& mrcs, util::FiducialStack& fidstk, float& diameter, const char* output);
	
};

#endif