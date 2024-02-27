#include "mrcfile.h"

char geo_init_fn[] = "geo_init.par";
char geo_ref_fn[] = "geo.par";
char xyshift_fn[] = "xyshift.par";
char xyshift2_fn[] = "xyshift2.par";
char ang_init_fn[] = "ang_init.par";
/*******************************************************************************************/
int mrc_read_head (FILE *fin,  MrcHeader *head)
{

	if(ftell(fin)!=0)rewind(fin);
	fread(head,1024,1,fin);

	if(!(head->cmap[0]=='M'&&head->cmap[1]=='A'&&head->cmap[2]=='P'))
	{
		printf("Error with function 'mrc_read_head()'! Warning: Not MRC format! \n");
		exit(1);
	}

	return 0;

}


/*******************************************************************************************/
int mrc_write_head(FILE *fout, MrcHeader *head)
{

	if(ftell(fout)!=0)rewind(fout);

	if(!head||!(head->cmap[0]=='M'&&head->cmap[1]=='A'&&head->cmap[2]=='P'))
	{
		printf("Error with function 'mrc_write_head()'! Can not write the MrcHeader!");
		exit(1);
	}
	fwrite(head,1024,1,fout);

  return 0;
}




/*******************************************************************************************/
int mrc_init_head(MrcHeader *head)
{

	head->nx=1;
	head->ny=1;
	head->nz=1;

	head->mode=MRC_MODE_BYTE;

	head->nxstart=0;
	head->nystart=0;
	head->nzstart=0;

	head->mx=1;
	head->my=1;
	head->mz=1;

	head->xlen=1;
	head->ylen=1;
	head->zlen=1;

	head->alpha=90;
	head->beta=90;
	head->gamma=90;

	head->mapc=1;
	head->mapr=2;
	head->maps=3;

	head->amin=0;
	head->amax=255;
	head->amean=128;

	head->ispg=1;
	head->nsymbt=0;

	head->next=0;

	head->creatid=1000;
	head->cmap[0]='M';
	head->cmap[1]='A';
	head->cmap[2]='P';

	head->stamp[0]='D';

}



/*******************************************************************************************/
MrcHeader *mrc_new_head()
{

	MrcHeader *head;
	head=(MrcHeader *)malloc(sizeof(MrcHeader));
	mrc_init_head(head);

	return  head;
}


/*******************************************************************************************/
int mrc_replace_head(char *outf,MrcHeader *head)
{

	FILE *fout;
	if((fout=fopen(outf,"r+"))==NULL)
	{
		printf("Cannot open file strike any key exit!\n");
		exit(1);
	}
	mrc_write_head(fout,head);
	fclose(fout);

}




/*******************************************************************************************/
int mrc_check_head(char *inf)
{
	MrcHeader  *head;
	head=(MrcHeader *)malloc(sizeof(MrcHeader));

	FILE *fin;

	if((fin=fopen(inf,"r+"))==NULL)
	{
		printf("Cannot open file strike any key exit!\n");
		exit(1);
	}

	mrc_read_head(fin,head);
	fclose(fin);

	if(!head||!(head->cmap[0]=='M'&&head->cmap[1]=='A'&&head->cmap[2]=='P'))
	{
		return FALSE;
	}
	return TRUE;
	free(head);
}





/*******************************************************************************************/
//only MRC_MODE_BYTE MRC_MODE_SHORT MRC_MODE_FLOAT will be considered in this function

