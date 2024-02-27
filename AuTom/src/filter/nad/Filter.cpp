 
#define MIN_SIZE_FOR_THREADING 100000

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>
#include <omp.h>
#include "Filter.h"
#include "filter/func/simplex.h"
#include <cstring>
#include <malloc.h>
#include <sstream>
#include <climits>
#include <string>
// #include <cv.hpp>

using namespace std;

extern int 	verbose;
double	img_fit_gauss_to_his_R(Bsimplex* simp);

Filter::Filter()
{

}

Filter::Filter(int x, int y, int z,int type,unsigned char* data)
{
	nx=x;
	ny=y;
	nz=z;
	datatype=type;
	datasize=nx*ny*nz;
	imgsize=nx*ny;
	d.uc=data;
	//update_statistic();
}

Filter::Filter(float* data, int width, int height, int depth)
{
	d.f=data;
	nx=width;
	ny=height;
	nz=depth;
	datatype=2;
	datasize=nx*ny*nz;
	update_statistic();

}

Filter::~Filter()
{
	if ( d.uc ) delete[] d.uc;
	d.uc = NULL;
}

/*
 * 读取mrc文件，tag为是否读取数据标志：1--读取，0--不读取
 */

int Filter::readmrc(const char* infname,int tag)
{
    in.open(infname);
    if(!in.good())
    {
	cout<<"The input file error!"<<endl;
        return false;
    }
    if(header)
    {
        delete header;
    }
    header = new MRCheader();

    //读取文件头部
    in.read((char*)header, sizeof(MrcHeader));
    int readsize=header->nx*header->ny*header->nz;
    //读取数据
    int type=header->mode;
    /*
     * 0:byte
     * 1:short
     * 2:float
     */
    if(tag)
    {
// 	  cout<<"begin read data!"<<endl;
        in.seekg(MRC_HEADER_SIZE+header->next, std::ios::beg);
        switch(type)
        {
	      case 0:
		  d.uc=new unsigned char[readsize];
		  in.read((char*)d.uc, readsize);
		  break;
	      case 1:
		  d.uc=new unsigned char[readsize*sizeof(short)];
		  in.read((char*)d.ss, readsize*sizeof(short));
		  break;
	      case 2:
		  d.uc=new unsigned char[readsize*sizeof(float)];
		  in.read((char*)d.f, readsize*sizeof(float));
		  break;
	      default:
		  cout<<"Unknown mrc type!"<<endl;
		  break;
        }
    }

    //数据赋值
    nx=header->nx;
    ny=header->ny;
    nz=header->nz;
    datasize=header->nx*header->ny*header->nz;
    imgsize=nx*ny;
    fmean=header->amean;
    fmax=header->amax;
    fmin=header->amin;
    fstd=header->rms;
    //cout<<"header:fmean:"<<fmean<<"\tfstd:"<<fstd<<"\tfmax:"<<fmax<<"\tfmin:"<<fmin<<endl;
    tag=0;
    datatype=header->mode;//0/1/2
    update_statistic();	  
    
}

int Filter::writemrc(const char* oufname)
{
      if(out) out.close();
      out.open(oufname);
      out.seekp(std::ios::beg);
   
	if(header)	update_header();
      out.write((char*)header, MRC_HEADER_SIZE);
      int writesize=header->nx*header->ny*header->nz;
      int type=header->mode;
      //转换类型前无法写成一种格式
      if(!d.uc)
      {
	      cout<<"writemrc:no data！"<<endl;
}
      switch(type)
      {
	    case 0:
	      out.write((char*)d.uc,writesize);
	      break;
	    case 1:
	      out.write((char*)d.ss,writesize*sizeof(short));
	      break;
	    case 2:
	      out.seekp(sizeof(MRCheader)+header->next,std::ios::beg);
	      out.write((char*)d.f,writesize*sizeof(float));
	      break;
	    default:
	      cout<<"error in writemrc!"<<endl;
	      break;
      }

      out.close();
      cout<<"end writemrc\n";
      return 0;
}

int Filter::update_statistic()
{
	if(!d.uc)
      {
	    cout<<"update_statistic:no data!\n";
	    return 1;
      }
      
	double	imin=1e37, imax=-1e37,oavg=0,ostd=0;
      if ( imin < data_type_min() ) imin = data_type_min();
      if ( imax > data_type_max() ) imax = data_type_max();

      float sumavg=0,sumstd=0;
      int k=0;
      double v;
      
	//cout<<"old mean:"<<fmean<<endl;
      for(int i=0;i<nz;i++)
      {
	    for(int j=0;j<imgsize;j++,k++)
	    {		 
		  v=(*this)[k];
		  oavg+=v;
		  ostd+=v*v;
		  if(imax<v) imax=v;			
		  if(imin>v) imin=v;			
	    }
	    oavg=oavg/imgsize;//每张的avg
	    ostd = ostd/imgsize - oavg*oavg;//每张的std
	    if ( ostd > 0 ) ostd = sqrt(ostd);
	    else ostd = 0;
	    
	    sumavg+=oavg;
	    sumstd+=ostd*ostd;
	    oavg=0;
	    ostd=0;
	    
	    /*
	    if(i>0)
	    {
		  sum=sum/imgsize;
		  iavg=(iavg+sum)/2;
		  istd+=ostd;
	    }
	    else
	    {
		  iavg=sum/imgsize;
	    }
	    sum=0;*/
	    
      }
      fmean=sumavg/nz;
	if ( sumstd > 0 ) fstd = sqrt(sumstd/nz);
	else fstd = 0;
	//cout<<"new mean:"<<fmean<<endl;
	fmax=imax;
	fmin=imin;
	//cout<<"update:fmean:"<<fmean<<"\tfstd:"<<fstd<<"\tfmax:"<<fmax<<"\tfmin:"<<fmin<<endl;
	
	return 0;

}

int Filter::update_header()
{
	update_statistic();
	header->amean=fmean;
	header->amax=fmax;
	header->amin=fmin;
	header->mode=datatype;
	header->rms=fstd;

}


double Filter::operator[](long unsigned int j) const
{
	if ( j >= datasize ) return 0;
	
	switch ( datatype ) 
	{		
		case MRC_MODE_BYTE:	return d.uc[j];
		case MRC_MODE_SHORT:	return d.ss[j];
		case MRC_MODE_FLOAT:	return d.f[j];
		default:		return 0;
	}
}


