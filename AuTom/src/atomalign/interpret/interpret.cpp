#include "interpret.h"
#include "matrix/matrix.h"
#include "dataf/camera.h"
#include "math.h"
#include "util/qsort.h"
#include "mx2to3/mx2to3.h"
#include <algorithm>

void Interpreter::PrintPoints(const std::vector<v3_t>& pt_v, const char* filename)
{
    std::ofstream out(filename);
    for(int i = 0; i < pt_v.size(); i++){
        out<<pt_v[i].p[0]<<" "<<pt_v[i].p[1]<<" "<<pt_v[i].p[2]<<std::endl;
    }

    out.close();
}

/** @brief ax+by+cz+d=0 -> z=ax+by+c print the abc*/
void Interpreter::PrintPlaneABCDAsABC(const double abcd[4], std::ostream& o){
    o<<-abcd[0]/abcd[2]<<" "<<-abcd[1]/abcd[2]<<" "<<-abcd[3]/abcd[2]<<std::endl;
}

void Interpreter::PrintMxPlaneVecAsABC(const std::vector<mx::Matrix<4, 1, double> >& plane_v, const char* filename){
    std::ofstream out(filename);
    for(int i = 0; i < plane_v.size(); i++){
        PrintPlaneABCDAsABC(plane_v[i].D(), out);
    }
    out.close();
}

static void InverseProjectionToPlane(const double m[3], const double P_inv[12], const double C[4], const double abcd[4], double v3[4])
{
    double P_invxm[4];
    matrix_product(4, 3, 3, 1, P_inv, m, P_invxm);
    double r1, r2, u;
    matrix_product(1, 4, 4, 1, P_invxm, abcd, &r1);
    matrix_product(1, 4, 4, 1, C, abcd, &r2);
    u = -r2/r1;
    v3[0] = P_invxm[0]*u+C[0];
    v3[1] = P_invxm[1]*u+C[1];
    v3[2] = P_invxm[2]*u+C[2];
    v3[3] = P_invxm[3]*u+C[3];
}

void Interpreter::BodyPlaneEstimate(const std::vector< v3_t >& pt_v, double abcd[4])
{
    double rslt[3];

    util::LeastSquare3DPanel(pt_v, rslt);

    double* errs = new double[pt_v.size()];
    for(int i = 0; i < pt_v.size(); i++){
        errs[i] = util::PlanePointError(pt_v[i], rslt);
    }
    double err_tol = kth_element(pt_v.size(), pt_v.size()*.9, errs);
    delete [] errs;

    std::vector<v3_t> inliers, outliers;

#define     ERR_TOL 0.0010
    bool effective = util::RansacPlane(pt_v, 20, 0.01, err_tol/*ERR_TOL*/, rslt, &inliers, &outliers);
    std::cout<<"ERR_TOL: "<<err_tol<<" ("<<inliers.size()<<"/"<<pt_v.size()<<")"<<std::endl;
    std::cout<<"z=ax+by+c: ";
    for(int i = 0; i < 3; i++){
        std::cout<<rslt[i]<<" ";
    }
    std::cout<<std::endl;

    abcd[0] = rslt[0];
    abcd[1] = rslt[1];
    abcd[2] = -1;
    abcd[3] = rslt[2];
}

v3_t Interpreter::BodyCentreEstimate(double x, double y, const mx::camera_params& camparam, const double abcd[4])
{
    double m[3] = {x, y, 1};
    double v3[4];
    double P_inv[12], C[4];
    v3_t ctre;
    mx::MxCFromCameraParams(camparam, C);
    mx::MxPinvFromCameraParams(camparam, P_inv);
    InverseProjectionToPlane(m ,P_inv, C, abcd, v3);
    Vx(ctre) = v3[0]/v3[3];
    Vy(ctre) = v3[1]/v3[3];
    Vz(ctre) = v3[2]/v3[3];
    return ctre;
}

