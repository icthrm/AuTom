 
#ifndef FILTER_H__
#define FILTER_H__

#include <fstream>
#include "mrcheader.h"
#include "Vector3.h"
// #include <cv.hpp>
// #include <core/core.hpp>

// using namespace cv;

union TPointer
{
    unsigned char*	uc;
    short*		ss;
    float*		f;
};
/**
 * 赋值:nx,ny,nx,d,mean
 * header:用于写入文件，其它时候不使用
 */
class Filter
{
private:
    std::ifstream in;
    std::ofstream out;
    unsigned long datasize;//nx*ny*nz
    unsigned long imgsize;
    double fmean,fmax,fmin,fstd;
    int nx,ny,nz;
    int datatype;
    int meantag;//用于Iplimage数据更新
    
public:
    MRCheader* header;
    TPointer d;
    FILE *fp;
    //结构体使用
    Filter(int x,int y,int z,int type,unsigned char* data);
    //需要readmrc
    Filter();
    //et专用,float数据
    Filter(float* data,int width,int height,int depth);
    ~Filter();

    /*****滤波函数*****/
    void 	nad3d();
    void	nad3d(int nx, int ny, int nz, float* &odata);
    /*nad3d step*/
    int	img_nad_chunk(int width,int height,int depth,float lambda, float C, float alpha, float ht, long zs, long zw,float* mdata,float* temp);
    int	get_slab_size(int depth);
    long 	struct_tensor(int nx,int ny,int nz,double mean,float* data,float *&st,long zs, long zw);
    void 	diffusion_tensor(float* st,float alpha,float C, float lambda,long tempsize);
    void 	diffuse(float* st,float* odata,float* temp,int nx,int ny,int nz,long zw,long zs,float ht);

    /*2d filter*/
    void 	nad2d(float* data ,int width , int height);
    void 	nadTukey(float* data , float lambda , float k , int width , int height);
    
    /*默认3×3×3kernel*/
    //int 	gaissian_convolve3d(Vector3<int> ksize, float sigma,float *&odata);
    float* 	gaussian_kernel(Vector3<int> ksize, float sigma, float max);
    int 	convolve(float* ker,float* &data,int width,int height,int depth);
    int		get_chunk_size(unsigned long datasize);
    int		img_convolve_chunk(float* data, float* ker, float* nudata, int i, int len,int width,int height,int depth);
    unsigned long	kmin(unsigned long i, unsigned long hk);
    unsigned long	kmax(unsigned long i, unsigned long km, unsigned long m);
    void		coordinates(unsigned long i, unsigned long &nc, unsigned long &nnx, unsigned long &nny, unsigned long &nnz, unsigned long &nn);

    /****norm****
     * extractimg:提取一张图片数据
     */
    int	img_mass_normalize(float avg, float std, int norm_type);
    int	img_mass_normalize_subimage(int imgnum, float avg, float std, int norm_type, int bins);
    float* 	img_fit_gauss_to_histogram(Filter* fl,int bins, int n);
    Filter*  extractimg(int nn);
    int*	img_calc_histogram(Filter* fl,int bins, float* new_scale, float* new_offset);
    int 	img_rescale_to_avg_std(Filter* fl,double avg, double std,double lavg, double lstd);
    int	img_rescale(Filter* fl, double scale, double shift,double lstd);
    int 	img_truncate(Filter* fl, double min, double max, double setmin, double setmax);
    
    /*****mrc文件操作*****/   
    /*
     * readmrc--包括内部变量赋值
     * mrc_update_class--更新类内部的赋值，在数据d赋值的情况下,在写入文件前的最后一步
     * mrc_update_header--更新header，若要写入文件，最后一步
     * writemrc--目前只可写float类型数据
     * update_statistic--在data,nx,ny,nz,type赋值后可进行的计算mean,max,min操作
     */
    int		readmrc(const char* infname,int tag);
    //int		mrc_update_class();
    int 		update_header();
    int		writemrc(const char* oufname);
    int 		update_statistic();
    
    void	change_type(int newtype);
    
    /***opencv相关函数***
     * GetIplImage--把文件转换为float类型，index:0～n-1
     */
    /*
    IplImage* 	GetIplImage(int index);
*/
    
    /*****类内部变量调用函数*****/
    int	depth(){return nz;}
    int 	width(){return nx;}
    int	height(){return ny;}
    double 	maximum(){return fmax;};
    double 	minimum(){return fmin;};
    double  average(){return fmean;};
    double 	standard_deviation(){return fstd;};
    unsigned long  data_size(){return datasize;};
    int 	data_type(){return datatype;};
    void	close()
		{
		      if(in) in.close();
		      if(out) out.close();
		}
    unsigned long	data_type_size();
    double		data_type_min();
    double		data_type_max();
    unsigned long	alloc_size() { return nx*ny*nz*data_type_size(); }
    void		set(unsigned long j, double v);
    double		operator[](unsigned long j) const;
    unsigned char* 	data_point(){return d.uc;}
    int		replace(unsigned long nn,float* data);
    
    /*****其它函数*****
     * printhead：输出Filter内部参数值
     * corr_coefficient：计算第t次滤波图像与剩余图像互相关系数，默认float类型
     * residual_img:计算剩余图像res
     */
    void 	printhead();
    void 	corr_coefficient(string infile,string outfile,int t);
    
    
};

/***Filter图像操作***/
void		residual_img(Filter* res,Filter* originimg,Filter* flimg);

#endif
