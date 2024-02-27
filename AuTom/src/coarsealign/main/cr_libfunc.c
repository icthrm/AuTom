#include "cr_libfunc.h"
//finished
//void align(/*const*/ char * inf��raw stack��,/*const*/ char * inf_shift��shift parameter file��,/*const*/ char * outf��aligned stack��,/*const*/ char * outf_proj��projection of aligned stack��,/*const*/ char * inf_rtt��rotate parameter file��) 
void align(__IN char * inf,__IN char * inf_shift,__OUT char * outf, __IN char * inf_rtt) 
{
	/*infԭʼͼ��st��inf_shift,shift�����ļ���outfƥ������stͼ���ļ�������inf_rtt,rotation�����ļ�������ΪNULL*/
	//long *rtt_dgr;//��תУ�����������ǵ��������ݹ�����Ϊָ��.ÿ1����ֵΪ0.1�ȣ�ȡֵ��ΧΪ+-180��
	//rtt_dgr = (long *)malloc(sizeof(long));
	long rtt_dgr;
	long smp_dgr = 1800;//һ�ܶ���Ϊ���ٵȷ�

	//open files..........
	FILE *fin=NULL, *fout=NULL/*, *fin_shift=NULL, *fin_rtt=NULL*/;

	if((fin = fopen(inf,"rb")) == NULL ||/* (fin_shift = fopen(inf_shift,"rb")) == NULL || */(fout = fopen(outf,"wb")) == NULL)
	{
		fprintf(stdout,"Cannot open file strike any key exit!\n");
		exit(1);
	}

	if (!read_rotation(inf_rtt,&rtt_dgr))
	{
		rtt_dgr=0;
	}
/*	
	if (inf_rtt!=NULL && (fin_rtt = fopen(inf_rtt,"rb")) != NULL)//�����������⣩����û��rot������shift_align��������align
	{
		fread(&rtt_dgr, sizeof(long), 1, fin_rtt);
		if (rtt_dgr[0]>1800 || rtt_dgr[0]< -1800)
		{
			rtt_dgr[0]=rtt_dgr[0]%1800;
		}
		
	}
	else
	{
		rtt_dgr = 0;
	}
	if(fin_rtt)
		fclose(fin_rtt);
*/
#ifdef _DEBUG
	fprintf(stdout,"rotation:%ld\n",rtt_dgr);
#endif // _DEBUG

	//define input and output heads..........
	MrcHeader  *inhead, *outhead;
	inhead = (MrcHeader *)malloc(sizeof(MrcHeader));
	outhead = (MrcHeader *)malloc(sizeof(MrcHeader));

	mrc_read_head(fin,inhead);

	if(inhead->nx % 2 == 1 || inhead->ny % 2 == 1  )
	{
		fprintf(stdout,"Error in fuction 'main()'! Image size must be even in both directon!\n");
		exit(1);
	}

	memcpy(outhead,inhead,sizeof(MrcHeader));
	//mrc_read_head(fin,outhead);
	outhead->mode = MRC_MODE_FLOAT;

	long z, x, y, i;

	//double *cntr_x, *cntr_y;
	//cntr_x = (double *)malloc(inhead->nz*sizeof(double));
	//cntr_y = (double *)malloc(inhead->nz*sizeof(double));
	float *cntr_x, *cntr_y;
	cntr_x = (float *)malloc(inhead->nz*sizeof(float));
	cntr_y = (float *)malloc(inhead->nz*sizeof(float));
	//read the shift parameters
	read_shift(inf_shift,cntr_x,cntr_y,inhead->nz);
//	fread(cntr_x, inhead->nz*sizeof(double), 1, fin_shift);
//	fread(cntr_y, inhead->nz*sizeof(double), 1, fin_shift);

#ifdef _DEBUG
	for(z = 0; z < inhead->nz; z++)
	{
		fprintf(stdout,"%d %f\n",z,cntr_x[z]);
	}
	for(z = 0; z < inhead->nz; z++)
	{
		fprintf(stdout,"%d %f\n",z,cntr_y[z]);
	}
#endif // _DEBUG

	//define the outputted range of rows and columns
	double max =0;//ȷ������ƽ�����������Ӧ�ı߽�������ͼ���п۳�
	for(z = 0; z < inhead->nz; z++)//��ȷ������ƽ�����
	{
		if (max <fabs(cntr_x[z])) max = fabs(cntr_x[z]);
		if (max <fabs(cntr_y[z])) max = fabs(cntr_y[z]);
	}
	double max_int;///////ֱ�Ӹ�Ϊint��������
	modf(max,&max_int);//modf������Ϊ������С���������������ڶ�������������С��

#ifdef _DEBUG
	fprintf(stdout,"%f\n",max_int);//��������Ӧ�ı߽�������ͼ���п۳���
#endif // _DEBUG

	long valid_nx = inhead->nx-2*(int)max_int-2;//����֤ͼ�񲻻��ӱ߽���������ţ�
	long valid_ny = inhead->ny-2*(int)max_int-2;

	double cos_rtt_dgr, sin_rtt_dgr, x_int, y_int;//����һ���Ļ����ϣ�ȷ����תУ��֮����ͼ��x��y��Χ
	sin_rtt_dgr=sin(3.1415926*rtt_dgr/smp_dgr);
	cos_rtt_dgr=cos(3.1415926*rtt_dgr/smp_dgr);
	modf((double)valid_nx/(fabs(sin_rtt_dgr)+fabs(cos_rtt_dgr)), &x_int);
	modf((double)valid_ny/(fabs(sin_rtt_dgr)+fabs(cos_rtt_dgr)), &y_int);
	outhead->nx=(int)x_int;
	outhead->ny=(int)y_int;
	if (outhead->nx%2!=0) outhead->nx=outhead->nx-1;//��֤nx��nyΪż��
	if (outhead->ny%2!=0) outhead->ny=outhead->ny-1;

	unsigned long image_in_size2d, image_out_size2d;
	image_in_size2d = inhead->nx*inhead->ny;
	image_out_size2d = outhead->nx*outhead->ny;
	//define input and output vectors..............

	float *image, *image_out, *image_proj_st;
	double i_x, i_x_flt, i_x_int, i_x_cntr, i_y, i_y_flt, i_y_int, i_y_cntr;
	image_out = (float *)malloc(image_out_size2d*sizeof(float));
	image = (float *)malloc(image_in_size2d*sizeof(float));
	image_proj_st = (float *)malloc(image_out_size2d*sizeof(float));

	union 
	{short s;
	unsigned char c;
	float f;
	}pxl_val;


	mrc_write_head(fout,outhead);


	//adjust the image by Bilinear Interpolation
/*	for(y = 0; y < outhead->ny; y++)
	{
		for(x = 0; x < outhead->nx; x++) 
		{
			image_proj_st[(outhead->nx * y) + x] = 0;
		}
	}
*/

	for(z = 0; z<outhead->nz; z++)
	{
		i_y_cntr=inhead->ny/2-cntr_y[z];
		i_x_cntr=inhead->nx/2-cntr_x[z];


		if (inhead->mode == MRC_MODE_FLOAT)
		{
			fseek(fin,1024+z*image_in_size2d*sizeof(float),SEEK_SET);
			for(i=0;i<image_in_size2d;i++)
			{
				fread(&pxl_val.f,sizeof(float),1,fin);
				image[i]=pxl_val.f;
			}
		}
		else if (inhead->mode == MRC_MODE_BYTE)
		{
			fseek(fin,1024+z*image_in_size2d*sizeof(char),SEEK_SET);
			for(i=0;i<image_in_size2d;i++)
			{
				fread(&pxl_val.c,sizeof(char),1,fin);
				image[i]=(float)pxl_val.c;
			}
		}
		else if (inhead->mode == MRC_MODE_SHORT)
		{
			fseek(fin,1024+z*image_in_size2d*sizeof(short),SEEK_SET);
			for(i=0;i<image_in_size2d;i++)
			{
				fread(&pxl_val.s,sizeof(short),1,fin);
				image[i]=(float)pxl_val.s;
			}
		}

#ifdef _DEBUG
		fprintf(stdout,"%d %f\n",z,image[inhead->nx*inhead->ny/2+inhead->nx/2]);
#endif // _DEBUG

		for(y = 0; y < outhead->ny/2; y++)
		{
			//fill the quadrant I(including the center of image) 
			for(x = 0; x < outhead->nx/2; x++)
			{
				i_x=i_x_cntr+x*cos_rtt_dgr+y*sin_rtt_dgr;
				i_y=i_y_cntr-x*sin_rtt_dgr+y*cos_rtt_dgr;
				i_x_flt = modf(i_x, &i_x_int);
				i_y_flt = modf(i_y, &i_y_int);

				image_out[outhead->nx*(outhead->ny/2+y)+outhead->nx/2+x]
				=image[inhead->nx*((int)i_y_int)+(int)i_x_int]*(1-i_x_flt)*(1-i_y_flt)
					+image[inhead->nx*((int)i_y_int)+(int)i_x_int+1]*(i_x_flt)*(1-i_y_flt)
					+image[inhead->nx*((int)i_y_int+1)+(int)i_x_int]*(1-i_x_flt)*(i_y_flt)
					+image[inhead->nx*((int)i_y_int+1)+(int)i_x_int+1]*(i_x_flt)*(i_y_flt);
			}
			//fill the quadrant II
			for(x = 0; x <= outhead->nx/2; x++)
			{
				i_x=i_x_cntr-x*cos_rtt_dgr+y*sin_rtt_dgr;
				i_y=i_y_cntr+x*sin_rtt_dgr+y*cos_rtt_dgr;
				i_x_flt = modf(i_x, &i_x_int);
				i_y_flt = modf(i_y, &i_y_int);

				image_out[outhead->nx*(outhead->ny/2+y)+outhead->nx/2-x]
				=image[inhead->nx*((int)i_y_int)+(int)i_x_int]*(1-i_x_flt)*(1-i_y_flt)
					+image[inhead->nx*((int)i_y_int)+(int)i_x_int+1]*(i_x_flt)*(1-i_y_flt)
					+image[inhead->nx*((int)i_y_int+1)+(int)i_x_int]*(1-i_x_flt)*(i_y_flt)
					+image[inhead->nx*((int)i_y_int+1)+(int)i_x_int+1]*(i_x_flt)*(i_y_flt);
			}
		}
		for(y = 0; y <= outhead->ny/2; y++)
		{
			//fill the quadrant III
			for(x = 0; x <= outhead->nx/2; x++)
			{
				i_x=i_x_cntr-x*cos_rtt_dgr-y*sin_rtt_dgr;
				i_y=i_y_cntr+x*sin_rtt_dgr-y*cos_rtt_dgr;
				i_x_flt = modf(i_x, &i_x_int);
				i_y_flt = modf(i_y, &i_y_int);

				image_out[outhead->nx*(outhead->ny/2-y)+outhead->nx/2-x]
				=image[inhead->nx*((int)i_y_int)+(int)i_x_int]*(1-i_x_flt)*(1-i_y_flt)
					+image[inhead->nx*((int)i_y_int)+(int)i_x_int+1]*(i_x_flt)*(1-i_y_flt)
					+image[inhead->nx*((int)i_y_int+1)+(int)i_x_int]*(1-i_x_flt)*(i_y_flt)
					+image[inhead->nx*((int)i_y_int+1)+(int)i_x_int+1]*(i_x_flt)*(i_y_flt);
			}
			//fill the quadrant IV
			for(x = 0; x < outhead->nx/2; x++)
			{
				i_x=i_x_cntr+x*cos_rtt_dgr-y*sin_rtt_dgr;
				i_y=i_y_cntr-x*sin_rtt_dgr-y*cos_rtt_dgr;
				i_x_flt = modf(i_x, &i_x_int);
				i_y_flt = modf(i_y, &i_y_int);

				image_out[outhead->nx*(outhead->ny/2-y)+outhead->nx/2+x]
				=image[inhead->nx*((int)i_y_int)+(int)i_x_int]*(1-i_x_flt)*(1-i_y_flt)
					+image[inhead->nx*((int)i_y_int)+(int)i_x_int+1]*(i_x_flt)*(1-i_y_flt)
					+image[inhead->nx*((int)i_y_int+1)+(int)i_x_int]*(1-i_x_flt)*(i_y_flt)
					+image[inhead->nx*((int)i_y_int+1)+(int)i_x_int+1]*(i_x_flt)*(i_y_flt);
			}
		}

/*		for(y = 0; y < outhead->ny; y++)
		{
			for(x = 0; x < outhead->nx; x++) 
			{
				image_proj_st[(outhead->nx * y) + x] += image_out[(outhead->nx * y) + x]/outhead->nz;
			}
		}
*/

#ifdef _DEBUG
		fprintf(stdout,"%d %f\n",z,image_out[inhead->nx*inhead->ny/2+inhead->nx/2]);
#endif // _DEBUG

		fwrite(image_out, image_out_size2d*sizeof(float), 1, fout);
	}
	/*
	outhead->amax=image_proj_st[(outhead->nx * y) + x];
	outhead->amin=image_proj_st[(outhead->nx * y) + x];
	outhead->amean=0;
	for(y = 0; y < outhead->ny; y++)
	{
		for(x = 0; x < outhead->nx; x++) 
		{
			if (outhead->amax<image_proj_st[(outhead->nx * y) + x]) outhead->amax=image_proj_st[(outhead->nx * y) + x];
			if (outhead->amin<image_proj_st[(outhead->nx * y) + x]) outhead->amin=image_proj_st[(outhead->nx * y) + x];
			outhead->amean+=image_proj_st[(outhead->nx * y) + x]/image_out_size2d;
		}
	}
	outhead->nz = 1;
	mrc_write_head(fout1,outhead);
	fwrite(image_proj_st, image_out_size2d*sizeof(float), 1, fout1);
*/

	free(image_proj_st);
	free(image_out);
	free(image);
	free(inhead);
	free(outhead);
	free(cntr_x);
	free(cntr_y);
	fclose(fin);
	//fclose(fin_shift);
	fclose(fout);
	//fclose(fout1);
}

