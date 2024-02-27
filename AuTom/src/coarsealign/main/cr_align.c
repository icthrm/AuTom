#include "cr_libfunc.h"

void display_usage()
{
		printf(" description:\n");
		printf("	align the stack referring to the coarse alignment parameters(only countaining the valid region for reconstruct).\n");
		printf(" usage:\n");
		printf("	./cr_align -i <input_raw_stack> -o <output_stack> -r <input_rotation_data> -s <input_shift_data>\n");
		printf("	-i <input_raw_stack>: raw stack in mrc format;\n");
		printf("	-o <output_stack>: outputted aligned stack in mrc format;\n");
		printf("	-r <input_rotation_data>: inputted data of rotation adjustment parameters(don't use, if inavailable);\n");
		printf("	-s <input_shift_data>: inputted data of shift adjustment parameters.\n");
		printf(" example:\n");
		printf("	./cr_align -i caveolae060.st -o caveolae060.shift2.st -r caveolae060.rotation2.dat -s f_xyshift_glb1\n");
		exit(1);

}
void display_version()
{
	fprintf(stdout,"cr_align Version: 1.0\n");
	fprintf(stdout," Copyright by ibp & inspur.\n");
}


int main(int argc, char **argv)
{
	char *inf = NULL;//������stack����raw images��ֻ���argv[1]
	char *outf =NULL ;//������stack���������򸲸ǡ�argv[2]
	char *inf_rtt =NULL ;//��������תУ��������ֻ���argv[3]
	char *inf_shift =NULL ;///������ƽ��У��������ֻ���argv[4]
	/******************************opt*************/
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
		inf = optarg; 
		break;

	case 'o':
		outf = optarg;
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

/*	if(inf_rtt==NULL)
	{
		fprintf(stdout,"rotation file must be haven\n");
		exit(1);
	}
	*/
	clock_t start,end;
	start=clock();

	align(inf,inf_shift,outf,inf_rtt);

	end=clock();
	fprintf(stdout,"%s run time:%fs\n",argv[0],(double)(end-start)/CLOCKS_PER_SEC);

	return 0;
}
