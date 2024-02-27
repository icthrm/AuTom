#include "sfm_driver.h"
#include <cmath>
#include <iostream>
#include "util/exception.h"
#include "util/matrix.h"
#include "matrix/matrix.h"
#include "util/qsort.h"
#include <cfloat>
#include <matrix/vector.h>
#include "geometry_data.h"
#include <ext/algorithm>
// #include <ext/hash_map>

#define MAXITER 		800

using namespace mx;

bundle::SFMDriver::SFMDriver()
{
    Initialize();
    
    for(int i = 1; i < WEIGHT_SIZE; i++){
	weight[i] = 1;
    }
//     for(int i = 1; i <= 25; i++){
// 	weight[i] = 1/pow(i, 1/3.0);//sqrt(i); 
//     }
//     for(int i = 26; i < WEIGHT_SIZE; i++){
// 	weight[i] = 1/5;
//     }
}
bundle::SFMDriver::~SFMDriver() {}

void bundle::SFMDriver::Initialize()
{
    /* call sparse LM routine */
    opts[0]=SBA_INIT_MU; opts[1]=SBA_STOP_THRESH; opts[2]=SBA_STOP_THRESH;
    opts[3]=SBA_STOP_THRESH;
    //opts[3]=0.05*numprojs; // uncomment to force termination if the average reprojection error drops below 0.05
    opts[4]=0.0;
    //opts[4]=1E-12; // uncomment to force termination if the relative reduction in the RMS reprojection error drops below 1E-05
    /* set up globs structure */
    
    globs.cnp = cnp;
    globs.pnp = pnp;
    globs.mnp = mnp;
    globs.initial_rot_params  =  NULL;		// should be link to escore late    
    globs.intrinsic_calib = NULL;			// intrinsics are to be found 
    /* specify the number of intrinsic parameters that are to be fixed
    * equal to their initial values, as follows:
    * 0: all free, 1: skew fixed, 2: skew, ar fixed, 4: skew, ar, ppt fixed
    * Note that a value of 3 does not make sense
    */
    globs.nccalib  =  5;//4; 			// number of intrinsics to keep fixed, must be between 0 and 5    
}

#ifndef MIN_POINTS
#define MIN_POINTS		20
#endif

void bundle::SFMDriver::Project(const double P[12], const double X[4], double r[3])
{
    matrix_product(3, 4, 4, 1, P, X, r);
}

