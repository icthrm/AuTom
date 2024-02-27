#include <vector>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <malloc.h>
#include "datatype.h"
#include "sift_core.h"
#include "mrcimg/img_util.h"
#include <unistd.h>
#include <fcntl.h>

Sift::Sift():dbl(0)
{

}

Sift::~Sift()
{

}

//CV_IMAGE_ELEM
static inline float pixval32f( IplImage* img, int r, int c )
{
    return ( (float*)(img->imageData + img->widthStep*r) )[c];
}

IplImage* Sift::CreateInitImg(IplImage* img, int img_dbl, double sigma)
{
    IplImage* gray,*dbl;
    double sig_diff;

    gray = cvCreateImage(cvGetSize(img), IPL_DEPTH_32F, 1);

    cvCopy(img, gray);
    if(img_dbl) {
        sig_diff = sqrt(sigma*sigma-SIFT_INIT_SIGMA*SIFT_INIT_SIGMA*4);
        dbl = cvCreateImage(cvSize(img->width*2, img->height*2),
                            IPL_DEPTH_32F, 1);
        cvResize(gray, dbl, CV_INTER_CUBIC);
        cvSmooth(dbl, dbl, CV_GAUSSIAN, 0, 0, sig_diff, sig_diff);
        cvReleaseImage(&gray);
        return dbl;
    }
    else {
        sig_diff = sqrt(sigma*sigma-SIFT_INIT_SIGMA*SIFT_INIT_SIGMA);
        cvSmooth(gray, gray, CV_GAUSSIAN, 0, 0, sig_diff, sig_diff);
        return gray;
    }
}

IplImage*** Sift::BuildGaussPyr(IplImage* base, int octvs, int intvls, double sigma)
{
    IplImage*** gauss_pyr;
    const int _intvls = intvls;
    double sig[_intvls+3], sig_total, sig_prev, k;

    gauss_pyr =(IplImage***)calloc(octvs, sizeof(IplImage**));

    for(int i = 0; i < octvs; i++) {
        gauss_pyr[i] =(IplImage**)calloc(intvls + 3, sizeof(IplImage *));
    }

    /*
      precompute Gaussian sigmas using the following formula:
      \sigma_{total}^2 = \sigma_{i}^2 + \sigma_{i-1}^2
    */
    sig[0] = sigma;
    k = pow(2.0, 1.0 / intvls);
    for(int i = 1; i < intvls + 3; i++) {
        sig_prev = pow(k, i-1)*sigma;
        sig_total = sig_prev*k;
        sig[i] = sqrt(sig_total*sig_total-sig_prev*sig_prev);
    }

    for(int o = 0; o < octvs; o++) {
        for(int i = 0; i < intvls + 3; i++) {
            if(o == 0  &&  i == 0) {
                gauss_pyr[o][i] = cvCloneImage(base);
            }
            /* base of new octvave is halved image from end of previous octave */
            else if(i == 0) {
                gauss_pyr[o][i] = DownSample(gauss_pyr[o-1][intvls]);
            }
            /* blur the current octave's last image to create the next one */
            else {
                gauss_pyr[o][i] = cvCreateImage(cvGetSize(gauss_pyr[o][i-1]), IPL_DEPTH_32F, 1);
                cvSmooth(gauss_pyr[o][i-1], gauss_pyr[o][i], CV_GAUSSIAN, 0, 0, sig[i], sig[i]);
            }
        }
    }

    return gauss_pyr;
}

IplImage* Sift::DownSample(IplImage* img)
{
    IplImage* smaller = cvCreateImage(cvSize(img->width/2, img->height/2),
                                      img->depth, img->nChannels);
    cvResize(img, smaller, CV_INTER_NN);

    return smaller;
}

