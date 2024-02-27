/*
	simplex.c
	Nelder and Mead downhill simplex method for generalized parameter fitting
	Adapted from Numerical Recipes, 2nd edition, Press et al. 1992
	Author: Bernard Heymann
	Created: 20000426	Modified: 20110810		
*/

#include "simplex.h"
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <cstring>

using namespace std;
// Declaration of global variables
extern int 	verbose;		// Level of output to the screen

// Internal function prototypes
double		amotry(Bsimplex* simp, double* mp, double* R, int ihi, double fac,
				double (funk)(Bsimplex *));

/************************************************************************
@Function: simplex
@Reference:
	Press W.H. et al (1992) Numerical Recipes in C.
@Description:
	Estimates parameters using a generalized search algorithm.
@Algorithm:
	Downhill simplex method of Nelder and Mead.
	The limits for each parameter must be set in "lolimit" and "hilimit".
	If the limits are not specified (i.e., the pointers are NULL) they are
	just ignored.
	The function "funk" is user-defined and references the "Bsimplex" structure.
	It returns the "R" value and is called as:
		double	R = (funk)(simplex_struct);
	The objective is to lower the R value calculated in the evaluation function.
	The tolerance or exit condition parameter is related to the evaluation function,
	and must therefore be supplied by the calling function.
@Arguments:
	Bsimplex* simp				simplex structure which holds data and parameters.
	int maxcycles 				maximum number of evaluation cycles to use. 
	double tolerance 			absolute tolerance on the R function. 
	double (funk)(Bsimplex *)	evaluation function returning an R value.
@Returns:
	double						final R value (such as a correlation index).
**************************************************************************/
long 		get_rand_max()
{
	long 		i, rand_max = 32767;
	
	for ( i=0; i<100; i++ )
		if ( random() > rand_max ) rand_max = 2147483647;
	
	return(rand_max);
}
double		simplex(Bsimplex* simp, int maxcycles, double tolerance, 
				double (funk)(Bsimplex *))
{
	int			npar = simp->nparam;
	double*		par = simp->param;
	double*		lolimit = simp->lolimit;
	double*		hilimit = simp->hilimit;
	
	int			i, j, npnt = npar + 1, cycle = 0;
	int			converged = 0, ilo = 0, inhi = 1, ihi = 2;
	int 		rand_max = get_rand_max();	// My own random maximum due to system differences
	double		Rtry, Rsave, reltol = tolerance/10;
	
	if ( maxcycles < 100 ) maxcycles = 100;
	
	srandom(getpid());

	// Initialize the R's
	double*		R = new double[npnt];
	for ( i=0; i<npnt; i++ ) R[i] = 0;
	R[ihi] = 1e30;
	
	// Initialize the simplex multiple point parameter array
	double*		mp = new double[npnt*npar];
	for ( j=0; j<npar; j++ ) mp[j] = par[j];	// Set up the first point
	for ( i=1; i<npnt; i++ ) {					// Set up the other points
		for ( j=0; j<npar; j++ ) {
			if ( lolimit && hilimit ) { 		// Stay within limits
				mp[i*npar+j] = lolimit[j] + (hilimit[j] - lolimit[j])
									*(0.25 + random()*0.5/rand_max);
			} else {							// No limits are specified
				if ( par[j] == 0 )
					mp[i*npar+j] = random()*2.0/rand_max - 1.0;
				else
					mp[i*npar+j] = par[j]*exp(random()*4.0/rand_max-2.0);
			}
		}
	}
	
	
	while ( !converged ) {
		for ( i=0; i<npnt; i++ ) {				// Set up simplex points
			for ( j=0; j<npar; j++ ) par[j] = mp[i*npar+j];
			R[i] = (funk)(simp);
		}
		ilo = ihi = inhi = 0;					// Rank the points
		for ( i=0; i<npnt; i++ ) {
			if ( R[i] <= R[ilo] ) ilo = inhi = i;
			if ( R[i] > R[ihi] ) ihi = i;
		}
		for ( i=0; i<npnt; i++ )
			if ( R[i] >= R[inhi] && i != ihi ) inhi = i;
			
		// Exit here if the process converged
		if ( cycle >= maxcycles || R[ihi] < tolerance ) {
			converged = 1;
		} else if ( (R[ihi]-R[ilo])/R[ilo] < reltol ) {
				// Set the first point to the lowest
			for ( j=0; j<npar; j++ ) mp[j] = mp[ilo*npar+j];
			for ( i=1; i<npnt; i++ ) {			// Randomize other points
				for ( j=0; j<npar; j++ ) {		
					if ( lolimit && hilimit ) { // Stay within limits
						mp[i*npar+j] = lolimit[j] + (hilimit[j] - lolimit[j])
										*(0.25 + random()*0.5/rand_max);
					} else {					// If no limits are specified
						if ( mp[i*npar+j] == 0 )
							mp[i*npar+j] = random()*2.0/rand_max - 1.0;
						else
							mp[i*npar+j] = mp[i*npar+j]*exp(random()*4.0/rand_max-2.0);
					}
				}
			}
		} else {
					// Reflect the simplex opposite the highest point
			Rtry = amotry(simp, mp, R, ihi, -1.0, funk);
					// If it is better, try an additional extrapolation
			if ( Rtry <= R[ilo] )
				Rtry = amotry(simp, mp, R, ihi, 2.0, funk);
					// If it is worse, contract around the lowest point
			else if ( Rtry >= R[inhi] ) {
				Rsave = R[ihi];
				Rtry = amotry(simp, mp, R, ihi, 0.5, funk);
				if ( Rtry >= Rsave ) {
					for ( i=0; i<npnt; i++ ) {
						for ( j=0; j<npar; j++ ) {
							par[j] = 0.5*(mp[i*npar+j] + mp[ilo*npar+j]);
							mp[i*npar+j] = par[j];
						}
						R[i] = (funk)(simp);
					}
				}
			}
		}
		
		cycle++;
	}
	
	
	
	for ( j=0; j<npar; j++ ) par[j] = mp[ilo*npar+j];
	
	Rsave = R[ilo];
	
	delete[] R;
	delete[] mp;
	

	return(Rsave);
}