/** the first index of @c pt_views is the index of @c added_order*/
double bundle::SFMDriver::Run(int* added_order, mx::camera_params* camparams, const int start_camera, const int ncams, const int nconcam,
				 v3_t* init_pts, const int n3Dpts, const int ncon3Dpts, std::vector<ImageKeyVector>& pt_views, GeometryData& data, bool remove_outliers)
{
    int total_removed_points = 0;
    int num_outliers = 0;
    
    double dist_total = 0.0;
    int num_dists = 0;

    int *remap = new int [n3Dpts];		//store the index of 3D points in nz_pts, if no, store -1
    double* motstruct = new double[ncams*cnp + n3Dpts*pnp];
    double* nz_pts = motstruct+ncams*cnp;
    
    char* vmask = new char[n3Dpts*ncams];

    int num_projections = 0;
    for(int i = 0; i < n3Dpts; i++){
	num_projections += (int)pt_views[i].size();
    }

    double* projections = new double[mnp*num_projections];
    
    int num_3d_pts;
    
    do{
        if((num_3d_pts = n3Dpts - total_removed_points) < MIN_POINTS){
            EX_PRINT("# Too few points remaining, exiting!\n")

            dist_total = DBL_MAX;
            break;
        }

        int arr_idx = 0;
        int nz_count = 0;
	
	/* Set up the vmask and projections */
	memset(vmask, 0, sizeof(char)*num_3d_pts*ncams);
	
	int fixed_pt_num = ncon3Dpts;
	
        for(int i = 0; i < n3Dpts; i++){
            int num_views =(int)pt_views[i].size();

            if(num_views > 0){
                for(int j = 0; j < num_views; j++){
                    int c = pt_views[i][j].first;
                    int v = added_order[c];
                    int k = pt_views[i][j].second;
                    vmask[nz_count * ncams + c] = 1;

                    projections[2 * arr_idx + 0] = data.GetKey(v,k).m_x;
                    projections[2 * arr_idx + 1] = data.GetKey(v,k).m_y;

                    arr_idx++;
                }

                remap[i] = nz_count;
                memcpy(&nz_pts[nz_count*3], init_pts[i].p, sizeof(double)*3);
                nz_count++;
            }
            else{
		if(i < ncon3Dpts){
		    fixed_pt_num--;
		}
                remap[i] = -1;
            }
        }

        double* motparams = motstruct;
        for(int i = 0; i < ncams; i++){
	    MotCopyFormCameraParams(motparams, camparams[i+start_camera]);
	    motparams += cnp;
	}
        
        dist_total = 0.0;
        num_dists = 0;

	EX_BEGIN_CLOCK()
	Run(motstruct, projections, vmask, num_3d_pts, ncams, nconcam, fixed_pt_num);
	EX_END_CLOCK()
	EX_TRACE("# SFM using %d 3D pts(%d fixed), %d frames(%d fixed) and %d image projections(%g p/p), error %g [initial %g](elapse: %ld)\n",
		num_3d_pts, ncon3Dpts, ncams, nconcam, arr_idx, ((double)arr_idx)/num_3d_pts, info[1]/arr_idx, info[0]/arr_idx, EX_ELAPSE());
	
	motparams = motstruct;
	for(int i = 0; i < ncams; i++){
	    CameraCopyFormMotParams(&camparams[i+start_camera], motparams);
	    motparams += cnp;
	}
	
	std::vector<std::pair<int, int> > outliers;
	
	if(!remove_outliers){
	    goto end;
	}

        for(int i = 0; i < ncams; i++){
	    double P[12];
	    MxPFromCameraParams(camparams[i+start_camera], P);

            int num_keys = data.GetNumKeys(added_order[i]);

            int num_pts_proj = 0;
            for(int j = 0; j < num_keys; j++){
                if(data.GetKey(added_order[i], j).m_extra >= 0){
                    num_pts_proj++;
                }
            }

            double *dists = new double[num_pts_proj];
            int pt_count = 0;

            std::vector<Keypoint>::iterator iter;

            for(iter = data.m_image_data[added_order[i]].m_keys.begin(); iter != data.m_image_data[added_order[i]].m_keys.end(); iter++){
                const Keypoint &key = *iter;

                if(key.m_extra >= 0){
		    int pt_idx = key.m_extra;
		    double X[4], pr[3];
		    memcpy(X, &nz_pts[remap[pt_idx]*3], sizeof(double)*3);
		    X[3] = 1;
		    Project(P, X, pr);
		    pr[0] /= pr[2]; pr[1] /= pr[2];
		    double dx = pr[0]-key.m_x;
		    double dy = pr[1]-key.m_y;

                    double dist = sqrt(dx * dx + dy * dy);
                    dist_total += dist;
                    num_dists++;

                    dists[pt_count] = dist;

                    pt_count++;
                }
            }
	    
            /* Estimate the median of the distances */
            double med = kth_element_copy(num_pts_proj, int(0.7/*0.8/* 0.9 */* num_pts_proj), dists);

#define NUM_STDDEV 2.0//3.0//6.0
            double thresh = 1.2 * NUM_STDDEV * med;/* k * stddev */
            thresh = CLAMP(thresh, min_proj_error_threshold, max_proj_error_threshold);

            /* Compute the average reprojection error for this camera */
            double sum = 0.0;
            for(int j = 0; j < num_pts_proj; j++){
                sum += dists[j];
            }

            double avg = sum/num_pts_proj;
            EX_PRINT("# Camera %d[%d] (%d pts), mean error: %0.3f [median %0.3f(0.7 quantile %0.3f), error thresh %0.3f]\n",
                   i, added_order[i], num_pts_proj, avg, kth_element_copy(num_pts_proj, int(0.5 * num_pts_proj), dists), med, thresh);

            pt_count = 0;
            for(int j = 0; j < num_keys; j++){
                int pt_idx = data.GetKey(added_order[i],j).m_extra;

                if(pt_idx < 0)
                    continue;

                if(dists[pt_count]*weight[pt_views[pt_idx].size()] > thresh*weight[2] || dists[pt_count] > max_proj_error_threshold){
                    /* Remove this point from consideration */  
                    outliers.push_back(std::pair<int, int>(pt_idx, i));
                }
                pt_count++;
            }

            delete [] dists;
        }

        /* Remove outlying points */
	num_outliers = 0;
	for(int i = 0; i <(int)outliers.size(); i++){
	    int idx = outliers[i].first;

	    if(idx < ncon3Dpts){
		continue;
	    }

	    if(!pt_views[idx].size()){
		continue;
	    }
	    
	    for(ImageKeyVector::iterator itr = pt_views[idx].begin(); itr != pt_views[idx].end(); ){
		int v = (*itr).first;
		int k = (*itr).second;
		if(v == outliers[i].second){
		    if(data.GetKey(added_order[v], k).m_extra != idx){
			EX_ERROR("Error!  Entry for(%d,%d) should be %d, but is %d\n",
				added_order[v], k, idx, data.GetKey(added_order[v], k).m_extra);
		    }
		    data.GetKey(added_order[v], k).m_extra = -2;
		    pt_views[idx].erase(itr);
		    num_outliers++;
		    break;
		}
		else{
		    itr++;
		}
	    }
	    
	    if(pt_views[idx].size() < 2){
		for(ImageKeyVector::iterator itr = pt_views[idx].begin(); itr != pt_views[idx].end(); itr++){
		    int v = (*itr).first;
		    int k = (*itr).second;

		    data.GetKey(added_order[v], k).m_extra = -2;
		}
		
		pt_views[idx].clear();
		total_removed_points++;
	    }
	}

	outliers.clear();

	EX_PRINT("# Removing %d outliers\n", num_outliers);
	
end:
        for(int i = 0; i < n3Dpts; i++){
            if(remap[i] != -1){
                memcpy(init_pts[i].p, &nz_pts[remap[i]*3], sizeof(double)*3);
            }
        }

    }while(num_outliers > 0);

    delete [] vmask;
    delete [] projections;
    
    delete [] remap;
    delete [] motstruct;

    return dist_total/num_dists;
}

