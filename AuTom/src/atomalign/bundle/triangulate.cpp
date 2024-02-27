#include "triangulate.h"
#include <cmath>
#include <ctime>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include "util/qsort.h"

extern "C"{
#include <clapack.h>
#include <fblaswr.h>
#include <f2c.h>
#include <math.h>
#include <sys/stat.h>
}

v3_t bundle::triangulate_n(int num_points, const v2_t* pv, const double* Ps, double *error_out)
{
    int num_eqs = 2*num_points;
    int num_vars = 3;

    double *As = new double[num_eqs*num_vars];
    double *bs = new double[num_eqs];
    double *x = new double[num_vars];

    v3_t r;

    for (int i = 0; i < num_points; i++) {
        const double* P = Ps+12*i;
        double* A = As+6*i;
        double* b = bs+2*i;
        A[0] = P[0]-Vx(pv[i])*P[8];
        A[1] = P[1]-Vx(pv[i])*P[9];
        A[2] = P[2]-Vx(pv[i])*P[10];
        A[3] = P[4]-Vy(pv[i])*P[8];
        A[4] = P[5]-Vy(pv[i])*P[9];
        A[5] = P[6]-Vy(pv[i])*P[10];
        
        b[0] = Vx(pv[i])*P[11]-P[3];
        b[1] = Vy(pv[i])*P[11]-P[7];
    }
    
    /* Find the least squares result */
    dgelsy_driver(As, bs, x, num_eqs, num_vars, 1);

    if(error_out != NULL){
        double error = 0.0;
        for(int i = 0; i < num_points; i++){
            double dx, dy;
            const double* P = Ps+12*i;
            double X[4] = {x[0], x[1], x[2], 1};
            double pp[3];

            matrix_product331(P, X, pp);

            dx = pp[0] / pp[2] - Vx(pv[i]);
            dy = pp[1] / pp[2] - Vy(pv[i]);
            error += dx*dx+dy*dy;
        }
        error = sqrt(error / num_points);
    
        *error_out = error;
    }

    r = v3_new(x[0], x[1], x[2]);

    delete [] As;
    delete [] bs;
    delete [] x;

    return r;
}

bool bundle::find_projection_3x4(int num_pts, const v3_t *points, const v2_t *projs, double *P){
    if(num_pts < 6){
        std::cerr<<"[find_projection_3x4] Need at least 6 points!\n";
        return false;
    }
    else{
        int num_eqns = 2*num_pts;
        int num_vars = 12;

        double* AT = new double[num_eqns*num_vars];
        double* A = new double[num_eqns*num_vars];

        double error = 0.0;

        for (int i = 0; i < num_pts; i++) {
            double* row1 = A+2*i*num_vars;
            double* row2 = A+(2*i+1)*num_vars;
            
            row1[0]  = Vx(points[i]);
            row1[1]  = Vy(points[i]);
            row1[2]  = Vz(points[i]);
            row1[3]  = 1.0;
        
            row1[4]  = 0.0;
            row1[5]  = 0.0;
            row1[6]  = 0.0;
            row1[7]  = 0.0;
        
            row1[8]  = -Vx(projs[i])*Vx(points[i]);
            row1[9]  = -Vx(projs[i])*Vy(points[i]);
            row1[10] = -Vx(projs[i])*Vz(points[i]);
            row1[11] = -Vx(projs[i]);
            
            row2[0]  = 0.0;
            row2[1]  = 0.0;
            row2[2]  = 0.0;
            row2[3]  = 0.0;

            row2[4]  = Vx(points[i]);
            row2[5]  = Vy(points[i]);
            row2[6]  = Vz(points[i]);
            row2[7]  = 1.0;     

            row2[8]  = -Vy(projs[i])*Vx(points[i]);
            row2[9]  = -Vy(projs[i])*Vy(points[i]);
            row2[10] = -Vy(projs[i])*Vz(points[i]);
            row2[11] = -Vy(projs[i]);
        }

        //row-major to column-major
        matrix_transpose(num_eqns, num_vars, A, AT);
        delete [] A;
        A = AT;
        
        char jobu = 'N', jobvt = 'A';
        int m = num_eqns, n = num_vars;
        double* a = A;
        int lda = max(m, n), ldu = 1, ldvt = n;
        double* s = new double[min(m,n)], *u = NULL;
        double* vt = new double[n*n];
        
        double wsize, *work;
        int lwork = -1, info;
        
        dgesvd_(&jobu, &jobvt, &m, &n, a, &lda, s, u, &ldu, vt, &ldvt, &wsize, &lwork, &info);
        lwork = (int)wsize;
        work = new double[lwork];
        dgesvd_(&jobu, &jobvt, &m, &n, a, &lda, s, u, &ldu, vt, &ldvt, work, &lwork, &info);
        
        delete [] s; delete [] work;
        
        for(int i = 0; i < num_vars; i++){
            P[i] = vt[i*num_vars+num_vars-1];
        }
        delete [] vt;

        for(int i = 0; i < num_pts; i++){
            double pt[4] = { Vx(points[i]), Vy(points[i]), Vz(points[i]), 1.0 };
            double pr[3];
            double dx, dy, dist;

            matrix_product341(P, pt, pr);

            pr[0] /= pr[2];
            pr[1] /= pr[2];
            
            dx = pr[0] - Vx(projs[i]);
            dy = pr[1] - Vy(projs[i]);

            dist = dx*dx+dy*dy;

            error += dist;
        }

        printf("[find_projection_3x4] Average error is %0.3f\n", sqrt(error)/num_pts);

        delete [] A;

        return true;
    }
}