//finished
//��д��float�汾������slcdata����������дΪdouble
//int mrc_read_slice_double(FILE *fin��������stack��, MrcHeader  *head��mrc�ļ�ͷ�����׳�����.mrc,.st,.prj��ʹ��ͬ�����ļ�ͷ��, int slcN����Ҫ��ȡ��ͼ����stack�е�������,char axis����X��Y��Z������ȡͼ����,double *slcdata�������ͼ����)
int mrc_read_slice_double(FILE *fin, MrcHeader  *head, int slcN,char axis,double *slcdata)
{
	int psize;
	short buf_short;
	unsigned char buf_byte;
	float buf_float;
	int i,k;

	switch(head->mode)
	{
	case MRC_MODE_BYTE :
		psize=sizeof(unsigned char);

		break;

	case MRC_MODE_SHORT :
		psize=sizeof(short);

		break;

	case MRC_MODE_FLOAT :
		psize=sizeof(float);

		break;
	}

	switch(axis)
	{

		/***********************************X************************************/
	case 'x':
	case 'X':

		fseek(fin,1024+slcN*psize,SEEK_SET);


		switch(head->mode)
		{
		case MRC_MODE_BYTE:
			for(i=0;i<head->ny*head->nz;i++)
			{
				fread(&buf_byte,psize,1,fin);
				slcdata[i]=(double)buf_byte;
				fseek(fin,(head->nx-1)*psize,SEEK_CUR);
			}

			break;

		case MRC_MODE_SHORT:
			for(i=0;i<head->ny*head->nz;i++)
			{
				fread(&buf_short,psize,1,fin);
				slcdata[i]=(double)(buf_short);
				fseek(fin,(head->nx-1)*psize,SEEK_CUR);
			}

			break;

		case MRC_MODE_FLOAT:
			for(i=0;i<head->ny*head->nz;i++)
			{
				fread(&buf_float,psize,1,fin);
				slcdata[i]=(double)buf_float;
				fseek(fin,(head->nx-1)*psize,SEEK_CUR);
			}
			break;

		}

		break;

		/***********************************Y************************************/
	case 'y':
	case 'Y':

		for(k=0;k<head->nz;k++)
		{
			fseek(fin,1024+(k*head->nx*head->ny+head->nx*slcN)*psize,SEEK_SET);


			switch(head->mode)
			{
			case MRC_MODE_BYTE:
				for(i=0;i<head->nx;i++)
				{
					fread(&buf_byte,psize,1,fin);
					slcdata[k*head->nx+i]=(double)buf_byte;
				}

				break;

			case MRC_MODE_SHORT:
				for(i=0;i<head->nx;i++)
				{
					fread(&buf_short,psize,1,fin);
					slcdata[k*head->nx+i]=(double)(buf_short);
				}

				break;

			case MRC_MODE_FLOAT:
				for(i=0;i<head->nx;i++)
				{
					fread(&buf_float,psize,1,fin);
					slcdata[k*head->nx+i]=(double)buf_float;
				}

				break;

			}

		}
		break;

		/***********************************Z************************************/
	case 'z':
	case 'Z':
		fseek(fin,1024+slcN*head->nx*head->ny*psize ,SEEK_SET);

		if(head->mode==MRC_MODE_FLOAT)/*fread(slcdata,psize*head->nx*head->ny,1,fin); */
		{
			for(i=0;i<head->nx*head->ny;i++)
			{
				fread(&buf_float,psize,1,fin);
				slcdata[i]=(double)buf_float;
			}
		}

		if(head->mode==MRC_MODE_BYTE)
		{
			for(i=0;i<head->nx*head->ny;i++)
			{
				fread(&buf_byte,psize,1,fin);
				slcdata[i]=(double)buf_byte;
			}
		}

		if(head->mode==MRC_MODE_SHORT)
		{
			for(i=0;i<head->nx*head->ny;i++)
			{
				fread(&buf_short,psize,1,fin);
				slcdata[i]=(double)buf_short;
			}
		}
		break;

	}
	return 0;
}