int mrc_update_head(char *inoutf)
{

	MrcHeader  *head;
	head=(MrcHeader *)malloc(sizeof(MrcHeader));

	FILE *finout;

	if((finout=fopen(inoutf,"r+"))==NULL)
	{
		printf("Cannot open file strike any key exit!\n");
		exit(1);

	}

	mrc_read_head(finout,head);
	
	if(!head||!(head->cmap[0]=='M'&&head->cmap[1]=='A'&&head->cmap[2]=='P'))
	{
		printf("The file '%s' is not a valid mrc file!\n",inoutf);
		exit(1);

	}

	double sum, sum_xy, amin, amax, amean;
	int k, j, i, pNum, pLen;
	//pNum is the number of pixels in one XoY  slice, 
	//pLen is the 
	unsigned long site;
	unsigned char *p_uchar;
	short *p_short;
	float *p_float;

	fseek(finout,1024,SEEK_SET);

	pNum=head->nx*head->ny;

	switch(head->mode)
	{
		//switch start

		/**********case MRC_MODE_BYTE ***********/
		case MRC_MODE_BYTE :
		
			pLen=pNum*sizeof(unsigned char);

			if((p_uchar=(unsigned char *)malloc(pLen))==NULL)
			{
				printf("Function 'malloc' erro, while updating head!\n");
				exit(1);

			}

			printf("updating head!\n");

			fread(p_uchar,pLen,1,finout);

			sum=sum_xy=0.0;
			amin=amax=p_uchar[0];

			for(site=0;site<pNum;site++)
			{
				if(p_uchar[site]>amax)amax=p_uchar[site];
				if(p_uchar[site]<amin)amin=p_uchar[site];
				sum_xy+=p_uchar[site];

			}

			sum+=sum_xy;

			for(k=1;k<head->nz;k++)
			{

				sum_xy=0.0;

				fread(p_uchar,pLen,1,finout);

				for(site=0;site<pNum;site++)
				{
					if(p_uchar[site]>amax)amax=p_uchar[site];
					if(p_uchar[site]<amin)amin=p_uchar[site];
					sum_xy+=p_uchar[site];

				}

				sum+=sum_xy;

			}

			amean=sum/(head->nx*head->ny*head->nz);

			free(p_uchar);

		break;



		/**********case MRC_MODE_SHORT ***********/
		case MRC_MODE_SHORT :
		
			pLen=pNum*sizeof(short);

			if((p_short=( short *)malloc(pLen))==NULL)
			{
				printf("Function 'malloc' erro, while updating head!\n");
				exit(1);

			}

			printf("updating head!\n");

			fread(p_short,pLen,1,finout);

			sum=sum_xy=0.0;
			amin=amax=p_short[0];

			for(site=0;site<pNum;site++)
			{
				if(p_short[site]>amax)amax=p_short[site];
				if(p_short[site]<amin)amin=p_short[site];
				sum_xy+=p_short[site];

			}

			sum+=sum_xy;

			for(k=1;k<head->nz;k++)
			{

				sum_xy=0.0;

				fread(p_short,pLen,1,finout);

				for(site=0;site<pNum;site++)
				{
					if(p_short[site]>amax)amax=p_short[site];
					if(p_short[site]<amin)amin=p_short[site];
					sum_xy+=p_short[site];

				}

				sum+=sum_xy;

			}

			amean=sum/(head->nx*head->ny*head->nz);

			free(p_short);

		break;



		/**********case MRC_MODE_FLOAT ***********/
		case MRC_MODE_FLOAT :
		
			pLen=pNum*sizeof(float);

			if((p_float=( float *)malloc(pLen))==NULL)
			{
				printf("Function 'malloc' erro, while updating head!\n");
				exit(1);

			}

			printf("updating head!\n");

			fread(p_float,pLen,1,finout);

			sum=sum_xy=0.0;
			amin=amax=p_float[0];

			for(site=0;site<pNum;site++)
			{
				if(p_float[site]>amax)amax=p_float[site];
				if(p_float[site]<amin)amin=p_float[site];
				sum_xy+=p_float[site];

			}

			sum+=sum_xy;

			for(k=1;k<head->nz;k++)
			{

				sum_xy=0.0;

				fread(p_float,pLen,1,finout);

				for(site=0;site<pNum;site++)
				{
					if(p_float[site]>amax)amax=p_float[site];
					if(p_float[site]<amin)amin=p_float[site];
					sum_xy+=p_float[site];

				}

				sum+=sum_xy;

			}

			amean=sum/(head->nx*head->ny*head->nz);

			free(p_float);

		break;

	}     //switch end

	head->amin=amin;
	head->amax=amax;
	head->amean=amean;

	fclose(finout);

	mrc_replace_head(inoutf,head);
	free(head);
	printf("updating head finished!\n");

}



	
	
/****************************************************************************************/
void mrc_new_file(char *newf,  MrcHeader  *outhead)
{
	FILE *fnew;
	if((fnew=fopen(newf,"w+"))==NULL)
	{
		printf("\nCannot open file strike any key exit!");
	}
	mrc_write_head(fnew,outhead);
	fclose(fnew);
}
	
	
	
	
/*******************************************************************************************/
	