IplImage*** Sift::BuildDogPyr(IplImage*** gauss_pyr, int octvs, int intvls)
{
    IplImage*** dog_pyr;

    dog_pyr =(IplImage***)calloc(octvs, sizeof(IplImage**));

    for(int i = 0; i < octvs; i++) {
        dog_pyr[i] =(IplImage**)calloc(intvls+2, sizeof(IplImage*));
    }

    for(int o = 0; o < octvs; o++) {
        for(int i = 0; i < intvls+2; i++) {
            dog_pyr[o][i] = cvCreateImage(cvGetSize(gauss_pyr[o][i]),
                                          IPL_DEPTH_32F, 1);
            cvSub(gauss_pyr[o][i+1], gauss_pyr[o][i], dog_pyr[o][i], NULL);
        }
    }

    return dog_pyr;
}

CvSeq* Sift::ScaleSpaceExtrema(IplImage*** dog_pyr, int octvs, int intvls,
                               double contr_thr, int curv_thr, CvMemStorage* storage)
{
    CvSeq* features;
    double prelim_contr_thr = 0.5*contr_thr/intvls;
    __feature* feat;
    detection_data* ddata;

    features = cvCreateSeq(0, sizeof(CvSeq), sizeof(__feature), storage);
    for(int o = 0; o < octvs; o++) {
        for(int i = 1; i <= intvls; i++) {
            for(int r = SIFT_IMG_BORDER; r < dog_pyr[o][0]->height-SIFT_IMG_BORDER; r++) {
                for(int c = SIFT_IMG_BORDER; c < dog_pyr[o][0]->width-SIFT_IMG_BORDER; c++) {
                    /* perform preliminary check on contrast */
                    if(ABS(pixval32f(dog_pyr[o][i], r, c)) > prelim_contr_thr) {
                        if(IsExtremum(dog_pyr, o, i, r, c)) {
                            feat = InterpExtremum(dog_pyr, o, i, r, c, intvls, contr_thr);
                            if(feat) {
                                ddata = feat_detection_data(feat);
                                if(! IsTooEdgeLike(dog_pyr[ddata->octv][ddata->intvl],
                                                   ddata->r, ddata->c, curv_thr)) {
                                    cvSeqPush(features, feat);
                                }
                                else {
                                    free(ddata);
                                }
                                free(feat);
                            }
                        }
                    }
                }
            }
        }
    }
    return features;
}

int Sift::IsExtremum(IplImage*** dog_pyr, int octv, int intvl, int r, int c)
{
    double val = pixval32f(dog_pyr[octv][intvl], r, c);

    if(val > 0) {	// check for maximum
        for(int i = -1; i <= 1; i++) {
            for(int j = -1; j <= 1; j++) {
                for(int k = -1; k <= 1; k++) {
                    if(val < pixval32f(dog_pyr[octv][intvl+i], r+j, c+k)) {
                        return 0;
                    }
                }
            }
        }
    }
    else {			// check for minimum
        for(int i = -1; i <= 1; i++) {
            for(int j = -1; j <= 1; j++) {
                for(int k = -1; k <= 1; k++) {
                    if(val > pixval32f(dog_pyr[octv][intvl+i], r+j, c+k)) {
                        return 0;
                    }
                }
            }
        }
    }

    return 1;
}

__feature* Sift::InterpExtremum(IplImage*** dog_pyr, int octv, int intvl, int r, int c, int intvls, double contr_thr)
{
    __feature* feat;
    detection_data* ddata;
    double xi, xr, xc, contr;
    int i = 0;

    while(i < SIFT_MAX_INTERP_STEPS) {
        InterpStep(dog_pyr, octv, intvl, r, c, &xi, &xr, &xc);
        if(ABS(xi) < 0.5  &&  ABS(xr) < 0.5  &&  ABS(xc) < 0.5) {
            break;
        }

        c += cvRound(xc);
        r += cvRound(xr);
        intvl += cvRound(xi);

        if(intvl < 1  || intvl > intvls  || c < SIFT_IMG_BORDER  || r < SIFT_IMG_BORDER  ||
                c >= dog_pyr[octv][0]->width-SIFT_IMG_BORDER  ||
                r >= dog_pyr[octv][0]->height-SIFT_IMG_BORDER)
        {
            return NULL;
        }

        i++;
    }

    /* ensure convergence of interpolation */
    if(i >= SIFT_MAX_INTERP_STEPS) {
        return NULL;
    }

    contr = InterpContr(dog_pyr, octv, intvl, r, c, xi, xr, xc);
    if(ABS(contr) < contr_thr/intvls) {
        return NULL;
    }

    feat = NewFeature();
    ddata = feat_detection_data(feat);
    feat->x =(c+xc)*pow(2.0, octv);
    feat->y =(r+xr)*pow(2.0, octv);
    ddata->r = r;
    ddata->c = c;
    ddata->octv = octv;
    ddata->intvl = intvl;
    ddata->subintvl = xi;

    return feat;
}