//finished
void cr_project(__IN char* in_st,__IN char* out_Prj/*,float * image_proj_st */)
{//in_outStack��align����output_stack
	int x,y,z;
	MrcHeader   *outhead;
	FILE *fin=NULL,*fout=NULL;
	unsigned long  image_out_size2d;
	float *image_proj_st,*image;

	fin=fopen(in_st,"rb");
	if(fin==NULL)
	{
		fprintf(stdout,"Error to open %s",in_st);
		exit(1);
	}
	fout=fopen(out_Prj,"wb");
	if(fout==NULL)
	{
		fprintf(stdout,"Error to open %s",out_Prj);
		exit(1);
	}


	outhead = (MrcHeader *)malloc(sizeof(MrcHeader));
	mrc_read_head(fin,outhead);
	image_out_size2d = outhead->nx*outhead->ny;
	image_proj_st=(float*)malloc(image_out_size2d*sizeof(float));
	image=(float*)malloc(image_out_size2d*sizeof(float));

	/*
	for(y = 0; y < outhead->ny; y++)
	{
		for(x = 0; x < outhead->nx; x++) 
		{
			image_proj_st[(outhead->nx * y) + x] = 0;
		}
	}
*/
	memset(image_proj_st,0,image_out_size2d*sizeof(float));

	for (z = 0; z<outhead->nz; z++)
	{
		fread(image, image_out_size2d*sizeof(float), 1, fin);
/*		for(y = 0; y < outhead->ny; y++)
		{
			for(x = 0; x < outhead->nx; x++) 
			{
				image_proj_st[(outhead->nx * y) + x] += image_out[(outhead->nx * y) + x]/outhead->nz;
			}
		}
*/	
		for (x=0;x < image_out_size2d;x++)
		{
			image_proj_st[x]+=image[x]/outhead->nz;
		}
		
	}
	

	outhead->amax=image_proj_st[0];
	outhead->amin=image_proj_st[0];
	outhead->amean=0;
	//image_out_size2d = outhead->nx*outhead->ny;
/*	for(y = 0; y < outhead->ny; y++)
	{
		for(x = 0; x < outhead->nx; x++) 
		{
			if (outhead->amax<image_proj_st[(outhead->nx * y) + x]) outhead->amax=image_proj_st[(outhead->nx * y) + x];
			if (outhead->amin>image_proj_st[(outhead->nx * y) + x]) outhead->amin=image_proj_st[(outhead->nx * y) + x];//Ӧ���ǡ��ɣ�
			outhead->amean+=image_proj_st[(outhead->nx * y) + x]/image_out_size2d;
		}
	}
*/	
	for (x=0;x< image_out_size2d ; x++)
	{
		outhead->amax=outhead->amax<image_proj_st[x]? image_proj_st[x]:outhead->amax;
		outhead->amin=outhead->amin>image_proj_st[x]? image_proj_st[x]:outhead->amin;
		outhead->amean+=image_proj_st[x];
	}
	outhead->amean /= image_out_size2d;
	
	outhead->nz = 1;
	mrc_write_head(fout,outhead);
	fwrite(image_proj_st, image_out_size2d*sizeof(float), 1, fout);

	free(outhead);
	free(image_proj_st);
	free(image);
	fclose(fin);
	fclose(fout);
}

//finished
int correlation_2d(__IN double *Image1,__IN double *Image2,__OUT double *Corr_out,__IN int Nx,__IN int Ny)
{
/*!
*function:
*	calculate the correlative factors between neighbor images.
*parameters:
*	Image1: inputted image1;
*	Image2: inputted image2;
*	Corr_out: outputted plot of corretative factor;
*	Nx: range of columns;
*	Ny: range of rows.
*return:
*	0, if successful.
*/

	unsigned long n;

	if(Nx% 2 == 1 || Ny % 2 == 1  )
	{
		fprintf(stdout,"Error in fuction 'correlation_2d()'! Image size must be even in both directon!\n");
		exit(1);
	}

	//define and allocate memory for the output fft data of image1 and image2
	unsigned long Image_in_size2d = Nx*Ny;
	unsigned long Image_out_size2d = (Nx / 2 + 1)*Ny;
	double Sqrt_xy = sqrt((double)Nx*Ny);

	fftw_complex *Fft_out1, *Fft_out2, *Fft_mult;
	Fft_out1 = (fftw_complex *) fftw_malloc(sizeof(fftw_complex)*Image_out_size2d);//Fourier Transform of image1
	Fft_out2 = (fftw_complex *) fftw_malloc(sizeof(fftw_complex)*Image_out_size2d);//Fourier Transform of image2
	Fft_mult = (fftw_complex *) fftw_malloc(sizeof(fftw_complex)*Image_out_size2d);//Fourier Transform of correlation factor of image1 and image2, which is Fourier Transform of image1 multiplied by conjugate of Fourier Transform of image2

	//executing fft........
	fftw_plan Plan;

	Plan = fftw_plan_dft_r2c_2d(Ny, Nx, Image1, Fft_out1, FFTW_ESTIMATE);
	fftw_execute(Plan);
	fftw_destroy_plan(Plan);

	Plan = fftw_plan_dft_r2c_2d(Ny, Nx, Image2, Fft_out2, FFTW_ESTIMATE);
	fftw_execute(Plan);
	fftw_destroy_plan(Plan);

	//mult fft_out1 and conjugate of fft_out2
	//(a + bi)(c - di) = (ac + bd) + (bc - ad)i
	for(n=0;n<Image_out_size2d;n++)
	{
		//normallization
		Fft_out1[n][0] /= Sqrt_xy;
		Fft_out1[n][1] /= Sqrt_xy;
		Fft_out2[n][0] /= Sqrt_xy;
		Fft_out2[n][1] /= Sqrt_xy;

		Fft_mult[n][0] = Fft_out1[n][0]*Fft_out2[n][0] + Fft_out1[n][1]*Fft_out2[n][1];
		Fft_mult[n][1] = Fft_out1[n][1]*Fft_out2[n][0] - Fft_out1[n][0]*Fft_out2[n][1];
	}

	//executing inverse fft
	Plan = fftw_plan_dft_c2r_2d(Ny, Nx, Fft_mult, Corr_out, FFTW_ESTIMATE);
	fftw_execute(Plan);
	fftw_destroy_plan(Plan);

	//calculate the image module for normalization
	double Img_mod1, Img_mod2, Norm_factor;
	Img_mod1 = 0.0;
	Img_mod2 = 0.0;

	for(n=0;n<Image_in_size2d;n++)
	{
		Img_mod1 += Image1[n]*Image1[n];
		Img_mod2 += Image2[n]*Image2[n];
	}
	Norm_factor = sqrt(Img_mod1*Img_mod2);

	//normalize corr_out and convert it to float
	for(n=0;n<Image_in_size2d;n++)
	{
		Corr_out[n] = Corr_out[n] / Norm_factor;
	}

	//free fft_out1, fft_out2, fft_mult;
	fftw_free(Fft_out1);
	fftw_free(Fft_out2);
	fftw_free(Fft_mult);

	return 0;
}

//finished
double max_pixel_2d(__IN double *Image,__OUT long *Max_x,__OUT long *Max_y,__IN int Nx,__IN int Ny)
{
/*!
*function:
*	find the pixel with maximal value in the image.
*parameters:
*	Image: inputted image(plot of correlation factor);
*	Max_x: the column containing pixel with maximal value;
*	Max_y: the row containing pixel with maximal value;
*	Nx: range of columns;
*	Ny: range of rows.
*return:
*	maximal value of pixels, if successful.
*/
	double Max = -1.0;
	long Image_in_size2d=Nx*Ny;
	long Max_n, n;
	for(n=0;n<Image_in_size2d;n++)
	{
		if(Image[n] > Max)
		{
			Max = Image[n];
			Max_n = n;
		}
	}
	*Max_x = Max_n % Nx;
	*Max_y = Max_n / Nx;

	return Max;
}