long Filter::struct_tensor(int width, int height, int depth, double amean,float* mdata,float *&st,long zs,long zw)
{
// 	cout<<"传入：  zs:"<<zs<<"   zw:"<<zw<<endl;
// 	cout<<"begin compute struct_tensor"<<endl;		
	double 	dx, dy, dz;
	int 	x,y,z=zs,i=0,k=0,j,zf;
	int 	size=width*height;	
	
	if ( zs > 0 ) zw++;
	if ( zs+zw < depth ) zw++;
	long 	r_size=width*height*zw;
// 	cout<<"rsize:"<<r_size<<endl;
	
	if ( zs > 0 ) zs--;
	//分配线程各自空间
	if (!(st = (float*)malloc(r_size*sizeof(float)*6))) printf("Not Enough Memory!\n");
// 	cout<<"new后st:"<<st<<endl;
	z=zs;
	zf=zs+zw;
// 	cout<<"start(z):"<<z<<"  end(zf):"<<zf<<"   i:"<<i<<endl;
	
	for (k=0, i=size*zs; z<zf; z++ ) 
// 	for(z=0; z<depth; z++)
	{
		for(y=0; y<height; y++)
		{
		    for(x=0; x<width; x++,i++)
		    {
		        dx = dy = dz = -amean;
		        if ( x > 0 )
		            dx = -mdata[i-1];
		        if ( x < width-1 )
		            dx += mdata[i+1];
		        else
		            dx += amean;
		        if ( y > 0 )
		            dy = -mdata[i-width];
		        if ( y < height-1 )
		            dy += mdata[i+width];
		        else
		            dy += amean;
		        if ( z > 0 )
		            dz = -mdata[i-size];
		        if ( z < depth-1 )
		            dz += mdata[i+size];
		        else
		            dz += amean;

		        dx /= 2;
		        dy /= 2;
		        dz /= 2;
		        st[k++]= dx*dx;
		        st[k++]= dx*dy;
		        st[k++]= dx*dz;
		        st[k++]= dy*dy;
		        st[k++]= dy*dz;
		        st[k++]= dz*dz;
			
		    }
		}
		
	}

	return r_size;
}

void Filter::diffusion_tensor(float* st, float alpha,float C, float lambda,long tempsize)
{
    double	lam[3] = {1,1,1};	/* eigenvalues of diffusion tensor */
    double	d[3];			/* vector with unordered eigenvalues（特征值） */
    double	a[9];			/* real tensor matrix（结构张量矩阵J0） */
    double	v[9];			/* matrix with unordered eigenvectors（特征向量矩阵） */
    double	beta =1 - alpha;	/* time saver */
    double	thexp = 16, grd, gol, denom;
    int 	j,i;       

//     cout<<"imgsize:"<<tempsize<<endl;
    for ( i=0; i<tempsize; i++ )
    {
        j = i*6;
        a[0] = st[j++];
        a[1] = a[3] = st[j++];
        a[2] = a[6] = st[j++];
        a[4] = st[j++];
        a[5] = a[7] = st[j++];
        a[8] = st[j++];
        //		cout << i << "\t" << a[0] << "\t" << a[1] << "\t" << a[2] << "\t" << a[4] << "\t" << a[5] << "\t" << a[8] << endl;

        /* calculate third eigenvalue */
        grd = a[0] + a[4] + a[8];
        //计算出d：特征值，v：特征向量，第一个参数为矩阵大小3×3,a为要计算特征值的矩阵
        jacobian_rotation(3, a, d, v);
        eigen_sort(3, d, v);//使特征值从大到小排列并排序其对应特征向量

        if ( C ) //若指定CED参数c
        {
            // for plane-like structures, calculate the second eigenvalue
            lam[0] = lam[1] = lam[2] = alpha;
            denom = d[0] - d[1];
            if ( denom && denom > d[2] && ( denom > d[1] - d[2] ) )
                lam[1] += beta * exp (- C / (denom*denom));
            /* calculate third eigenvalue */
            denom = d[0] - d[2];
            if ( denom ) lam[2] += beta * exp (- C / (denom*denom));
        }
        else //若为EED
        {
            if ( grd > 0.0) //必>0
            {
                gol = sqrt(grd)/lambda;
                lam[0] = lam[1] = 1.0 - exp (-3.31488 / pow(gol, thexp));
            }
            else
            {
                lam[0] = lam[1] = 1;
            }
        }

        /* principal axis backtransformation（倒置） */
        j = i*6;//重新计数，计算后pd内存有扩散系数矩阵G（对称矩阵）
        st[j++]=lam[0]*v[0]*v[0] + lam[1]*v[1]*v[1] + lam[2]*v[2]*v[2];
        st[j++]=lam[0]*v[0]*v[3] + lam[1]*v[1]*v[4] + lam[2]*v[2]*v[5];
        st[j++]=lam[0]*v[0]*v[6] + lam[1]*v[1]*v[7] + lam[2]*v[2]*v[8];
        st[j++]=lam[0]*v[3]*v[3] + lam[1]*v[4]*v[4] + lam[2]*v[5]*v[5];
        st[j++]=lam[0]*v[3]*v[6] + lam[1]*v[4]*v[7] + lam[2]*v[5]*v[8];
        st[j++]=lam[0]*v[6]*v[6] + lam[1]*v[7]*v[7] + lam[2]*v[8]*v[8];

    }
    //return st;
}

