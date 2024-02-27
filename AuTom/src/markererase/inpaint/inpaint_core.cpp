#include "inpaint_core.h"
#include "mrcimg/img_util.h"
#include <stack>

InpaintApp::InpaintApp(){}

InpaintApp::~InpaintApp(){}

bool InpaintApp::RegionGrow(IplImage* img, const util::point2d& seed, float diameter, std::vector<CvPoint>& region, double& bg_avg)
{
	int idia;
	CvRect roi;
	CvScalar pixel_avg1, pixel_std1, pixel_avg2, pixel_std2;
	
	region.clear();
	
	idia = int(diameter*3+.5);		//1.2533 = sqrt(pi/2)
	
	roi.x = int(seed.x - idia*.5+.5);
	roi.y = int(seed.y - idia*.5+.5);
	roi.width = idia;
	roi.height = idia;
	IplImage* sub = util::GetSubImage(img, roi);
	
	if(!sub){
		return false;
	}
	
	cvAvgSdv(sub, &pixel_avg1, &pixel_std1);
	
	cvReleaseImage(&sub);
	
	idia = int(diameter*1.2533+.5);		//1.2533 = sqrt(pi/2)
	roi.x = int(seed.x - idia*.5+.5);
	roi.y = int(seed.y - idia*.5+.5);
	roi.width = idia;
	roi.height = idia;
	sub = util::GetSubImage(img, roi);
	
	if(!sub){
		return false;
	}
	
	cvAvgSdv(sub, &pixel_avg2, &pixel_std2);
	
	cvReleaseImage(&sub);
	
	bg_avg = (pixel_avg1.val[0]*9-pixel_avg2.val[0])*.125;
	
	int seed_y = seed.y;
	int seed_x = seed.x;

//     double d = roi_avg+.3*pixel_std1.val[0];							//reversal
	double d = (bg_avg+pixel_avg2.val[0])*.5-.5*pixel_std1.val[0];
	
    std::stack<CvPoint> seedd;
    float* img_data = (float*)img->imageData;
    int step = img->widthStep;
    seedd.push(cvPoint(int(seed.x), int(seed.y)));

    int width = img->width;
    int height = img->height;
	
	float d2 = diameter*diameter*1.5;
	
    while(!seedd.empty()){
		CvPoint point = seedd.top();
        seedd.pop();
		
		if(!(point.x >= 0 && point.x < width-1 && point.y >= 0 && point.y < height-1)){
			continue;
		}
		
		if((point.x-seed.x)*(point.x-seed.x)+(point.y-seed.y)*(point.y-seed.y) > d2){
			continue;
		}
		
		((float*)(img->imageData + point.y*img->widthStep))[point.x] = bg_avg;
		region.push_back(point);
		
		float value = ((float*)(img->imageData + point.y*img->widthStep))[point.x-1];		//(x-1, y)
		if(value < d){
			seedd.push(cvPoint(point.x-1, point.y));
		}

		value = ((float*)(img->imageData + point.y*img->widthStep))[point.x+1];			//(x+1, y)
		if(value < d){
			seedd.push(cvPoint(point.x+1, point.y));
		}

		value = ((float*)(img->imageData + (point.y-1)*img->widthStep))[point.x];		//(x, y-1)
		if(value < d){
			seedd.push(cvPoint(point.x, point.y-1));
		}

		value = ((float*)(img->imageData + (point.y+1)*img->widthStep))[point.x];		//(x, y+1)
		if(value < d){
			seedd.push(cvPoint(point.x, point.y+1));
		}
    }
    
    int bound = int(diameter*.125+.5);
	
	std::vector<CvPoint> boundv;
	
	for(int i = 0; i < region.size(); i++){
		for(int j = -bound; j <= bound; j++){
			for(int k = -bound; k <= bound; k++){
				if(!(region[i].x+j >= 0 && region[i].x+j < width-1 && region[i].y+k >= 0 && region[i].y+k < height-1)){
					continue;
				}
				if(((float*)(img->imageData + (region[i].y+k)*img->widthStep))[region[i].x+j] == bg_avg ){
					continue;
				}
				((float*)(img->imageData + (region[i].y+k)*img->widthStep))[region[i].x+j] = bg_avg;
				
				boundv.push_back(cvPoint(region[i].x+j, region[i].y+k));
			}
		}
	}
	
	for(int i = 0; i < boundv.size(); i++){
		region.push_back(boundv[i]);
	}

    return true;
}

void InpaintApp::ConstInpaint(IplImage* img, const std::vector< util::point2d >& fids, float dia, IplImage* cpy)
{
	cvCopy(img, cpy);
	
	int r = dia*.6+2;
	for(int i = 0; i < fids.size(); i++){
		cvCircle(cpy, cvPoint((int)fids[i].x, (int)fids[i].y), r, cvScalar(0.5), -1); 
	}
}

void InpaintApp::ConstInpaintByRegionGrow(IplImage* img, const std::vector< util::point2d >& fids, float dia, IplImage* cpy)
{
	cvCopy(img, cpy);
	
	int r = dia*.6+2;
	for(int i = 0; i < fids.size(); i++){
		std::vector<CvPoint> region;
		double bg_avg;
		if(!RegionGrow(cpy, fids[i], dia, region, bg_avg)){
			cvCircle(cpy, cvPoint((int)fids[i].x, (int)fids[i].y), r, cvScalar(0.5), -1); 
		}
	}
}

void InpaintApp::InpaintingMain(util::MrcStack& mrcs, util::FiducialStack& fidstk, float& diameter, const char* output)
{
	util::MrcStack mrcscpy;
	
	mrcs.CopyToNewStack(mrcscpy);
	mrcscpy.SetName(output);
	mrcscpy.SetHeader(util::MrcStack::MODE_FLOAT, 0, 0.5, 1);
	mrcscpy.WriteHeaderToFile();
	
	InpaintApp ipapp;
	
	for(int i = 0; i < mrcs.Size(); i++){
		EX_TIME_BEGIN("Erasing fiducial markers in micrograph %d", i)
		IplImage* img = mrcs.GetIplImage(i);
		util::ConvertTo1(img);
		
		IplImage* cpy = cvCreateImage(cvGetSize(img), img->depth, img->nChannels);
		
		const std::vector<util::point2d>& fids = fidstk.V(i);
// 		ipapp.ConstInpaint(img, fids, diameter, cpy);
		ipapp.ConstInpaintByRegionGrow(img, fids, diameter, cpy);
		
		mrcscpy.AppendIplImageToFile(cpy);
		cvReleaseImage(&cpy);
		cvReleaseImage(&img);
		EX_TIME_END("Erasing finished for micrograph %d", i)
	}
	
	mrcscpy.Close();
}