/*************************************************************find_projection_3x4_ransac*********************************************************************/
/** RANSAC error tolerance in pixels */
#define RANSAC_ERR_TOL 3

/** pessimistic estimate of fraction of inlers for RANSAC */
#define RANSAC_INLIER_FRAC_EST 0.25

/** estimate of the probability that a correspondence supports a bad model */
#define RANSAC_PROB_BAD_SUPP 0.05

static double projection_error(const v3_t& point, const v2_t& proj, const double* P)
{
    double pt[4] = {Vx(point), Vy(point), Vz(point), 1.0};
    double pr[3];
    double dx, dy, dist;

    matrix_product341(P, pt, pr);

    pr[0] /= pr[2];
    pr[1] /= pr[2];
    
    dx = pr[0]-Vx(proj);
    dy = pr[1]-Vy(proj);

    dist = dx*dx+dy*dy;

    return dist;
}

/** Calculates the natural log of the factorial of a number @param n number @return Returns log(n!) */
static double log_factorial(int n)
{
    double f = 0;
    int i;

    for(i = 1; i <= n; i++)
        f += log(i);

    return f;
}

static int calc_min_inliers(int n, int m, double p_badsupp, double p_bad)
{
    double pi, sum;
    int i, j;

    for(j = m+1; j <= n; j++){
        sum = 0;
        for(i = j; i <= n; i++){
            pi = (i-m) * log(p_badsupp) + (n-i+m) * log(1.0 - p_badsupp) +
                 log_factorial(n - m) - log_factorial(i - m) - log_factorial(n - i);
            /*
             * Last three terms above are equivalent to log(n-m choose i-m)
             */
            sum += exp(pi);
            if(sum < p_bad){
                break;
            }
        }
        if(sum < p_bad){
            break;
        }
    }
    return j;
}

static int* draw_ransac_sample_idx(bool* flags, int ori_size, int sam_size)
{
    memset(flags, 0, sizeof(bool)*ori_size);
    
    int* sam_idxs = new int[sam_size];
    int ranx;
    
    for(int i = 0; i < sam_size; i++){
        do{
            ranx = random()%ori_size;
        }while(flags[ranx]);
        sam_idxs[i] = ranx;
        flags[ranx] = true;
    }
    
    return sam_idxs;
}

static void extract_corres_pts(const v3_t* points, const v2_t* projs, int* idxs, int sam_size, v3_t* points_cpy, v2_t* projs_cpy)
{
    for(int i = 0; i < sam_size; i++){
        points_cpy[i] = points[idxs[i]];
        projs_cpy[i] = projs[idxs[i]];
    }
}

static int find_consensus(int ori_size, const v3_t* points, const v2_t* projs, const double* P, double err_tol, int** consensus)
{
    double err;
    
    int* consensus_idxs = new int[ori_size];
    int index = 0;
    
    for(int i = 0; i < ori_size; i++){
        err = projection_error(points[i], projs[i], P);
        if(err <= err_tol){
            consensus_idxs[index++] = i;
        }
    }
    
    *consensus = consensus_idxs;
    return index;
}