void Filter::diffuse(float* st, float* odata,float* temp,int width,int height,int depth,long rsize,long zs,float ht)
{
// 	cout<<"传入：  zs:"<<zs<<endl;
	unsigned long	zf, ii, ik=0;
	long	iw, xw, yw,zw, ix, iy, iz,xf1,yf1,zf1,zs1;
	double	w[27], value;
	double	r = ht/4;
	int x,y,z,i=0,k=0,j;
	unsigned long 	size=width*height;
	long r_depth=rsize/size;
// 	cout<<"r_depth:"<<r_depth<<endl;
	
	xf1=width - 1;
	yf1=height-1;

	zs1 = zs;
	ik = 0;
	if ( zs > 0 ) 
	{	// The diffusion tensor reference starts one slice in
		ik = size;
		zs1 = zs - 1;
	}
	zf = zs1 + r_depth;
	zf1 = zf - 1;
	if ( zf < depth ) zf -= 1;
	if ( zf > depth ) zf = depth;

// 	cout << "pd->z=" << r_depth << " zs=" << zs << " zs1=" << zs1 << " zf=" << zf << " zf1=" << zf1 << endl;

	for ( z=zs, i=size*zs; z<zf; z++ ) 
// 	for ( z=0, i=0; z<depth; z++ )
	{
// 		if(z!=0)
// 			cout<<"z:"<<z<<endl;
		for ( y=0; y<height; y++ )
		{
		    for ( x=0; x<width; x++, i++, ik++ )
		    {
		        for ( iw=0; iw<27; iw++ ) w[iw] = 0;

		        k = 6*ik;
		        w[12] = w[14] = 2*st[k] - fabs(st[k+1]) - fabs(st[k+2]);
		        w[10] = w[16] = 2*st[k+3] - fabs(st[k+1]) - fabs(st[k+4]);
		        w[4]  = w[22] = 2*st[k+5] - fabs(st[k+4]) - fabs(st[k+2]);

		        w[9]  = w[17] =  st[k+1] + fabs(st[k+1]);
		        w[15] = w[11] = -st[k+1] + fabs(st[k+1]);

		        w[3]  = w[23] =  st[k+2] + fabs(st[k+2]);
		        w[5]  = w[21] = -st[k+2] + fabs(st[k+2]);

		        w[1]  = w[25] =  st[k+4] + fabs(st[k+4]);
		        w[7]  = w[19] = -st[k+4] + fabs(st[k+4]);

		        if ( x > 0 ) {
		            k = 6*(ik - 1);
		            w[12] += 2*st[k] - fabs(st[k+1]) - fabs(st[k+2]);
		        }
		        if ( x < xf1 ) {
		            k = 6*(ik + 1);
		            w[14] += 2*st[k] - fabs(st[k+1]) - fabs(st[k+2]);
		        }

		        if ( y > 0 ) {
		            k = 6*(ik - width);
		            w[10] += 2*st[k+3] - fabs(st[k+1]) - fabs(st[k+4]);
		        }
		        if ( y < yf1 ) {
		            k = 6*(ik + width);
		            w[16] += 2*st[k+3] - fabs(st[k+1]) - fabs(st[k+4]);
		        }

		        if ( z > 0 ) {
		            k = 6*(ik - size);
		            w[4] += 2*st[k+5] - fabs(st[k+4]) - fabs(st[k+2]);
		        }
		        if ( z < zf1 ) {
		            k = 6*(ik + size);
		            w[22] += 2*st[k+5] - fabs(st[k+4]) - fabs(st[k+2]);
		        }

		        if ( x > 0 && y > 0 ) {
		            k = 6*(ik - width - 1) + 1;
		            w[9] += st[k] + fabs(st[k]);
		        }
		        if ( x < xf1 && y > 0 ) {
		            k = 6*(ik - width + 1) + 1;
		            w[11] += -st[k] + fabs(st[k]);
		        }
		        if ( x > 0 && y < yf1 ) {
		            k = 6*(ik + width - 1) + 1;
		            w[15] += -st[k] + fabs(st[k]);
		        }
		        if ( x < xf1 && y < yf1 ) {
		            k = 6*(ik + width + 1) + 1;
		            w[17] += st[k] + fabs(st[k]);
		        }

		        if ( x > 0 && z > 0 ) {
		            k = 6*(ik - size - 1) + 2;
		            w[3] += st[k] + fabs(st[k]);
		        }
		        if ( x < xf1 && z > 0 ) {
		            k = 6*(ik - size + 1) + 2;
		            w[5] += -st[k] + fabs(st[k]);
		        }
		        if ( x > 0 && z < zf1 ) {
		            k = 6*(ik + size - 1) + 2;
		            w[21] += -st[k] + fabs(st[k]);
		        }
		        if ( x < xf1 && z < zf1 ) {
		            k = 6*(ik + size + 1) + 2;
		            w[23] += st[k] + fabs(st[k]);
		        }

		        if ( y > 0 && z > 0 ) {
		            k = 6*(ik - width - size) + 4;
		            w[1] += st[k] + fabs(st[k]);
		        }
		        if ( y < yf1 && z > 0 ) {
		            k = 6*(ik + width - size) + 4;
		            w[7] += -st[k] + fabs(st[k]);
		        }
		        if ( y > 0 && z < zf1 ) {
		            k = 6*(ik - width + size) + 4;
		            w[19] += -st[k] + fabs(st[k]);
		        }
		        if ( y < yf1 && z < zf1 ) {
		            k = 6*(ik + width + size) + 4;
		            w[25] += st[k] + fabs(st[k]);
		        }

		        /* modify weights to prevent flux across boundaries */
		        if ( x==0 ) w[15] = w[12] = w[9] = w[3] = w[21] = 0.0;
		        if ( x==xf1 ) w[11] = w[14] = w[17] = w[5] =w[23] = 0.0;
		        if ( y==0 ) w[9] = w[10] = w[11] = w[1] = w[19] = 0.0;
		        if ( y==yf1 ) w[17] = w[16] = w[15] = w[7] = w[25] = 0.0;
		        if ( z==0 ) w[7] = w[4] = w[1] = w[5] = w[3] = 0.0;
		        if ( z==zf1 )  w[23] = w[22] = w[21] = w[1] =w[25] = 0.0;

		        /* evolution */
		        value = odata[i];

		        for ( zw=0; zw<3; zw++ )
		        {
		            iz = (long)z + zw - 1;
		            if ( iz >= 0 && iz < depth )
		                for ( yw=0; yw<3; yw++ )
		                {
		                    iy = (long)y + yw - 1;
		                    if ( iy >= 0 && iy < height )
		                        for ( xw=0; xw<3; xw++ )
		                        {
		                            ix = (long)x + xw - 1;
		                            if ( ix >= 0 && ix < width )
		                            {
		                                iw = 9*zw + 3*yw + xw;
		                                if ( w[iw] )
		                                {
		                                    ii = (iz*height + iy)*width + ix;
		                                    value += w[iw]*r*(odata[ii] - odata[i]);
		                                }
		                            }
		                        }
		                }
		        }
			if(value)
				temp[i]=value;
			else
				cout<<"value not exist:"<<i<<"!\n";
		    }
		}
	}
	
	
}

int	Filter::img_nad_chunk(int width,int height,int depth,float lambda, float C, float alpha, float ht, long zs, long zw,float* odata,float* temp)
{

	if ( zs+zw > depth ) zw = depth - zs;
	
 	//cout << "Chunk start = " << zs << " and size = " << zw << endl;	
	
	/*共需要：1.输入文件原数据，2.分块数据，3.输出文件新数据*/
	
	float* 	st;
// 	long imgsize=width*height*depth;
	long rsize;
	
	//计算结构张量
	rsize=struct_tensor(width,height,depth,fmean,odata,st,zs,zw);

	diffusion_tensor(st, alpha,C,lambda,rsize);
	
	diffuse(st,odata,temp,width,height,depth,rsize,zs,ht);
	
	delete [] st;

	return 0;
}

int Filter::get_slab_size(int depth)
{
	int	np = 1;
  #ifdef OMP_H
      np = omp_get_num_procs();
  #endif
	int	slabsize=depth/np+1;
	return	slabsize;

}

void Filter::nad3d()
{
	cout<<"begin nad3d\n";
	if(nx||ny||nz||d.f)
		nad3d(nx,ny,nz,d.f);
	update_statistic();
	cout<<"end nad3d\n";
}

void Filter::nad3d(int nx, int ny, int nz, float*& odata)
{
	float	alpha=0;
	float	C=0;
	float   lambda=10;
	float	ht=0.1;
	long	z,slabsize=get_slab_size(nz);
	//long 	imgsize=nx*ny*nz;
	float*  temp=new float[datasize];
	
	for(int i=0;i<datasize;i++)
		temp[i]=odata[i];	

	int i=0;
 	//cout<<"slabsize:"<<slabsize<<endl;
#pragma omp parallel for	
	for ( z=0; z<nz; z+=slabsize )      
 	    img_nad_chunk(nx,ny,nz,lambda, C, alpha, ht, z, slabsize, odata,temp);        

	//cout<<"chunk end\n";
	
 	if(odata) delete [] odata;
	odata=temp;
	//cout<<"odata:"<<odata<<"   d.f:"<<d.f<<endl;
	//cout<<"nad end\n";
}