//finished
//����cr_xyshift_step��������������ͼƬ֮����ƽ�Ʋ���
void cr_xyshift_step(__IN double * image1,__IN double * image2,__IN  MrcHeader * inhead,__OUT long * shift_x,__IN long z,__OUT long * shift_y,__IN MrcHeader * outhead ) 
{
	double  *corr_out;//(plot of correlation factor)
	corr_out = (double *)malloc(inhead->nx*inhead->ny*sizeof(double));

	//������ͼ���Ļ�����ϵ����������ϵ���������غ�����ֵ���������õ�fftw�����⣩
	correlation_2d( image1, image2, corr_out, inhead->nx, inhead->ny);

	//to find the max and culculate the shift
	//������ϵ������ֵ�������������ͼ��֮����ƽ�����������ϵ������ֵ�����������º���max_pixel_2d����
	max_pixel_2d(corr_out, &shift_x[z], &shift_y[z], outhead->nx, outhead->ny);//cr_xyshift_step

	//�����ÿ��ٸ���Ҷ�任���еĺ���fftw�����õĸ��Ҷ�任ͼ���ǵߵ��ģ����Ծ���correlation_2d���������Ļ�����ϵ��ͼ����һ�͵�������֮�䣬�ڶ��͵�������֮��Ҳ�ǵߵ��ģ�������������д�������У����
	if (shift_x[z] > (outhead->nx / 2)) shift_x[z] = shift_x[z] - outhead->nx;
	if (shift_y[z] > (outhead->ny / 2)) shift_y[z] = shift_y[z] - outhead->ny;

	//to debug
#ifdef _DEBUG
	fprintf(stdout,"shift_x[%d] = %d, shift_y[%d] = %d\n", z, shift_x[z], z, shift_y[z]);
#endif
	free(corr_out);
}

//finished
//����cr_pos_para������㵥��ͼƬ����������ͼƬ����ƽ�Ʋ����������ڼ���������ĳһ�ʵ�������������֮��λʸ�ķ�����
//!!!��������ͼ�����δ���д�ɺ������������Ӻ������ͷź����ڶ�����ָ��ʱ����double free�����⣬�ڲ��Ե�ʱ��Ӧ��ע�⡣
//shift_x, shift_y���������ͼƬ֮����shift��, dgr_rtt����һ�ֻ��õ�rotate parameter��, smp_dgr��һ�ܶ���Ϊ���ٵȷ֣�) 
void cr_pos_para(__IN const int nz,__IN __OUT  long * shift_x,__IN __OUT long * shift_y,__IN const long  dgr_rtt,__IN const char* filepath_in_shift,__IN long smp_dgr,__IN const char* filepath_outf_shift ) 
{
	//count the shift adjustment parameters of each image	
	int i,j,z;
	long *shift_total_x, *shift_total_y;
	/*double*/float *i_x, *i_y;
	/*double*/float *i_x_rtt, *i_y_rtt;
	FILE *fin_shift=NULL,*fout=NULL;
	//����shift_total_x�е�Ԫ��A��i��j������ͼ��i��ͼ��j֮����x����ƽ�Ʋ���
	shift_total_x = (long *)malloc( nz* nz*sizeof(long));
	shift_total_y = (long *)malloc( nz* nz*sizeof(long));
	//����i_x�е�Ԫ��A��i��������i��ͼƬ����������ͼƬ����ƽ�Ʋ���
	//i_x = (double *)malloc( nz*sizeof(double));
	//i_y = (double *)malloc( nz*sizeof(double));
	i_x = (float *)malloc( nz*sizeof(float));
	i_y = (float *)malloc( nz*sizeof(float));
	//����i_x_rtt������rotate parameter�����ڵ���У�����м�����
	//i_x_rtt = (double *)malloc( nz*sizeof(double));
	//i_y_rtt = (double *)malloc( nz*sizeof(double));
	i_x_rtt = (float *)malloc( nz*sizeof(float));
	i_y_rtt = (float *)malloc( nz*sizeof(float));
	//fin_shift=fopen(filepath_in_shift,"rb");//����û�������ļ�������ʱ�ǲ����õ���
   // fout = fopen(filepath_outf_shift,"wb");//Ҫ��д���Ҹ���֮ǰ���ڵ��ļ��������У�
    //������Ϊ���ǵ���ģ�������������С�������������ݵļ��飬�����ļ���Ӧ�ø��������ļ������������������ļ�����Ϊ���ļ���.crxyshift.rnd(ѭ����).glb����
/*	if(!fout)
	{
		fprintf(stdout,"Error to open %s\n",filepath_outf_shift);
		exit(1);
	}
*/	/*
	for(i=0;i<(inhead->nz*inhead->nz);i++)
	{
		shift_total_x[i]=0;
		shift_total_y[i]=0;
	}
	*/
	memset(shift_total_x,0, nz* nz*sizeof(long));
	memset(shift_total_y,0, nz* nz*sizeof(long));

//����shift_total_x�е�Ԫ��A��i��j������ͼ��i��ͼ��j֮����x����ƽ�Ʋ��������ԶԽ����ϵ�Ԫ��A��i��i��=0��Ԫ��A��i��i-1��ֱ�ӴӾ���shift_x�ж�ȡ��A��i��i-n��=A��i��i-n+1��+A��i-n+1��i-n����A��j,i��=-A��i,j��
	for(i=0;i< nz-1;i++)
	{
		shift_total_x[((i +1) *  nz)+i]= shift_x[i];
		shift_total_x[(i *  nz)+i+1]= -shift_x[i];
		shift_total_y[((i +1) *  nz)+i]= shift_y[i];
		shift_total_y[(i *  nz)+i+1]= -shift_y[i];
		for(j=(i-1);j>=0;j--)
		{
			shift_total_x[((i+1) *  nz) + j] += shift_total_x[((j + 1) *  nz) + j] + shift_total_x[((i+1) *  nz) + j + 1];
			shift_total_x[(j *  nz) + (i+1)] = (-shift_total_x[((i+1) *  nz) + j]);
			shift_total_y[((i+1) *  nz) + j] += shift_total_y[((j + 1) *  nz) + j] + shift_total_y[((i+1) *  nz) + j + 1];
			shift_total_y[(j *  nz) + (i+1)] = (-shift_total_y[((i+1) *  nz) + j]);
		}
	}
	//debug
#ifdef _DEBUG
	for(i=0;i< nz;i++)
	{
		for(j=0;j< nz;j++)
		{
			fprintf(stdout,"%4d,", shift_total_x[(i *  nz) + j]);
		}
		fprintf(stdout,"\n");
	}
#endif
	/*
	for(i=0;i< nz;i++)
	{
		shift_x[i] = 0;
		shift_y[i] = 0;
	}
*/
	memset(shift_x,0,nz*sizeof(long));
	memset(shift_y,0,nz*sizeof(long));
	for(i=0;i< nz;i++)
	{
		for(j=0;j< nz;j++)
		{
			shift_x[i] += shift_total_x[(i *  nz) + j];
			shift_y[i] += shift_total_y[(i *  nz) + j];
		}
		//count the shift adjustment parameters relative to the center of inputted images
		i_x[i] = (/*double*/float)shift_x[i]/ nz;
		i_y[i] = (/*double*/float)shift_y[i]/ nz;
	}

#ifdef _DEBUG
	for(i = 0; i <  nz; i++)
	{
		fprintf(stdout,"%d %f\n",i,i_x[i]);
	}
	for(i = 0; i <  nz; i++)
	{
		fprintf(stdout,"%d %f\n",i,i_y[i]);
	}
#endif // _DEBUG

	//calculate the shift parameters among raw images, basing on the parameters from last circle.
	if (/*fin_shift */read_shift(filepath_in_shift,i_x_rtt,i_y_rtt,nz))
	{
		//fread(i_x_rtt,  nz*sizeof(double), 1, fin_shift);
		//fread(i_y_rtt,  nz*sizeof(double), 1, fin_shift);
		//�������԰�cos/sin�ڵ�ֵ�������һ���µı���������cos/sin��ֵ��Ϊ�±�������ټ����
		for(z = 0; z <  nz; z++)
		{
			i_x_rtt[z] = i_x_rtt[z]+i_x[z]*cos(3.1415926*dgr_rtt/smp_dgr) + i_y[z]*sin(3.1415926*dgr_rtt/smp_dgr);
			i_y_rtt[z] = i_y_rtt[z]-i_x[z]*sin(3.1415926*dgr_rtt/smp_dgr) + i_y[z]*cos(3.1415926*dgr_rtt/smp_dgr);
			i_x[z]=i_x_rtt[z];//Ҳ��������memsetһ��/ѭ����
			i_y[z]=i_y_rtt[z];
		}
	}
	write_shift(filepath_outf_shift,i_x,i_y,nz);
	//fwrite(i_x,  nz*sizeof(double), 1, fout);
	//fwrite(i_y,  nz*sizeof(double), 1, fout);
#ifdef _DEBUG
	for(z = 0; z <  nz; z++)
	{
		fprintf(stdout,"%d %f\n",z,i_x[z]);
	}
	for(z = 0; z <  nz; z++)
	{
		fprintf(stdout,"%d %f\n",z,i_y[z]);
	}
#endif // _DEBUG

	if(fin_shift)
		fclose(fin_shift);
	//fclose(fout);
	free(shift_total_x);
	free(shift_total_y);
	free(i_x);
	free(i_y);
	free(i_x_rtt);
	free(i_y_rtt);

}

