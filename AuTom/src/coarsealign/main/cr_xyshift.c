#include "cr_libfunc.h"

/*���������в���*/
void display_usage()
{
//	if (argc<5)
		fprintf(stdout," description:\n");
		fprintf(stdout,"	calculate the shift adjustment parameters iteratively, basing on the coarse alignment parameters from last circle.\n");
		fprintf(stdout," usage:\n");
		fprintf(stdout,"	./cr_xyshift -i <input_stack> -o [output_shift_data] -r [input_rotation_data] -s [input_shift_data]>\n");
		fprintf(stdout,"	-i <input_stack>: raw stack in mrc format;\n");//ȫ��st�ļ�
		fprintf(stdout,"	-o [output_shift_data]: outputted data of parameters for shift adjustment;(default is f_xyshift_glb)\n");//����cr֮�����ɵ�shiftȫ���ļ�f_xyshift_glob
		fprintf(stdout,"	-r [input_rotation_data]: inputted data of rotation adjustment parameters from last circle(Please don't use it, during the first circle);\n");
		fprintf(stdout,"	-s [input_shift_data]: inputted data of shift adjustment parameters from last circle(Please don't use it, during the first circle).\n");
		fprintf(stdout," example:\n");
		fprintf(stdout,"./cr_xyshift -i rawImage.st -o xyshift_glb.dat -r rotation_glb.dat -s xyshift_glb_last.dat\n");

		exit(1);
//	char otp[]="f_xyshift_glob";
//	outf_shift=otp;
//	char itp[]="f_rotation_glb";
//	inf_rtt=itp;
}
void display_version()
{
	fprintf(stdout,"cr_xyshift Version: 1.0\n");
	fprintf(stdout," Copyright by ibp & inspur.\n");
}



/******************************************************************/
/******************************************************************/
/*********************    main       ******************************/
/******************************************************************/
/******************************************************************/