v2_t bundle::SFMDriver::Project(const mx::camera_params& params, v3_t pt)
{
    double P[12];
    MxPFromCameraParams(params, P);
    double X[4];
    X[3] = 1;
    memcpy(X, pt.p, sizeof(double)*3);
    double r[3];
    Project(P, X, r);

    return v2_new(r[0]/r[2], r[1]/r[2]);
}

void bundle::SFMDriver::RefineCameraParameters(v3_t* points, v2_t* projs, int num_points, mx::camera_params* camera)
{
    double* motstruct = new double[cnp + num_points*pnp];
    double* nz_pts = motstruct+cnp;
    
    char* vmask = new char[num_points];
    double* projections = new double[mnp*num_points];
    
    memset(vmask, 0, sizeof(char)*num_points);
    
    double* proj = projections;
    double* z_3dpt = nz_pts;
    for(int i = 0; i < num_points; i++){
	memcpy(proj, projs[i].p, sizeof(double)*2);
	proj += 2;
	memcpy(z_3dpt, points[i].p, sizeof(double)*3);
	z_3dpt +=3;
	vmask[i] = 1;
    }

    MotCopyFormCameraParams(motstruct, *camera);
    
    EX_BEGIN_CLOCK()
    Run(motstruct, projections, vmask, num_points, 1, 0, num_points);
    EX_END_CLOCK()
    EX_TRACE("# CameraRefine using %d pts, %d frames(%d fixed) and %d image projections, error %g [initial %g](elapse: %ld)\n",
	    num_points, 1, 0, num_points, info[1]/num_points, info[0]/num_points, EX_ELAPSE());
    
    delete [] motstruct;
    delete [] vmask;
    delete [] projections;
    
    CameraCopyFormMotParams(camera, motstruct);

    return;
}

int bundle::SFMDriver::Run(double* motstruct, double* imgpts, char* vmask, 
			  const int n3Dpts, const int ncams, const int nconcam, const int ncon3Dpts)
{
    globs.initial_rot_params = new double[ncams*FULLQUATSZ];
    double* initrot = globs.initial_rot_params;
    double* params = motstruct;
    for(int i = 0; i < ncams; i++){
	initrot[1]=params[cnp-6];
	initrot[2]=params[cnp-5];
	initrot[3]=params[cnp-4];
	initrot[0]=sqrt(1.0 - initrot[1]*initrot[1] - initrot[2]*initrot[2] - initrot[3]*initrot[3]);
	initrot += FULLQUATSZ;
	params += cnp;
    }
    
    /* initialize the local rotation estimates to 0, corresponding to local quats (1, 0, 0, 0) */
    for (int k = 0; k < ncams; k++) {
        int j = (k+1)*globs.cnp; // note the +1, below we move from right to left, assuming 3 parameters for the translation!
        motstruct[j-4] = motstruct[j-5] = motstruct[j-6] = 0.0; // clear rotation
    }

    int nvars = ncams*cnp+n3Dpts*pnp;
    int n = sba_motstr_levmar_x(n3Dpts, ncon3Dpts, ncams, nconcam, vmask, motstruct,
                                cnp, pnp, imgpts, NULL, mnp, img_projsKRTS_x,
                                img_projsKRTS_jac_x, (void *)(&globs), MAXITER, verbose, opts, info);

    /* combine the local rotation estimates with the initial ones */
    initrot = globs.initial_rot_params;
    for (int k = 0; k < ncams; k++) {
        double *v, qs[FULLQUATSZ], *q0, prd[FULLQUATSZ];

        /* retrieve the vector part */
        v = motstruct + (k+1)*globs.cnp - 6; // note the +1, we access the motion parameters from the right, assuming 3 for translation!
        _MK_QUAT_FRM_VEC(qs, v);

        q0=initrot+k*FULLQUATSZ;
        QuatMultiply(qs, q0, prd); // prd=qs*q0

        /* copy back vector part making sure that the scalar part is non-negative */
        if (prd[0]>=0.0) {
            v[0]=prd[1];
            v[1]=prd[2];
            v[2]=prd[3];
        }
        else { // negate since two quaternions q and -q represent the same rotation
            v[0]=-prd[1];
            v[1]=-prd[2];
            v[2]=-prd[3];
        }
    }   
    delete [] globs.initial_rot_params;
    
    return n;
}