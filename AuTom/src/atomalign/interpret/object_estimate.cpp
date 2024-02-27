#include "object_estimate.h"
#include <ctime>
#include <stdlib.h>
extern "C" {
#include <clapack.h>
#include <fblaswr.h>
#include <f2c.h>
#include <math.h>
}


static void print_matrix(const char* desc, int m, int n, double* a, int lda) {
    int i, j;
    printf( " %s\n", desc );
    for(i = 0; i < m; i++) {
        for(j = 0; j < n; j++) {
            printf( "  %6.6f", a[i+j*lda] );
        }
        printf( "\n" );
    }
}

bool util::LeastSquare3DPanel(const std::vector< v3_t >& pt_v, double abc[3])
{
    double* xy1_mx = new double[pt_v.size()*3];
    double* z_vec = new double[pt_v.size()];
    double xy1_inv[9];

    double A[9];

    for(int i = 0; i < pt_v.size(); i++) {
        xy1_mx[i] = Vx(pt_v[i]);
        xy1_mx[i+pt_v.size()] = Vy(pt_v[i]);
        xy1_mx[i+2*pt_v.size()] = 1;
        z_vec[i] = Vz(pt_v[i]);
    }

    int M = pt_v.size(), N = 3, NRHS = 1, lda = M, ldb = pt_v.size(), info, lwork, rank;
    double rcond = -1.0;		//Negative rcond means using default (machine precision) value
    double wkopt;
    double* work;

    int nlvl =  max(0, int(log2( min(M, N)/(25+1)))+1);
    int* iwork = new int[3*min(M,N)*nlvl+11*min(M,N)];
    double s[N];

    /* Query and allocate the optimal workspace */
    lwork = -1;
    dgelsd_(&M, &N, &NRHS, xy1_mx, &lda, z_vec, &ldb, s, &rcond, &rank, &wkopt, &lwork, iwork, &info);

    lwork = (int)wkopt;
    work = new double[lwork];
    dgelsd_(&M, &N, &NRHS, xy1_mx, &lda, z_vec, &ldb, s, &rcond, &rank, work, &lwork, iwork, &info);	// Solve the equations A*X = B

    if(info > 0) {		// Check for convergence
        printf("The algorithm computing SVD failed to converge;\n");
        return false;
    }

    delete [] iwork;
    delete [] work;

    memcpy(abc, z_vec, sizeof(double)*3);
    /* Print minimum norm solution */
//     print_matrix( "Minimum norm solution", N, NRHS, z_vec, ldb );
//     /* Print effective rank */
//     printf(" Effective rank = %6i\n", rank );
//     /* Print singular values */
//     print_matrix( "Singular values", 1, N, s, 1 );

    delete [] xy1_mx;
    delete [] z_vec;

    return true;
}

double util::PlanePointError(const v3_t& pt, double abc[3])
{
    return abs(abc[0]*Vx(pt)+abc[1]*Vy(pt)-Vz(pt)+abc[2])/sqrt(abc[0]*abc[0]+abc[1]*abc[1]+1);
}

/************************* Local Function Prototypes for RANSAC *************************/
static int CalcMinInliers(int n, int m, double p_badsupp, double p_badplane);
static inline double log_factorial(int);
static int* DrawRansacSampleIndex(bool* flags, int ori_size, int sam_size);
static void ExtractCorresPts(const std::vector<v3_t>& pt_v, int* idxs, int sam_size, std::vector<v3_t>* pts);
static int FindConsensus(const std::vector<v3_t>& pt_v, double abc[3], double err_tol, int** consensus);