/** @c x @c y are the location of centre point in picture*/
v3_t Interpreter::BodyCentreEstimate(double x, double y, const mx::camera_params* camparams, int num, const double abcd[4])
{
    //     std::vector<v3_t> centre;
    double m[3] = {x, y, 1};
    double mean_x, mean_y, mean_z;
    for(int i = 0; i < num; i++){
        double v3[4];
        double P_inv[12], C[4];
        v3_t ctre;
        MxCFromCameraParams(camparams[i], C);
        MxPinvFromCameraParams(camparams[i], P_inv);
        InverseProjectionToPlane(m ,P_inv, C, abcd, v3);
        mean_x += Vx(ctre) = v3[0]/v3[3];
        mean_y += Vy(ctre) = v3[1]/v3[3];
        mean_z += Vz(ctre) = v3[2]/v3[3];
// 	centre.push_back(ctre);
    }
    mean_x /= num;
    mean_y /= num;
    mean_z /= num;
    double lenta = (abcd[0]*mean_x+abcd[1]*mean_y+abcd[2]*mean_z+abcd[3])/(abcd[0]*abcd[0]+abcd[1]*abcd[1]+abcd[2]*abcd[2]);// *2*n;
    double centre_x = -abcd[0]*lenta+mean_x;
    double centre_y = -abcd[1]*lenta+mean_y;
    double centre_z = -abcd[2]*lenta+mean_z;
    return v3_new(centre_x, centre_y, centre_z);
}

/** @brief P_c = R*P_w+t*/
void Interpreter::MotPointToTiltPlanePoint(const mx::camera_params* camparams, int num, const v3_t& point, std::vector<v3_t>& pt_v)
{
    v3_t pt;
    for(int i = 0; i < num; i++){
        matrix_product331(camparams[i].R.D(), point.p, pt.p);
        Vx(pt) += camparams[i].T.D(0);
        Vy(pt) += camparams[i].T.D(1);
        Vz(pt) += camparams[i].T.D(2);
        pt_v.push_back(pt);
    }
}

/** @brief R'*P_c +C_~ = R'*(P_c-t) = P_w*/
void Interpreter::TiltPlanePointToMotPoint(const mx::camera_params& camparam, const v3_t& point, v3_t* r)
{
    double R_inv[9];
    double C[4];
    mx::MxCFromCameraParams(camparam, C);
    matrix_transpose(3, 3, camparam.R.D(), R_inv);
    matrix_product331(R_inv, point.p, r->p);
    Vx(*r) += C[0];
    Vy(*r) += C[1];
    Vz(*r) += C[2];
}

/** @brief R_T*P_c +C_~ = P-->ax+by+cz+d=0-->a`x'+b`y'+c`z'+d`=0 */
void Interpreter::MotCameraToTiltPlaneABCD(const mx::camera_params* camparams, int num, const double abcd[4], std::vector<mx::Matrix<4,1,double> >& plane_v)
{
    for(int i = 0; i < num; i++){
        double R_inv[9];
        double C[4];
        mx::MxCFromCameraParams(camparams[i], C);
        matrix_transpose(3, 3, camparams[i].R.D(), R_inv);
        mx::Matrix<4, 1, double> plmx;
        plmx.D(0) = abcd[0]*R_inv[0]+abcd[1]*R_inv[3]+abcd[2]*R_inv[6];
        plmx.D(1) = abcd[0]*R_inv[1]+abcd[1]*R_inv[4]+abcd[2]*R_inv[7];
        plmx.D(2) = abcd[0]*R_inv[2]+abcd[1]*R_inv[5]+abcd[2]*R_inv[8];
        plmx.D(3) = abcd[0]*C[0]+abcd[1]*C[1]+abcd[2]*C[2]+abcd[3];
        plane_v.push_back(plmx);

        plmx.D(0) /= plmx.D(3);
        plmx.D(1) /= plmx.D(3);
        plmx.D(2) /= plmx.D(3);
        plmx.D(3) = 1;
// 	std::cout<<-plmx.D(0)/plmx.D(2)<<" "<<-plmx.D(1)/plmx.D(2)<<" "<<-plmx.D(3)/plmx.D(2)<<std::endl;
    }
}

void Interpreter::SetAnchorIndex(int idx)
{
    ank_idx = idx;
}

int Interpreter::AnchorIndex()
{
    return ank_idx;
}

