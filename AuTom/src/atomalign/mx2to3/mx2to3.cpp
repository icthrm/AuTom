#include "mx2to3.h"
#include "util/qsort.h"
#include "interpret/object_estimate.h"
extern "C" {
#include <clapack.h>
#include <fblaswr.h>
#include <f2c.h>
#include <math.h>
}

void mx2to3::RotCameraCoordinate(double angle, mx::camera_params* camparam)
{
    double tmp[9];
    double C[4];
    mx::MxCFromCameraParams(*camparam, C);
    double cos_rot = cos(angle);
    double sin_rot = sin(angle);
    double rot_mx[9] = {cos_rot, sin_rot, 0, -sin_rot, cos_rot, 0, 0, 0, 1};		//transposed

    matrix_product33(rot_mx, camparam->R.D(), tmp);
    memcpy(camparam->R.D(), tmp, sizeof(double)*9);

    matrix_product331(camparam->R.D(), C, camparam->T.D());
    camparam->T *= -1;
}

/** @brief abc[] is the direct of axis*/
void mx2to3::GlobalTiltAxisEstimate(const mx::camera_params* camparams, int num, double abc[3], int* inlier_size)
{
    std::vector<v3_t> pt_v;
    for(int i = 0; i < num; i++) {		//store direct as x z y
        v3_t tmp;
        Vx(tmp) = camparams[i].R.D(6);
        Vy(tmp) = camparams[i].R.D(8);
        Vz(tmp) = camparams[i].R.D(7);
        pt_v.push_back(tmp);
    }
    
    std::vector<v3_t> tmp_v = pt_v;
    for(int i = 0; i < 5*num; i++) {
		tmp_v.push_back(v3_new(0, 0, 0));
    }
    
    util::LeastSquare3DPanel(tmp_v, abc);
	tmp_v.clear();
	
	double* errs = new double[pt_v.size()];
    for(int i = 0; i < pt_v.size(); i++){
        errs[i] = util::PlanePointError(pt_v[i], abc);
    }
    double err_tol = kth_element(pt_v.size(), pt_v.size()*.95, errs);
    delete [] errs;
	
	std::vector<v3_t> pt_const;
	for(int i = 0; i < 5*num; i++) {
		pt_const.push_back(v3_new(0, 0, 0));
    }
	std::vector<v3_t> inliers;
	bool effective = util::RansacPlane(pt_v, 3, 0.01, err_tol/*ERR_TOL*/, abc, &inliers, NULL, &pt_const);
	*inlier_size = inliers.size();
	
    double norm = sqrt(abc[0]*abc[0]+abc[1]*abc[1]+1);
    abc[0] /= norm;
    abc[2] = abc[1]/norm;
    abc[1] = -1/norm;
    if(abc[1] < 0) {
        abc[0] *= -1;
        abc[1] *= -1;
        abc[2] *= -1;
    }
}

/** WARNING wrong model */
void mx2to3::GlobalTiltAxisEstimate2(const mx::camera_params* camparams, int num, double abc[3], int* inlier_size)
{
    std::vector<v3_t> pt_v;
    for(int i = 0; i < num; i++) {		//store direct as x z y
        v3_t tmp;
        Vx(tmp) = camparams[i].R.D(3);
        Vy(tmp) = camparams[i].R.D(4);
        Vz(tmp) = camparams[i].R.D(5);
        pt_v.push_back(tmp);
    }
#define CORRELATION_TILT(VX, ABC)  {double suma = 0, sumb = 0, sumc = 0;	for(int i = 0; i < (VX).size(); i++){	\
	suma+=Vx((VX)[i]);sumb+=Vy((VX)[i]);sumc+=Vz((VX)[i]);}\
	double mag = sqrt(suma*suma+sumb*sumb+sumc*sumc);\
	(ABC)[0]=suma/mag;(ABC)[1]=sumb/mag;(ABC)[2]=sumc/mag;}
	
#define CROSS_VEC(V1,V2)	((V1)[0]*(V2)[0]+(V1)[1]*(V2)[1]+(V1)[2]*(V2)[2])
    CORRELATION_TILT(pt_v, abc)
	
	double* crss_val = new double[pt_v.size()];
    for(int i = 0; i < pt_v.size(); i++){
        crss_val[i] = CROSS_VEC(pt_v[i].p, abc);
    }
    double err_tol = kth_element(pt_v.size(), pt_v.size()*.1, crss_val);
    delete [] crss_val;
	
	util::RansacVector(pt_v, 5, 0.01, err_tol, abc, inlier_size);
	
#undef CORRELATION_TILT
#undef CORRELATION_TILT
}

/** @brief if the image will rotate angle, then the R of camera will be R_result = R_T(angle)*R
 *	      |cos(a) -sin(a) 0|^T |r0 r1 r2|
 * 	R_r = |sin(a)  cos(a) 0|  *|r3 r4 r5|
 * 	      |0        0     1|   |r6 r7 r8|
 *  here, we just rotate R with rotation axis(r6, r7, r8);i.e. the image operation is opposite
 *  @WARNING the result are the image's real conditon; i.e. the revert should be @c -angle
 */
