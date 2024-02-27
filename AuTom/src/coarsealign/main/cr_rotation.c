#include "cr_libfunc.h"

void display_usage()
{
		fprintf(stdout," description:\n");
		fprintf(stdout,"	taper, rotate by -45 degrees, and pad the projection for calculation of rotation adjustment parameters, reducing the artifacts of the edge.\n");
		fprintf(stdout,"	calculate the rotation adjustment parameter iteratively, basing on the coarse alignment parameters from last circle.\n");
		fprintf(stdout," usage:\n");
		fprintf(stdout,"	./cr_rotation -p <output_projection> -t <output_pad_projection> -o <output_rotation_data> -f <output_fourier_transform> -r [input_rotation_data] -s <input_projection>\n");
		fprintf(stdout," -p <output_projection>: outputted projection after process in mrc format.\n");
		fprintf(stdout," -t <output_pad_projection>: outputted projection after pad in mrc format.\n");
		fprintf(stdout,"	-o <output_rotation_data>: outputted data of parameters for rotation adjustment;\n");
		fprintf(stdout,"	-f <output_fourier_transform>: outputted plot of fourier transform for verification of result;\n");
		fprintf(stdout,"	[-r input_rotation_data]: inputted data of rotation adjustment parameters from last circle(Please don't use it, during the first circle).\n");
		fprintf(stdout,"	-s <input_projection>:after shift projection in mrc format;\n");
		fprintf(stdout," example:\n");
		fprintf(stdout,"./cr_rotation -p caveolae060.shift2.proj.mrc -t caveolae060.shift2.proj.trp.st -o f_rotation_glb2.dat -f caveolae060.fft1.mrc -r f_rotation_glb1.dat -s caveolae060.shift2.st\n");
		exit(1);

}

void display_version()
{
	fprintf(stdout,"cr_rotation Version: 1.0\n");
	fprintf(stdout," Copyright by ibp & inspur.\n");
}