void Interpreter::RotCameraParams(double rot_angle, mx::camera_params* camparam)
{
    mx::camera_params camparams_cpy;
    double K[9], R[9], t[3];
    mx::MxKFromCameraParams(*camparam, K);
    double R_rot[9] = {cos(rot_angle), -sin(rot_angle), 0, sin(rot_angle), cos(rot_angle), 0, 0, 0, 1};
    matrix_product33(R_rot, (*camparam).R.D(), camparams_cpy.R.D());
    matrix_product331(R_rot, (*camparam).T.D(), camparams_cpy.T.D());

    mx::CameraInitFormKRT(&camparams_cpy, K, camparams_cpy.R.D(), camparams_cpy.T.D());
    *camparam = camparams_cpy;
}

void Interpreter::RotCameraParams(double rot_angle[], mx::camera_params* camparams, int num)
{
    for(int i = 0; i < num; i++){		//store the transformed camera_params to camparams
        RotCameraParams(rot_angle[i], &camparams[i]);
    }
}

void Interpreter::RotMxPlane(double rot_angle, mx::Matrix<4, 1, double>& plane)
{
    double a = plane.D(0);
    double b = plane.D(1);
    plane.D(0) = a*cos(rot_angle)-b*sin(rot_angle);
    plane.D(1) = a*sin(rot_angle)+b*cos(rot_angle);
}

void Interpreter::RotMxPlane(double rot_angle[], std::vector<mx::Matrix<4, 1, double> >& plane_v)
{
    for(int i = 0; i < plane_v.size(); i++){
        RotMxPlane(rot_angle[i], plane_v[i]);
    }
}

void Interpreter::StretchMxPlane(const double A[4], mx::Matrix<4, 1, double>& plane)
{
    double a = plane.D(0);
    double b = plane.D(1);
    plane.D(0) = a*A[0]-b*A[2];
    plane.D(1) = a*A[1]+b*A[3];
}

#define NORMALIZE_3V(X){double norm = sqrt((X)[0]*(X)[0]+(X)[1]*(X)[1]+(X)[2]*(X)[2]);			\
	(X)[0] /= norm;	\
	(X)[1] /= norm;	\
	(X)[2] /= norm;}
	
#define MAG_3V(X)	((X)[0]*(X)[0]+(X)[1]*(X)[1]+(X)[2]*(X)[2])
	
