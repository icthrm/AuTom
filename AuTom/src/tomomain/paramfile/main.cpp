#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <getopt.h>
#include <regex.h>
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
	if(argc<2)//no param
	{
		fprintf(stdout,"paramfile:Modify param.par \n");
		fprintf(stdout,"Usage:paramfile\n");
		fprintf(stdout,"\t -n  --new  create param.par\n");
		fprintf(stdout,"\t -a  --angle  angle.par   input angle file\n");
		fprintf(stdout,"\t -s  --shift  shift.dat   input shift file\n");
		fprintf(stdout,"\t -r  --rotation  rotation.dat   input rotation file\n");
		fprintf(stdout,"\t -t  --thickness  thickness   input thickness number\n");
		fprintf(stdout,"\t -z  --zshift  zshift   input zshift number\n");
		fprintf(stdout,"\t -o  --offset  offset   input offset number\n");
		fprintf(stdout,"\t -p  --pitch pitch   input pitch number\n");
		return -1;
	}
	char parFilepath[]="param.par";
	char par1Filepath[]="param1.par";
	char *angleFilepath=NULL;
	char *shiftFilepath=NULL;
	char *rotFilepath=NULL;
	int ifNew=0,ifThickness=0,ifZshift=0,ifOffset=0,ifPitch=0;
	int thickness=0;
	float zshift=0;
	float offset=0;
	float pitch=0;

	int opt;
	static const char *shortopts = "a:s:r:t:z:o:p:nvh?";
	struct option longopts[] = {
		{"new", no_argument, NULL, 'n'},
		{"angle", required_argument, NULL, 'a'},
		{"shift", required_argument, NULL, 's'},
		{"rotation", required_argument, NULL, 'r'},
		{"thickness", required_argument, NULL, 't'},
		{"zshift", required_argument, NULL, 'z'},
		{"offset", required_argument, NULL, 'o'},
		{"pitch", required_argument, NULL, 'p'},
		{0,0,0,0},
	};

	while ((opt = getopt_long (argc, argv, shortopts, longopts, NULL)) != -1)
	{
		switch( opt ) {
			case 'n':
				ifNew=1;
				break;
			case 'a':
				angleFilepath = optarg;
				break;
			case 'r':
				rotFilepath = optarg;
				break;

			case 's':
				shiftFilepath=optarg;
				break;
			case 't':
				thickness=atoi(optarg);
				ifThickness=1;
				break;
			case 'z':
				zshift=atof(optarg);
				ifZshift=1;
				break;
			case 'o':
				offset=atof(optarg);
				ifOffset=1;
				break;
			case 'p':
				pitch=atof(optarg);
				ifPitch=1;
				break;

			case 'v':
				display_version();
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

	FILE * shiftFile,*rotFile,*angleFile,*parFile;
	int ifShift,ifAngle,rot,ifRot;
#if 0
	if(!(parFile=fopen(parFilepath,"r+")))
	{
		fprintf(stdout,"Can not found param.par in current path,create new one!\n");
		if(!(parFile=fopen(parFilepath,"w+")))
		{
			fprintf(stderr,"Error to create param.par!\n");
			exit(-1);
		}
	}
#endif
	if(ifNew)
	{
		if(!(parFile=fopen(parFilepath,"w+")))
		{
			fprintf(stderr,"Error to create param.par!\n");
			exit(-1);
		}
	}
	else
	{
		if(!(parFile=fopen(parFilepath,"r+")))
		{
			fprintf(stderr,"Error to open param.par!\n");
			exit(-1);
		}
	}
	//now we have param.par
	float *angle=NULL,*shiftx=NULL,*shifty=NULL;
	int eNum=0;
	if(!(angleFile=fopen(angleFilepath,"r")))
	{
		fprintf(stdout,"Don't have angle file .\n");
		ifAngle=0;
	}
	else
	{
		ifAngle=1;
		char cmd[PATH_MAX]={0};
		sprintf(cmd,"wc -l %s",angleFilepath);
		FILE *getLineNum=popen(cmd,"r");
		fscanf(getLineNum,"%d %s\n",&eNum,cmd);
		pclose(getLineNum);
	}
	if(!(shiftFile=fopen(shiftFilepath,"r")))
	{
		fprintf(stdout,"Don't have shift file .\n");
		ifShift=0;
	}
	else
	{
		ifShift=1;
		char cmd[PATH_MAX]={0};
		sprintf(cmd,"wc -l %s",shiftFilepath);
		int tNum;
		FILE *getLineNum=popen(cmd,"r");
		fscanf(getLineNum,"%d %s\n",&tNum,cmd);
		pclose(getLineNum);
		if(ifAngle)
		{
			if(tNum/2 != eNum)
			{
				fprintf(stderr,"The Angle file and the shift file mismatches\n");
				exit(-1);
			}
		}
		else
		{
			eNum=tNum/2;
		}
	}

	if(!(rotFile=fopen(rotFilepath,"r")))
	{
		fprintf(stdout,"Don't have rotation file .\n");
		rot=0;
		ifRot=0;
	}
	else
	{
		ifRot=1;
		fscanf(rotFile,"rotation %d",&rot);
		fclose(rotFile);
	}
	if(ifAngle)
	{
		angle=(float*)malloc(eNum*sizeof(float));
		for(int i=0;i<eNum;++i)
		{
			fscanf(angleFile,"%f\n",&angle[i]);
		}
		fclose(angleFile);
	}
	if(ifShift)
	{
		shiftx=(float*)malloc(eNum*sizeof(float));
		shifty=(float*)malloc(eNum*sizeof(float));
		int idx;
		for(int i=0;i<eNum;++i)
		{
			fscanf(shiftFile,"x%d:%f\n",&idx,&shiftx[i]);
			//if(idx!=i)//error
		}
		for(int i=0;i<eNum;++i)
		{
			fscanf(shiftFile,"y%d:%f\n",&idx,&shifty[i]);
			//if(idx!=i)//error
		}
		fclose(shiftFile);
	}
	if(ifNew)
	{
		//new param.par
		fprintf(parFile,"#Alignment parameters\n");
		fprintf(parFile,"#TILT\tOFFSET\tXSHIFT\tYSHIFT\tROT\tMAG\n");
		float anglet,shiftxt,shiftyt;
		for(int i=0;i<eNum;++i)
		{
			if(ifAngle)
				anglet=angle[i];
			if(ifShift)
			{
				shiftxt=shiftx[i];
				shiftyt=shifty[i];
			}
			fprintf(parFile,"%f\t%f\t%f\t%f\t%d\t%f\n",anglet,offset,shiftxt,shiftyt,rot,0.0f);
		}
		fprintf(parFile,"#Geometry parameters\n");
		fprintf(parFile,"THICKNESS %d\n",thickness);
		fprintf(parFile,"ZSHIFT %f\n",zshift);
		fprintf(parFile,"OFFSET %f\n",offset);
		fprintf(parFile,"PITCH %f\n",pitch);
	}
	else
	{
		//exist param.par
		//FILE * par1File=fopen(parFilepath,"w");
		if(ifAngle || ifRot || ifShift)
		{	
			char buf[PATH_MAX]={0};
			char cmd[500]={0};
			int linenum;
			sprintf(cmd,"sed -n -e '/^[ \t]*#Alignment[ \t]*parameters[ \t]*$/=' %s",parFilepath);
			FILE * pfile=popen(cmd,"r");
			if(EOF==fscanf(pfile,"%d",&linenum))
			{
				fprintf(stderr,"Error:Can not find Alignment parameters in %s!\n",parFilepath);
				return(-1);
			}
			pclose(pfile);
			int countLine;
			if(ifAngle || ifShift)
			{
				countLine=eNum;
			}
			else
			{
				memset(cmd,0,500);
				sprintf(cmd,"sed -n -e '/^\\([-0-9.]\\+[ \t]\\+\\)\\{5\\}[-0-9.]\\+$/=' %s | wc -l",parFilepath);
				pfile=popen(cmd,"r");
				if(EOF==fscanf(pfile,"%d",&countLine))
				{
					fprintf(stderr,"Error:Can not find Align param in %s!\n",parFilepath);
					return -1;
				}
				pclose(pfile);
			}
			float anglet,shiftxt,shiftyt,rott,offsett;
			linenum+=2;
			for(int i=0;i<countLine;++i)
			{
				memset(cmd,0,500);
				sprintf(cmd,"sed -n -e '%dp' %s",linenum,parFilepath);
				pfile=popen(cmd,"r");
				fscanf(pfile,"%f %f %f %f %d",&anglet,&offsett,&shiftxt,&shiftyt,&rott);
				pclose(pfile);
				if(ifAngle)
					anglet=angle[i];
				if(ifShift)
				{
					shiftxt=shiftx[i];
					shiftyt=shifty[i];
				}
				if(ifRot)
					rott=rot;
				if(ifOffset)
					offsett=offset;
				memset(cmd,0,500);
				sprintf(cmd,"sed -i -e "
						"'%ds/^\\([-0-9.]\\+[ \t]\\+\\)\\{5\\}[-0-9.]\\+$/%f\t%f\t%f\t%f\t%d\t%f/' %s"
						,linenum,anglet,offsett,shiftxt,shiftyt,rott,0,parFilepath);
				system(cmd);
				++linenum;

			}

		}
		if(ifThickness)
		{
			char cmd[500]={0};
			sprintf(cmd,"sed -i -e 's/^[ \t]*THICKNESS[ \t]\\+[0-9]\\+[ \t]*$/THICKNESS %d/' %s",thickness,parFilepath);
			system(cmd);
			ifThickness=0;
		}
		if(ifZshift)
		{
			char cmd[500]={0};
			sprintf(cmd,"sed -i -e 's/^[ \t]*ZSHIFT[ \t]\\+[-0-9.]\\+[ \t]*$/ZSHIFT %f/' %s",zshift,parFilepath);
			system(cmd);
			ifZshift=0;
		}
		if(ifOffset)
		{
			char cmd[500]={0};
			sprintf(cmd,"sed -i -e 's/^[ \t]*OFFSET[ \t]\\+[-0-9.]\\+[ \t]*$/OFFSET %f/' %s",offset,parFilepath);
			system(cmd);
			ifOffset=0;
		}
		if(ifPitch)
		{
			char cmd[500]={0};
			sprintf(cmd,"sed -i -e 's/^[ \t]*PITCH[ \t]\\+[-0-9.]\\+[ \t]*$/PITCH %f/' %s",pitch,parFilepath);
			system(cmd);
			ifPitch=0;
		}
	}

	if(!shiftx)
		free(shiftx);
	if(!shifty)
		free(shifty);
	if(!angle)
		free(angle);
	fclose(parFile);

	return 0;
}