void Filter::nad2d(float* data, int width, int height)
{
      //参数定义
      double alpha=0.01,c=0.01;
      //1.计算img_structure_tensor
      //结构张量矩阵
      float *st;
      long imgsize=width*height;
      //一个点对应一个2×2矩阵，3个不同值
      st = (float*)malloc(imgsize*sizeof(float)*3);
      double dx, dy;
      /**
      * i代表图像点序号
      * k为st的序号，一个i对应3个k
      */
      int x,y,i=0,k=0;
      for(y=0;y<height;y++)
      {
	  for(x=0;x<width;x++,i++)
	  {
		  dx=dy=-header->amean;
		  
		  if(x>0)   //非每行的第一个点
			  dx=-data[i-1];
		  if(x<width-1)		//非每行的最后一个点
			  dx+=data[i+1];
		  else
			  dx+=header->amean;
		  
		  if(y>0)		//非图片的第一行
			  dy=-data[i-width];
		  if(y<height-1)		//非图片的最后一行
			  dy+=data[i+width];
		  else
			  dy+=header->amean;
		  
		  dx /= 2;
		  dy /= 2;
		  st[k++]=dx*dx;
		  st[k++]=dx*dy;
		  st[k++]=dy*dy;
	  }
      }
      
      //2.计算img_diffusion_tensor
      double d[2];	//特征值
      double a[4];	//原矩阵
      double v[4];	//特征向量矩阵
      long j;

      double grd,gol,thexp = 16,lambda=0.3;
      double lam[2] = {1,1};
      float* tmp=(float*)malloc(imgsize*sizeof(float));;
      //i为像素编号，j
      for(y=0,i=0;y<height;y++)
      {
	    for(x=0;x<width;x++,i++)
	    {
		j=i*3;
		a[0]=st[j++];
		a[1]=a[2]=st[j++];
		a[3]=st[j++];
		
		grd=a[0]+a[3];
		jacobian_rotation(2, a, d, v);
		eigen_sort(2, d, v);
		
		//D参数定义
		if(c)//CED:
		{
		      lam[0]=alpha;
		      if(d[0]=d[1])
			    lam[1]=alpha;
		      else
			    lam[1]=alpha+(1-alpha)*exp(-c/pow((d[0]-d[1]),2));
		}
		else//EED:
		{
		      if ( grd > 0.0) //必>0
		      {
			      gol = sqrt(grd)/lambda;
			      lam[0] = 1.0 - exp (-3.31488 / pow(gol, thexp));
		      } 
		      else 
		      {
			      lam[0] = lam[1] = 1;
		      }
		}
		//计算D
		j=i*3;
		st[j++]=lam[0]*v[0]*v[0]+lam[1]*v[1]*v[1];
		st[j++]=lam[0]*v[0]*v[2]+lam[1]*v[1]*v[3];
		st[j++]=lam[0]*v[2]*v[2]+lam[1]*v[3]*v[3];

	    // 		float h=0.00000009;
		float h=0.000001/4;
		float xx,xy,yx,yy;		
		float res;
		if(x>0&&x<(width-1)&&y>0&&y<(height-1))
		{
		    j=i*3;
		    xx=st[j+3]*(data[i+1]-data[i])-st[j-3]*(data[i]-data[i-1]);
		    xy=st[j+4]*(data[i+1+width]-data[i+1-width])-st[j-2]*(data[i-1+width]-data[i-1-width]);
		    j=(i+width)*3;
		    k=(i-width)*3;
		    yx=st[j+1]*(data[i+1+width]-data[i-1+width])-st[k+1]*(data[i+1-width]-data[i-1-width]);
		    yy=st[j+2]*(data[i+width]-data[i])-st[k+2]*(data[i]-data[i-width]);
		    res=h*(xx+xy+yx+yy);		    
		}
		
		if(res)
		  tmp[i]=data[i]+res;
		else
		  tmp[i]=data[i];
		//tmp[i]=res;
	    }
      }

      for (y = 1 ; y < height - 1 ; y++)
	    for ( x = 1 ; x < width - 1; x++)
	    {
		  i = y*width+x;
		  data[i]=tmp[i];
	    }	
      
      delete st,tmp;
}

void Filter::nadTukey(float* data, float lambda, float k, int width, int height)
{
      int p,q,pq;
      float NI,SI,EI,WI,cN,cS,cE,cW;
      float *data_tmp;

      data_tmp = (float*)malloc(width*height*sizeof(float));
      for (p = 2 ; p < height - 1 ; p++)
      for ( q = 2 ; q < width - 1; q++)
      {
	 pq = p*width+q;
	 NI=data[(p-1)*width+q]-data[pq];
	 SI=data[(p+1)*width+q]-data[pq];
	 EI=data[pq-1]-data[pq];
	 WI=data[pq+1]-data[pq];


	 if ( fabs(NI) < k )
	 {
	    cN = NI / k;
	    cN *= cN;
	    cN = 0.5 * ( 1 - cN )*(1-cN);
	 }
	 else
	    cN = 0 ;

	 if ( fabs(SI) < k )
	 {
	    cS = SI / k;
	    cS *= cS;
	    cS = 0.5 * ( 1 - cS )*(1-cS);
	 }
	 else
	    cS = 0 ;
	 if ( fabs(EI) < k )
	 {
	    cE = EI / k;
	    cE *= cE;
	    cE = 0.5 * ( 1 - cE )*(1-cE);
	 }
	 else
	    cE = 0 ;

	 if ( fabs(WI) < k )
	 {
	    cW = WI / k;
	    cW *= cW;
	    cW = 0.5 * ( 1 - cW )*(1-cW);
	 }
	 else
	    cW = 0 ;

	 data_tmp[pq]=data[pq]+lambda*(cN*NI+cS*SI+cE*EI+cW*WI);
      }
      for (p = 2 ; p < height - 1 ; p++)
	    for ( q = 2 ; q < width - 1; q++)
	    {
	       pq = p*width+q;
	       data[pq]=data_tmp[pq];
	    }

}

void Filter::printhead()
{
      printf("nx:%d ny:%d nz:%d\n",nx,ny,nz);
      cout<<"mean:"<<fmean<<endl;
      cout<<"max:"<<fmax<<endl;
      cout<<"min:"<<fmin<<endl;
	  cout<<"std:"<<fstd<<endl;
	  cout<<"type:"<<datatype<<endl;
      cout<<"datasize:"<<datasize<<endl;
	  cout<<"d:"<<d.f<<endl;
	  

}

float* Filter::gaussian_kernel(Vector3<int> ksize, float sigma, float max)
{
	cout<<"begin gaussian convolve\n";
      if ( sigma < 0.001 ) sigma = 1;
      if ( max < 1e-10 ) max = 1;

      int 	side = 2*( (int) (4*sigma + 0.9) ) + 1;
      if ( ksize.volume() < 3 ) ksize = Vector3<int>(side, side, side);

      Vector3<float>	h((ksize.x()-1)/2.0, (ksize.y()-1)/2.0, (ksize.z()-1)/2.0);
      float* ker=new float[27];
      
      change_type(MRC_MODE_FLOAT);

      int	i, x, y, z;
      double	x2, y2, z2, s2;
      double	invsigma2 = -0.5/(sigma*sigma);

      for ( i=z=0; z<ksize.z(); z++ ) 
      {
	  z2 = z - h.z();
	  z2 *= z2;
	  for ( y=0; y<ksize.y(); y++ ) 
	  {
		  y2 = y - h.y();
		  y2 *= y2;
		  for ( x=0; x<ksize.x(); x++, i++ ) 
		  {
			  x2 = x - h.x();
			  x2 *= x2;
			  s2 = x2 + y2 + z2;
			  ker[i]= max*exp(s2*invsigma2);
		  }
	  }
      }
      return ker;
}