void Sift::InterpStep(IplImage*** dog_pyr, int octv, int intvl, int r, int c, double* xi, double* xr, double* xc)
{
    CvMat* dD,*H,*H_inv, X;
    double x[3] = { 0 };

    dD = Deriv3D(dog_pyr, octv, intvl, r, c);
    H = Hessian3D(dog_pyr, octv, intvl, r, c);
    H_inv = cvCreateMat(3, 3, CV_64FC1);
    cvInvert(H, H_inv, CV_SVD);
    cvInitMatHeader(&X, 3, 1, CV_64FC1, x, CV_AUTOSTEP);
    cvGEMM(H_inv, dD, -1, NULL, 0, &X, 0);

    cvReleaseMat(&dD);
    cvReleaseMat(&H);
    cvReleaseMat(&H_inv);

    *xi = x[2];
    *xr = x[1];
    *xc = x[0];
}

CvMat* Sift::Deriv3D(IplImage*** dog_pyr, int octv, int intvl, int r, int c)
{
    CvMat* dI;
    double dx, dy, ds;

    dx =(pixval32f(dog_pyr[octv][intvl], r, c+1) -
         pixval32f(dog_pyr[octv][intvl], r, c-1))/2.0;
    dy =(pixval32f(dog_pyr[octv][intvl], r+1, c) -
         pixval32f(dog_pyr[octv][intvl], r-1, c))/2.0;
    ds =(pixval32f(dog_pyr[octv][intvl+1], r, c) -
         pixval32f(dog_pyr[octv][intvl-1], r, c))/2.0;

    dI = cvCreateMat(3, 1, CV_64FC1);
    cvmSet(dI, 0, 0, dx);
    cvmSet(dI, 1, 0, dy);
    cvmSet(dI, 2, 0, ds);

    return dI;
}

CvMat* Sift::Hessian3D(IplImage*** dog_pyr, int octv, int intvl, int r, int c)
{
    CvMat* H;
    double v, dxx, dyy, dss, dxy, dxs, dys;

    v = pixval32f(dog_pyr[octv][intvl], r, c);
    dxx =(pixval32f(dog_pyr[octv][intvl], r, c+1) +
          pixval32f(dog_pyr[octv][intvl], r, c-1)-2*v);
    dyy =(pixval32f(dog_pyr[octv][intvl], r+1, c) +
          pixval32f(dog_pyr[octv][intvl], r-1, c)-2*v);
    dss =(pixval32f(dog_pyr[octv][intvl+1], r, c) +
          pixval32f(dog_pyr[octv][intvl-1], r, c)-2*v);
    dxy =(pixval32f(dog_pyr[octv][intvl], r+1, c+1) -
          pixval32f(dog_pyr[octv][intvl], r+1, c-1) -
          pixval32f(dog_pyr[octv][intvl], r-1, c+1) +
          pixval32f(dog_pyr[octv][intvl], r-1, c-1))/4.0;
    dxs =(pixval32f(dog_pyr[octv][intvl+1], r, c+1) -
          pixval32f(dog_pyr[octv][intvl+1], r, c-1) -
          pixval32f(dog_pyr[octv][intvl-1], r, c+1) +
          pixval32f(dog_pyr[octv][intvl-1], r, c-1))/4.0;
    dys =(pixval32f(dog_pyr[octv][intvl+1], r+1, c) -
          pixval32f(dog_pyr[octv][intvl+1], r-1, c) -
          pixval32f(dog_pyr[octv][intvl-1], r+1, c) +
          pixval32f(dog_pyr[octv][intvl-1], r-1, c))/4.0;

    H = cvCreateMat(3, 3, CV_64FC1);
    cvmSet(H, 0, 0, dxx);
    cvmSet(H, 0, 1, dxy);
    cvmSet(H, 0, 2, dxs);
    cvmSet(H, 1, 0, dxy);
    cvmSet(H, 1, 1, dyy);
    cvmSet(H, 1, 2, dys);
    cvmSet(H, 2, 0, dxs);
    cvmSet(H, 2, 1, dys);
    cvmSet(H, 2, 2, dss);

    return H;
}