int mrc_read_pixel(void *pix_gray, FILE *fin, MrcHeader  *head, int x, int y, int z )
{
	
	
		switch(head->mode)
		
		{
		case MRC_MODE_BYTE:
	
		fseek(fin,1024+(z*head->nx*head->ny+y*head->nx+x)*sizeof(char),SEEK_SET);
	
		if((fread(pix_gray, sizeof(char),1,fin)==0))
		{
			printf("Error with Function 'mrc_read_pixel()'! Reading file failed!");
			return -1;
		}
		break;
	
		case MRC_MODE_SHORT:
	
	
		fseek(fin,1024+(z*head->nx*head->ny+y*head->nx+x)*sizeof(short),SEEK_SET);
	
		if((fread(pix_gray, sizeof(short),1,fin)==0))
		{
			printf("Error with Function 'mrc_read_pixel()'! Reading file failed!");
			return -1;
		}
		break;
	
		case MRC_MODE_FLOAT:
	
		fseek(fin,1024+(z*head->nx*head->ny+y*head->nx+x)*sizeof(float),SEEK_SET);
	
		if((fread(pix_gray, sizeof(float),1,fin)==0))
		{
			printf("Error with Function 'mrc_read_pixel()'! Reading file failed!");
			return -1;
		}
		break;
	
		}
	
}
	
	
	
/*******************************************************************************************/
/*******slcN couts from 0 to N-1, so if you want to read the first slice slcN shoud be 0****/

int mrc_read_slice(FILE *fin, MrcHeader  *head, int slcN,char axis,float *slcdata)
{
	int psize;
	short buf_short;
	unsigned char buf_byte;
	float buf_float;
	int i,j,k;
	
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
					slcdata[i]=(float)buf_byte;
					fseek(fin,(head->nx-1)*psize,SEEK_CUR);
				}
	
			break;
	
			case MRC_MODE_SHORT:
			for(i=0;i<head->ny*head->nz;i++)
				{
					fread(&buf_short,psize,1,fin);
					slcdata[i]=(float)(buf_short);
					fseek(fin,(head->nx-1)*psize,SEEK_CUR);
				}
	
			break;
	
			case MRC_MODE_FLOAT:
			for(i=0;i<head->ny*head->nz;i++)
				{
				fread(&buf_float,psize,1,fin);
				slcdata[i]=buf_float;
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
					slcdata[k*head->nx+i]=(float)buf_byte;
				}
	
			break;
	
			case MRC_MODE_SHORT:
			for(i=0;i<head->nx;i++)
				{
					fread(&buf_short,psize,1,fin);
					slcdata[k*head->nx+i]=(float)(buf_short);
				}
	
			break;
	
			case MRC_MODE_FLOAT:
			for(i=0;i<head->nx;i++)
			{
				fread(&buf_float,psize,1,fin);
				slcdata[k*head->nx+i]=buf_float;
			}
	
			break;
	
		}

		}
		break;
	
	/***********************************Z************************************/
		case 'z':
		case 'Z':
		fseek(fin,1024+slcN*head->nx*head->ny*psize ,SEEK_SET);
	
		if(head->mode==MRC_MODE_FLOAT)fread(slcdata,psize*head->nx*head->ny,1,fin); 
	
		if(head->mode==MRC_MODE_BYTE)
		{
			for(i=0;i<head->nx*head->ny;i++)
			{
			fread(&buf_byte,psize,1,fin);
			slcdata[i]=(float)buf_byte;
			}
		}
	
		if(head->mode==MRC_MODE_SHORT)
		{
			for(i=0;i<head->nx*head->ny;i++)
			{
				fread(&buf_short,psize,1,fin);
				slcdata[i]=(float)buf_short;
			}
		}
		break;
	
	}
}
	
	
	
	
	
	/*****************************************************************************************************/
	
	
int mrc_read_all(FILE *fin, MrcHeader  *head, void *mrc_data_all)
{
	
		fseek(fin,1024,SEEK_SET);
	
		switch(head->mode)
		
		{
		case MRC_MODE_BYTE:
		if((fread(mrc_data_all,head->nx*head->ny*head->nz*sizeof(char),1,fin)==0))
		{
			printf("Error with Function 'mrc_read_all()'! Reading file failed!");
			return -1;
		}
		break;
	
		case MRC_MODE_SHORT:
		if((fread(mrc_data_all,head->nx*head->ny*head->nz*sizeof(short),1,fin)==0))
		{
			printf("Error with Function 'mrc_read_all()'! Reading file failed!");
			return -1;
		}
		break;
	
		case MRC_MODE_FLOAT:
		if((fread(mrc_data_all,head->nx*head->ny*head->nz*sizeof(float),1,fin)==0))
		{
			printf("Error with Function 'mrc_read_all()'! Reading file failed!");
			return -1;
		}
		break;
	
		}
}
	
	
	