//finished
//�˺������Ǵ����ĵ��ж�����img_rotate������һ�������ĺ��������Խ�img_taper_rotate45������������ִ�������Ƕȵ�rotate��ֻ������45�Ƚ�rotate�����Ľǵ�ͼ������ɾ�����������ĵ�ƽ�����϶������Ҫ����img_rotate��Ҫ��align�����з���
int taper_rotate_plus_45degree(__IN double *Image,__OUT float *Image_out,__IN int Out_nx,__IN int Out_ny,__IN int In_nx,__IN int In_ny)
{
/*!taper_rotate_plus_45degree
*function:
*	taper the image, while rotating it by 45 degrees. 
*parameters:
*	Image: inputted image;
*	Image_out: outputted image, after process;
*	In_nx: range of columns of the inputted image;
*	In_ny: range of rows of the inputted image;
*	Out_nx: range of columns of the outputted image;
*	Out_ny: range of rows of the outputted image.
*return:
*	0, if successful.
*/
	long x, y;

	//taper and rotate the image
	for(y = 0; y < Out_ny/2; y++)
	{
		for(x = 0; x < Out_nx/2; x++) 
		{
			Image_out[Out_nx*(y*2)+x*2] = (float)Image[(In_nx*(y+x))+In_ny/2-y+x];
			Image_out[Out_nx*(y*2+1)+x*2+1] = (float)Image[(In_nx*(y+1+x))+In_ny/2-y+x];
		}
	}

	//fill the interval
	for(y = 0; y <= Out_ny/2; y++)
	{
		if (y==0)
		{
			Image_out[Out_nx*(y*2)+(Out_nx-1)]
			=(Image_out[Out_nx*(y*2)+(Out_nx-1)-1]/2
				+Image_out[Out_nx*((y*2)+1)+(Out_nx-1)]/2);
			for (x=1;x<Out_nx/2;x++)
			{
				Image_out[Out_nx*(y*2)+(x*2-1)]
				=(Image_out[Out_nx*(y*2)+(x*2-1)-1]/3
					+Image_out[Out_nx*(y*2)+(x*2-1)+1]/3
					+Image_out[Out_nx*((y*2)+1)+(x*2-1)]/3);
			}
		}

		else if (y==Out_ny/2)
		{
			Image_out[Out_nx*(y*2-1)]
			=(Image_out[Out_nx*(y*2-1)+1]/2
				+Image_out[Out_nx*((y*2-1)-1)]/2);
			for (x=1;x<Out_nx/2;x++)
			{
				Image_out[Out_nx*(y*2-1)+(x*2)]
				=(Image_out[Out_nx*(y*2-1)+(x*2)-1]/3
					+Image_out[Out_nx*(y*2-1)+(x*2)+1]/3)
					+Image_out[Out_nx*((y*2-1)-1)+(x*2)]/3;
			}
		}

		else 
		{
			for(x = 0; x < Out_nx/2; x++) 
			{
				if (x==0)
				{
					Image_out[Out_nx*(y*2-1)+(x*2)]
					=(Image_out[Out_nx*(y*2-1)+(x*2)+1]/3
						+Image_out[Out_nx*((y*2-1)+1)+(x*2)]/3
						+Image_out[Out_nx*((y*2-1)-1)+(x*2)]/3);

					Image_out[Out_nx*(y*2)+(x*2+1)]
					=(Image_out[Out_nx*(y*2)+(x*2+1)-1]/4
						+Image_out[Out_nx*(y*2)+(x*2+1)+1]/4
						+Image_out[Out_nx*((y*2)-1)+(x*2+1)]/4
						+Image_out[Out_nx*((y*2)+1)+(x*2+1)]/4);
				}

				else if (x==(Out_nx/2-1))
				{
					Image_out[Out_nx*(y*2-1)+(x*2)]
					=(Image_out[Out_nx*(y*2-1)+(x*2)+1]/4
						+Image_out[Out_nx*(y*2-1)+(x*2)-1]/4
						+Image_out[Out_nx*((y*2-1)+1)+(x*2)]/4
						+Image_out[Out_nx*((y*2-1)-1)+(x*2)]/4);

					Image_out[Out_nx*(y*2)+(x*2+1)]
					=(Image_out[Out_nx*(y*2)+(x*2+1)-1]/3
						+Image_out[Out_nx*((y*2)-1)+(x*2+1)]/3
						+Image_out[Out_nx*((y*2)+1)+(x*2+1)]/3);
				}
				else
				{
					Image_out[Out_nx*(y*2-1)+(x*2)]
					=(Image_out[Out_nx*(y*2-1)+(x*2)+1]/4
						+Image_out[Out_nx*(y*2-1)+(x*2)-1]/4
						+Image_out[Out_nx*((y*2-1)+1)+(x*2)]/4
						+Image_out[Out_nx*((y*2-1)-1)+(x*2)]/4);

					Image_out[Out_nx*(y*2)+(x*2+1)]
					=(Image_out[Out_nx*(y*2)+(x*2+1)-1]/4
						+Image_out[Out_nx*(y*2)+(x*2+1)+1]/4
						+Image_out[Out_nx*((y*2)-1)+(x*2+1)]/4
						+Image_out[Out_nx*((y*2)+1)+(x*2+1)]/4);
				}
			}
		}
	}

	return 0;
}