double Sift::InterpContr(IplImage*** dog_pyr, int octv, int intvl, int r, int c, double xi, double xr, double xc)
{
    CvMat* dD, X, T;
    double t[1], x[3] = { xc, xr, xi };

    cvInitMatHeader(&X, 3, 1, CV_64FC1, x, CV_AUTOSTEP);
    cvInitMatHeader(&T, 1, 1, CV_64FC1, t, CV_AUTOSTEP);
    dD = Deriv3D(dog_pyr, octv, intvl, r, c);
    cvGEMM(dD, &X, 1, NULL, 0, &T,  CV_GEMM_A_T);
    cvReleaseMat(&dD);

    return pixval32f(dog_pyr[octv][intvl], r, c)+t[0]*0.5;
}

__feature* Sift::NewFeature(void )
{
    __feature* feat;
    detection_data* ddata;

    feat =(__feature*)malloc(sizeof(__feature));
    memset(feat, 0, sizeof(__feature));
    ddata =(detection_data*)malloc(sizeof(detection_data));
    memset(ddata, 0, sizeof(detection_data));
    feat->feature_data = ddata;

    return feat;
}

int Sift::IsTooEdgeLike(IplImage* dog_img, int r, int c, int curv_thr)
{
    double d, dxx, dyy, dxy, tr, det;

    /* principal curvatures are computed using the trace and det of Hessian */
    d = pixval32f(dog_img, r, c);
    dxx = pixval32f(dog_img, r, c+1)+pixval32f(dog_img, r, c-1)-2*d;
    dyy = pixval32f(dog_img, r+1, c)+pixval32f(dog_img, r-1, c)-2*d;
    dxy =(pixval32f(dog_img, r+1, c+1)-pixval32f(dog_img, r+1, c-1) -
          pixval32f(dog_img, r-1, c+1)+pixval32f(dog_img, r-1, c-1))/4.0;
    tr = dxx+dyy;
    det = dxx*dyy-dxy*dxy;

    /* negative determinant -> curvatures have different signs; reject feature */
    if(det <= 0) {
        return 1;
    }

    if(tr*tr/det <(curv_thr+1.0)*(curv_thr+1.0)/curv_thr) {
        return 0;
    }

    return 1;
}

void Sift::CalcFeatureScales(CvSeq* features, double sigma, int intvls)
{
    __feature* feat;
    detection_data* ddata;
    double intvl;

    int n = features->total;
    for(int i = 0; i < n; i++) {
        feat = CV_GET_SEQ_ELEM(__feature, features, i);
        ddata = feat_detection_data(feat);
        intvl = ddata->intvl+ddata->subintvl;
        feat->scl = sigma*pow(2.0, ddata->octv+intvl/intvls);
        ddata->scl_octv = sigma*pow(2.0, intvl/intvls);
    }
}

void Sift::AdjustForImgDbl(CvSeq* features)
{
    __feature* feat;

    int n = features->total;
    for(int i = 0; i < n; i++) {
        feat = CV_GET_SEQ_ELEM(__feature, features, i);
        feat->x /= 2.0;
        feat->y /= 2.0;
        feat->scl /= 2.0;
    }
}

