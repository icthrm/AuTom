#ifndef SIFT_CORE_H__
#define SIFT_CORE_H__

#include "mrcimg/mrc2img.h"
#include "util/exception.h"
#include "dataf/keypoint.h"
#include "dataf/dataf.h"
#include "datatype.h"
#include "macro.h"
#include "cxcore.hpp"
#include "cv.hpp"


#ifndef ABS
#define ABS(x) ( ( (x) < 0 )? -(x) : (x) )
#endif


class Sift {
private:
    const static int intvls = SIFT_INTVLS;
    constexpr  const static double sigma = SIFT_SIGMA;
    constexpr  const static double contr_thr= SIFT_CONTR_THR;
    const static int curv_thr = SIFT_CURV_THR;
    int dbl /*SIFT_IMG_DBL*/;
    const static int descr_width = SIFT_DESCR_WIDTH;
    const static int descr_hist_bins = SIFT_DESCR_HIST_BINS;

public:
    Sift();
    ~Sift();

private:
    IplImage* CreateInitImg(IplImage* img, int img_dbl, double sigma);

    IplImage*** BuildGaussPyr(IplImage* base, int octvs, int intvls, double sigma);

    IplImage* DownSample(IplImage* img);

    IplImage*** BuildDogPyr(IplImage*** gauss_pyr, int octvs, int intvls);

    void ReleasePyr(IplImage**** pyr, int octvs, int n);

private:
    CvSeq* ScaleSpaceExtrema(IplImage*** dog_pyr, int octvs, int intvls,
                             double contr_thr, int curv_thr, CvMemStorage* storage);

    int IsExtremum(IplImage*** dog_pyr, int octv, int intvl, int r, int c);

    __feature* InterpExtremum(IplImage*** dog_pyr, int octv, int intvl,
                              int r, int c, int intvls, double contr_thr);

    void InterpStep(IplImage*** dog_pyr, int octv, int intvl, int r, int c,
                    double* xi, double* xr, double* xc);

    CvMat* Deriv3D(IplImage*** dog_pyr, int octv, int intvl, int r, int c);

    CvMat* Hessian3D(IplImage*** dog_pyr, int octv, int intvl, int r, int c);

    double InterpContr(IplImage*** dog_pyr, int octv, int intvl, int r, int c, double xi, double xr, double xc);

    __feature* NewFeature(void);

    int IsTooEdgeLike(IplImage* dog_img, int r, int c, int curv_thr);

    void CalcFeatureScales(CvSeq* features, double sigma, int intvls);

    void AdjustForImgDbl(CvSeq* features);

    void CalcFeatureOris(CvSeq* features, IplImage*** gauss_pyr);

    double* OriHist(IplImage* img, int r, int c, int n, int rad, double sigma);

    int CalcGradMagOri(IplImage* img, int r, int c, double* mag, double* ori);

    void SmoothOriHist(double* hist, int n);

    double DominantOri(double* hist, int n);

    void AddGoodOriFeatures(CvSeq* features, double* hist, int n, double mag_thr, __feature* feat);

    __feature* CloneFeature(__feature* feat);

    void ComputeDescriptors(CvSeq* features, IplImage*** gauss_pyr, int d, int n);

    double*** DescrHist(IplImage* img, int r, int c, double ori, double scl, int d, int n);

    void InterpHistEntry(double*** hist, double rbin, double cbin, double obin, double mag, int d, int n);

    void HistToDescr(double*** hist, int d, int n, __feature* feat);

    void NormalizeDescr(__feature* feat);

    static int FeatureCmp(void* feat1, void* feat2, void* param);

    void ReleaseDescrHist(double**** hist, int d);

private:
    void FeatureConverse(const __feature& basefeat, util::feature* feat);
    int ExtractFeatures(IplImage* img, std::vector<util::feature>& feats, int img_dbl);

public:
    void Process(IplImage* img, std::vector<util::feature>& features, float ratio = 1);

public:
    static int SiftMain(util::MrcStack& mrcr, util::FeatureStack* featsk);

    static void Test(util::MrcStack& mrcr, const util::FeatureStack& featsk, const char* folder= "feats");
};

#endif