int Filter::convolve(float* ker,float* &data,int width,int height,int depth)
{	
//      cout<<"begin convolve"<<endl;
	unsigned long  dsize=width*height*depth;
	int	i, x, y, chunk_size = get_chunk_size(dsize);

	float*	nudata = new float[dsize];

	int	kernel_size = 3*3*3;
	double	norm = 0;	
	for ( i=0; i<kernel_size; i++ ) norm += fabs(ker[i]);
	for ( i=0; i<kernel_size; i++ ) ker[i]=ker[i]/norm;

	 #pragma omp parallel for
	for ( i=0; i<dsize; i+=chunk_size )
	  img_convolve_chunk(data, ker, nudata, i, chunk_size,width,height,depth);
	//       cout<<"data:"<<data<<"    nudata:"<<nudata<<endl;
	if(data) delete [] data;

	if(nudata)
	    data=nudata;
	else
	    cout<<"data not assigned!\n";
	//       cout<<"data:"<<data<<"    nudata:"<<nudata<<endl;
	//       cout<<"end convolve_chunk"<<endl;/**/
	cout<<" gaussian convolve end\n";
	update_statistic();
	return(0);
}

int Filter::get_chunk_size(unsigned long dsize)
{
    int	np = 1;
  #ifdef OMP_H
      np = omp_get_num_procs();
  #endif

    int		chunk_size = dsize/np;

    if ( chunk_size < dsize && chunk_size < MIN_SIZE_FOR_THREADING ) 
    {
        chunk_size = MIN_SIZE_FOR_THREADING;
        np = (dsize - 1)/chunk_size + 1;
    }
        cout << "Number of processors: " << np << endl << endl;
        cout << "get_chunk_size: chunk_size = " << chunk_size << endl;
    return chunk_size;
}

int Filter::img_convolve_chunk(float* data, float* ker, float* nudata, int i, int len,int width,int height,int depth)
{
     // cout<<"begin chunk:"<<i<<endl;
	unsigned long dsize=width*height*depth;
	unsigned long	j, jy, jz, k, ky, kz, m, n, x, y, z, c, xx, yy, zz;
	Vector3<int>	hk(1,1,1);
	unsigned long	xmin, ymin, zmin, xmax, ymax, zmax;
// 	printf("i:%d c:%d x:%d y:%d z:%d n:%d\n",i,c,x,y,z,n);
	coordinates(i, c, x, y, z, n);
// 	printf("i:%d c:%d x:%d y:%d z:%d n:%d\n",i,c,x,y,z,n);

	xmin = kmin(x, hk.x());
	xmax = kmax(x, 3, width);
	ymin = kmin(y, hk.y());
	ymax = kmax(y, 3, height);
	zmin = kmin(z, hk.z());
	zmax = kmax(z, 3, depth);
//  	printf("initial:	xmin:%ld ymin:%ld zmin:%ld xmax:%ld ymax:%ld zmax:%ld\n",xmin,ymin,zmin,xmax,ymax,zmax);
	m = i+len;
// 	cout<<"m:	"<<m<<endl;
	if ( m > dsize ) m = dsize;

	int ta=1;
	for ( ; i<m; i++, c++ ) 
	{
		//cout<<"i:"<<i<<"\t";
		if ( c >= 1 ) 
		{
			c = 0;
			x++;
			if ( x >= width ) 
			{
				x = 0;
				y++;
				if ( y >= height ) 
				{
					y = 0;
					z++;
					if ( z >= depth ) 
					{
						z = 0;
						n++;
					}
					zmin = kmin(z, hk.z());
					zmax = kmax(z, 3, depth);
				}
				ymin = kmin(y, hk.y());
				ymax = kmax(y, 3, height);
			}
			xmin = kmin(x, hk.x());
			xmax = kmax(x, 3, width);
		}

		nudata[i] = 0;
		for ( zz=zmin; zz<zmax; zz++ ) 
		{
			jz = (z + zz - hk.z())*height;
			kz = zz*3;
			for ( yy=ymin; yy<ymax; yy++ ) 
			{
				jy = (jz + y + yy - hk.y())*width;
				ky = (kz + yy)*3;
				for ( xx=xmin; xx<xmax; xx++ ) 
				{
				        
					j = (jy + x + xx - hk.x()) + c;
					k = ky + xx;
					if(data[j]||ker[k])
						nudata[i] += data[j]*ker[k];
				}
			}
		}
	}

	return 0;
}

long unsigned int Filter::kmin(long unsigned int i, long unsigned int hk)
{
      return (i < hk)? hk - i: 0;
}

long unsigned int Filter::kmax(long unsigned int i, long unsigned int km, long unsigned int m)
{
      unsigned long	hk = km/2;
      return (i + km < m + hk)? km: m + hk - i;

}

void Filter::coordinates(long unsigned int i, long unsigned int& nc, long unsigned int& nnx, long unsigned int& nny, long unsigned int& nnz, long unsigned int& nn)
{    
      nc = 0;
      i = (i - nc);
      nnx = i%nx;
      i = (i - nnx)/nx;
      nny = i%ny;
      i = (i - nny)/ny;
      nnz = i%nz;
      i = (i - nnz)/nz;
      nn = i;   
}