void Sift::CalcFeatureOris(CvSeq* features, IplImage*** gauss_pyr)
{
    __feature* feat;
    detection_data* ddata;
    double* hist;
    double omax;

    int n = features->total;
    for(int i = 0; i < n; i++) {
        feat =(__feature*)malloc(sizeof(__feature));
        cvSeqPopFront(features, feat);
        ddata = feat_detection_data(feat);
        hist = OriHist(gauss_pyr[ddata->octv][ddata->intvl], ddata->r, ddata->c, SIFT_ORI_HIST_BINS,
                       cvRound(SIFT_ORI_RADIUS*ddata->scl_octv), SIFT_ORI_SIG_FCTR*ddata->scl_octv);
        for(int j = 0; j < SIFT_ORI_SMOOTH_PASSES; j++) {
            SmoothOriHist(hist, SIFT_ORI_HIST_BINS);
        }
        omax = DominantOri(hist, SIFT_ORI_HIST_BINS);
        AddGoodOriFeatures(features, hist, SIFT_ORI_HIST_BINS, omax*SIFT_ORI_PEAK_RATIO, feat);
        free(ddata);
        free(feat);
        free(hist);
    }
}

double* Sift::OriHist(IplImage* img, int r, int c, int n, int rad, double sigma)
{
    double* hist;
    double mag, ori, w, exp_denom, PI2 = CV_PI*2.0;
    int bin;

    hist =(double*)calloc(n, sizeof(double));
    exp_denom = 2.0*sigma*sigma;
    for(int i = -rad; i <= rad; i++) {
        for(int j = -rad; j <= rad; j++) {
            if(CalcGradMagOri(img, r+i, c+j, &mag, &ori)) {
                w = exp(-(i*i+j*j)/exp_denom);
                bin = cvRound(n *(ori+CV_PI)/PI2);
                bin =(bin < n)? bin : 0;
                hist[bin] += w*mag;
            }
        }
    }

    return hist;
}

int Sift::CalcGradMagOri(IplImage* img, int r, int c, double* mag, double* ori)
{
    double dx, dy;

    if(r > 0  &&  r < img->height-1  &&  c > 0  &&  c < img->width-1) {
        dx = pixval32f(img, r, c+1)-pixval32f(img, r, c-1);
        dy = pixval32f(img, r-1, c)-pixval32f(img, r+1, c);
        *mag = sqrt(dx*dx+dy*dy);
        *ori = atan2(dy, dx);
        return 1;
    }

    else {
        return 0;
    }
}

void Sift::SmoothOriHist(double* hist, int n)
{
    double prev, tmp, h0 = hist[0];

    prev = hist[n-1];
    for(int i = 0; i < n; i++) {
        tmp = hist[i];
        hist[i] = 0.25*prev+0.5*hist[i] +
                  0.25 *((i+1 == n)? h0 : hist[i+1]);
        prev = tmp;
    }
}

double Sift::DominantOri(double* hist, int n)
{
    double omax = hist[0];
    int maxbin = 0;

    for(int i = 1; i < n; i++) {
        if(hist[i] > omax) {
            omax = hist[i];
            maxbin = i;
        }
    }
    return omax;
}

#define interp_hist_peak(l, c, r)(0.5 *((l)-(r)) /((l)-2.0*(c) +(r)))

void Sift::AddGoodOriFeatures(CvSeq* features, double* hist, int n, double mag_thr, __feature* feat)
{
    __feature* new_feat;
    double bin, PI2 = CV_PI*2.0;
    int l, r;

    for(int i = 0; i < n; i++) {
        l =(i == 0)? n-1 : i-1;
        r =(i+1) % n;

        if(hist[i] > hist[l]  &&  hist[i] > hist[r]  &&  hist[i] >= mag_thr) {
            bin = i+interp_hist_peak(hist[l], hist[i], hist[r]);
            bin =(bin < 0)? n+bin :(bin >= n)? bin-n : bin;
            new_feat = CloneFeature(feat);
            new_feat->ori =((PI2*bin)/n)-CV_PI;
            cvSeqPush(features, new_feat);
            free(new_feat);
        }
    }
}

__feature* Sift::CloneFeature(__feature* feat)
{
    __feature* new_feat;
    detection_data* ddata;

    new_feat = NewFeature();
    ddata = feat_detection_data(new_feat);
    memcpy(new_feat, feat, sizeof(__feature));
    memcpy(ddata, feat_detection_data(feat), sizeof(detection_data));
    new_feat->feature_data = ddata;

    return new_feat;
}