//finished
int img_pad(__IN float *Image,__OUT float *Image_out,__IN const int In_nx,__IN const int In_ny,__IN const int Out_nx,__IN const int Out_ny,__IN const float Mean,__IN const  long Pad_x,__IN const long Pad_y)
{
/*!pad_image2d
*function:
*	pad the image by weighted averaging and attenuating to the mean.
*parameters:
*	Image: inputted image;
*	Image_out: outputted image, after process;
*	In_nx: range of columns of the inputted image;
*	In_ny: range of rows of the inputted image;
*	Out_nx: range of columns of the outputted image;
*	Out_ny: range of rows of the outputted image;
*	Mean: mean of the image;
*	Pad_x: pad factor, defining the range of rows added to the image;
*	Pad_y: pad factor, defining the range of columns added to the image.
*return:
*	0, if successful.
*/
	long x, y;

	for(y = 0; y < In_ny; y++)
	{
		for(x = 0; x < In_nx; x++) 
		{
			Image_out[(Out_nx * (y + Pad_y)) + x + Pad_x] = Image[(In_nx * y) + x];
		}
	}

	for(x =Pad_x-1; x >=0; x--)
	{
		for(y = 0; y < In_ny; y++)
		{
			if (y==0)
			{
				//fill the outer circuit with weighted average multipied with attenuative factor
				Image_out[(Out_nx*(y+Pad_y))+x]=
					((Image_out[(Out_nx*(y+Pad_y))+x+1]/2
					+Image_out[(Out_nx*(y+Pad_y))+x+2]/4
					+Image_out[(Out_nx*(y+Pad_y+1))+x+1]/4
					-Mean)
					*(1-cos(3.1415926*(float)(x)/Pad_x))/(1-cos(3.1415926*(float)(x+1)/Pad_x))
					+Mean);

				Image_out[(Out_nx*(y+Pad_y))+(Out_nx-1-x)]=
					((Image_out[(Out_nx*(y+Pad_y))+(Out_nx-1-x-1)]/2
					+Image_out[(Out_nx*(y+Pad_y))+(Out_nx-1-x-2)]/4
					+Image_out[(Out_nx*(y+Pad_y+1))+(Out_nx-1-x-1)]/4
					-Mean)
					*(1-cos(3.1415926*(float)(x)/Pad_x))/(1-cos(3.1415926*(float)(x+1)/Pad_x))
					+Mean);
			}
			else if (y==(In_ny-1))
			{
				Image_out[(Out_nx*(y+Pad_y))+x]=
					((Image_out[(Out_nx*(y+Pad_y))+x+1]/2
					+Image_out[(Out_nx*(y+Pad_y))+x+2]/4
					+Image_out[(Out_nx*(y+Pad_y-1))+x+1]/4
					-Mean)
					*(1-cos(3.1415926*(float)(x)/Pad_x))/(1-cos(3.1415926*(float)(x+1)/Pad_x))
					+Mean);

				Image_out[(Out_nx*(y+Pad_y))+(Out_nx-1-x)]=
					((Image_out[(Out_nx*(y+Pad_y))+(Out_nx-1-x-1)]/2
					+Image_out[(Out_nx*(y+Pad_y))+(Out_nx-1-x-2)]/4
					+Image_out[(Out_nx*(y+Pad_y-1))+(Out_nx-1-x-1)]/4
					-Mean)
					*(1-cos(3.1415926*(float)(x)/Pad_x))/(1-cos(3.1415926*(float)(x+1)/Pad_x))
					+Mean);
			}
			else 
			{
				Image_out[(Out_nx*(y+Pad_y))+x]=
					((Image_out[(Out_nx*(y+Pad_y))+x+1]/2
					+Image_out[(Out_nx*(y+Pad_y))+x+2]/6
					+Image_out[(Out_nx*(y+Pad_y-1))+x+1]/6
					+Image_out[(Out_nx*(y+Pad_y+1))+x+1]/6
					-Mean)
					*(1-cos(3.1415926*(float)(x)/Pad_x))/(1-cos(3.1415926*(float)(x+1)/Pad_x))
					+Mean);

				Image_out[(Out_nx*(y+Pad_y))+(Out_nx-1-x)]=
					((Image_out[(Out_nx*(y+Pad_y))+(Out_nx-1-x-1)]/2
					+Image_out[(Out_nx*(y+Pad_y))+(Out_nx-1-x-2)]/6
					+Image_out[(Out_nx*(y+Pad_y-1))+(Out_nx-1-x-1)]/6
					+Image_out[(Out_nx*(y+Pad_y+1))+(Out_nx-1-x-1)]/6
					-Mean)
					*(1-cos(3.1415926*(float)(x)/Pad_x))/(1-cos(3.1415926*(float)(x+1)/Pad_x))
					+Mean);

			}
		}
	}

	for(y = Pad_y-1; y >=0 ; y--)
	{
		for(x = 0; x < Out_nx; x++) 
		{
			if (x==0)
			{
				Image_out[(Out_nx*y)+x]=
					((Image_out[(Out_nx*(y+1))+x]/2
					+Image_out[(Out_nx*(y+2))+x]/4
					+Image_out[(Out_nx*(y+1))+x+1]/4
					-Mean)
					*(1-cos(3.1415926*(float)(y)/Pad_y))/(1-cos(3.1415926*(float)(y+1)/Pad_y))
					+Mean);

				Image_out[(Out_nx*(Out_ny-1-y))+x]=
					((Image_out[(Out_nx*(Out_ny-1-y-1))+x]/2
					+Image_out[(Out_nx*(Out_ny-1-y-2))+x]/4
					+Image_out[(Out_nx*(Out_ny-1-y-1))+x+1]/4
					-Mean)
					*(1-cos(3.1415926*(float)(y)/Pad_y))/(1-cos(3.1415926*(float)(y+1)/Pad_y))
					+Mean);
			}
			else if (x==(Out_nx-1))
			{
				Image_out[(Out_nx*y)+x]=
					((Image_out[(Out_nx*(y+1))+x]/2
					+Image_out[(Out_nx*(y+2))+x]/4
					+Image_out[(Out_nx*(y+1))+x-1]/4
					-Mean)
					*(1-cos(3.1415926*(float)(y)/Pad_y))/(1-cos(3.1415926*(float)(y+1)/Pad_y))
					+Mean);

				Image_out[(Out_nx*(Out_ny-1-y))+x]=
					((Image_out[(Out_nx*(Out_ny-1-y-1))+x]/2
					+Image_out[(Out_nx*(Out_ny-1-y-2))+x]/4
					+Image_out[(Out_nx*(Out_ny-1-y-1))+x-1]/4
					-Mean)
					*(1-cos(3.1415926*(float)(y)/Pad_y))/(1-cos(3.1415926*(float)(y+1)/Pad_y))
					+Mean);
			}
			else
			{
				Image_out[(Out_nx*y)+x]=
					((Image_out[(Out_nx*(y+1))+x]/2
					+Image_out[(Out_nx*(y+2))+x]/6
					+Image_out[(Out_nx*(y+1))+x-1]/6
					+Image_out[(Out_nx*(y+1))+x+1]/6
					-Mean)
					*(1-cos(3.1415926*(float)(y)/Pad_y))/(1-cos(3.1415926*(float)(y+1)/Pad_y))
					+Mean);

				Image_out[(Out_nx*(Out_ny-1-y))+x]=
					((Image_out[(Out_nx*(Out_ny-1-y-1))+x]/2
					+Image_out[(Out_nx*(Out_ny-1-y-2))+x]/6
					+Image_out[(Out_nx*(Out_ny-1-y-1))+x-1]/6
					+Image_out[(Out_nx*(Out_ny-1-y-1))+x+1]/6
					-Mean)
					*(1-cos(3.1415926*(float)(y)/Pad_y))/(1-cos(3.1415926*(float)(y+1)/Pad_y))
					+Mean);
			}
		}
	}

	return 0;
}

//finished
int img_fft(__IN double *Pre_fft_image,__IN float *Image_out,__IN int Nx,__IN int Ny)
{
/*!fft_image_2d
*function:
*	fast fourier transform and log the outputed image.
*parameters:
*	Pre_fft_image: inputted image;
*	Image_out: outputted image;
*	Nx: range of columns;
*	Ny: range of rows.
*return:
*	0, if successful.
*/
	/*unsigned long*/int  n, x, y;
	int Nx_half = (Nx/2)+1;
	//define and allocate memory for the output fft data of pre_fft_image
	unsigned long Image_out_size2d = Nx_half*Ny;
	double sqrt_xy = sqrt((double)Nx*Ny);

	float *Fft_out;//����ͼ��
	Fft_out = (float *) malloc(sizeof(float)*Image_out_size2d);	

	fftw_complex *Fft_oper;
	Fft_oper = (fftw_complex *) fftw_malloc(sizeof(fftw_complex)*Image_out_size2d);

	//executing fft........
	fftw_plan Plan;

	Plan = fftw_plan_dft_r2c_2d(Ny, Nx, Pre_fft_image, Fft_oper, FFTW_ESTIMATE);
	fftw_execute(Plan);
	fftw_destroy_plan(Plan);

	//formalization
	for(n=0;n<Image_out_size2d;n++)
	{
		Fft_oper[n][0] = Fft_oper[n][0] / sqrt_xy;
		Fft_oper[n][1] = Fft_oper[n][1] / sqrt_xy;

		Fft_out[n] = (float)(sqrt(Fft_oper[n][0]*Fft_oper[n][0] + Fft_oper[n][1]*Fft_oper[n][1]));

		//weigh the input point whose value is 0
		if (abs(Fft_out[n])<0.001f) //��ֹ�ӽ���0�ĵ���ȡ����ʱ��������
			Fft_out[n]=0.1;
		Fft_out[n] = log(Fft_out[n]);//������Ƶ�ź�

		//weigh the output point whose value is smaller than 0
		if (Fft_out[n]<0) Fft_out[n]=0;//��ֹ���ֵĸ�ֵ������ת���źŵ�����
	}

	//centralize the fft image
	for(y=0;y<Ny/2;y++)
	{
		x=0;
		Image_out[Nx*(Ny/2+y)+Nx/2+x]=Fft_out[((Nx/2)+1)*y+x];

		for(x=1;x<Nx/2;x++)
		{
			Image_out[Nx*(Ny/2+y)+Nx/2+x]=Fft_out[((Nx/2)+1)*y+x];
			Image_out[Nx*(Ny/2-y)+Nx/2-x]=Fft_out[((Nx/2)+1)*y+x];
			if (y==Ny/2-1) Image_out[Nx*(Ny/2-y-1)+Nx/2-x]=Fft_out[((Nx/2)+1)*y+x];
		}

		x=Nx/2;
		Image_out[Nx*(Ny/2-y)+Nx/2-x]=Fft_out[((Nx/2)+1)*y+x];
		if (y==Ny/2-1) Image_out[Nx*(Ny/2-y-1)+Nx/2-x]=Fft_out[((Nx/2)+1)*y+x];
	}

	y=Ny/2;
	for(x=0;x<Nx/2;x++)
	{
		Image_out[Nx*(y-Ny/2)+Nx/2+x]=Fft_out[((Nx/2)+1)*y+x];
	}

	for(y=Ny/2+1;y<Ny;y++)
	{
		x=0;
		Image_out[Nx*(y-Ny/2)+Nx/2+x]=Fft_out[((Nx/2)+1)*y+x];

		for(x=1;x<Nx/2;x++)
		{
			Image_out[Nx*(y-Ny/2)+Nx/2+x]=Fft_out[((Nx/2)+1)*y+x];
			Image_out[Nx*(Ny-(y-Ny/2))+Nx/2-x]=Fft_out[((Nx/2)+1)*y+x];
		}

		x=Nx/2;
		Image_out[Nx*(Ny-(y-Ny/2))+Nx/2-x]=Fft_out[((Nx/2)+1)*y+x];
	}

	//free Fft_out;
	free(Fft_out);
	fftw_free(Fft_oper);

	return 0;
}