void Interpreter::InterpretAndCalibrate(const mx::camera_params* result, int num, double abcd[4], std::vector<util::calibrated>* calib_v, util::globalcalib* gcalib)
{
    double K[9];				//recover intrinsic matrix
    mx::MxKFromCameraParams(result[0], K);
    mx::camera_params camparams[num];
    memcpy(camparams, result, sizeof(mx::camera_params)*num);		//copy result to camparams

    /* output the original state*/
    std::vector<mx::Matrix<4, 1, double> > plane_v;
    MotCameraToTiltPlaneABCD(camparams, num, abcd, plane_v);
    PrintMxPlaneVecAsABC(plane_v, "planes.abc");

    double nworld_yaxis[3];		//y-axis direction
    int inlier_num;
    mx2to3::GlobalTiltAxisEstimate(camparams, num, nworld_yaxis, &inlier_num);
    std::cout<<"tilt axis: "<<nworld_yaxis[0]<<" "<<nworld_yaxis[1]<<" "<<nworld_yaxis[2]<<"("<<inlier_num<<"/"<<num<<")"<<std::endl;
	
	//x-axis direction(vector(-z) x vector(y))
	double nworld_xaxis[3] = {abcd[1]*nworld_yaxis[2]-abcd[2]*nworld_yaxis[1], 
				abcd[2]*nworld_yaxis[0]-abcd[0]*nworld_yaxis[2], abcd[0]*nworld_yaxis[1]-abcd[1]*nworld_yaxis[0]};
	NORMALIZE_3V(nworld_xaxis)
	
	//z-axis direction(vector(x) x vector(y))
	double nworld_zaxis[3] = {-nworld_yaxis[1]*nworld_xaxis[2]+nworld_yaxis[2]*nworld_xaxis[1], 
				-nworld_yaxis[2]*nworld_xaxis[0]+nworld_yaxis[0]*nworld_xaxis[2], -nworld_yaxis[0]*nworld_xaxis[1]+nworld_yaxis[1]*nworld_xaxis[0]};
	NORMALIZE_3V(nworld_zaxis)
	
	double plane_norm_vec[3] = {-abcd[0],-abcd[1],-abcd[2]};
	NORMALIZE_3V(plane_norm_vec)
	
	gcalib->x_axis_shift = acos((nworld_zaxis[0]*plane_norm_vec[0]
		+nworld_zaxis[1]*plane_norm_vec[1]+nworld_zaxis[2]*plane_norm_vec[2])/sqrt(MAG_3V(nworld_zaxis)*MAG_3V(plane_norm_vec)));
	
	if(plane_norm_vec[1] > nworld_zaxis[1]){
			gcalib->x_axis_shift *= -1;		// WARNING OK!
	}
	///////////////////
	
    double rot_v[num];
	double xtilt[num];
	
    for(int i = 0; i < num; i++){
        //mx2to3::HorizonEstimateRot(camparams[i], &rot_v[i]);
//         mx2to3::DirectDotEstimateRot(camparams[i], tilt_axis_dir, &rot_v[i]);		//make the x-direction unchanged
		mx2to3::DirectDotEstimateRot2(camparams[i], nworld_xaxis, &rot_v[i]);			//make the y-direction unchanged
// 	std::cout<<i<<": "<<rot_v[i]*180/M_PI<<std::endl;
        mx2to3::RotCameraCoordinate(rot_v[i], &camparams[i]);
		
		const double* r = camparams[i].R.D();
		
		xtilt[i] = acos(r[3]*nworld_yaxis[0]+r[4]*nworld_yaxis[1]+r[5]*nworld_yaxis[2]);
		if(r[5] < nworld_yaxis[2]){
			xtilt[i] *= -1;				// WARNING OK!
		}
// 	camparams[i].R.Print(std::cout);
    }

    v3_t centre;
    mx2to3::DirectionVerticalEstimateCentre(camparams, num, &centre);
    std::cout<<"focus: "<<Vx(centre)<<" "<<Vy(centre)<<" "<<Vz(centre)<<std::endl;

//     v3_t centre = BodyCentreEstimate(K[2], K[5], camparams[AnchorIndex()], abcd);

    std::vector<v3_t> pt_v;
    MotPointToTiltPlanePoint(camparams, num, centre, pt_v);
    PrintPoints(pt_v, "tilt_centres.mot");

    std::vector<double> depth;
    for(int i = 0; i < pt_v.size(); i++){
        depth.push_back(Vz(pt_v[i]));
    }
    std::sort(depth.begin(), depth.end());

    double mean_depth = 0;
    for(int i = int(depth.size()*.25); i < int(depth.size()*.75); i++){
        mean_depth += depth[i];
    }
    mean_depth /= int(depth.size()*.75)-int(depth.size()*.25);
    depth.clear();

    double ankx = 0;//Vx(pt_v[anchor]);
    double anky = 0;//Vy(pt_v[anchor]);
    double ankz = mean_depth;

    for(int i = 0; i < num; i++){		//store the transformation value to pt_v
        Vx(pt_v[i]) = ankx-Vx(pt_v[i]);
        Vy(pt_v[i]) = anky-Vy(pt_v[i]);
        Vz(pt_v[i]) = ankz-Vz(pt_v[i]);
    }

    for(int i = 0; i < num; i++){		//store the transformed camera_params to camparams
        double K[9], t[3];
        MxKFromCameraParams(camparams[i], K);
        t[0] = camparams[i].T.D(0)+Vx(pt_v[i]);
        t[1] = camparams[i].T.D(1)+Vy(pt_v[i]);
        t[2] = camparams[i].T.D(2)+Vz(pt_v[i]);
        mx::CameraInitFormKRT(&camparams[i], K, camparams[i].R.D(), t);
    }

    for(int i = 0; i < num; i++){		//store the transformed plane abcd to plane_v
        double r;
        matrix_product131(plane_v[i].D(), pt_v[i].p, &r);
        plane_v[i].D(3) -= r;
    }
    PrintMxPlaneVecAsABC(plane_v, "planes.fin.abc");

    v3_t base[num];
    for(int i = 0; i < num; i++){
        Vx(base[i]) = camparams[i].R.D(0);
        Vy(base[i]) = camparams[i].R.D(1);
        Vz(base[i]) = camparams[i].R.D(2);
		v3_unit(base[i]);
    }

     v3_t zero_dir; memcpy(zero_dir.p, nworld_xaxis, sizeof(double)*3);
//    mx2to3::TiltVectorEstimate(camparams[AnchorIndex()], tilt_axis_dir, zero_dir.p);
//     v3_t zero_base = base[AnchorIndex()];

    util::calibrated calib;

    for(int i = 0; i < num; i++){
        if(!result[i].valid){
            calib.valid = false;
            calib.scale = 1;
            calib.global_rot = 0;
            calib.x_shift = 0;
            calib.y_shift = 0;
            calib.x_stretch_rot = 0;
            calib.y_stretch_rot = 0;
            calib_v->push_back(calib);
            continue;
        }

        calib.x_shift = Vx(pt_v[i])*K[0]/ankz;
        calib.y_shift = Vy(pt_v[i])*K[4]/ankz;

        calib.global_rot = rot_v[i]; //0
        calib.x_stretch_rot = 0;
        calib.y_stretch_rot = 0;

        calib.scale = ankz/(ankz-Vz(pt_v[i]));
//         calib.tilt_angle = -1*atan2(v3_dotp(base[i], zero_dir), v3_dotp(base[i], zero_base));
		double dotp = v3_dotp(base[i], zero_dir)/sqrt(v3_magsq(base[i])*v3_magsq(zero_dir));
		calib.tilt_angle = acos(dotp >= 1 ? 1 : dotp);
		if(Vz(base[i]) > Vz(zero_dir)){
			calib.tilt_angle *= -1;		// WARNING	OK!
		}
		calib.xtilt = xtilt[i];
        calib.valid = true;
        calib_v->push_back(calib);
    }
}

