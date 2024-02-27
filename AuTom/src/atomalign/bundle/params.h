#ifndef PARAMS_H__
#define PARAMS_H__

#include "dataf/dataf.h"
#include "sba.h"
#include <vector>
#include <list>
#include <sys/stat.h>
#include <iostream>
#include "matrix/matrix.h"
#include "matrix/vector.h"
#include "dataf/quat_vec.h"
#include "util/matrix.h"
#include "dataf/camera.h"
#include <algorithm>

#define CNP		6+5
#define PNP		3
#define MNP		2
#define FULLQUATSZ	4

namespace bundle {

class Initating {
public:
    constexpr static const double f = 200000;		//focal length
    constexpr static const double z = 10;			//rate = z/f
    //f = 100000~1000000, z = 10~100
public:
    static void InitKMatrix3x3(double* K, int width_, int height_)
    {
        K[0] = f;
        K[1] = 0;
        K[2] = width_/2;
        K[3] = 0;
        K[4] = f;
        K[5] = height_/2;
        K[6] = 0;
        K[7] = 0;
        K[8] = 1;
    }

    static void InitKCalib5(double* calibration, int width_, int height_)
    {
        calibration[0] = f; 			// fu
        calibration[1] = width_/2; 		// u0
        calibration[2] = height_/2; 		// v0
        calibration[3] = 1; 			// ar
        calibration[4] = 0; 			// s
    }

    static void InitRV3(double* params, double angle)
    {
        //code for rotation; euler angle sequence (1 = 0, 2 = angle, 3 = 0)
        params[0] = 0;
        params[1] = -sin(angle/180/2*M_PI);//-sin(angle/180/2);		//angle/180 ->360~2pi
        params[2] = 0;
    }

    static void InitTV3(double* T, double angle)
    {
        double C[] = {-sin(angle/180*M_PI)*z, 0, z-cos(angle/180*M_PI)*z};
        double R[] = {cos(angle/180*M_PI), 0, -sin(angle/180*M_PI),
                      0, 1, 0,
                      sin(angle/180*M_PI), 0, cos(angle/180*M_PI)
                     };
        double r[3];
        matrix_product331(R, C, r);
        T[0] = -r[0];
        T[1] = -r[1];
        T[2] = -r[2];
        
//         mx::Matrix<3,1, double> C = _DMX(-sin(angle/180*M_PI)*z, 0, z-cos(angle/180*M_PI)*z);
//         mx::Matrix<3,3, double> R = _DMX(cos(angle/180*M_PI), 0, -sin(angle/180*M_PI),
//                                          0, 1, 0,
//                                          sin(angle/180*M_PI), 0, cos(angle/180*M_PI));
//         mx::Matrix<3,1, double> r;
//         mx::Product331(R, C, &r);
// 
//         T[0] = -r.D(0);
//         T[1] = -r.D(1);
//         T[2] = -r.D(2);
    }

    static void InitRMx9(double* R, double angle)
    {
        R[0] = cos(angle/180*M_PI);
        R[1] = 0;
        R[2] = -sin(angle/180*M_PI);
        R[3] = 0;
        R[4] = 1;
        R[5] = 0;
        R[6] = sin(angle/180*M_PI);
        R[7] = 0;
        R[8] = cos(angle/180*M_PI);
    }

    static void InverseProjection(const util::_point ori, double P_inv[12], double C[4], double pt3D[3])
    {
        double m[3];
        m[0] = ori.x;
        m[1] = ori.y;
        m[2] = 1;
        double P_invxm[4];
        matrix_product(4, 3, 3, 1, P_inv, m, P_invxm);
        double u = (C[2]-z*C[3])/(z*P_invxm[3]-P_invxm[2]);
        double k = u*P_invxm[3]+C[3];
        pt3D[0] = (u*P_invxm[0]+C[0])/k;
        pt3D[1] = (u*P_invxm[1]+C[1])/k;
        pt3D[2] = z;
// 	std::cout<<"("<<ori.x<<","<<ori.y<<")"<<std::endl;
// 	matrix_print(1, 3, pt3D);
// 	std::cout<<"-------------------------------------------"<<std::endl;
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

    static util::_point ContraMapping(util::_point ori, int width, int height, float angle)
    {
        int half_width = width/2;
        if(ori.x < half_width) {
            return util::_point(half_width-(half_width-ori.x)/cos(angle/180*M_PI), ori.y);
        }
        else if(ori.x > half_width) {
            return util::_point(half_width+(ori.x-half_width)/cos(angle/180*M_PI), ori.y);
        }
        else {
            return ori;
        }
    }
};

}

#endif