/*
@Function: amotry
@Reference:
	Press W.H. et al (1992) Numerical Recipes in C.
@Description:
	Generates new points in parameter space for the simplex function.
@Algorithm:
	Downhill simplex method of Nelder and Mead.
@Arguments:
	Bsimplex* simp	simplex structure which holds data and parameters.
	double* mp		multiple point matrix.
	double* R		R vector.
	int ihi 		index of highest R.
	double fac		direction and magnitude of simplex modification.
	double (funk)(Bsimplex *)	evaluation function returning an R value.
@Returns:
	double 			an R value.
**************************************************************************/
double		amotry(Bsimplex* simp, double* mp, double* R, int ihi, double fac,
				double (funk)(Bsimplex *))
{
	int			npar = simp->nparam;
	double*		par = simp->param;
	double*		lolimit = simp->lolimit;
	double*		hilimit = simp->hilimit;
	
	int			i, j, npnt = npar + 1;
	double		psum, pnew, Rtry;
	double		fac1 = (1 - fac)/npar;
	double		fac2 = fac1 - fac;
	
	for ( j=0; j<npar; j++ ) {
		psum = 0;
		for ( i=0; i<npnt; i++ ) psum += mp[i*npar+j];
		par[j] = mp[ihi*npar+j];
		pnew = psum*fac1 - mp[ihi*npar+j]*fac2;
		if ( lolimit ) if ( pnew < lolimit[j] ) pnew = par[j];
		if ( hilimit ) if ( pnew > hilimit[j] ) pnew = par[j];
		par[j] = pnew;
	}
	
	Rtry = (funk)(simp);
	
	if ( Rtry <= R[ihi] ) {
		R[ihi] = Rtry;
		for ( j=0; j<npar; j++ ) mp[ihi*npar+j] = par[j];
	}
	
	return(Rtry);
}

/************************************************************************
@Function: init_simplex
@Description:
	Initializes a Bsimplex structure.
@Algorithm:
	The minimum setup of the simplex structure requires a number of
	parameters greater than 0
@Arguments:
	int nvar		number of independent variables.
	int nparam		number of parameters to determine.
	int nconstant	number of constants.
	int npoint	 	number of points or function values.
@Returns:
	Bsimplex* simp	initialized and allocated simplex structure.
**************************************************************************/
Bsimplex*	init_simplex(int nvar, int nparam, int nconstant, int npoint)
{
	if ( nparam < 1 ) {
		error_show("Error in init_simplex: Zero parameters!", __FILE__, __LINE__);
		return(NULL);
	}
	
	unsigned long	i;
	
	Bsimplex*	simp = new Bsimplex;
	simp->nvar = nvar;
	simp->nparam = nparam;
	simp->nconstant = nconstant;
	simp->npoint = npoint;

	if ( npoint && nvar ) {
		simp->x = new float[simp->npoint*simp->nvar];
		for ( i=0; i<simp->npoint*simp->nvar; i++ ) simp->x[i] = 0;
	}
	
	if ( npoint ) {
		simp->fx = new float[simp->npoint];
		for ( i=0; i<simp->npoint; i++ ) simp->fx[i] = 0;
	}
	
	simp->param = new double[simp->nparam];
	simp->lolimit = new double[simp->nparam];
	simp->hilimit = new double[simp->nparam];
	
	for ( i=0; i<simp->nparam; i++ )
		simp->param[i] = simp->lolimit[i] = simp->hilimit[i] = 0;
	
	if ( nconstant ) {
		simp->constant = new double[simp->nconstant];
		for ( i=0; i<simp->nconstant; i++ ) simp->constant[i] = 0;
	}
	
	return(simp);
}