void Sift::ComputeDescriptors(CvSeq* features, IplImage*** gauss_pyr, int d, int n)
{
    __feature* feat;
    detection_data* ddata;
    double*** hist;
    int i, k = features->total;

    for(i = 0; i < k; i++) {
        feat = CV_GET_SEQ_ELEM(__feature, features, i);
        ddata = feat_detection_data(feat);
        hist = DescrHist(gauss_pyr[ddata->octv][ddata->intvl], ddata->r,
                         ddata->c, feat->ori, ddata->scl_octv, d, n);
        HistToDescr(hist, d, n, feat);
        ReleaseDescrHist(&hist, d);
    }
}

double*** Sift::DescrHist(IplImage* img, int r, int c, double ori, double scl, int d, int n)
{
    double*** hist;
    double cos_t, sin_t, hist_width, exp_denom, r_rot, c_rot, grad_mag,
           grad_ori, w, rbin, cbin, obin, bins_per_rad, PI2 = 2.0*CV_PI;
    int radius;

    hist =(double***)calloc(d, sizeof(double**));
    for(int i = 0; i < d; i++) {
        hist[i] =(double**)calloc(d, sizeof(double*));
        for(int j = 0; j < d; j++) {
            hist[i][j] =(double*)calloc(n, sizeof(double));
        }
    }

    cos_t = cos(ori);
    sin_t = sin(ori);
    bins_per_rad = n/PI2;
    exp_denom = d*d*0.5;
    hist_width = SIFT_DESCR_SCL_FCTR*scl;
    radius = hist_width*sqrt(2) *(d+1.0)*0.5+0.5;
    for(int i = -radius; i <= radius; i++) {
        for(int j = -radius; j <= radius; j++) {
            /*
              Calculate sample's histogram array coords rotated relative to ori.
              Subtract 0.5 so samples that fall e.g. in the center of row 1(i.e.
              r_rot = 1.5) have full weight placed in row 1 after interpolation.
            */
            c_rot =(j*cos_t-i*sin_t)/hist_width;
            r_rot =(j*sin_t+i*cos_t)/hist_width;
            rbin = r_rot+d/2-0.5;
            cbin = c_rot+d/2-0.5;

            if(rbin > -1.0  &&  rbin < d  &&  cbin > -1.0  &&  cbin < d) {
                if(CalcGradMagOri(img, r+i, c+j, &grad_mag, &grad_ori)) {
                    grad_ori -= ori;
                    while(grad_ori < 0.0) {
                        grad_ori += PI2;
                    }
                    while(grad_ori >= PI2) {
                        grad_ori -= PI2;
                    }

                    obin = grad_ori*bins_per_rad;
                    w = exp(-(c_rot*c_rot+r_rot*r_rot)/exp_denom);
                    InterpHistEntry(hist, rbin, cbin, obin, grad_mag*w, d, n);
                }
            }
        }
    }

    return hist;
}

void Sift::InterpHistEntry(double*** hist, double rbin, double cbin, double obin, double mag, int d, int n)
{
    double d_r, d_c, d_o, v_r, v_c, v_o;
    double** row, *h;
    int r0, c0, o0, rb, cb, ob;

    r0 = cvFloor(rbin);
    c0 = cvFloor(cbin);
    o0 = cvFloor(obin);
    d_r = rbin-r0;
    d_c = cbin-c0;
    d_o = obin-o0;

    /*
      The entry is distributed into up to 8 bins.  Each entry into a bin
      is multiplied by a weight of 1-d for each dimension, where d is the
      distance from the center value of the bin measured in bin units.
    */
    for(int r = 0; r <= 1; r++) {
        rb = r0+r;
        if(rb >= 0  &&  rb < d) {
            v_r = mag *((r == 0)? 1.0-d_r : d_r);
            row = hist[rb];
            for(int c = 0; c <= 1; c++) {
                cb = c0+c;
                if(cb >= 0  &&  cb < d) {
                    v_c = v_r *((c == 0)? 1.0-d_c : d_c);
                    h = row[cb];
                    for(int o = 0; o <= 1; o++) {
                        ob =(o0+o) % n;
                        v_o = v_c *((o == 0)? 1.0-d_o : d_o);
                        h[ob] += v_o;
                    }
                }
            }
        }
    }
}