void mx2to3::DirectDotEstimateRot(const mx::camera_params& camparam, const double abc[3], double* angle)
{
    const double* r = camparam.R.D();
    *angle = atan(-(r[0]*abc[0]+r[1]*abc[1]+r[2]*abc[2])/(r[3]*abc[0]+r[4]*abc[1]+r[5]*abc[2]));
}

void mx2to3::DirectDotEstimateRot2(const mx::camera_params& camparam, const double abc[3], double* angle)
{
	const double* r = camparam.R.D();
   // *angle = -atan((r[3]*abc[0]+r[4]*abc[1]+r[5]*abc[2])/(r[0]*abc[0]+r[1]*abc[1]+r[2]*abc[2]));
	*angle = atan((r[3]*abc[0]+r[4]*abc[1]+r[5]*abc[2])/(r[0]*abc[0]+r[1]*abc[1]+r[2]*abc[2]));
}

/**  @WARNING the result are the image's real conditon; i.e. the revert should be @c -angle */
void mx2to3::HorizonEstimateRot(const mx::camera_params& camparam, double* angle)
{
    const double* r = camparam.R.D();
    *angle = atan(-r[1]/r[4]);
}

/** @brief estimate angle by cross product tilt axis and camera's x-axis */
void mx2to3::TiltVectorEstimate(const mx::camera_params& camparam, const double abc[3], double dir[3])
{
    const double* r = camparam.R.D();

    dir[0] = r[1]*abc[2]-r[2]*abc[1];
    dir[1] = r[2]*abc[0]-r[0]*abc[2];
    dir[2] = r[0]*abc[1]-r[1]*abc[0];
    if(dir[2] < 0) {
        dir[0]*=-1;
        dir[1]*=-1;
        dir[2]*=-1;
    }
}

float mx2to3::CompensativeScale(const mx::camera_params& camparam, const double abc[3])
{
    const double* r = camparam.R.D();
    double cosa = r[3]*abc[0]+r[4]*abc[1]+r[5]*abc[2];
    if(cosa < 0) {
        cosa *= -1;
    }
    return cosa;
}


/** @brief if a point is centre, then sum of the distance to every direction vector is minimum
 *      |r0 r1 r2|
 *  R = |r3 r4 r5|; (A,B,C) = (r6 r7 r8); camera positon (x0 y0 z0)
 * 	|r6 r7 r8|
 *  line formula: (x-x0)/A = (y-y0)/B = (z-z0)/C; the estimate point (m n k)
 *  dis^2  =   [(A^2-1)(m-x0)+AB(n-y0)+AC(k-z0)]^2
 *  	      +[AB(m-x0)+(B^2-1)(n-y0)+BC(k-z0)]^2
 * 	      +[AC(m-x0)+BC(n-y0)+(C^2-1)(k-z0)]^2
 *  f = sum((dis_i)^2/); min f            ->           derivative(f) = 0
 * deri(f,m) = (A^2-1)^2m+AB(A^2-1)n+AC(A^2-1)k+[-(A^2-1)x0-ABy0-ACz0](A^2-1)
 * 	      +(AB)^2m+(B^2-1)ABn+AB^2Ck+[-ABx0-(B^2-1)y0-BCz0]AB
 * 	      +(AC)^2m+ABC^2n+(C^2-1)ACk+[-ACx0-BCy0-(C^2-1)z0]AC
 * deri(f,n) = (A^2-1)ABm+(AB)^2n+A^2BCk+[-(A^2-1)x0-ABy0-ACz0]AB
 * 	      +AB(B^2-1)m+(B^2-1)^2n+BC(B^2-1)k+[-ABx0-(B^2-1)y0-BCz0](B^2-1)
 * 	      +ABC^2m+(BC)^2n+(C^2-1)BCk+[-ACx0-BCy0-(C^2-1)z0]BC
 * deri(f,k) = (A^2-1)ACm+A^2BCn+(AC)^2k+[-(A^2-1)x0-ABy0-ACz0]AC
 * 	      +AB^2Cm+(B^2-1)BCn+(BC)^2k+[-ABx0-(B^2-1)y0-BCz0]BC
 * 	      +AC(C^2-1)m+BC(C^2-1)n+(C^2-1)^2k+[-ACx0-BCy0-(C^2-1)z0](C^2-1)
 *
 * deri(f,m) = [(A^2-1)^2+(AB)^2+(AC)^2]m+[AB(A^2-1)+(B^2-1)AB+ABC^2]n+[AC(A^2-1)+AB^2C+(C^2-1)AC]k
 * 	      +[-(A^2-1)x0-ABy0-ACz0](A^2-1)+[-ABx0-(B^2-1)y0-BCz0]AB+[-ACx0-BCy0-(C^2-1)z0]AC = 0
 * deri(f,n) = [(A^2-1)AB+AB(B^2-1)+ABC^2]m+[(AB)^2+(B^2-1)^2+(BC)^2]n+[A^2BC+BC(B^2-1)+(C^2-1)BC]k
 * 	      +[-(A^2-1)x0-ABy0-ACz0]AB+[-ABx0-(B^2-1)y0-BCz0](B^2-1)+[-ACx0-BCy0-(C^2-1)z0]BC = 0
 * deri(f,k) = [(A^2-1)AC+AB^2C+AC(C^2-1)]m+[A^2BC+(B^2-1)BC+BC(C^2-1)]n+[(AC)^2+(BC)^2+(C^2-1)^2]k
 * 	      +[-(A^2-1)x0-ABy0-ACz0]AC+[-ABx0-(B^2-1)y0-BCz0]BC+[-ACx0-BCy0-(C^2-1)z0](C^2-1) = 0
 *
 * |(A^2-1)^2+(AB)^2+(AC)^2  AB(A^2-1)+(B^2-1)AB+ABC^2  AC(A^2-1)+AB^2C+(C^2-1)AC||m|
 * |(A^2-1)AB+AB(B^2-1)+ABC^2  (AB)^2+(B^2-1)^2+(BC)^2  A^2BC+BC(B^2-1)+(C^2-1)BC||n| =
 * |(A^2-1)AC+AB^2C+AC(C^2-1)  A^2BC+(B^2-1)BC+BC(C^2-1)  (AC)^2+(BC)^2+(C^2-1)^2||k|
 *
 *  |[(A^2-1)x0+ABy0+ACz0](A^2-1)+[ABx0+(B^2-1)y0+BCz0]AB+[ACx0+BCy0+(C^2-1)z0]AC|
 * =|[(A^2-1)x0+ABy0+ACz0]AB+[ABx0+(B^2-1)y0+BCz0](B^2-1)+[ACx0+BCy0+(C^2-1)z0]BC|
 *  |[(A^2-1)x0+ABy0+ACz0]AC+[ABx0+(B^2-1)y0+BCz0]BC+[ACx0+BCy0+(C^2-1)z0](C^2-1)|
 */