bool bundle::find_projection_3x4_ransac(int num_pts, const v3_t* points, const v2_t* projs, double* P)
{
    find_projection_3x4(num_pts, points, projs, P);
    
    double* errs = new double[num_pts];    
    for(int i = 0; i < num_pts; i++){
        errs[i] = projection_error(points[i], projs[i], P);
    }
    double err_tol = kth_element(num_pts, num_pts*.9, errs);
    delete [] errs;
    
    #define     ERR_TOL 0.0010
    double inl_ratio;
    bool effective = find_projection_3x4_ransac(num_pts, points, projs, P, 6, 0.01, err_tol/*ERR_TOL*/, &inl_ratio);
    std::cout<<"ERR_TOL: "<<err_tol<<" ("<<inl_ratio<<")"<<std::endl;
    return effective;
}

bool bundle::find_projection_3x4_ransac(int num_pts, const v3_t* points, const v2_t* projs, double* P, 
                                       int min_req, double p_bad, double err_tol, double* inl_ratio)
{
    int* sample, * consensus, * consensus_max = NULL;
    int consen_size, consen_min, consen_max = 0;
    
    int ori_num = num_pts;
    bool* flags = new bool[ori_num];
    
    double p, in_frac = RANSAC_INLIER_FRAC_EST;
    int k = 0;
    bool result = true;
    
    v3_t* points_cpy;
    v2_t* projs_cpy;
    
    if(ori_num < min_req){
        EX_TRACE("Warning: not enough matches to compute P(%d < %d)", ori_num, min_req)
        result = false;
        goto end;
    }

    srandom(time(NULL));

    consen_min = calc_min_inliers(ori_num, min_req, RANSAC_PROB_BAD_SUPP, p_bad);
    p = pow(1.0 - pow(in_frac, min_req), k);
    
    points_cpy = new v3_t[num_pts];
    projs_cpy = new v2_t[num_pts];
    
    while(p > p_bad){
        sample = draw_ransac_sample_idx(flags, ori_num, min_req);
        
        extract_corres_pts(points, projs, sample, min_req, points_cpy, projs_cpy);
        
        find_projection_3x4(min_req, points_cpy, projs_cpy, P);
        
        consen_size = find_consensus(ori_num, points, projs, P, err_tol, &consensus);
        
        if(consen_size > consen_max){
            if(consensus_max){
                delete [] consensus_max;
            }
            consensus_max = consensus;
            consen_max = consen_size;
            in_frac = (double)consen_max/ori_num;
        }
        else{
            delete [] consensus;
        }

iteration_end:
        delete [] sample;
        p = pow(1.0 - pow(in_frac, min_req), ++k);
    }

    /* calculate final transform based on best consensus set */
    if(consen_max >= consen_min){
        extract_corres_pts(points, projs, consensus_max, consen_max, points_cpy, projs_cpy);
        find_projection_3x4(consen_max, points_cpy, projs_cpy, P);
        consen_size = find_consensus(ori_num, points, projs, P, err_tol, &consensus);

        delete [] consensus_max;
        extract_corres_pts(points, projs, consensus, consen_size, points_cpy, projs_cpy);
        find_projection_3x4(consen_size, points_cpy, projs_cpy, P);
        
        if(inl_ratio){
            *inl_ratio = (double)consen_max/ori_num;
        }
    }
    else if(consensus_max){
        delete [] consensus_max;
    }
    
end:
    return result;
}

/*************************************************************pattern search*********************************************************************/