void Sift::HistToDescr(double*** hist, int d, int n, __feature* feat)
{
    int k = 0;

    for(int r = 0; r < d; r++) {
        for(int c = 0; c < d; c++) {
            for(int o = 0; o < n; o++) {
                feat->descr[k++] = hist[r][c][o];
            }
        }
    }

    feat->d = k;
    NormalizeDescr(feat);
    for(int i = 0; i < k; i++) {
        if(feat->descr[i] > SIFT_DESCR_MAG_THR) {
            feat->descr[i] = SIFT_DESCR_MAG_THR;
        }
    }
    NormalizeDescr(feat);

    /* convert floating-point descriptor to integer valued descriptor */
    for(int i = 0; i < k; i++) {
        int int_val = SIFT_INT_DESCR_FCTR*feat->descr[i];
        feat->descr[i] = MIN(255, int_val);
    }
}

void Sift::NormalizeDescr(__feature* feat)
{
    double cur, len_inv, len_sq = 0.0;
    int d = feat->d;

    for(int i = 0; i < d; i++) {
        cur = feat->descr[i];
        len_sq += cur*cur;
    }
    len_inv = 1.0/sqrt(len_sq);
    for(int i = 0; i < d; i++) {
        feat->descr[i] *= len_inv;
    }
}

int Sift::FeatureCmp(void* feat1, void* feat2, void* param)
{
    __feature* f1 =(__feature*) feat1;
    __feature* f2 =(__feature*) feat2;

    if(f1->scl < f2->scl) {
        return 1;
    }
    if(f1->scl > f2->scl) {
        return -1;
    }
    return 0;
}

void Sift::ReleaseDescrHist(double**** hist, int d)
{
    for(int i = 0; i < d; i++) {
        for(int j = 0; j < d; j++) {
            free((*hist)[i][j]);
        }
        free((*hist)[i]);
    }
    free(*hist);
    *hist = NULL;
}

void Sift::ReleasePyr(IplImage**** pyr, int octvs, int n)
{
    for(int i = 0; i < octvs; i++) {
        for(int j = 0; j < n; j++) {
            cvReleaseImage(&(*pyr)[i][j]);
        }
        free((*pyr)[i]);
    }
    free(*pyr);
    *pyr = NULL;
}

int Sift::ExtractFeatures(IplImage* img, std::vector<util::feature>& feats, int img_dbl)
{
    IplImage* init_img;
    IplImage*** gauss_pyr, *** dog_pyr;
    CvMemStorage* storage;
    CvSeq* features;
    int octvs, n = 0;

    /* build scale space pyramid; smallest dimension of top level is ~4 pixels */
    init_img = CreateInitImg(img, img_dbl, sigma);
    octvs = log(MIN(init_img->width, init_img->height))/log(2) - 2;
//     if(octvs > 3){octvs = 3;}
    gauss_pyr = BuildGaussPyr(init_img, octvs, intvls, sigma);
    dog_pyr = BuildDogPyr(gauss_pyr, octvs, intvls);

    storage = cvCreateMemStorage(0);
    features = ScaleSpaceExtrema(dog_pyr, octvs, intvls, contr_thr, curv_thr, storage);

    CalcFeatureScales(features, sigma, intvls);
    if(img_dbl) {
        AdjustForImgDbl(features);
    }
    CalcFeatureOris(features, gauss_pyr);
    ComputeDescriptors(features, gauss_pyr, descr_width, descr_hist_bins);

    /* sort features by decreasing scale and move from CvSeq to array */
    cvSeqSort(features,(CvCmpFunc)FeatureCmp, NULL);
    n = features->total;

    __feature basefeat;
    CvSeqReader reader;
    feats.clear();
    cvStartReadSeq(features,&reader,0);
    for(int i = 0; i<features->total; i++) {
        util::feature tmp;
        CV_READ_SEQ_ELEM(basefeat,reader);
        FeatureConverse(basefeat, &tmp);
        feats.push_back(tmp);
    }

    cvReleaseMemStorage(&storage);
    cvReleaseImage(&init_img);
    ReleasePyr(&gauss_pyr, octvs, intvls+3);
    ReleasePyr(&dog_pyr, octvs, intvls+2);
    return n;
}