void Filter::change_type(int newtype)
{
      if(!d.uc)
      {
	    cout<<"change_type:no data!\n";
	    return;
      }
      if ( datatype == newtype ) return;

      if ( fmax - fmin < 1e-37 ) fmax = fmin + 1;

      unsigned long	j, k, l, m;
      unsigned long	oldtypesize = data_type_size();
      double		v;
      double		omn = data_type_min();
      double		omx = data_type_max();
//       cout<<"old type:"<<datatype<<endl;
      
      TPointer	p = d;
      int		oldtype = datatype;

      datatype = newtype;
      unsigned long	typesize = data_type_size();

      unsigned long	allocsize = alloc_size();

      double		mn = data_type_min();
      double		mx = data_type_max();
//       cout<<"new type:"<<datatype<<endl;
      double		scale = 1, shift = 0;

      // Integer switches between signed and unsigned
      if ( datatype >= MRC_MODE_BYTE && datatype <= MRC_MODE_SHORT ) 
      {
	  if ( oldtype >= MRC_MODE_FLOAT ) 
	  {		
		  if ( fmin >= 0 ) scale = mx/fmax;
		  else {
			  scale = (mx - mn)/(fmax - fmin);
			  shift = mn - fmin*scale;
		       }
	  } 
	  else 
	  {
		  if ( fmin == 0 && fmax == 1 ) 
		  {	
			  scale = 1;
			  shift = 0;
		  } 
		  else if ( oldtype > datatype && ( fmin < mn || fmax > mx ) ) 
		  {	
			scale = (mx - mn)/(fmax - fmin);
			shift = mn - fmin*scale;
		  }
	  }
      }

      if ( datatype == MRC_MODE_FLOAT && ( fmin < mn || fmax > mx ) ) 
      {
		  scale = (mx - mn)/(fmax - fmin);
      }

// 	  cout << "DEBUG Bimage::change_type: scale=" << scale << " shift=" << shift << endl;

      if ( oldtypesize != typesize ) 
      {
// 	    cout << "DEBUG Bimage::change_type: allocsize=" << allocsize << endl;
	    d.uc = new unsigned char[allocsize];
	    for ( j=0; j<allocsize; j++ ) d.uc[j] = 0;
      }

      for ( j=l=0; j<datasize; j++, l++ ) 
      {
	    switch ( oldtype ) 
	    {
		  case MRC_MODE_BYTE:	v = p.uc[j]; break;
		  case MRC_MODE_SHORT:	v = p.ss[j]; break;
		  case MRC_MODE_FLOAT:	v = p.f[j]; break;	
		  default: ;
	    }
	    v = scale*v + shift;
	    k = j;
	    set(k, v);
       }

       if ( oldtypesize != typesize)
	       delete [] p.uc;
       //cout<<"type change successful!\n";
	 //datatype=newtype;
	 
	 update_statistic();
}

long unsigned int Filter::data_type_size()
{
      	unsigned long	typesize = 0;
	
	switch ( datatype ) 
	{
		case MRC_MODE_BYTE:	typesize = sizeof(char); break;
		case MRC_MODE_SHORT:	typesize = sizeof(short); break;
		case MRC_MODE_FLOAT:	typesize = sizeof(float); break;
		default: typesize = 0;
	}
	
	return typesize;
}

double Filter::data_type_max()
{
      	double			max = 0;
	
	switch ( datatype ) 
	{
		case MRC_MODE_BYTE:	max = 255; break;
		case MRC_MODE_SHORT:	max = USHRT_MAX; break;
		case MRC_MODE_FLOAT:	max = 1e37; break;		      
		default: break;
	}

	return max;
}

double Filter::data_type_min()
{
      	double			min = 0;
	
	switch ( datatype ) 
	{
	      case MRC_MODE_BYTE:	min = 0; break;
		case MRC_MODE_SHORT:	min = SHRT_MIN; break;
		case MRC_MODE_FLOAT:	min = -1e37; break;	
		default: break;
	}

	return min;
}

void Filter::set(long unsigned int j, double v)
{
      if ( j >= datasize ) return;
	
	unsigned long	k;
	
	switch ( datatype ) 
	{
	      case MRC_MODE_BYTE:	d.uc[j] = (unsigned char)v; break;
		case MRC_MODE_SHORT:	d.ss[j] = (short)v; break;
		case MRC_MODE_FLOAT:	d.f[j] = v; break;
		default: ;
	}

}

int Filter::img_mass_normalize(float avg, float std, int norm_type)
{
	int	n;
	int	bins = (int) fmax;
	//cout<<"bins:"<<bins<<endl;
	if ( bins < 256 ) bins = 256;
	if ( bins > 1024 ) bins = 1024;
	
	if ( std <= 0 ) 
	{		
		switch (datatype) 
		{
			case MRC_MODE_BYTE: avg = 127; std = 40; break;
			case MRC_MODE_SHORT: avg = USHRT_MAX/2; std = 256; break;
			//case Short: avg = 0; std = 256; break;
			case MRC_MODE_FLOAT: avg = 100000; std = 1000; break;
			//case Integer: avg = 0; std = 1000; break;
			default: avg = 0; std = 1; break;
		}
	}

	change_type(MRC_MODE_FLOAT);
	
	printf("target:average:%lf   standard_deviation:%lf",avg,std);
	fflush(stdout);
	//printf("avg:%f\tstd:%f\tbins:%d\n",avg,std,bins);
	
//#pragma omp parallel for//并行执行for程序(openmp)
	for ( n=0; n<nz; n++ )
		img_mass_normalize_subimage(n, avg, std, norm_type, bins);
	
// 	change_type(MRC_MODE_SHORT);
	//img_calculate_background(p);
	update_statistic();

	return(0);

}

Filter* Filter::extractimg(int nn)
{
	float* data=(float*)malloc(imgsize*sizeof(float));
	Filter* 	ftemp=new Filter(nx,ny,1,MRC_MODE_FLOAT,(unsigned char*)data);
	for(int i=0,j=nn*imgsize;i<imgsize;i++,j++)
	{
		ftemp->set(i,(*this)[j]);		
	}
	ftemp->update_statistic();
	return(ftemp);

}

int Filter::img_mass_normalize_subimage(int imgnum, float avg, float std, int norm_type, int bins)
{

	printf("begin norm slice%d\n",imgnum);
	fflush(stdout);
	float*     fit = NULL;

	Filter* 	ftemp=extractimg(imgnum);
	//ftemp->printhead();
	double 	lavg,lstd;
	//printf("fmean:%lf\tfstd:%lf\n",fmean,fstd);
	double min=fmean - 10*fstd;
	double max=fmean + 10*fstd;
	//printf("img_mass_normalize_subimagemin:%lf\tmax:%lf\n",min,max);
	img_truncate(ftemp, min,max , min, max);
	
	switch ( norm_type ) 
	{
		case 1:
			fit = img_fit_gauss_to_histogram(ftemp, bins, 1);
			lavg=fit[1];
			lstd=fit[2];
			//printf("img_mass_normalize_subimage:lavg:%lf\tlstd:%lf\n",lavg,lstd);
			delete[] fit;
			break;
		default:
			ftemp->update_statistic();
/*			
		case 2:
			fit = img_fit_poisson_to_histogram(ndata, bins);
			ptemp->average(fit[2]);
			ptemp->standard_deviation(sqrt(fit[2]));
			delete[] fit;
			break;
			*/
	}
	
	if ( img_rescale_to_avg_std(ftemp,avg, std,lavg,lstd) )
		cerr << "Error: The extrema for image " << imgnum+1 << " are the same: " <<
			fmin << " " << fmax << endl;
	
	replace(imgnum, ftemp->d.f);
	
	delete ftemp;

	return(0);

}

int Filter::replace(long unsigned int nn,float* data)
{
	unsigned long	j, k;
	
	for ( j=nn*imgsize, k=0; k<imgsize; j++, k++ ) set(j, data[k]);
	
	return 0;

}