static double cost_function(int num_pts, const v3_t* points, const v2_t* projs, double K[9], double euler_angle[3], double T[3])
{
    double P[12]; 
    double alpha = euler_angle[0], beta = euler_angle[1], gamma = euler_angle[2];
    double R_alpha[9] = {1, 0, 0, 0, cos(alpha), -sin(alpha), 0, sin(alpha), cos(alpha)};
    double R_beta[9] = {cos(beta), 0, -sin(beta), 0, 1, 0, sin(beta), 0, cos(beta)};
    double R_gamma[9] = {cos(gamma), -sin(gamma), 0, sin(gamma), cos(gamma), 0, 0, 0, 1};
    double temp[12];
    matrix_product33(R_alpha, R_beta, temp);
    matrix_product33(temp, R_gamma, P);
    memcpy(temp+0, P+0, sizeof(double)*3);
    memcpy(temp+4, P+3, sizeof(double)*3);
    memcpy(temp+8, P+6, sizeof(double)*3);
    temp[3] = T[0]; temp[7] = T[1]; temp[11] = T[2];
    matrix_product(3, 3, 3, 4, K, temp, P);
    
    double err = 0;
    for(int i = 0; i < num_pts; i++){
        err += projection_error(points[i], projs[i], P);
    }
    return err;
}

bool bundle::pattern_search_projection(int num_pts, const v3_t* points, const v2_t* projs, double K[9], double euler_angle[3], double T[3], const double* steps, double tol)
{
#define VARNUM          6
    double* base0 = new double[VARNUM];
    memcpy(base0, euler_angle, sizeof(double)*3);
    memcpy(base0+3, T, sizeof(double)*3);
    
//     double orib = cost_function(num_pts, points, projs, K, base0, base0+3);
    
    double* __steps = new double[VARNUM];
    memcpy(__steps, steps, sizeof(double)*VARNUM);
    
    double* base1 = new double[VARNUM];
    double* Tc = new double[VARNUM];
    
    memcpy(Tc, base0, sizeof(double)*VARNUM);
    
//     int cap = VARNUM-1;
    double init_err;
    bool init = true;
    
    double final_err;
    while(true){
        bool valid = false;
        for(int i = 0; i < VARNUM /*&& i <= cap*/; i++){
            double orib = cost_function(num_pts, points, projs, K, Tc, Tc+3);
            if(init){init_err = orib; init = false;}
            final_err = orib;
            
            Tc[i] += __steps[i];
            if(cost_function(num_pts, points, projs, K, Tc, Tc+3) < orib){
                valid = true;
                continue;
            }
            Tc[i] -= __steps[i]*2;
            if(cost_function(num_pts, points, projs, K, Tc, Tc+3) < orib){
                valid = true;
                continue;
            }
            Tc[i] += __steps[i];
        }
        if(valid){
            memcpy(base1, Tc, sizeof(double)*VARNUM);
            for(int i = 0; i < VARNUM; i++){
                Tc[i] = 2*base1[i]-base0[i];
            }
            if(cost_function(num_pts, points, projs, K, Tc, Tc+3) < cost_function(num_pts, points, projs, K, base1, base1+3)){
            }
            else{
                memcpy(Tc, base1, sizeof(double)*VARNUM);
            }
            double* tmp = base0;
            base0 = base1;
            base1 = tmp;
            
//             cap = VARNUM-1;
        }
        else{
//             cap = (cap+1)%VARNUM;
//             __steps[cap] *= .75;
            
            bool go_out = true;
            for(int i = 0; i < VARNUM; i++){
                if((__steps[i] *= .75) > tol){
                    go_out = false;
                }
            }
            if(go_out){
                break;
            }
        }
    }
    
    EX_TRACE("      error(%.6f/%.6f)\n", sqrt(final_err/num_pts), sqrt(init_err/num_pts))
    memcpy(euler_angle, Tc, sizeof(double)*3);
    memcpy(T, Tc+3, sizeof(double)*3);
    
    delete [] Tc;
    delete [] base0;
    delete [] base1;
    delete [] __steps;
}

void bundle::euler_to_R(const double euler_angle[3], double R[9])
{
    double tmp[9]; 
    double alpha = euler_angle[0], beta = euler_angle[1], gamma = euler_angle[2];
    double R_alpha[9] = {1, 0, 0, 0, cos(alpha), -sin(alpha), 0, sin(alpha), cos(alpha)};
    double R_beta[9] = {cos(beta), 0, -sin(beta), 0, 1, 0, sin(beta), 0, cos(beta)};
    double R_gamma[9] = {cos(gamma), -sin(gamma), 0, sin(gamma), cos(gamma), 0, 0, 0, 1};
    matrix_product33(R_alpha, R_beta, tmp);
    matrix_product33(tmp, R_gamma, R); 
}