/************************************************************************
@Function: kill_simplex
@Author: Dan Krainak & Bernard Heymann
@Description:
	Frees all memory associated with a Bsimplex structure.
@Algorithm:
	If x or fx were recast as a pointer different from float and not
	using npoint and nvar to specify their sizes, x or fx has to be
	set to NULL before calling kill_simplex.
@Arguments:
	Bsimplex* simp	simplex structure which holds data and parameters.
@Returns:
	int 			0.
**************************************************************************/
int 		kill_simplex(Bsimplex* simp)
{

	if (simp == NULL)	return(0);
	
	if ( simp->npoint && simp->nvar && simp->x )
		delete[] simp->x;
	
	if ( simp->npoint && simp->fx )
		delete[] simp->fx;
	
	if ( simp->nparam && simp->lolimit )
		delete[] simp->lolimit;
	
	if ( simp->nparam && simp->hilimit )
		delete[] simp->hilimit;
	
	if ( simp->nparam && simp->param )
		delete[] simp->param;
	
	if ( simp->nconstant && simp->constant )
		delete[] simp->constant;
	
	delete simp;
	
	return(0);
} // end kill_simplex

void printctime(FILE* ft)
{
	time_t curtime;
	time(&curtime);
	fprintf(ft,ctime(&curtime));
}

void printutime(FILE* ft,time_t begint,time_t endt)
{
	int use=difftime(endt,begint);
	int min=(use/60)%60;
	int sec=use%60;
	int hour=use/3600;
	fprintf(ft,"use time:%02d:%02d:%02d\n",hour,min,sec);

}

int jacobian_rotation(int n, double* a, double* d, double* v)
{
    int			j, iq, ip, i, nrot = 0;
    double		tresh, theta, tau, t, sm, s, h, g, c;

    for ( ip=i=j=0; ip<n; ip++ )
    {
        for ( iq=0; iq<n; iq++, i++ )
        {
            v[i] = 0.0;
            j += ( fabs(a[i]) < 1e-12 );	// Check if small to prevent too many iterations bailout
        }
        v[ip*n+ip] = 1.0;
        d[ip] = a[ip*n+ip];
    }

    if ( j == n*n ) return(0);			// The matrix is zero

    double* b=(double*)malloc(sizeof(double)*n);
    double* z=(double*)malloc(sizeof(double)*n);
// 	double*		b = new double[n];
// 	double*		z = new double[n];

    for ( ip=0; ip<n; ip++ ) {
        b[ip] = d[ip];
        z[ip] = 0;
    }

    for ( i=1; i<=50; i++ ) {
        sm=0.0;
        for ( ip=0; ip<n-1; ip++ ) {
            for ( iq=ip+1; iq<n; iq++ )
                sm += fabs(a[ip*n+iq]);
        }
        if ( sm == 0.0 ) {
            free(b);
            free(z);
// 			delete[] b;
// 			delete[] z;
            return(nrot);						// Function exit
        }
        if ( i < 4 )
            tresh=0.2*sm/(n*n);
        else
            tresh=0.0;
        for ( ip=0; ip<n-1; ip++ ) {
            for ( iq=ip+1; iq<n; iq++ ) {
                g = 100.0*fabs(a[ip*n+iq]);
                if ( i > 4 && (float)(fabs(d[ip])+g) == (float)fabs(d[ip])
                        && (float)(fabs(d[iq])+g) == (float)fabs(d[iq]))
                    a[ip*n+iq] = 0.0;
                else if ( fabs(a[ip*n+iq]) > tresh ) {
                    h = d[iq] - d[ip];
                    if ( (float)(fabs(h)+g) == (float)fabs(h))
                        t = a[ip*n+iq]/h;
                    else {
                        theta = 0.5*h/a[ip*n+iq];
                        t = 1.0/(fabs(theta)+sqrt(1.0+theta*theta));
                        if (theta < 0.0) t = -t;
                    }
                    c = 1.0/sqrt(1+t*t);
                    s = t*c;
                    tau = s/(1.0+c);
                    h = t*a[ip*n+iq];
                    z[ip] -= h;
                    z[iq] += h;
                    d[ip] -= h;
                    d[iq] += h;
                    a[ip*n+iq] = 0.0;
                    for ( j=0; j<ip; j++ ) {
                        rotate(n, a, s, tau, j, ip, j, iq);
                    }
                    for ( j=ip+1; j<iq; j++ ) {
                        rotate(n, a, s, tau, ip, j, j, iq);
                    }
                    for ( j=iq+1; j<n; j++ ) {
                        rotate(n, a, s, tau, ip, j, iq, j);
                    }
                    for ( j=0; j<n; j++ ) {
                        rotate(n, v, s, tau, j, ip, j, iq);
                    }
                    nrot++;
                }
            }
        }
        for ( ip=0; ip<n; ip++ ) {
            b[ip] += z[ip];
            d[ip] = b[ip];
            z[ip] = 0.0;
        }
    }
    error_show("Error in jacobian_rotation", __FILE__, __LINE__);
    cerr << "Too many iterations (" << i << ")" << endl;

    for ( ip=i=0; ip<n; ip++ )
    {
        for ( iq=0; iq<n; iq++, i++ ) v[i] = 0.0;
        v[ip*n+ip] = 1.0;
        d[ip] = 0;
    }

    return(-1);

}