void mx2to3::DirectionVerticalEstimateCentre(const mx::camera_params* camparams, int num, v3_t* centre)
{
    double suma0, suma1, suma2, suma3, suma4, suma5, suma6, suma7, suma8;
    double sumb0, sumb1, sumb2;
    suma0 = suma1 = suma2 = suma3 = suma4 = suma5 = suma6 = suma7 = suma8 = sumb0 = sumb1 = sumb2 = 0;
    for(int i = 0; i < num; i++) {
        double A = camparams[i].R.D(6);
        double B = camparams[i].R.D(7);
        double C = camparams[i].R.D(8);
        double campos[4];
        mx::MxCFromCameraParams(camparams[i], campos);
        double x0 = campos[0];
        double y0 = campos[1];
        double z0 = campos[2];
        suma0 += (A*A-1)*(A*A-1)+A*A*B*B+A*A*C*C;
        suma1 += A*B*(A*A-1)+(B*B-1)*A*B+A*B*C*C;
        suma2 += A*C*(A*A-1)+A*B*B*C+(C*C-1)*A*C;
        suma3 += (A*A-1)*A*B+A*B*(B*B-1)+A*B*C*C;
        suma4 += A*A*B*B+(B*B-1)*(B*B-1)+B*B*C*C;
        suma5 += A*A*B*C+B*C*(B*B-1)+(C*C-1)*B*C;
        suma6 += (A*A-1)*A*C+A*B*B*C+A*C*(C*C-1);
        suma7 += A*A*B*C+(B*B-1)*B*C+B*C*(C*C-1);
        suma8 += A*A*C*C+B*B*C*C+(C*C-1)*(C*C-1);
        double subb0 = (A*A-1)*x0+A*B*y0+A*C*z0;
        double subb1 = A*B*x0+(B*B-1)*y0+B*C*z0;
        double subb2 = A*C*x0+B*C*y0+(C*C-1)*z0;
        sumb0 += subb0*(A*A-1)+subb1*A*B+subb2*A*C;
        sumb1 += subb0*A*B+subb1*(B*B-1)+subb2*B*C;
        sumb2 += subb0*A*C+subb1*B*C+subb2*(C*C-1);
    }
    int n = 3, nrhs = 1, lda = 3, ipiv[3], ldb = 3, info;
    double a[9] = {suma0, suma1, suma2, suma3, suma4, suma5, suma6, suma7, suma8};
    double xyz[3] = {sumb0, sumb1, sumb2};
    dgesv_(&n, &nrhs, a, &lda, ipiv, xyz, &ldb, &info);
    if(info != 0) {
        std::cerr<<" dgesv_ error!"<<std::endl;
    }

    memcpy(centre->p, xyz, sizeof(double)*3);
}
