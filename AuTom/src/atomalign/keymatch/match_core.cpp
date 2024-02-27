#include "match_core.h"
#include <vector>
#include <cmath>
#include <sys/stat.h>
#include <sys/types.h>
#include <cassert>
#include <unistd.h>
#include <fcntl.h>

#define MINFEATURENUM		120

void KeyMatch::MatchMain(util::FeatureStack &fstack, util::ImgMatchVector *imvector)
{
#define STEP_ARRAY_SIZE			3//3
    assert(STEP_ARRAY_SIZE < fstack.Size());

    imvector->Clear();
    int step_length[STEP_ARRAY_SIZE];
    for(int i = 0; i < STEP_ARRAY_SIZE; i++){
        step_length[i] = i + 1;
    }

    mthd::ForceMatch forcematch;
    forcematch.InitEngine(fstack.Size());
//     mthd::ANNTreeMatch treematch;
//     treematch.InitEngine(fstack.Size());
    int turn = 0;
    while(turn < STEP_ARRAY_SIZE){
        for(int i = 0; i + step_length[turn] < fstack.Size(); i++){
            int idx1 = i;
            int idx2 = i + step_length[turn];
            EX_TIME_BEGIN("#\nMatching Key Set (MRC[%d] & MRC[%d])", idx1, idx2)
            util::img_match &imatch = imvector->MallocNewMatch();
            imatch.idx1 = idx1;
            imatch.idx2 = idx2;
            forcematch.SetCashIndex(idx2);
            forcematch.MatchKeys(&(imatch.pairs), fstack.V(idx1), fstack.V(idx2), fstack.Width());
// 	    treematch.SetCashIndex(idx2);
// 	    treematch.MatchKeys(&(imatch.pairs), fstack.V(idx1), fstack.V(idx2));
            EX_TIME_END("MRC[%d] & MRC[%d]: %ld Pairs found", idx1, idx2, imatch.size())
        }
        turn++;
    }

    forcematch.DestoryEngine();
//     treematch.DestoryEngine();

    //to do ransac

}

void KeyMatch::DrawMatch(IplImage *canvas, const IplImage *img1,
                         const IplImage *img2, const std::vector<mthd::pair>& vpair)
{
    cvZero(canvas);
    cvSetImageROI(canvas, cvRect(0, 0, img1->width, img1->height));
    cvAdd(img1, canvas, canvas, NULL);
    cvSetImageROI(canvas, cvRect(img1->width, 0, img1->width + img2->width, img2->height));
    cvAdd(img2, canvas, canvas, NULL);
    cvResetImageROI(canvas);
    for(int i = 0; i < vpair.size(); i++){
        CvScalar color = CV_RGB(255, 255, 255);
        util::DrawX(canvas, vpair[i].first.x, vpair[i].first.y);
        util::DrawX(canvas, img1->width + vpair[i].second.x, vpair[i].second.y);
    }
}

void KeyMatch::Test(util::MrcStack &mrcr, const util::ImgMatchVector &imvector, float ratio, const char *folder)
{
    EX_TIME_BEGIN("%sKeyMatch Testing", _DASH)

    IplImage *p[2], * canvas, *tmp;
    canvas = cvCreateImage(cvSize(2048, 1024), IPL_DEPTH_32F, 1);

    for(int i = 0; i < imvector.Size(); i++){
        tmp = mrcr.GetIplImage(imvector[i].idx1);
        p[0] = cvCreateImage(cvSize(tmp->width * ratio, tmp->height * ratio), IPL_DEPTH_32F, 1);
        cvResize(tmp, p[0], CV_INTER_CUBIC);
        util::ConvertTo1(p[0]);
        cvReleaseImage(&tmp);

        tmp = mrcr.GetIplImage(imvector[i].idx2);
        p[1] = cvCreateImage(cvSize(tmp->width * ratio, tmp->height * ratio), IPL_DEPTH_32F, 1);
        cvResize(tmp, p[1], CV_INTER_CUBIC);
        util::ConvertTo1(p[1]);
        cvReleaseImage(&tmp);

        DrawMatch(canvas, p[0], p[1], imvector[i].pairs);
        if(access(folder, 0) == -1){		//create file folder
            mkdir(folder, 0777);
        }
        std::ostringstream oss;
        oss << folder << "/" << "(" << imvector[i].idx1 << ")&(" << imvector[i].idx2 << ").pgm";
        try {
            util::SaveImage(canvas, oss.str().c_str());
        }
        catch(ex::Exception &e){
            EX_TRACE("%s\n", e.Msg())
        }
        cvReleaseImage(&p[0]);
        cvReleaseImage(&p[1]);
    }
    cvReleaseImage(&canvas);
    EX_TIME_END("KeyMatch Testing")
}

util::_point Mapping(const util::_point &ori, int width, int height, float angle)
{
    int half_width = width / 2;
    if(ori.x < half_width){
        return util::_point(half_width - (half_width - ori.x) * cos(angle / 180 * M_PI), ori.y);
    }
    else if(ori.x > half_width){
        return util::_point(half_width + (ori.x - half_width) * cos(angle / 180 * M_PI), ori.y);
    }
    else {
        return ori;
    }
}