int main(int argc, char **argv)
{
	/******************************opt*************/
	char *inf_image=NULL ;//.st
	char *outf_shift=NULL;//f_xyshift_glb?
	char *inf_rtt=NULL ;//f_rotation_glb of last round
	char *inf_shift=NULL;//f_xyshift_glb of last round
	//������Ϊ���ǵ���ģ�������������С�������������ݵļ��飬�����ļ���Ӧ�ø��������ļ������������������ļ�����Ϊ���ļ���.crxyshift.rnd(ѭ����).glb����
	if(argc<2)
	{
		display_usage();
		exit(1);
	}
	int opt;
	opt = getopt( argc, argv, "i:o:r:s:vh?" );
	while( opt != -1 ) {
		switch( opt ) {
	case 'i':
		inf_image = optarg; 
		break;

	case 'o':
		outf_shift = optarg;
		break;

	case 'r':
		inf_rtt = optarg;
		break;

	case 's':
		inf_shift=optarg;
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

		opt = getopt( argc, argv, "i:o:r:s:vh?" );
	}
	/*********************opt end***********************/

	fprintf(stdout,"Calculating shift\n");
	clock_t start,end;
	start=clock();

	long /*n,*/ i;
/*	char *inf_image = argv[1];//������stack����aligned images��ֻ���
	char *outf_shift = argv[2];//������ƽ��У���������ļ�����������ȷ����������һ�ֽ�����
	char *inf_rtt = argv[3];//������һ�ֵĵ���תУ��������ֻ���
	char *inf_shift = argv[4];//������һ�ֵĵ�ƽ��У��������ֻ���
*/	
	//������Ϊ���ǵ���ģ�������������С�������������ݵļ��飬�����ļ���Ӧ�ø��������ļ������������������ļ�����Ϊ���ļ���.crxyshift.rnd(ѭ����).glb����

	//define the range of rotative parameter with the unit of 0.1 degree.
	long smp_dgr = 1800;

	//define the rotative parameter with the unit of 0.1 degree.
	//long *dgr_rtt;
	//dgr_rtt = (long *)malloc(sizeof(long));
	long dgr_rtt;

	//open files
	FILE *fin=NULL, *fin_shift=NULL, /**fout,*/ *fin_rtt=NULL;

	if((fin = fopen(inf_image,"rb")) == NULL)
	{
		fprintf(stdout,"Cannot open file strike,any key exit!\n");
		exit(1);
	}
	if(read_rotation(inf_rtt,&dgr_rtt) && (fin_shift = fopen(inf_shift,"rb")) != NULL)
		;
	else
		dgr_rtt=0;
/*	if ((fin_rtt = fopen(inf_rtt,"rb")) != NULL && (fin_shift = fopen(inf_shift,"rb")) != NULL)
	{
		fread(dgr_rtt, sizeof(long), 1, fin_rtt);
	}
	else
	{
		dgr_rtt[0] = 0;
	}
	if(fin_rtt)
		fclose(fin_rtt);
		*/
	if(fin_shift)
		fclose(fin_shift);

	//define input and output heads..........
	MrcHeader  *inhead, *outhead;
	inhead = (MrcHeader *)malloc(sizeof(MrcHeader));
	outhead = (MrcHeader *)malloc(sizeof(MrcHeader));

	mrc_read_head(fin,inhead);

	if(inhead->nx % 2 == 1 || inhead->ny % 2 == 1  )
	{
		fprintf(stdout,"Error in fuction 'main()'! Image size must be even in both direction!\n");
		exit(1);
	}

	memcpy(outhead,inhead,sizeof(MrcHeader));
	//mrc_read_head(fin,outhead);
	outhead->mode = MRC_MODE_FLOAT;

/*	outhead->amin = inhead->amin;
	outhead->amax = inhead->amax;
	outhead->amean = inhead->amean;
*/	//�������������������Ƕ����ģ�

	unsigned long image_in_size2d, image_out_size2d;

	image_in_size2d = inhead->nx * inhead->ny;
	image_out_size2d = outhead->nx*outhead->ny;
	//define input and output vectors..............

	double *image1, *image2;
	image1 = (double *)malloc(image_in_size2d*sizeof(double));
	image2 = (double *)malloc(image_in_size2d*sizeof(double));

/*	float *corr_out_float;
	corr_out_float = (float *)malloc(image_in_size2d*sizeof(float));
*/
	long /**shift_number, */*shift_x, *shift_y/*, shift_max*/;
	shift_x = (long *)malloc(inhead->nz*sizeof(long));
	shift_y = (long *)malloc(inhead->nz*sizeof(long));
	//shift_number = (long *)malloc(inhead->nz*sizeof(long));


/*	double *i_x_flt, *i_x_int, *i_y_flt, *i_y_int;
	i_x_flt = (double *)malloc(inhead->nz*sizeof(double));
	i_x_int = (double *)malloc(inhead->nz*sizeof(double));
	i_y_flt = (double *)malloc(inhead->nz*sizeof(double));
	i_y_int = (double *)malloc(inhead->nz*sizeof(double));
*/

/*	float *image_out, *image_pre_fft;
	double *image;
	image_pre_fft = (float *)malloc(image_in_size2d*sizeof(float));
	image_out = (float *)malloc(image_in_size2d*sizeof(float));
	image = (double *)malloc(image_in_size2d*sizeof(double));
*/
	long /*x, y,*/ z;
	
	//read two neighbor images
	//from this is function: mrc_read_slice_double
	//������һ�����⣬���ǵ�ʱ���ڲ��Ե�ʱ�������ſ�����ͷ�ļ�mrcfile.c������������mrc_read_slice_double�ڶ�ȡ����4G���ļ�ʱ������4G�����ݲ��ֲ����������������Ұ����Ĵ���չ���������Ͳ��������������⡣������ԭ���в�������
	//���Դӵڶ��ֿ�ʼ����һ��image2�����ݶ��뵽image1��ٵ�����ļ��ж�ȡimage2����֪���Ƿ�����������Ч�ʡ�
	
	union 
	{
		short s;
		unsigned char c;
		float f;
	}pxl_val;

	for(z=0;z<(inhead->nz-1);z++)
	{
		if (inhead->mode == MRC_MODE_FLOAT)
		{
			fseek(fin,1024+z*image_in_size2d*sizeof(float),SEEK_SET);
			for(i=0;i<image_in_size2d;i++)
			{
				fread(&pxl_val.f,sizeof(float),1,fin);
				image1[i]=(double)pxl_val.f;
			}
			fseek(fin,1024+(z+1)*image_in_size2d*sizeof(float),SEEK_SET);
			for(i=0;i<image_in_size2d;i++)
			{
				fread(&pxl_val.f,sizeof(float),1,fin);
				image2[i]=(double)pxl_val.f;
			}
		}
		else if (inhead->mode == MRC_MODE_BYTE)
		{
			fseek(fin,1024+z*image_in_size2d*sizeof(char),SEEK_SET);
			for(i=0;i<image_in_size2d;i++)
			{
				fread(&pxl_val.c,sizeof(char),1,fin);
				image1[i]=(double)pxl_val.c;
			}
			fseek(fin,1024+(z+1)*image_in_size2d*sizeof(char),SEEK_SET);
			for(i=0;i<image_in_size2d;i++)
			{
				fread(&pxl_val.c,sizeof(char),1,fin);
				image2[i]=(double)pxl_val.c;
			}
		}
		else if (inhead->mode == MRC_MODE_SHORT)
		{
			fseek(fin,1024+z*image_in_size2d*sizeof(short),SEEK_SET);
			for(i=0;i<image_in_size2d;i++)
			{
				fread(&pxl_val.s,sizeof(short),1,fin);
				image1[i]=(double)pxl_val.s;
			}
			fseek(fin,1024+(z+1)*image_in_size2d*sizeof(short),SEEK_SET);
			for(i=0;i<image_in_size2d;i++)
			{
				fread(&pxl_val.s,sizeof(short),1,fin);
				image2[i]=(double)pxl_val.s;
			}
		}
		//to this is function: mrc_read_slice_double
		
#ifdef _DEBUG
		fprintf(stdout,"%d %f\n", z, image1[image_in_size2d/2+inhead->ny/2]);
#endif // _DEBUG

		cr_xyshift_step(image1, image2,  inhead, shift_x, z, shift_y, outhead);
	}

	cr_pos_para(inhead->nz,  shift_x, shift_y, dgr_rtt, inf_shift, smp_dgr, outf_shift);
	//
/*	free(image_out);
	free(image_pre_fft);
	free(image);
	free(corr_out_float);
*/	free(image1);
	free(image2);
	free(inhead);
	free(outhead);
	//free(dgr_rtt);
	free(shift_x);
	free(shift_y);
/*	free(shift_number);
	free(i_x_flt);
	free(i_x_int);
	free(i_y_flt);
	free(i_y_int);
*/	fclose(fin);
	//fclose(fout);

	end=clock();
	fprintf(stdout,"%s run time:%fs\n",argv[0],(double)(end-start)/CLOCKS_PER_SEC);

	return 0;

}
