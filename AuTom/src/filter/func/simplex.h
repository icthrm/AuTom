/*
	simplex.h
	Nelder and Mead downhill simplex method for generalized parameter fitting
	Adapted from Numerical Recipes, 2nd edition, Press et al. 1992
	Author: Bernard Heymann
	Created: 20000426	Modified: 20041011
	
	The function "funk" is user-defined and references the "Bsimplex" structure.
	It returns the "R" value and is called as:
		double	R = (funk)(simplex_struct);
*/

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <math.h>

/************************************************************************
@Object: struct Bsimplex
@Description:
	Structure used in the downhill simplex method.
@Features:
	Nelder and Mead downhill simplex method for generalized parameter fitting
	Adapted from Numerical Recipes, 2nd edition, Press et al. 1992
	The structure is set up to accommodate any number of variables, parameters,
	constants(常量) and points.
	The structure is very flexible in the sense that only some fields
	are absolutely required and with a fixed meaning for the simplex method.
	The required fields are:
		nparam, param, lolimit, hilimit
	The other fields may be recast and used as desired in the user function.
	Intended sizes:
		param		nparam.
		lolimit 	nparam.
		hilimit 	nparam.
		constant	nconstant.
		x			npoint*nvar.
		fx			npoint.
	x or fx can be recast as a different pointer, as long as it is handled
	by the user before calling kill_simplex.
*************************************************************************/
#ifndef _Bsimplex_
struct Bsimplex {
	int		nvar;		// Number of variables
	int		nparam;		// Number of parameters
	int		nconstant;	// Number of constants
	int		npoint; 	// Number of function values
	double*	param;		// Parameter values
	double*	lolimit;	// Lower limits on parameter values
	double*	hilimit;	// Upper limits on parameter values
	double*	constant;	// Constant values
	float*	x;			// Independent variables: npoint*nvar array
	float*	fx;			// Function values
} ;
#define _Bsimplex_
#endif

// Function prototypes
double	simplex(Bsimplex* simp, int maxcycles, double tolerance, double (funk)(Bsimplex *));//~
Bsimplex*	init_simplex(int nvar, int nparam, int nconstant, int npoint);//~
int 		kill_simplex(Bsimplex* simp);//~


void		printctime(FILE* ft);
void 		printutime(FILE* ft,time_t begint,time_t endt);
/***求特征值***/
int		jacobian_rotation(int n, double* a, double* d, double* v);
/***特征向量排序***/
void		eigen_sort(int n, double* d, double* v);
int		rotate(int n, double* a, double s, double tau, int i, int j, int k, int l);
int		error_show(const char* message, const char* file, int line);
/***文件名操作***
 * ex_name:从传入路径+文件名中（文件名中不得有字符“/”和“.")提取无后缀文件名
 * extract_path:提取文件路径
 * extract_pname：提取文件路径+名称无后缀
 */
std::string 	extract_name(std::string infile);
std::string 	extract_path(std::string infile);
std::string 	extract_pname(std::string infile);

/***计算相关系数***
 *
 * initMat:float型
 */
float		ncc(float * test , float * data , int width , int height);
// Scalar	getMSSIM( const Mat& i1, const Mat& i2);