bool util::RansacPlane(const std::vector<v3_t>& pt_v, int min_req, double p_badplane, double err_tol,
                       double abc[3], std::vector<v3_t>* inliers, std::vector<v3_t>* outliers, const std::vector<v3_t>* pts_const)
{
    int* sample, * consensus, * consensus_max = NULL;
    int consen_size, consen_min, consen_max = 0;

    std::vector<v3_t> pts;
    int ori_size = pt_v.size();
    bool* flags = new bool[ori_size];

    double p, in_frac = RANSAC_INLIER_FRAC_EST;
    int k = 0;
    bool result = true;

    if(ori_size < min_req) {
        EX_TRACE("Warning: not enough matches to compute plane(%d < %d)", ori_size, min_req)
        result = false;
        goto end;
    }

    srandom(time(NULL));

    consen_min = CalcMinInliers(ori_size, ori_size/3, RANSAC_PROB_BAD_SUPP, p_badplane);
    p = pow(1.0 - pow(in_frac, min_req), k);

    while(p > p_badplane) {
        sample = DrawRansacSampleIndex(flags, ori_size, min_req);

        ExtractCorresPts(pt_v, sample, min_req, &pts);

        if(pts_const) {
            for(int i = 0; i < pts_const->size(); i++) {
                pts.push_back((*pts_const)[i]);
            }
        }

        bool out = LeastSquare3DPanel(pts, abc);
        if(!out) {
            result = false;
            goto iteration_end;
        }

        consen_size = FindConsensus(pt_v, abc, err_tol, &consensus);

        if(consen_size > consen_max) {
            if(consensus_max) {
                delete [] consensus_max;
            }
            consensus_max = consensus;
            consen_max = consen_size;
            in_frac = (double)consen_max/ori_size;
        }
        else {
            delete [] consensus;
        }

iteration_end:
        delete [] sample;
        pts.clear();
        p = pow(1.0 - pow(in_frac, min_req), ++k);
    }

    /* calculate final transform based on best consensus set */
    if(consen_max >= consen_min) {
        ExtractCorresPts(pt_v, consensus_max, consen_max, &pts);
		if(pts_const) {
            for(int i = 0; i < pts_const->size(); i++) {
                pts.push_back((*pts_const)[i]);
            }
        }
        LeastSquare3DPanel(pts, abc);
        consen_size = FindConsensus(pt_v, abc, err_tol, &consensus);

        delete [] consensus_max;
		pts.clear();
        ExtractCorresPts(pt_v, consensus, consen_size, &pts);
		if(pts_const) {
            for(int i = 0; i < pts_const->size(); i++) {
                pts.push_back((*pts_const)[i]);
            }
        }
        LeastSquare3DPanel(pts, abc);

        if(inliers) {
            for(int i = 0; i < consen_size; i++) {
                inliers->push_back(pt_v[consensus[i]]);
            }
        }

        if(outliers) {
            int* ptr = consensus;
            int* end = consensus+consen_size;

            for(int i = 0; i < ori_size; i++) {
                if(ptr != end) {
                    if(i < *ptr) {
                        outliers->push_back(pt_v[i]);
                    }
                    else if(i == *ptr) {
                        ptr++;
                    }
                }
                else {
                    outliers->push_back(pt_v[i]);
                }
            }
        }
    }
    else if(consensus_max) {
        delete [] consensus_max;
    }

end:
    return result;
}


bool util::RansacVector(const std::vector<v3_t>& pt_v, int min_req, double p_bad, double err_tol, double abc[3], int* inlier_size)
{
#define CORRELATION_TILT(VX, ABC)  {double suma = 0, sumb = 0, sumc = 0;	for(int i = 0; i < (VX).size(); i++){	\
	suma+=Vx((VX)[i]);sumb+=Vy((VX)[i]);sumc+=Vz((VX)[i]);}\
	double mag = sqrt(suma*suma+sumb*sumb+sumc*sumc);\
	(ABC)[0]=suma/mag;(ABC)[1]=sumb/mag;(ABC)[2]=sumc/mag;}
	
#define CROSS_VEC(V1,V2)	((V1)[0]*(V2)[0]+(V1)[1]*(V2)[1]+(V1)[2]*(V2)[2])