int main(int argc, char **argv)
{
	char *outf_Prj = NULL;//����project,.argv[1]
	char *inf_st=NULL;//������stack����aligned images��ֻ���argv[6]
	char *outf_Pad = NULL;//����pad����project���������򸲸ǡ�argv[2]
	char *inf_Pad = NULL;//�����������л��õ�pad����project��ֻ���argv[2]
	char *outf_rtt = NULL;//��������תУ���������ļ�����������ȷ����������һ�ֽ�����argv[3]
	char *outf_FFT = NULL;//����pad��project�ĸ��Ҷ�任ͼ�����������򸲸ǡ�argv[4]
	char *inf_rtt =NULL;//������һ�ֵĵ���תУ��������ֻ��� argv[5]
/******************************opt*************/
	if(argc<2)
	{
		display_usage();
		exit(1);
	}
	int opt;
	opt = getopt( argc, argv, "p:t:o:f:r:s:vh?" );
	while( opt != -1 ) {
		switch( opt ) {
	case 'p':
		outf_Prj = optarg; 
		break;

	case 't':
		outf_Pad = optarg; 
		inf_Pad = optarg; 
		break;

	case 'o':
		outf_rtt = optarg;
		break;

	case 'f':
		outf_FFT = optarg;
		break;

	case 'r':
		inf_rtt = optarg;
		break;

	case 's':
		inf_st=optarg;
		break;

	case 'v':
		display_version();
		break;

	case 'h':
	case '?':
		display_usage();
		break;

	default:
		fprintf(stdout,"The opt -%c can not use\n",opt);
		break;
		}

		opt = getopt( argc, argv, "p:t:o:f:r:s:vh?" );
	}
	/*********************opt end***********************/
	clock_t start,end;
	start=clock();

	//open files..........
	FILE *fin=NULL, *fout_Pad=NULL;
	//////////
	cr_project(inf_st,outf_Prj);
	//////////

	if(((fin = fopen(outf_Prj,"rb")) == NULL) || ((fout_Pad = fopen(outf_Pad,"wb")) == NULL))
	{
		fprintf(stdout,"Cannot open file strike any key exit!\n");
		exit(1);
	}

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
	mrc_read_head(fin,outhead);

	outhead->mode = MRC_MODE_FLOAT;

	unsigned long image_in_size2d, image_out_size2d;
	image_in_size2d = inhead->nx*inhead->ny;
	image_out_size2d = outhead->nx*outhead->ny;


	double *image;
	image = (double *)malloc(image_in_size2d*sizeof(double));

	mrc_read_slice_double(fin, inhead, 0, 'z', image);
	/*float *imagef=(float*)malloc(image_in_size2d*sizeof(float));
	mrc_read_slice(fin,inhead,0,'z',imagef);
	int ii;
	for(ii=0;ii<image_in_size2d;++ii)
		image[ii]=(double)imagef[ii];
	free(imagef);
	*/
	//taper, rotate 45 degrees
	float *image_taper_rotate;
	fprintf(stdout,"executing trp\n");
	image_taper_rotate = (float *)malloc(image_in_size2d*sizeof(float));

	taper_rotate_plus_45degree(image, image_taper_rotate, outhead->nx, outhead->ny, inhead->nx, inhead->ny);



	//pad
	long pad_y, pad_x;
	pad_x = inhead->nx * 0.1;
	pad_y = inhead->ny * 0.1;

	outhead->nx = inhead->nx + 2*pad_x;
	outhead->ny = inhead->ny + 2*pad_y;
	image_out_size2d = outhead->nx*outhead->ny;

	float *image_pad;
	image_pad = (float *)malloc(image_out_size2d*sizeof(float));

	mrc_write_head(fout_Pad,outhead);

	img_pad(image_taper_rotate, image_pad, inhead->nx, inhead->ny, outhead->nx, outhead->ny, outhead->amean, pad_x, pad_y);

	fwrite(image_pad, image_out_size2d*sizeof(float), 1, fout_Pad);

	free(image_taper_rotate);
	free(image_pad);
	free(image);

	fclose(fin);
	fclose(fout_Pad);
	

	/*mrc_rotation_parameter.c*/
	fprintf(stdout,"Calculating rotation\n");
	//long n, i, j, x, y;
	long i;


	//open files..........
	FILE /**fin, *fout_rtt=NULL,*/ *fout2=NULL, *fin_rtt=NULL;

	if((fin = fopen(inf_Pad,"rb")) == NULL /*|| (fout_rtt = fopen(outf_rtt,"wb+")) == NULL*/ || (fout2 = fopen(outf_FFT,"wb+")) == NULL)
	{
		fprintf(stdout,"Cannot open file strike any key exit!\n");
		exit(1);
	}

	//long *rtt_dgr;
	//rtt_dgr = (long *)malloc(sizeof(long));
	long rtt_dgr;

	if(!read_rotation(inf_rtt,&rtt_dgr))
		rtt_dgr=0;
/*	if ((fin_rtt = fopen(inf_rtt,"rb")) != NULL)
	{
		fread(rtt_dgr, sizeof(long), 1, fin_rtt);
		fclose(fin_rtt);
	}
	else
	{
		rtt_dgr[0] = 0;
	}
*/
#ifdef _DEBUG
	fprintf(stdout,"%ld\n",rtt_dgr);
#endif // _DEBUG


	//define input and output heads..........
	//MrcHeader  *inhead, *outhead;
	//inhead = (MrcHeader *)malloc(sizeof(MrcHeader));
	//outhead = (MrcHeader *)malloc(sizeof(MrcHeader));

	mrc_read_head(fin,inhead);

	if(inhead->nx % 2 == 1 || inhead->ny % 2 == 1  )
	{
		fprintf(stdout,"Error in fuction 'main()'! Image size must be even in both directon!\n");
		exit(1);
	}

	mrc_read_head(fin,outhead);

//	unsigned long image_in_size2d, image_out_size2d;
	image_in_size2d = inhead->nx*inhead->ny;
	image_out_size2d = ((outhead->nx/2)+1)*outhead->ny;
	//define input and output vectors..............

	//double *image;
	float *image_out;
	image_out = (float *) malloc(sizeof(float)*image_in_size2d);
	image = (double *)malloc(image_in_size2d*sizeof(double));

/*	//��Ϊmrc_read_sliceû��double�͵ĺ��������Խ���image_out����image�ĸ�ֵ����
	mrc_read_slice(fin, inhead, 0, 'z', image_out);
	for(i=0;i<image_in_size2d;++i)
		image[i]=image_out[i];//float ��ֵ��double����������
*/
	mrc_read_slice_double(fin,inhead,0,'z',image);
	//fft and log
	img_fft(image, image_out, outhead->nx, outhead->ny);

	//smooth the image to reduce the artifact of low frequency signal
	img_smooth(image_out, outhead->nx, outhead->ny);

	//get the rotation adjustment parameter
	long smp_dgr, ddct_dgr, *max_n;
	max_n = (long *) malloc(sizeof(long));
	smp_dgr=1800;
	ddct_dgr=smp_dgr*10/180;

	cr_rotation_para(image_out, inhead->nx, inhead->ny, smp_dgr, ddct_dgr, max_n);
#ifdef _DEBUG
	fprintf(stdout,"%d\n", max_n[0]);
#endif // _DEBUG

	//figure out the orientation
	float max, mean, min;
	max=0;
	mean=0;
	min=0;
	for (i=0;i<image_in_size2d;i++)
	{
		if (image_out[i] > max) max=image_out[i];
		if (image_out[i] < min) min=image_out[i];
		mean += (image_out[i]/image_in_size2d);
	}

	outhead->amean = mean;
	outhead->amax = max;
	outhead->amin = min;
	outhead->nz=2;
	outhead->mode = MRC_MODE_FLOAT;
	mrc_write_head(fout2,outhead);
	fwrite(image_out, image_in_size2d*sizeof(float), 1, fout2);

	double i_x, i_x_flt, i_x_int, i_y, i_y_flt, i_y_int;
	long max_indecator;

	//
	if (max_n[0]<smp_dgr/2-2*ddct_dgr)
	{
		max_indecator=max_n[0]+ddct_dgr;
		for(i=1;i<inhead->nx/2;i++)
		{
			i_x=i*sin(3.1415926*max_indecator/smp_dgr);
			i_y=i*cos(3.1415926*max_indecator/smp_dgr);
			i_x_flt = modf(i_x, &i_x_int);
			i_y_flt = modf(i_y, &i_y_int);

			image_out[inhead->nx*(inhead->ny/2+((int)i_y_int))+inhead->nx/2+((int)i_x_int)]
			+=2*(1-i_x_flt)*(1-i_y_flt);
			image_out[inhead->nx*(inhead->ny/2+((int)i_y_int))+inhead->nx/2+((int)i_x_int)+1]
			+=2*(i_x_flt)*(1-i_y_flt);
			image_out[inhead->nx*(inhead->ny/2+((int)i_y_int)+1)+inhead->nx/2+((int)i_x_int)]
			+=2*(1-i_x_flt)*(i_y_flt);
			image_out[inhead->nx*(inhead->ny/2+((int)i_y_int)+1)+inhead->nx/2+((int)i_x_int)+1]
			+=2*(i_x_flt)*(i_y_flt);
			image_out[inhead->nx*(inhead->ny/2-((int)i_y_int))+inhead->nx/2-((int)i_x_int)]
			+=2*(1-i_x_flt)*(1-i_y_flt);
			image_out[inhead->nx*(inhead->ny/2-((int)i_y_int))+inhead->nx/2-((int)i_x_int)-1]
			+=2*(i_x_flt)*(1-i_y_flt);
			image_out[inhead->nx*(inhead->ny/2-((int)i_y_int)+1)+inhead->nx/2-((int)i_x_int)]
			+=2*(1-i_x_flt)*(i_y_flt);
			image_out[inhead->nx*(inhead->ny/2-((int)i_y_int)+1)+inhead->nx/2-((int)i_x_int)-1]
			+=2*(i_x_flt)*(i_y_flt);
		}
	}
	else
	{
		max_indecator=max_n[0]+3*ddct_dgr;
		for(i=1;i<inhead->nx/2;i++)
		{
			i_x=i*sin(3.1415926*max_indecator/smp_dgr);
			i_y=i*cos(3.1415926*max_indecator/smp_dgr);
			i_x_flt = modf(i_x, &i_x_int);
			i_y_flt = -modf(i_y, &i_y_int);

			image_out[inhead->nx*(inhead->ny/2+((int)i_y_int))+inhead->nx/2+((int)i_x_int)]
			+=2*(1-i_x_flt)*(1-i_y_flt);
			image_out[inhead->nx*(inhead->ny/2+((int)i_y_int))+inhead->nx/2+((int)i_x_int)+1]
			+=2*(i_x_flt)*(1-i_y_flt);
			image_out[inhead->nx*(inhead->ny/2+((int)i_y_int)-1)+inhead->nx/2+((int)i_x_int)]
			+=2*(1-i_x_flt)*(i_y_flt);
			image_out[inhead->nx*(inhead->ny/2+((int)i_y_int)-1)+inhead->nx/2+((int)i_x_int)+1]
			+=2*(i_x_flt)*(i_y_flt);
			image_out[inhead->nx*(inhead->ny/2-((int)i_y_int))+inhead->nx/2-((int)i_x_int)]
			+=2*(1-i_x_flt)*(1-i_y_flt);
			image_out[inhead->nx*(inhead->ny/2-((int)i_y_int))+inhead->nx/2-((int)i_x_int)-1]
			+=2*(i_x_flt)*(1-i_y_flt);
			image_out[inhead->nx*(inhead->ny/2-((int)i_y_int)-1)+inhead->nx/2-((int)i_x_int)]
			+=2*(1-i_x_flt)*(i_y_flt);
			image_out[inhead->nx*(inhead->ny/2-((int)i_y_int)-1)+inhead->nx/2-((int)i_x_int)-1]
			+=2*(i_x_flt)*(i_y_flt);
		}
	}
	fwrite(image_out, image_in_size2d*sizeof(float), 1, fout2);

	if (*max_n<smp_dgr/2-2*ddct_dgr)
	{
		*max_n=*max_n+ddct_dgr-450;
	}
	else if (*max_n>=smp_dgr/2-2*ddct_dgr && *max_n<(smp_dgr-4*ddct_dgr)*3/4)
	{
		*max_n=*max_n+3*ddct_dgr-450;
	}
	else
	{
		*max_n=*max_n+3*ddct_dgr-smp_dgr-450;
	}

	max_n[0]= rtt_dgr + max_n[0];
#ifdef _DEBUG
	fprintf(stdout,"%d(+450)\n", max_n[0]);
#endif // _DEBUG

	//fwrite(max_n, sizeof(long), 1, fout_rtt);
	write_rotation(outf_rtt,max_n[0]);

	//free(rtt_dgr);
	free(max_n);
	free(image);
	free(image_out);
	free(inhead);
	free(outhead);
	//fclose(fout_rtt);
	fclose(fout2);

	end=clock();
	fprintf(stdout,"%s run time:%fs\n",argv[0],(double)(end-start)/CLOCKS_PER_SEC);

	return 0;
}