/*******************************************************************************************/
/*int  mrc_cut_one_slice(FILE *fin, FILE *fout, int slcN, char axis )
{
	MrcHeader  *inhead, *outhead;

	inhead=(MrcHeader *)malloc(sizeof(MrcHeader));
	outhead=(MrcHeader *)malloc(sizeof(MrcHeader));

	mrc_read_head(fin,inhead);

	switch(axis)
	{	
		case 'x':
		case 'X':
		
		if(inhead->mode==MRC_MODE_FLOAT)
		{
		float *slcdata;
		slcdata = (float*)malloc(sizeof(float) * inhead->ny *inhead->nz);
		}	
		if(inhead->mode==MRC_MODE_BYTE)
		{
		unsigned char *slcdata;
		slcdata = (unsigned char*)malloc(sizeof(unsigned char) * inhead->ny *inhead->nz);
		}
		if(inhead->mode==MRC_MODE_SHORT)
		{
		short *slcdata;
		slcdata = (short*)malloc(sizeof(short) * inhead->ny *inhead->nz);
		}	
		break;

		float *slcdata;
		slcdata = (float*)malloc(sizeof(float) * inhead->ny *inhead->nz);
		break;

		case 'y':
		case 'Y':
	
		if(inhead->mode==MRC_MODE_FLOAT)
		{
		float *slcdata;
		slcdata = (float*)malloc(sizeof(float) * inhead->nx *inhead->nz);
		}	
		if(inhead->mode==MRC_MODE_BYTE)
		{
		unsigned char *slcdata;
		slcdata = (unsigned char*)malloc(sizeof(unsigned char) * inhead->nx *inhead->nz);
		}
		if(inhead->mode==MRC_MODE_SHORT)
		{
		short *slcdata;
		slcdata = (short*)malloc(sizeof(short) * inhead->nx *inhead->nz);
		}

		break;

		float *slcdata;
		slcdata = (float*)malloc(sizeof(float) * inhead->ny *inhead->nz);
		break;


		case 'z':
		case 'Z':
		
		if(inhead->mode==MRC_MODE_FLOAT)
		{
		float *slcdata;
		slcdata = (float*)malloc(sizeof(float) * inhead->nx *inhead->ny);
		}	
		if(inhead->mode==MRC_MODE_BYTE)
		{
		unsigned char *slcdata;
		slcdata = (unsigned char*)malloc(sizeof(unsigned char) * inhead->nx *inhead->ny);
		}
		if(inhead->mode==MRC_MODE_SHORT)
		{
		short *slcdata;
		slcdata = (short*)malloc(sizeof(short) * inhead->nx *inhead->ny);
		}

		float *slcdata;
		slcdata = (float*)malloc(sizeof(float) * inhead->ny *inhead->nz);
		break;

			
		default:
		printf("Can not recognize the axis in mrc_cut_one_slice\n");
		break;
	}
	mrc_read_slice(fin, inhead, slcN, axis, slcdata);

	*outhead = *inhead;
	switch(axis)
	{	
		case 'x':
		case 'X':
		outhead ->nx = 1;
		break;

		case 'y':
		case 'Y':
		outhead ->ny = 1;
		break;
	
		case 'z':
		case 'Z':
		outhead ->nz = 1;
		break;

		default:
		printf("Can not recognize the axis in mrc_cut_one_slice\n");
		break;
	}
	mrc_write_head(fout, outhead);
	mrc_add_slice(fout , outhead, slcdata);
 
	free(inhead);
	free(outhead);
	free(slcdata);
}

*/


/*******************************************************************************************/

int mrc_add_sliceN(FILE *fout, MrcHeader  *headout, float *slcdata, int slcN)
{
	
	
	fseek(fout,1024+sizeof(float)*headout->nx*headout->ny*slcN,SEEK_SET);
	if(fwrite(slcdata,sizeof(float)*headout->nx*headout->ny,1,fout) != 1) 
	{
		printf("Error with Function 'mrc_add_sliceN()'! Adding slice to file failed!");
		return -1;
	}
	
}
	
	
	
	
/*******************************************************************************************/
int mrc_add_slice(FILE *fout , MrcHeader  *headout, float *slcdata)
{
	
	fseek(fout,0,SEEK_END);
	fwrite(slcdata,headout->nx*headout->ny*sizeof(float),1,fout);
	
}
	
	
	
	
	/****************************************************************************************/
void mrc_write_slcX(FILE *fout,int slcN,float *data)
{
	int i;
	MrcHeader  *inhead;
	inhead=(MrcHeader *)malloc(sizeof(MrcHeader));
	mrc_read_head(fout,inhead);
	
	fseek(fout,1024+slcN*sizeof(float),SEEK_SET);
	for(i=0;i<inhead->ny*inhead->nz;i++)
	{
		fwrite(data+i,sizeof(float),1,fout);
		fseek(fout,sizeof(float)*(inhead->nx-1),SEEK_CUR);
	}
	free(inhead);
}
	
	
	
	