void Sift::FeatureConverse(const __feature& basefeat, util::feature* feat)
{
    feat->kpt.x = basefeat.x;
    feat->kpt.y = basefeat.y;
    feat->kpt.orient = basefeat.ori;
    feat->kpt.sigma = basefeat.scl;
    feat->d = basefeat.d;
    for(int i = 0; i < feat->d; i++) {
        feat->descr[i] = basefeat.descr[i];
    }
}


void Sift::Process(IplImage* img, std::vector<util::feature>& features, float ratio)
{
    EX_TIME_BEGIN("\nCompute Keypoint:X Y Orientation and Descpritor")

    IplImage* init;
    if(ratio != 1) {
        init = cvCreateImage(cvSize(img->width*ratio, img->height*ratio), IPL_DEPTH_32F, 1);
        cvResize(img, init, CV_INTER_CUBIC);
    }
    else {
        init = img;
    }

    if(init->width <= 512 /*1024*/) {
        dbl = SIFT_IMG_DBL;
    }
    else {
        dbl = 0;
    }

    int n = ExtractFeatures(init, features, dbl);

    if(ratio != 1) {
        cvReleaseImage(&init);
    }

    EX_TIME_END("Totally found %ld features", features.size())
}

#include <sstream>

int Sift::SiftMain(util::MrcStack& mrcr, util::FeatureStack* featsk)
{
#define fw   1024 	//2048			//feasible width
    int m_width, m_height;

    m_width = mrcr.Width()<fw?mrcr.Width():fw;
    m_height = mrcr.Height()<fw?mrcr.Height():fw;

    Sift sift;
    featsk->ReSize(mrcr.Size());
    featsk->SetWxH(m_width, m_height);

    float ratio = m_width/(float)mrcr.Width();
    featsk->SetRatio(ratio);

    for(int i = 0; i < mrcr.Size(); i++) {
        EX_TIME_BEGIN("%sProcessing MRC[%d]", _DASH, i)
        IplImage* slice = mrcr.GetIplImage(i);
        util::ConvertTo1(slice);
        util::MedianSmooth(slice);
        util::HistogramStretch(slice);

// 	std::stringstream ss;
// 	ss<<"histo"<<i<<".pgm";
// 	util::SaveImage(slice, ss.str().c_str());

        std::vector<util::feature>& features = featsk->V(i);
        sift.Process(slice, features, ratio);
        cvReleaseImage(&slice);

        EX_TIME_END("Processing MRC[%d]", i)
    }

    return ratio;
}

void Sift::Test(util::MrcStack& mrcr, const util::FeatureStack& featsk, const char* folder)
{
    EX_TIME_BEGIN("%sSift Testing", _DASH)

    for(int i = 0; i < mrcr.Size(); i++) {
        IplImage* slice = mrcr.GetIplImage(i);

        IplImage* init = cvCreateImage(cvSize(slice->width*featsk.Ratio(), slice->height*featsk.Ratio()), IPL_DEPTH_32F, 1);
        cvResize(slice, init, CV_INTER_CUBIC);
        cvReleaseImage(&slice);
        slice = init;
        util::ConvertTo1(slice);

        const std::vector<util::feature>& features = featsk.V(i);

        util::DrawFeatures(slice, features);

        if(access(folder,0) == -1) {		//create file folder
            mkdir(folder,0777);
        }
        std::ostringstream oss;
        oss <<folder<<"/"<<mrcr.Name()<<"("<<i<<").pgm";
        try {
            util::SaveImage(slice, oss.str().c_str());
        } catch(ex::Exception& e) {
            EX_TRACE("%s\n", e.Msg())
        }
        cvReleaseImage(&slice);
    }
    EX_TIME_END("Sift Testing")
}