//finished
int img_smooth(__IN __OUT float *Image_out,__IN int Nx,__IN int Ny)
{
/*!smooth_image2d
*function:
*	smooth the image through weighted averaging.
*parameters:
*	Image_out: inputted image, as well as outputted;
*	Nx: range of columns;
*	Ny: range of rows.
*return:
*	0, if successful.
*/
	long y,x,i;
	long Image_in_size2d=Nx*Ny;

	float *Image_out_smooth;
	Image_out_smooth = (float *) malloc(sizeof(float)*Image_in_size2d);

	y=0;
	x=0;
	Image_out_smooth[Nx*y+x]
	=Image_out[Nx*y+x]/3
		+Image_out[Nx*(y+1)+x]/3
		+Image_out[Nx*y+x+1]/3;
	for (x=1; x<Nx-1; x++)
	{
		Image_out_smooth[Nx*y+x]
		=Image_out[Nx*y+x]/3
			+Image_out[Nx*(y+1)+x]*2/9
			+Image_out[Nx*y+x+1]*2/9
			+Image_out[Nx*y+x-1]*2/9;
	}
	x=Nx-1;
	Image_out_smooth[Nx*y+x]
	=Image_out[Nx*y+x]/3
		+Image_out[Nx*(y+1)+x]/3
		+Image_out[Nx*y+x-1]/3;

	for (y=1; y<Ny-1; y++)
	{
		x=0;
		Image_out_smooth[Nx*y+x]
		=Image_out[Nx*y+x]/3
			+Image_out[Nx*(y+1)+x]*2/9
			+Image_out[Nx*(y-1)+x]*2/9
			+Image_out[Nx*y+x+1]*2/9;
		for (x=1; x<Nx-1; x++)
		{
			Image_out_smooth[Nx*y+x]
			=Image_out[Nx*y+x]/3
				+Image_out[Nx*(y+1)+x]/6
				+Image_out[Nx*(y-1)+x]/6
				+Image_out[Nx*y+x+1]/6
				+Image_out[Nx*y+x-1]/6;
		}
		x=Nx-1;
		Image_out_smooth[Nx*y+x]
		=Image_out[Nx*y+x]/3
			+Image_out[Nx*(y+1)+x]*2/9
			+Image_out[Nx*(y-1)+x]*2/9
			+Image_out[Nx*y+x-1]*2/9;
	}

	y=Ny-1;
	x=0;
	Image_out_smooth[Nx*y+x]
	=Image_out[Nx*y+x]/3
		+Image_out[Nx*(y-1)+x]/3
		+Image_out[Nx*y+x+1]/3;
	for (x=1; x<Nx-1; x++)
	{
		Image_out_smooth[Nx*y+x]
		=Image_out[Nx*y+x]/3
			+Image_out[Nx*(y-1)+x]*2/9
			+Image_out[Nx*y+x+1]*2/9
			+Image_out[Nx*y+x-1]*2/9;
	}
	x=Nx-1;
	Image_out_smooth[Nx*y+x]
	=Image_out[Nx*y+x]/3
		+Image_out[Nx*(y-1)+x]/3
		+Image_out[Nx*y+x-1]/3;

	for (i=0; i<Image_in_size2d; i++) Image_out[i]=Image_out_smooth[i];

	free(Image_out_smooth);

	return 0;
}

//finished
int cr_rotation_para(__IN float *Image_fft,__IN int Nx,__IN int Ny,__IN long Smp_dgr,__IN long Ddct_dgr,__OUT long *Max_n)
{
/*!rotation_parameter
*function:
*	summarize the pixel values along different direction using Bilinear Interpolation(excluded four 10 degree segments around axis to avoid artifact).
*parameters:
*	Image_fft: inputted fourier transform plot;
*	Nx: range of columns;
*	Ny: range of rows;
*	Smp_dgr: range of entire sampled region, with unit of 0.1 degree;
*	Ddct_dgr: range of every deducted region, with unit of 0.1 degree;�۳�0�ȼ�90�ȸ���+-10�ȵ��źţ��Լ��ٶ����źŵ�Ӱ��
*	Max_n: the direction with maximal summation.
*return:
*	0, if successful.
*/
	double I_x, I_x_flt, I_x_int, I_y, I_y_flt, I_y_int, *I_val, *I_rdt_sum, Max, Min;	//I_x, I_y���Ҷ�任ͼ���ؾ�������������
	int Line1_n, Line2_n;
	long i,n;
	I_val=(double *)malloc((Smp_dgr-(4*Ddct_dgr))*Nx/2*sizeof(double));//���Ҷ�任ͼ���ؾ���������ֵ��
	I_rdt_sum=(double *)malloc((Smp_dgr-(4*Ddct_dgr))*sizeof(double));//���Ҷ�任ͼ���ؾ����Ļ���ֵ��

	for(n=0;n<(Smp_dgr-4*Ddct_dgr);n++)
	{
		I_rdt_sum[n]=0;
	}
	
//��ͶӰ�ĸ��Ҷ�任�ظ������򣬴��������߽�����
	n=0;
	for(i=1;i<Nx/2;i++)
	{
		//�����Ҷ�任ͼ���ؾ�������������
		I_x=i*sin(3.1415926*(n+Ddct_dgr)/Smp_dgr);
		I_y=i*cos(3.1415926*(n+Ddct_dgr)/Smp_dgr);
		I_x_flt = modf(I_x, &I_x_int);
		I_y_flt = modf(I_y, &I_y_int);

		//����˫���Բ�ֵ�������Ҷ�任ͼ���ؾ���������ֵ��
		I_val[i*(Smp_dgr-(4*Ddct_dgr))+n]
		=Image_fft[Nx*(Ny/2+((int)I_y_int))+Nx/2+((int)I_x_int)]*(1-I_x_flt)*(1-I_y_flt)
			+Image_fft[Nx*(Ny/2+((int)I_y_int))+Nx/2+((int)I_x_int)+1]*(I_x_flt)*(1-I_y_flt)
			+Image_fft[Nx*(Ny/2+((int)I_y_int)+1)+Nx/2+((int)I_x_int)]*(1-I_x_flt)*(I_y_flt)
			+Image_fft[Nx*(Ny/2+((int)I_y_int)+1)+Nx/2+((int)I_x_int)+1]*(I_x_flt)*(I_y_flt);
		//�����Ҷ�任ͼ���ؾ����Ļ���ֵ��
		I_rdt_sum[n] += I_val[i*(Smp_dgr-(4*Ddct_dgr))+n];
	}
	Max=I_rdt_sum[n];
	Min=I_rdt_sum[n];
	for(n=1;n<Smp_dgr/2-2*Ddct_dgr;n++)
	{
		for(i=1;i<Nx/2;i++)
		{
			I_x=i*sin(3.1415926*(n+Ddct_dgr)/Smp_dgr);
			I_y=i*cos(3.1415926*(n+Ddct_dgr)/Smp_dgr);
			I_x_flt = modf(I_x, &I_x_int);
			I_y_flt = modf(I_y, &I_y_int);

			I_val[i*(Smp_dgr-(4*Ddct_dgr))+n]
			=Image_fft[Nx*(Ny/2+((int)I_y_int))+Nx/2+((int)I_x_int)]*(1-I_x_flt)*(1-I_y_flt)
				+Image_fft[Nx*(Ny/2+((int)I_y_int))+Nx/2+((int)I_x_int)+1]*(I_x_flt)*(1-I_y_flt)
				+Image_fft[Nx*(Ny/2+((int)I_y_int)+1)+Nx/2+((int)I_x_int)]*(1-I_x_flt)*(I_y_flt)
				+Image_fft[Nx*(Ny/2+((int)I_y_int)+1)+Nx/2+((int)I_x_int)+1]*(I_x_flt)*(I_y_flt);
			I_rdt_sum[n] += I_val[i*(Smp_dgr-(4*Ddct_dgr))+n];
		}
		if (I_rdt_sum[n]>Max)
		{
			Max=I_rdt_sum[n];
			*Max_n=n;
		}
		if (I_rdt_sum[n]<Min)
		{
			Min=I_rdt_sum[n];
		}
	}
	//mirror the Bilinear Interpolation Method.

	for(n=Smp_dgr/2-2*Ddct_dgr;n<Smp_dgr-4*Ddct_dgr;n++)
	{
		for(i=1;i<Nx/2;i++)
		{
			I_x=i*sin(3.1415926*(n+3*Ddct_dgr)/Smp_dgr);
			I_y=i*cos(3.1415926*(n+3*Ddct_dgr)/Smp_dgr);
			I_x_flt = modf(I_x, &I_x_int);
			I_y_flt = -modf(I_y, &I_y_int);

			I_val[i*(Smp_dgr-(4*Ddct_dgr))+n]
			=Image_fft[Nx*(Ny/2+((int)I_y_int))+Nx/2+((int)I_x_int)]*(1-I_x_flt)*(1-I_y_flt)
				+Image_fft[Nx*(Ny/2+((int)I_y_int))+Nx/2+((int)I_x_int)+1]*(I_x_flt)*(1-I_y_flt)
				+Image_fft[Nx*(Ny/2+((int)I_y_int)-1)+Nx/2+((int)I_x_int)]*(1-I_x_flt)*(I_y_flt)
				+Image_fft[Nx*(Ny/2+((int)I_y_int)-1)+Nx/2+((int)I_x_int)+1]*(I_x_flt)*(I_y_flt);
			I_rdt_sum[n] += I_val[i*(Smp_dgr-(4*Ddct_dgr))+n];
		}
		if (I_rdt_sum[n]>Max)
		{
			Max=I_rdt_sum[n];
			*Max_n=n;
		}
		if (I_rdt_sum[n]<Min)
		{
			Min=I_rdt_sum[n];
		}
	}

	for(n=0;n<Smp_dgr-4*Ddct_dgr;n++)
	{
		if (I_rdt_sum[n]>((Max-Min)*2/3+Min))
		{
			Line1_n=n;
#ifdef _DEBUG
			fprintf(stdout,"%d %f\n",n,I_rdt_sum[n]);
#endif // _DEBUG
			break;
		}
	}

	for(n=Smp_dgr-4*Ddct_dgr-1;n>=0;n--)
	{
		if (I_rdt_sum[n]>((Max-Min)*2/3+Min))
		{
			Line2_n=n;
#ifdef _DEBUG
			fprintf(stdout,"%d %f\n",n,I_rdt_sum[n]);
#endif // _DEBUG
			break;
		}
	}


	//Linear Regression
	/*
	for(n=Line1_n;n<=*Max_n;n++)
	{
	if (n<Smp_dgr/2-2*Ddct_dgr)
	{
	fprintf(stdout,"%d %f\n",(n+Ddct_dgr), I_rdt_sum[n]);
	}
	else
	{
	fprintf(stdout,"%d %f\n",(n+3*Ddct_dgr), I_rdt_sum[n]);
	}
	}

	for(n=*Max_n;n<=Line2_n;n++)
	{
	if (n<Smp_dgr/2-2*Ddct_dgr)
	{
	fprintf(stdout,"%d %f\n",(n+Ddct_dgr), I_rdt_sum[n]);
	}
	else
	{
	fprintf(stdout,"%d %f\n",(n+3*Ddct_dgr), I_rdt_sum[n]);
	}
	}
	*/

	//output the rotation adjustment parameter

	free(I_val);
	free(I_rdt_sum);

	return 0;
}