float* Filter::img_fit_gauss_to_histogram(Filter* fl,int bins, int n)
{
	unsigned long	i, j, k, m, nh, ncol(n+2), datasize = fl->data_size();
	double		max = 0, avg, std;
	double		bin_half = (fl->maximum() - fl->minimum())/(4*bins);

	float			scale, offset;
	int*			histo = img_calc_histogram(fl, bins, &scale, &offset);

	
	//printf("img_fit_gauss_to_histogram:datasize:%ld   \tbin_half:%lf    fstd:%lf",datasize,bin_half,fstd);
	//printf("    \tscale:%lf   \toffset:%lf\n",scale,offset);
	int*			sel = new int[bins];
	for ( i=0; i<bins; i++ ) sel[i] = 0;
	
	for ( i=nh=0; i<bins; i++ ) 
	  if ( histo[i] ) 
	  {
		  avg = std = 0;
		  for ( m=0, j=(i>3)?i-3:0, k=(i<bins-4)?i+3:bins-4; j<=k; j++ ) if ( j != i ) 
		  {
			  if ( histo[j] ) 
			  {
				  avg += histo[j];
				  std += histo[j]*histo[j];
				  m++;
			  }
		  }
		  if ( m ) 
		  {
			  avg /= m;
			  std = std/m - avg*avg;
			  if ( std > 0 ) std = sqrt(std);
			  else std = 0;
		  }
		  if ( std ) 
		  {
			  if ( fabs(histo[i] - avg) < 5*std ) sel[i] = 1;//fabs求浮点数x的绝对值
		  } 
		  else 
		  {
			  if ( avg/histo[i] > 0.5 && avg/histo[i] < 2 ) sel[i] = 1;
		  }
		  nh += sel[i];
	  }
	
	// 3 parameters: amplitude, position, sigma
	Bsimplex*	simp = init_simplex(1, 3*n, 1, nh);
	simp->constant[0] = n;
	for ( i=j=0; i<bins; i++ ) if ( sel[i] ) 
	{
		simp->x[j] = fl->minimum() + i/scale;
		simp->fx[j++] = histo[i];
		if ( max < histo[i] ) max = histo[i];
	}
	
	for ( i=j=0; j<n; j++, i++ ) 
	{
		simp->param[i] = max;							// Amplitude
		simp->lolimit[i] = 100*max/datasize;
		simp->hilimit[i] = 5*max;
		i++;
		simp->param[i] = (j+0.5)*(fl->maximum() - fl->minimum())/n;	// Position
		simp->lolimit[i] = fl->minimum();
		simp->hilimit[i] = fl->maximum();
		i++;
		simp->param[i] = fl->standard_deviation();						// Sigma
		simp->lolimit[i] = 1/scale;
		simp->hilimit[i] = 5*fl->standard_deviation();
	}

	double	R = simplex(simp, 1000, 0.1, img_fit_gauss_to_his_R);

	float*	gauss = new float[3*n];
	for ( j=0; j<3*n; j++ ) gauss[j] = simp->param[j];

	kill_simplex(simp);

	delete[] histo;
	delete[] sel;
	
	return(gauss);

}

int* Filter::img_calc_histogram(Filter* fl, int bins, float* new_scale, float* new_offset)
{
	if ( !data_point() ) return(NULL);

	unsigned long		i, c;
	unsigned long		datasize = fl->data_size();
	long				j;
	double			scale = (bins - 1)*1.0/(fl->maximum() - fl->minimum());
	double			offset = 0.5 - scale*fl->minimum();
	
	int*				histo = new int[bins];
	for ( i=0; i<bins; i++ ) histo[i] = 0;

	if ( data_type() < MRC_MODE_FLOAT ) 
	{
	scale = 1/(ceil((fl->maximum() - fl->minimum() + 1)/bins));//ceil--返回大于或者等于指定表达式的最小整数
	offset = -scale*fl->minimum();
	}

	//printf("img_calc_histogram:fmax:%f\tfmin:%f\tbins:%d\n",fmax,fmin,bins);
	//printf("                   datasize:%ld\tscale:%lf   \toffset:%lf\n",imgsize,scale,offset);
	if ( new_scale ) *new_scale = scale;
	if ( new_offset ) *new_offset = offset;

	for ( i=0; i<datasize; i++) 
	{
	for ( c=0; c<1; c++ ) 
	{
		j = (long) (scale*(*fl)[i+c] + offset);
		if ( j >= 0 && j < bins ) histo[c*bins+j]++;
	}
	}

	return(histo);

}

int Filter::img_rescale_to_avg_std(Filter* fl, double avg, double std,double lavg, double lstd)
{
	if ( !fl->data_point() ) return(-1);

	if ( std < 0 ) {
		cerr << "Warning: Cannot use a negative standard deviation to scale to! (" << std << ")" << endl;
		return(-1);
	}

	if ( fl->standard_deviation() < 1e-30 ) if ( fl->update_statistic() < 0 ) {
		cerr << "Error in img_rescale_to_avg_std: Statistics not recalculated!" << endl;
		return(-1);
	}

	if ( fl->standard_deviation() == 0 ) {
		cerr << "Error in img_rescale_to_avg_std: Standard deviation is zero!" << endl;
		return(-1);
	}

	cout<<"flstd:"<<fl->standard_deviation()<<"  avg:"<<fl->average()<<endl;
	double	scale = std/ fl->standard_deviation();
	double	shift = avg - fl->average()*scale;

	//cout << "Rescaling to average and stdev: " << avg << " " << std << endl;

	return(img_rescale(fl, scale, shift,lstd));

}

int Filter::img_rescale(Filter* fl, double scale, double shift,double lstd)
{
	int				err = -1;

	if ( !fl->data_point() ) return(err);

	unsigned long   i,datasize =fl->data_size();

	double		tdata;
	double		min = fl->data_type_min();
	double		max = fl->data_type_max();

	cout << "img_rescale:Scale and shift:                " << scale << " " << shift << endl << endl;
/*
	// Adjust the background values
	for ( i=0; i<p->images(); i++ )
		p->image[i].background(p->image[i].background()*scale + shift);*/

	for ( i=0; i<datasize; i++ ) 
	{
		tdata = (*fl)[i]*scale + shift;
		if ( tdata < min ) tdata = min;
		else if ( tdata > max ) tdata = max;
		fl->set(i, tdata);
	}

	fl->update_statistic();

	if ( fl->standard_deviation() > 0 ) err = 0;

	return(err);

}


double img_fit_gauss_to_his_R(Bsimplex* simp)
{
	int			i, j, k;
	double		x, df, R = 0;
	
	for ( i=0; i<simp->npoint; i++ ) {
		for ( j=k=0, df=0; j<simp->constant[0]; j++, k+=3 ) {
			x = (simp->x[i] - simp->param[k+1])/simp->param[k+2];
			df += simp->param[k]*exp(-0.5*x*x);
		}
		df -= simp->fx[i];
		R += df*df;
	}
	
	R = sqrt(R/i);
			
	return(R);

}