int rotate(int n, double* a, double s, double tau, int i, int j, int k, int l)
{
    double	g, h;

    g = a[i*n+j];
    h = a[k*n+l];
    a[i*n+j] = g - s*(h + g*tau);
    a[k*n+l] = h + s*(g - h*tau);

    return(0);

}

int error_show(const char* message, const char* file, int line)
{
    char		string[1024];

    sprintf(string, "[%s:%d] %s", file, line, message);

    perror(string);

    if ( errno ) cerr << "errno " << errno << ": ";

    switch ( errno ) {
    case ENOENT:
        cerr << "Make sure the file exists and the path to the file is correct" << endl;
        break;
    case EACCES:
        cerr << "Make sure you have permission to write to this directory" << endl;
        break;
    case ENOMEM:
        cerr << "The memory available on this computer may be insufficient" << endl;
        break;
    default:
        cerr << endl;
    }

    return(errno);

}

void eigen_sort(int n, double* d, double* v)
{
    int	k,j,i;
    double		p;

    for ( i=0; i<n-1; i++ )
    {
        p = d[k=i];
        for ( j=i+1; j<n; j++ )
            if ( d[j] >= p ) p = d[k=j];
        if ( k != i )
        {
            d[k] = d[i];
            d[i] = p;
            for ( j=0; j<n; j++ )
            {
                p = v[j*n+i];
                v[j*n+i] = v[j*n+k];
                v[j*n+k] = p;
            }
        }
    }

}

string extract_name(string infile)
{
	string txt=infile;		
	int pos1=txt.find_last_of("/");
	txt=txt.substr(pos1+1);
	int pos2=txt.find(".");		
	txt=txt.substr(0,pos2);
	
	return txt;
}

string extract_path(string infile)
{
	int pos=infile.find_last_of("/");
	string path=infile.substr(0,pos+1);
	
	return path;
}

string extract_pname(string infile)
{
	int pos=infile.find(".");
	string pname=infile.substr(0,pos);
	
	return pname;
}

float ncc(float* test, float* data, int width, int height)
{
	float *tmp1,*tmp2,dataavg,testavg,t1,t2,t3;

	tmp1 = new float[width*height];
	tmp2 = new float[width*height];
	memset(tmp1, 0, sizeof(float)*width*height);
	memset(tmp2, 0, sizeof(float)*width*height);

	dataavg = testavg = 0;
	int count = 0;

	float blankv = 0;			

	for (int i = 0 ; i < width*height; i++)
	{
		if(data[i]==0||test[i]==blankv)
		{
			continue;
		}
		dataavg += data[i];
		testavg += test[i];
		count++;
	}

	dataavg /= count;
	testavg /= count;

	for (int i = 0 ; i < width*height; i++)
	{
		if(data[i]==0||test[i]==blankv)
		{
			continue;
		}
		tmp1[i] = data[i] - dataavg;
		tmp2[i] = test[i] - testavg;
	}

	t1 = t2 = t3 =0;
	for (int i = 0 ; i < width*height; i++)
	{
		t1+=tmp1[i]*tmp2[i];
		t2+=tmp1[i]*tmp1[i];
		t3+=tmp2[i]*tmp2[i];
	}

	delete [] tmp1;
	delete [] tmp2;

	return t1/sqrt(t2*t3);
}
