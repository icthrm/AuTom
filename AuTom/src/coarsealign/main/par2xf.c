#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <getopt.h>
#define PATH_MAX 256
void display_version()
{
	;
}
void display_usage()
{
	;
}
int  main(int argc,char* argv[])
{
	if(argc<3)//no param
	{
		fprintf(stdout,"par2xf:Trans shift.dat and rot.dat to param.xf\n");
		fprintf(stdout,"Usage:par2xf -s shift.dat -r rot.dat -o param.par -I\n");
		fprintf(stdout,"\t -s  --shift  shift_param_filename   input shift parma (this file create by cr_align_shift)\n");
		fprintf(stdout,"\t -r  --rotation  rotation_param_filename    input rotation parma (this file create by cr_rotation)\n");
		fprintf(stdout,"\t -o  --output  output_param_filename(default is param.par)    output parma (if use option \"-i\" ,the output format is imod's else it's ATOM's.)\n");
		fprintf(stdout,"\t -I  --IMOD      use imod format\n");
		return -1;
	}
	char xfname[]="param.par";
	char *shiftFilepath=NULL;
	char *rotFilepath=NULL;
	char *xfFilepath=xfname;
	int use_imod=0;
	int opt;
	static const char *shortopts = "o:r:s:Ivh?";
	struct option longopts[] = {
		{"shift", required_argument, NULL, 's'},
		{"rotation", required_argument, NULL, 'r'},
		{"output", required_argument, NULL, 'o'},
		{"IMOD", no_argument, NULL, 'I'},
		{0, 0, 0, 0},
	};

	while ((opt = getopt_long (argc, argv, shortopts, longopts, NULL)) != -1)
	{
		switch( opt ) {
			case 'o':
				xfFilepath = optarg;
				break;

			case 'r':
				rotFilepath = optarg;
				break;

			case 's':
				shiftFilepath=optarg;
				break;

			case 'v':
				display_version();
				break;
			case 'I':
				use_imod=1;
				break;

			case 'h':
			case '?':
				display_usage();
				break;

			default:
				fprintf(stdout,"The opt -%c is undefine\n",opt);
				break;
		}

	}
	/*********************opt end***********************/

	FILE * shiftFile,*rotFile,*xfFile,*shiftyFile;
	if(!(shiftFile=fopen(shiftFilepath,"r")))
	{
		fprintf(stderr,"Error to open shift file : %s\n",shiftFilepath==NULL?"NULL":shiftFilepath);
		return -1;
	}
	char PATH[PATH_MAX]={0};
	char name[PATH_MAX]={0};
	if(shiftFilepath!=NULL)
	{
		sprintf(name,"%s_tmp",shiftFilepath);
		sprintf(PATH,"cp %s %s",shiftFilepath,name);
		system(PATH);

		if(!(shiftyFile=fopen(name,"r")))
		{
			fprintf(stderr,"Error to open file : %s\n",name);
			return -1;
		}
	}
	if(!(rotFile=fopen(rotFilepath,"r")))
	{
		fprintf(stderr,"No rot file : %s\n",rotFilepath==NULL?"NULL":rotFilepath);
		//return -1;
	}
	if(!(xfFile=fopen(xfFilepath,"w")))
	{
		fprintf(stderr,"Error to open xf file : %s\n",xfFilepath);
		return -1;
	}
	int rot;
	if(rotFilepath!=NULL)
	{
		fscanf(rotFile,"rotation %d",&rot);
		fclose(rotFile);
	}
	else
		rot=0;
	int idx,idx1;
	float shift,shifty;
	int ret=1;
	float sin_rot=sin(rot*M_PI/1800);
	float cos_rot=cos(rot*M_PI/1800);
	if(shiftFilepath!=NULL)
		while(fscanf(shiftyFile,"x%d:%f\n",&idx,&shift))
			;
	if(!use_imod)
	{
		fprintf(xfFile,"#Alignment parameters\n");
		fprintf(xfFile,"#TILT\tOFFSET\tXSHIFT\tYSHIFT\tROT\tMAG\n");
	}
	while(1)
	{
		if(shiftFilepath!=NULL)
		{
			ret=fscanf(shiftFile,"x%d:%f\n",&idx,&shift);
			if(ret<1 || ret==EOF)
				break;
			fscanf(shiftyFile,"y%d:%f\n",&idx1,&shifty);
		}
		else
		{
			shift=0.0f;
			shifty=0.0f;
		}
		if(use_imod)
		{
			fprintf(xfFile,"%f\t%f\t%f\t%f\t%f\t%f\n",cos_rot,-sin_rot,sin_rot,cos_rot,shift,shifty);
		}
		else
			fprintf(xfFile,"%f\t%f\t%f\t%f\t%d\t%f\n",0.0f,0.0f,shift,shifty,rot,0.0f);
	}
	if(shiftFilepath!=NULL)
	{
		fclose(shiftFile);
		fclose(shiftyFile);
		memset(PATH,0,PATH_MAX);
		sprintf(PATH,"rm -f %s",name);
		system(PATH);
	}
	fclose(xfFile);

	return 0;
}