int Filter::img_truncate(Filter* fl, double min, double max, double setmin, double setmax)
{
	
	if ( !data_point() ) return(-1);

	unsigned long	i, datasize = fl->data_size();
	double		tdata;
	double		tmin = fl->data_type_min();
	double		tmax = fl->data_type_max();

	//printf("min:%lf\tmax:%lf\n",min,max);
	//printf("fl->fmin:%lf\tfl->fmax:%lf\n",fmin,fmax);
	// Make sure the extremes don't exceed the data
	if ( setmin < fmin ) setmin =fmin;
	if ( setmax > fmax ) setmax =fmax;
	if ( min < tmin ) min = tmin;
	if ( max > tmax ) max = tmax;
	if ( setmin < tmin ) setmin = tmin;
	if ( setmax > tmax ) setmax = tmax;
	
	//printf("datasize:%ld\tsetmin:%lf\tsetmax:%lf\n",datasize,setmin,setmax);
/*
	// Adjust the background values
	for ( i=0; i<p->images(); i++ ) 
	{
		if ( p->image[i].background() < min ) p->image[i].background(setmin);
		if ( p->image[i].background() > max ) p->image[i].background(setmax);
	}*/

	for ( i=0; i<datasize; i++ ) 
	{
		tdata = (*fl)[i];
		if ( tdata < min ) tdata = setmin;
		if ( tdata > max ) tdata = setmax;
		fl->set(i, tdata);
		
	}

	//cout<<"img_truncate:";
	fl->update_statistic();

	return(0);

}

void Filter::corr_coefficient(string infile,string outfile,int t)
{
	cout<<"输入文件:"<<infile<<"\n输出文件:"<<outfile<<"\nt:"<<t<<endl;
	//读入原始图像
	Filter* 	ori_img=new Filter;
	ori_img->readmrc(infile.c_str(),1);
	ori_img->change_type(2);
	
	string inf=infile,outf=outfile,txt;
	//获取输入文件名，无路径，无后缀
	string inname=extract_name(inf);

	//path为输出文件路径
	string path=extract_path(outf);
	string pout=extract_pname(outf);
	//定义输出结果文件名
	string result=path+inname+"_result.txt";
	cout<<"result:"<<result<<endl;
	ofstream total_mean(result.c_str(),ios::app);
	//剩余图像申请空间
	float* 	res_data;
	if(res_data=(float*)malloc(sizeof(float)*datasize))
		memset(res_data,0,sizeof(float)*datasize);
	else
		cout<<"内存不足！\n";
	Filter* 	res=new Filter(res_data,nx,ny,nz);
	//进行滤波
	for(int i=0;i<t;i++)
	{				
		//命名次数index
		stringstream ss;
		int k=i+1;
		ss<<k;
		string index=ss.str();					
		//定义txt文件路径
		
		txt=path+"ncc_"+inname+"_"+index+".txt";	
		//定义输出文件名
		outf=pout+"_"+index+".mrc";
		
		cout<<i<<":txt:"<<txt<<endl;
		cout<<i<<":outf:"<<outf<<endl;
		ofstream o(txt.c_str());		
		nad3d();
		//计算剩余图像		
		residual_img(res,ori_img,this);
		/*
		for(int j=0;j<datasize;j++)
		{
			if(ori_img->d.f[j]&&(*this)[j])
			{
				float value=ori_img->d.f[j]-(*this)[j];
				res->set(j,value);
			}				
			else
			{
				cout<<j<<":值不存在\n";	
			}					
		}
		cout<<"compute the residual picture end!\n";
		*/
		float arr_ncc[nz];
		float* ut=d.f;
		float* ures=res->d.f;
		
		for(int k=0;k<nz;k++)
		{
			//指针移动
			ut=&(d.f[imgsize*k]);
			ures=&(res->d.f[imgsize*k]);
			//计算一张图片的ncc
			arr_ncc[k]=ncc(ut,ures,nx,ny);
		}
		cout<<"compute ncc end！\n";
		//delete res;
		float ncc_mean=0;
		for(int m = 0; m < nz; m++)
		{
		    ncc_mean+=arr_ncc[m];
			o<<m<<":"<<arr_ncc[m]<<std::endl;
		}
		o<<"ncc均值："<<ncc_mean/depth()<<endl;
		total_mean<<"ncc均值_"<<i<<":"<<ncc_mean/depth()<<endl;
		o.close();
	
		if(i<15||i%5==0)
			writemrc(outf.c_str());
	}
	delete [] res_data;
	delete ori_img;
	total_mean.close();

}

/*
IplImage* Filter::GetIplImage(int index)
{
    assert(header != NULL && index >= 0 && index < header->nz);
    if(!in.good())
    {
	cout<<"The input file error!"<<endl;
        return NULL;
    }
    
    const int& nx = header->nx;	//列数		//the coordinate is different(have not been tested)
    const int& ny = header->ny; //行数

    CvSize size;
    size.width = nx;
    size.height = ny;
    IplImage *img = cvCreateImage(size, IPL_DEPTH_32F, 1); 

    int bufsize = nx*ny;//一张图片的方格数
    
    switch(header->mode)
      {
	  case MRC_MODE_BYTE:
	  {
		  unsigned char* tmpbuf = new unsigned char[bufsize];
		  for(int y = 0; y < ny; y++)
		  {
			  in.seekg(sizeof(MrcHeader)+header->next+(index*nx*ny+y*nx)*sizeof(unsigned char), std::ios::beg);//设置输入流指针位置
			  in.read((char*)&(tmpbuf[y*nx]), nx*sizeof(unsigned char));
		  }
		  float* start = (float*)img->imageData;
		  float* end  = start+bufsize;
		  unsigned char* src = tmpbuf;
		  while(start != end)
		  {
			  *start++ = *src++/255.0f;
		  }
		  delete [] tmpbuf;
		  break;	  
	  }
	  case MRC_MODE_SHORT:
	  {
		  short* tmpbuf = new short[bufsize];
		  
		  //改写,一次全部取出一张图片
		  in.seekg(sizeof(MRCheader)+header->next+index*nx*ny*sizeof(short),std::ios::beg);
		  in.read((char*)tmpbuf,nx*ny*sizeof(short));
		  
 		  float* start = (float*)img->imageData;
 		  float* end  = start+bufsize;//指针与整型数相加非单纯地址加,而是地址+整数*数据类型=新地址
 		  short* src = tmpbuf;

		  while(start != end)
		  {	      
			  *(start++) = *(src++);
		  }		  
		  delete [] tmpbuf;	
		  break;	  
	  }
	  case MRC_MODE_FLOAT:
	  {
		  for(int y = 0; y < ny; y++)
		  {
			  in.seekg(sizeof(MrcHeader)+header->next+(index*nx*ny+y*nx)*sizeof(float), std::ios::beg);
			  in.read((char*)&(((float*)(img->imageData))[y*nx]),nx*sizeof(float));
		  }	
		  break;
	  }
	  default:
		  cvReleaseImage(&img);
		  return NULL;
    }  
    return img;
}*/

void residual_img(Filter* res, Filter* origin, Filter* flimg)
{
	res->change_type(2);
	origin->change_type(2);
	flimg->change_type(2);
	
	for(int j=0;j<flimg->data_size();j++)
	{
		if(origin->d.f[j]&&flimg->d.f[j])
		{
			float value=origin->d.f[j]-flimg->d.f[j];
			res->set(j,value);
		}				
		else
		{
			cout<<j<<":值不存在\n";	
		}					
	}
	cout<<"compute the residual picture end!\n";
}