#define FINDCONSENSUS(VX, ABC, ERR_TOL, CONSENSUS, SIZE)	{double err;	\
	int* consensus_idxs = new int[VX.size()];int index = 0;	\
	for(int i = 0; i < VX.size(); i++) {		\
        err = CROSS_VEC(VX[i].p, ABC);	\
        if(err >= err_tol) {		\
            consensus_idxs[index++] = i;		\
        }		\
    }		\
    CONSENSUS = consensus_idxs;		\
    SIZE = index;		\
}

    int* sample, * consensus, * consensus_max = NULL;
    int consen_size, consen_min, consen_max = 0;

    std::vector<v3_t> pts;
    int ori_size = pt_v.size();
    bool* flags = new bool[ori_size];

    double p, in_frac = RANSAC_INLIER_FRAC_EST;
    int k = 0;
    bool result = true;

    if(ori_size < min_req) {
        EX_TRACE("Warning: not enough matches to compute vector(%d < %d)", ori_size, min_req)
        result = false;
        goto end;
    }

    srandom(time(NULL));

    consen_min = CalcMinInliers(ori_size, ori_size/4, RANSAC_PROB_BAD_SUPP, p_bad);
    p = pow(1.0 - pow(in_frac, min_req), k);

    while(p > p_bad) {
        sample = DrawRansacSampleIndex(flags, ori_size, min_req);

        ExtractCorresPts(pt_v, sample, min_req, &pts);
		CORRELATION_TILT(pt_v, abc)

		FINDCONSENSUS(pt_v, abc, err_tol, consensus, consen_size)

        if(consen_size > consen_max) {
            if(consensus_max) {
                delete [] consensus_max;
            }
            consensus_max = consensus;
            consen_max = consen_size;
            in_frac = (double)consen_max/ori_size;
        }
        else {
            delete [] consensus;
        }

iteration_end:
        delete [] sample;
        pts.clear();
        p = pow(1.0 - pow(in_frac, min_req), ++k);
    }

    /* calculate final transform based on best consensus set */
    if(consen_max >= consen_min) {
        ExtractCorresPts(pt_v, consensus_max, consen_max, &pts);

        CORRELATION_TILT(pt_v, abc)
        FINDCONSENSUS(pt_v, abc, err_tol, consensus, consen_size)

        delete [] consensus_max;
		pts.clear();
        ExtractCorresPts(pt_v, consensus, consen_size, &pts);

        CORRELATION_TILT(pt_v, abc)
		*inlier_size = consen_size;
        
    }
    else if(consensus_max) {
        delete [] consensus_max;
    }

end:
    return result;
#undef CORRELATION_TILT
#undef CORRELATION_TILT
}


/**
  Calculates the minimum number of inliers as a function of the number of
  putative correspondences.  Based on equation (7) in

  Chum, O. and Matas, J.  Matching with PROSAC -- Progressive Sample Consensus.
  In <EM>Conference on Computer Vision and Pattern Recognition (CVPR)</EM>,
  (2005), pp. 220--226.

  @param n number of putative correspondences
  @param m min number of correspondences to compute the model in question
  @param p_badsupp prob. that a bad model is supported by a correspondence
  @param p_badplane desired prob. that the final transformation returned is bad

  @return Returns the minimum number of inliers required to guarantee, based
    on p_badsupp, that the probability that the final transformation returned
    by RANSAC is less than p_badplane
*/
int CalcMinInliers(int n, int m, double p_badsupp, double p_badplane)
{
    double pi, sum;
    int i, j;

    for(j = m+1; j <= n; j++) {
        sum = 0;
        for(i = j; i <= n; i++) {
            pi = (i-m) * log(p_badsupp) + (n-i+m) * log(1.0 - p_badsupp) +
                 log_factorial(n - m) - log_factorial(i - m) -
                 log_factorial(n - i);
            /*
             * Last three terms above are equivalent to log(n-m choose i-m)
             */
            sum += exp(pi);
            if(sum < p_badplane) {
                break;
            }
        }
        if(sum < p_badplane) {
            break;
        }
    }
    return j;
}

/** Calculates the natural log of the factorial of a number @param n number @return Returns log(n!) */
inline double log_factorial(int n)
{
    double f = 0;
    int i;

    for(i = 1; i <= n; i++)
        f += log(i);

    return f;
}

int* DrawRansacSampleIndex(bool* flags, int ori_size, int sam_size)
{
    memset(flags, 0, sizeof(bool)*ori_size);

    int* sam_idxs = new int[sam_size];
    int ranx;

    for(int i = 0; i < sam_size; i++) {
        do {
            ranx = random()%ori_size;
        } while(flags[ranx]);
        sam_idxs[i] = ranx;
        flags[ranx] = true;
    }

    return sam_idxs;
}

void ExtractCorresPts(const std::vector<v3_t>& pt_v, int* idxs, int sam_size, std::vector<v3_t>* pts)
{
    for(int i = 0; i < sam_size; i++) {
        pts->push_back(pt_v[idxs[i]]);
    }
}

/**For a given model and error function, finds a consensus from a set of feature correspondences.
  @param homog_xfer_err error function used to measure distance from M
  @param err_tol correspondences within this distance of M are added to the consensus set
  @param consensus output as an array of index in the pt_v set
  @return Returns the number of points in the consensus set
*/
int FindConsensus(const std::vector<v3_t>& pt_v, double abc[3], double err_tol, int** consensus)
{
    double err;

    int* consensus_idxs = new int[pt_v.size()];
    int index = 0;

    for(int i = 0; i < pt_v.size(); i++) {
        err = util::PlanePointError(pt_v[i], abc);
        if(err <= err_tol) {
            consensus_idxs[index++] = i;
        }
    }

    *consensus = consensus_idxs;
    return index;
}