#undef MAG_3V
#undef NORMALIZE_3V

void Interpreter::InterpretMain(const std::vector<mx::camera_params>& cameras, const std::vector<v3_t>& points,
                                const std::vector<float>& angles, std::vector<util::calibrated>* calib_v, util::globalcalib* gcalib)
{
    Interpreter interpreter;

    double abcd[4];
    interpreter.BodyPlaneEstimate(points, abcd);

    mx::camera_params results[cameras.size()];
    int idx[cameras.size()];
    int valid_idx = 0;

    for(int i = 0; i < cameras.size(); i++){
        if(cameras[i].valid){
            results[valid_idx] = cameras[i];
            idx[valid_idx] = i;
            valid_idx++;
        }
    }

    std::vector<util::calibrated> calib__;

    int min_idx = -1;
    for(int i = 0; i < valid_idx; i++){
        if(angles[idx[i]] > -0.1 && angles[idx[i]] < 0.1){
            min_idx = i;
            break;
        }
    }
    
    if(min_idx == -1){
        int abs_min_idx = -1;
        int min_val = 99999;
        for(int i = 0; i < valid_idx; i++){
            if(abs(angles[idx[i]]) < min_val){
                min_val = abs(angles[idx[i]]);
                abs_min_idx = i;
            }
        }
        min_idx = abs_min_idx;
    }
    interpreter.SetAnchorIndex(min_idx);

    interpreter.InterpretAndCalibrate(results, valid_idx, abcd, &calib__, gcalib);

    util::calibrated calib;
    for(int i = 0; i < cameras.size(); i++){
        calib.valid = false;
        calib.scale = 1;
        calib.global_rot = 0;
        calib.x_shift = 0;
        calib.y_shift = 0;
        calib.x_stretch_rot = 0;
        calib.y_stretch_rot = 0;
		calib.xtilt = 0;
        calib_v->push_back(calib);
    }

    for(int i = 0; i < valid_idx; i++){
        (*calib_v)[idx[i]] = calib__[i];
    }
}