int img_rotate( __IN char * inf,__OUT char * outf, __IN char * inf_rtt )
{
	char inf_shift[200];
	long i;
	float * shiftinf;
	FILE * fin=NULL;
	MrcHeader inHead;
	fin=fopen(inf,"rb");
	if (!fin)
	{
		fprintf(stdout,"Error to open %s in img_rotate\n",inf);
		exit(1);
	}
	memset(inf_shift,0,200);
	sprintf(inf_shift,"tmp_xyshift_zero");
	mrc_read_head(fin,&inHead);
	fclose(fin);
	if (inHead.nz<1)
	{
		fprintf(stdout,"nz<1 OR read mrchead fault\n");
		exit(1);
	}
	shiftinf=(float *)malloc(inHead.nz*sizeof(float));
for (i=0;i<inHead.nz;++i)
{
	shiftinf[i]=0;
}
	write_shift(inf_shift,shiftinf,shiftinf,inHead.nz);
	/*
	fin=fopen(inf_shift,"wb");
	fwrite(shiftinf,inHead.nz*2*sizeof(double),1,fin);
	fclose(fin);
	*/
	align(inf,inf_shift,outf,inf_rtt);
	free(shiftinf);
}

int read_rotation( __IN const char * rotation_filepath,__OUT long * rtt )
{
	//���ı��ļ��ж�ȡrotation��Ϣ����ʧ�ܷ��� 0 
	FILE * fin;
	char str[20];
	if(rotation_filepath ==NULL)
		return 0;
	if( (fin=fopen(rotation_filepath,"r"))==NULL)
	{
		fprintf(stdout,"Error to open rotation read file: %s\n",rotation_filepath);
		return 0;
	}
	fscanf(fin,"%s %ld",str,rtt);
	fclose(fin);
	if(strcmp(str,"rotation"))
	{
		fprintf(stdout,"This file is not a rotation para file\n");
		return 0;
	}
	return 1;


}

int write_rotation( __IN const char * rotation_filepath,__IN const long rtt )
{
	//��rotation��Ϣд���ı��ļ�
	FILE * fin;
	//char filepath[200];
	int i=1;
	if(rotation_filepath ==NULL)
	{
		fprintf(stdout,"Error to open (write)file :%s\n","NULL");
		return 0;
	}
/*	memcpy(filepath,rotation_filepath,200);
	while(fin=fopen(filepath,"r"))
	{
		sprintf(filepath,"%s_%4d",rotation_filepath,i++);
		fclose(fin);
		if(i>9999)
		{
			fprintf(stdout,"the rotation para file greater than 9999,please delete them\n");
			return 0;
		}
	}
	*/
	fin=fopen(/*filepath*/rotation_filepath,"w");
	
	fprintf(fin,"rotation %ld",rtt);
	fclose(fin);
	return 1;

}

int read_shift( __IN const char * shift_filepath,__OUT float * shift_x,__OUT float * shift_y,__IN const int shiftnum )
{
	//���ı��ж�ȡshift��Ϣ������ʧ�ܷ���0
	FILE *fin;
	int i;
	if(shift_filepath ==NULL)
		return 0;
	if((fin=fopen(shift_filepath,"r"))==NULL)
	{
		fprintf(stdout,"Error to open shift read file :%s\n",shift_filepath==NULL?"NULL":shift_filepath);
		return 0;
	}
	for (i=0;i<shiftnum;++i)
	{
		if (fscanf(fin,"x%*d:%f\n",&shift_x[i])==EOF)
		{
			fprintf(stdout,"Can not read x %d\n",i);
			return 0;
		}
	}
	for (i=0;i<shiftnum;++i)
	{
		if(fscanf(fin,"y%*d:%f\n",&shift_y[i])==EOF)
		{
			fprintf(stdout,"Can not read y %d\n",i);
			return 0;
		}
	}
	fclose(fin);
	return 1;
}

int write_shift( __IN const char * shift_filepath,__IN float * shift_x,__IN float * shift_y,__IN const int shiftnum )
{
	//���ı���д��shift��Ϣ������ʧ�ܷ���0
	FILE * fin;
	//char filepath[200];
	int i=1;
	if(shift_filepath ==NULL)
	{
		fprintf(stdout,"Error to open (write)file :%s\n","NULL");
		return 0;
	}
/*	memcpy(filepath,shift_filepath,200);
	while(fin=fopen(filepath,"r"))
	{
		sprintf(filepath,"%s_%4d",shift_filepath,i++);
		fclose(fin);
		if(i>9999)
		{
			fprintf(stdout,"the shift para file greater than 9999,please delete them\n");
			return 0;
		}
	}
	*/
	if((fin=fopen(shift_filepath,"w"))==NULL)
	{
		fprintf(stdout,"Error to open shift (write)file :%s\n",shift_filepath);
		return 0;
	}

	for (i=0;i<shiftnum;++i)
	{
		fprintf(fin,"x%d:%f\n",i,shift_x[i]);
	}
	for (i=0;i<shiftnum;++i)
	{
		fprintf(fin,"y%d:%f\n",i,shift_y[i]);
	}
	
	fclose(fin);
	return 1;
}








