/*
 * mrc_file.c
 *
 *  Created on: Apr 16, 2013
 *      Author: ict
 */
#include "mrc_file.h"

float rad(float ang) {
	return ang * PI / 180.0;
}

long get_file_size(FILE *fin) {
	fseek(fin, 0, SEEK_END);

	return ftell(fin);

}

/*******************************************************************************************/
int mrc_read_head(FILE *fin, MrcHeader *head) {
	if (ftello64(fin) != 0)
		rewind(fin);
	fread(head, 1024, 1, fin);

	/*if(!(head->cmap[0]=='M'&&head->cmap[1]=='A'&&head->cmap[2]=='P'))
	 {
	 printf("Error with function 'mrc_read_head()'! Warning: Not MRC format! \n");
	 return -1;
	 }*/

	return 0;
}

/*******************************************************************************************/
int mrc_write_head(FILE *fout, MrcHeader *head) {
	if (ftello64(fout) != 0)
		rewind(fout);
	if (!head
			|| !(head->cmap[0] == 'M' && head->cmap[1] == 'A'
					&& head->cmap[2] == 'P')) {
		printf(
				"Error with function 'mrc_write_head()'! Can not write the MrcHeader!");
		return -1;
	}
	fwrite(head, 1024, 1, fout);

	return 0;
}

/*******************************************************************************************/
int mrc_init_head(MrcHeader *head) {
	head->nx = 0;
	head->ny = 0;
	head->nz = 0;

	head->mode = MRC_MODE_FLOAT;

	head->nxstart = 0;
	head->nystart = 0;
	head->nzstart = 0;

	head->mx = 1;
	head->my = 1;
	head->mz = 1;

	head->xlen = 1;
	head->ylen = 1;
	head->zlen = 1;

	head->alpha = 90;
	head->beta = 90;
	head->gamma = 90;

	head->mapc = 1;
	head->mapr = 2;
	head->maps = 3;

	head->amin = 0;
	head->amax = 255;
	head->amean = 128;

	head->ispg = 0;
	head->nsymbt = 0;

	head->next = 0;

	head->creatid = 1000;
	head->cmap[0] = 'M';
	head->cmap[1] = 'A';
	head->cmap[2] = 'P';

	head->stamp[0] = 'D';
	return 0;
}

/*******************************************************************************************/
int mrc_replace_head(char *outf, MrcHeader *head) {
	FILE *fout;
	if ((fout = fopen(outf, "r+")) == NULL) {
		printf("Cannot open file strike any key exit!\n");
	}
	mrc_write_head(fout, head);
	fclose(fout);
	return 0;
}

/*******************************************************************************************/
int mrc_update_head(char *inoutf) {

	MrcHeader *head;
	head = (MrcHeader *) malloc(sizeof(MrcHeader));

	FILE *finout;

	if ((finout = fopen(inoutf, "r+")) == NULL) {
		printf("Cannot open file strike any key exit!\n");
	}

	mrc_read_head(finout, head);

	if (!head
			|| !(head->cmap[0] == 'M' && head->cmap[1] == 'A'
					&& head->cmap[2] == 'P')) {
		printf("Fatal erro! The file %s is not a valid mrc file!\n", inoutf);
		return -1;
	}

	float sum;
	float amin, amax, amean;
	int k, pNum;
	unsigned long site;
	unsigned char *p_uchar;
	short *p_short;
	float *p_float;

	fseek(finout, 1024, SEEK_SET);

	switch (head->mode) { //switch start

	/**********case MRC_MODE_BYTE ***********/
	case MRC_MODE_BYTE:

		pNum = head->nx * head->ny;

		if ((p_uchar = (unsigned char *) malloc(sizeof(unsigned char) * pNum))
				== NULL) {
			printf("Function 'malloc' erro, while updating head!\n");
			return -1;
		}

		printf("updating head!\n");
		fread(p_uchar, sizeof(unsigned char), pNum, finout);

		amin = amax = amean = p_uchar[0];
		sum = 0;
		for (site = 1; site < pNum; site++) {
			if (p_uchar[site] > amax)
				amax = p_uchar[site];
			if (p_uchar[site] < amin)
				amin = p_uchar[site];
			amean = amean + p_uchar[site];
		}
		amean /= pNum;
		sum = amean;
		for (k = 1; k < head->nz; k++) {
			amean = 0;
			fread(p_uchar, sizeof(unsigned char), pNum, finout);

			for (site = 0; site < pNum; site++) {
				if (p_uchar[site] > amax)
					amax = p_uchar[site];
				if (p_uchar[site] < amin)
					amin = p_uchar[site];
				amean = amean + p_uchar[site];
			}
			amean /= pNum;
			sum += amean;
		}
		amean = sum / head->nz;

		free(p_uchar);

		break;

		/**********case MRC_MODE_SHORT ***********/
	case MRC_MODE_SHORT:

		pNum = head->nx * head->ny;

		if ((p_short = (short *) malloc(sizeof(short) * pNum)) == NULL) {
			printf("Function 'malloc' erro, while updating head!\n");

			return -1;
		}

		printf("updating head!\n");

		fread(p_short, sizeof(short), pNum, finout);
		amin = amax = amean = p_short[0];
		sum = 0;
		for (site = 1; site < pNum; site++) {
			if (p_short[site] > amax)
				amax = p_short[site];
			if (p_short[site] < amin)
				amin = p_short[site];
			amean = amean + p_short[site];
		}
		amean /= pNum;
		sum = amean;
		for (k = 1; k < head->nz; k++) {
			amean = 0;
			fread(p_short, sizeof(short), pNum, finout);

			for (site = 0; site < pNum; site++) {
				if (p_short[site] > amax)
					amax = p_short[site];
				if (p_short[site] < amin)
					amin = p_short[site];
				amean = amean + p_short[site];
			}
			amean /= pNum;
			sum += amean;
		}
		amean = sum / head->nz;

		free(p_short);

		break;

		/**********case MRC_MODE_FLOAT ***********/
	case MRC_MODE_FLOAT:

		pNum = head->nx * head->ny;
		if ((p_float = (float *) malloc(sizeof(float) * pNum)) == NULL) {

			printf("Function malloc erro!\n");
			return -1;
		}

		printf("FLOAT:updating head!\n");

		fread(p_float, sizeof(float), pNum, finout);

		amin = amax = amean = p_float[0];
		sum = 0;
		for (site = 1; site < pNum; site++) {
			//printf("site :%d %d \n",pNum,site);
			if (p_float[site] > amax)
				amax = p_float[site];
			if (p_float[site] < amin)
				amin = p_float[site];
			amean = amean + p_float[site];
		}
		amean /= pNum;
		sum = amean;
		for (k = 1; k < head->nz; k++) {
			//printf("k :%d %d \n",head->nz,k);
			amean = 0;
			fread(p_float, sizeof(float), pNum, finout);

			for (site = 0; site < pNum; site++) {
				if (p_float[site] > amax)
					amax = p_float[site];
				if (p_float[site] < amin)
					amin = p_float[site];
				amean = amean + p_float[site];
			}
			amean /= pNum;
			sum += amean;
		}
		amean = sum / head->nz;

		free(p_float);

		break;
	} //switch end

	head->amin = amin;
	head->amax = amax;
	head->amean = amean;

	fclose(finout);

	mrc_replace_head(inoutf, head);
	free(head);
	printf("updating finished!\n");
	return 0;
}

/****************************************************************************************/
int mrc_new_file(char *newf, MrcHeader *outhead) {
	FILE *fnew;
	if ((fnew = fopen(newf, "w+")) == NULL) {
		printf("\nCannot open file strike any key exit!");
		return false;
	}
	mrc_write_head(fnew, outhead);
	fclose(fnew);
	return true;
}

/*******************************************************************************************/

int mrc_read_pixel(FILE *fin, MrcHeader *head, int x, int y, int z,
		void *pix_gray)

		{
	off_t offset;

	switch (head->mode)

	{
	case MRC_MODE_BYTE:
		offset = head->nx * head->ny;
		offset *= z;
		offset += (y * head->nx + x);
		offset *= sizeof(char);
		offset += 1024;
		fseeko(fin, offset, SEEK_SET);

		if ((fread(pix_gray, sizeof(char), 1, fin) == 0)) {
			printf(
					"Error with Function 'mrc_read_pixel()'! Reading file failed!");
			return false;
		}
		break;

	case MRC_MODE_SHORT:
		offset = head->nx * head->ny;
		offset *= z;
		offset += (y * head->nx + x);
		offset *= sizeof(short);
		offset += 1024;
		fseeko(fin, offset, SEEK_SET);
		//fseeko(fin,1024+(z*head->nx*head->ny+y*head->nx+x)*sizeof(short),SEEK_SET);

		if ((fread(pix_gray, sizeof(short), 1, fin) == 0)) {
			printf(
					"Error with Function 'mrc_read_pixel()'! Reading file failed!");
			return false;
		}
		break;

	case MRC_MODE_FLOAT:
		offset = head->nx * head->ny;
		offset *= z;
		offset += (y * head->nx + x);
		offset *= sizeof(float);
		offset += 1024;
		fseeko(fin, offset, SEEK_SET);
		//fseeko(fin,1024+(z*head->nx*head->ny+y*head->nx+x)*sizeof(float),SEEK_SET);

		if ((fread(pix_gray, sizeof(float), 1, fin) == 0)) {
			printf(
					"Error with Function 'mrc_read_pixel()'! Reading file failed!");
			return false;
		}
		break;

	}
	return 0;
}

/*******************************************************************************************/
int mrc_read_slice(FILE *fin, MrcHeader *head, int slcN, char axis,
		float *slcdata) {
//check the mrc file to make sure the size is exact in register with the head
	switch (head->mode) {
	case MRC_MODE_BYTE:
		if (get_file_size(fin) - (sizeof(MrcHeader) + head->next)
				!= head->nx * head->ny * head->nz * sizeof(char)) {
			printf("Error with Function 'mrc_read_slic()'! File size erro!\n");
		}
		break;

	case MRC_MODE_SHORT:
	case MRC_MODE_USHORT:
		if (get_file_size(fin) - (sizeof(MrcHeader) + head->next)
				!= (long int)head->nx * head->ny * head->nz * sizeof(short)) {
			printf("Error with Function 'mrc_read_slice()'! File size erro!\n");
		}
		break;

	case MRC_MODE_FLOAT:
		if (get_file_size(fin) - (sizeof(MrcHeader) + head->next)
				!= (long int)head->nx * head->ny * head->nz * sizeof(float)) {
			long int total_size = get_file_size(fin);
			int headsize = sizeof(MrcHeader) + head->next;
			printf("remainder:%lld shouldbe:%lld\n",get_file_size(fin) - (sizeof(MrcHeader) + head->next), (long long int)head->nx * head->ny * head->nz * sizeof(float));
			printf("%d %d %d %d \n", head->nx, head->ny, head->nz, head->next);
			fflush(stdout);
			printf("Error with Function 'mrc_read_slice()'! File size erro!\n");
		}
		break;

	default:
		printf("Error with Function 'mrc_read_slice()'! File type unknown!\n");

		break;
	}

	size_t psize;
	//int psize;
	short buf_short;
	unsigned short buf_ushort;
	unsigned char buf_byte;
	float buf_float;
	int i, k;

	switch (head->mode) {
	case MRC_MODE_BYTE:
		psize = sizeof(unsigned char);

		break;

	case MRC_MODE_SHORT:
	case MRC_MODE_USHORT:
		psize = sizeof(short);

		break;

	case MRC_MODE_FLOAT:
		psize = sizeof(float);

		break;
	}

	switch (axis) {

	/***********************************X************************************/
	case 'x':
	case 'X':

		fseek(fin, (sizeof(MrcHeader) + head->next) + slcN * psize, SEEK_SET);

		switch (head->mode) {
		case MRC_MODE_BYTE:
			for (i = 0; i < head->ny * head->nz; i++) {
				fread(&buf_byte, psize, 1, fin);
				slcdata[i] = (float) buf_byte;
				fseek(fin, (head->nx - 1) * psize, SEEK_CUR);
			}

			break;

		case MRC_MODE_SHORT:
			for (i = 0; i < head->ny * head->nz; i++) {
				fread(&buf_short, psize, 1, fin);
				slcdata[i] = (float) (buf_short);
				fseek(fin, (head->nx - 1) * psize, SEEK_CUR);
			}

			break;

		case MRC_MODE_USHORT:
			for (i = 0; i < head->ny * head->nz; i++) {
				fread(&buf_ushort, psize, 1, fin);
				slcdata[i] = (float) (buf_ushort);
				fseek(fin, (head->nx - 1) * psize, SEEK_CUR);
			}

			break;

		case MRC_MODE_FLOAT:
			for (i = 0; i < head->ny * head->nz; i++) {
				fread(&buf_float, psize, 1, fin);
				slcdata[i] = buf_float;
				fseek(fin, (head->nx - 1) * psize, SEEK_CUR);
			}
			break;

		}

		break;

		/***********************************Y************************************/
	case 'y':
	case 'Y':

		for (k = 0; k < head->nz; k++) {
			fseek(fin,
					(sizeof(MrcHeader) + head->next)
							+ (long int)psize
									* (k * (long int)head->nx * head->ny + head->nx * slcN),
					SEEK_SET);

			switch (head->mode) {
			case MRC_MODE_BYTE:
				for (i = 0; i < head->nx; i++) {
					fread(&buf_byte, psize, 1, fin);
					slcdata[k * head->nx + i] = (float) buf_byte;
				}

				break;

			case MRC_MODE_SHORT:
				for (i = 0; i < head->nx; i++) {
					fread(&buf_short, psize, 1, fin);
					slcdata[k * head->nx + i] = (float) (buf_short);
				}

				break;

			case MRC_MODE_USHORT:
				for (i = 0; i < head->nx; i++) {
					fread(&buf_ushort, psize, 1, fin);
					slcdata[k * head->nx + i] = (float) (buf_ushort);
				}

				break;

			case MRC_MODE_FLOAT:
				for (i = 0; i < head->nx; i++) {
					fread(&buf_float, psize, 1, fin);
					slcdata[k * head->nx + i] = buf_float;
				}

				break;

			}

		}
		break;

		/***********************************Z************************************/
	case 'z':
	case 'Z':
		fseek(fin,
				(sizeof(MrcHeader) + head->next)
						+ psize * slcN * head->nx * head->ny, SEEK_SET);

		if (head->mode == MRC_MODE_FLOAT)
			fread(slcdata, psize * head->nx * head->ny, 1, fin);

		else if (head->mode == MRC_MODE_BYTE) {
			for (i = 0; i < head->nx * head->ny; i++) {
				fread(&buf_byte, psize, 1, fin);
				slcdata[i] = (float) buf_byte;
			}
		}

		else if (head->mode == MRC_MODE_SHORT) {
			for (i = 0; i < head->nx * head->ny; i++) {
				fread(&buf_short, psize, 1, fin);
				slcdata[i] = (float) buf_short;
			}
		}

		else if (head->mode == MRC_MODE_USHORT) {
			for (i = 0; i < head->nx * head->ny; i++) {
				fread(&buf_ushort, psize, 1, fin);
				slcdata[i] = (float) buf_ushort;
			}
		}

		break;

	}

	return 0;
}
void mrc_read_slice_y(FILE *fin, MrcHeader *head, int slcN, float *slcdata) {
//check the mrc file to make sure the size is exact in register with the head
	int start =1024;
	switch (head->mode) {
	case MRC_MODE_BYTE:
		if (get_file_size(fin) - (sizeof(MrcHeader) + head->next)
				!= (long)head->nx * head->ny * head->nz * sizeof(char)) {
			printf("BYTE:Error with Function 'mrc_read_slic()'! File size erro!\n");
			start=get_file_size(fin) -head->nx * head->ny * head->nz * sizeof(char);
		}
		break;

	case MRC_MODE_SHORT:
	case MRC_MODE_USHORT:
		if (get_file_size(fin) - (sizeof(MrcHeader) + head->next)
				!= (long)head->nx * head->ny * head->nz * sizeof(short)) {
			printf("SHORT:Error with Function 'mrc_read_slice()'! File size erro!\n");
			start=get_file_size(fin)-head->nx * head->ny * head->nz * sizeof(short);
		}
		break;

	case MRC_MODE_FLOAT:
		if (get_file_size(fin) - (sizeof(MrcHeader) + head->next)
				!= (long)head->nx * head->ny * head->nz * sizeof(float)) {
			printf("FLOAT:Error with Function 'mrc_read_slice()'! File size erro!\n");
			start=get_file_size(fin) -head->nx * head->ny * head->nz * sizeof(float);
		}
		break;

	default:
		printf("Error with Function 'mrc_read_slice()'! File type unknown!\n");

		break;
	}

	size_t psize;
	//int psize;
	short buf_short;
	unsigned short buf_ushort;
	unsigned char buf_byte;
	float buf_float;
	int i, k;

	switch (head->mode) {
	case MRC_MODE_BYTE:
		psize = sizeof(unsigned char);
		break;
	case MRC_MODE_SHORT:
	case MRC_MODE_USHORT:
		psize = sizeof(short);
		break;
	case MRC_MODE_FLOAT:
		psize = sizeof(float);
		break;
	}

	for (k = 0; k < head->nz; k++) {
		fseek(fin, (sizeof(MrcHeader) + head->next) + psize * (k * head->nx * head->ny + head->nx * slcN),
				SEEK_SET);

		switch (head->mode) {
		case MRC_MODE_BYTE:
			//printf("MRC_MODE_BYTE\n");
			for (i = 0; i < head->nx; i++) {
				fread(&buf_byte, psize, 1, fin);
				slcdata[k * head->nx + i] = (float) buf_byte;
			}
			break;
		case MRC_MODE_SHORT:
			//printf("MRC_MODE_SHORT\n");
			for (i = 0; i < head->nx; i++) {
				fread(&buf_short, psize, 1, fin);
				slcdata[k * head->nx + i] = (float) (buf_short);
			}
			break;
		case MRC_MODE_USHORT:
			//printf("MRC_MODE_USHORT\n");
			for (i = 0; i < head->nx; i++) {
				fread(&buf_ushort, psize, 1, fin);
				slcdata[k * head->nx + i] = (float) (buf_ushort);
			}
			break;
		case MRC_MODE_FLOAT:
			//printf("MRC_MODE_FLOAT\n");
			for (i = 0; i < head->nx; i++) {
				fread(&buf_float, psize, 1, fin);
				slcdata[k * head->nx + i] = buf_float;
			}
			break;
		}

	}
}
/*****************************************************************************************************/

int mrc_read_block(FILE *fin, MrcHeader *head, int start, int end, char axis,
		float *mrc_data_block) {

//check the mrc file to make sure the size is exact in register with the head
	switch (head->mode) {
	case MRC_MODE_BYTE:
		if (get_file_size(fin) - 1024
				!= head->nx * head->ny * head->nz * sizeof(char)) {
			printf("Error with Function 'mrc_read_block()'! File size erro!");
		}
		break;

	case MRC_MODE_SHORT:
	case MRC_MODE_USHORT:
		if (get_file_size(fin) - 1024
				!= head->nx * head->ny * head->nz * sizeof(short)) {
			printf("Error with Function 'mrc_read_block()'! File size erro!");
		}
		break;

	case MRC_MODE_FLOAT:
		if (get_file_size(fin) - 1024
				!= head->nx * head->ny * head->nz * sizeof(float)) {
			printf("Error with Function 'mrc_read_block()'! File size erro!");
		}
		break;

	default:
		printf("Error with Function 'mrc_read_block()'! File type unknown!");

		break;
	}

	int i, k;
	unsigned char buf_byte;
	short buf_short;
	short buf_ushort;

	int psize;

	off_t offset;
	switch (head->mode) {
	case MRC_MODE_BYTE:
		psize = sizeof(unsigned char);

		break;

	case MRC_MODE_SHORT:
	case MRC_MODE_USHORT:
		psize = sizeof(short);

		break;

	case MRC_MODE_FLOAT:
		psize = sizeof(float);

		break;
	}

	switch (axis) {

	/***********************************X************************************/
	case 'x':
	case 'X':

		/*fseeko(fin,1024+slcN*psize,SEEK_SET);

		 switch(head->mode)
		 {
		 case MRC_MODE_BYTE:
		 for(i=0;i<head->ny*head->nz;i++)
		 {
		 fread(&buf_byte,psize,1,fin);
		 slcdata[i]=(float)buf_byte;
		 fseeko(fin,(head->nx-1)*psize,SEEK_CUR);
		 }

		 break;

		 case MRC_MODE_SHORT:
		 for(i=0;i<head->ny*head->nz;i++)
		 {
		 fread(&buf_short,psize,1,fin);
		 slcdata[i]=(float)(buf_short);
		 fseeko(fin,(head->nx-1)*psize,SEEK_CUR);
		 }

		 break;

		 case MRC_MODE_FLOAT:
		 for(i=0;i<head->ny*head->nz;i++)
		 {
		 fread(&buf_float,psize,1,fin);
		 slcdata[i]=buf_float;
		 fseeko(fin,(head->nx-1)*psize,SEEK_CUR);
		 }
		 break;

		 }*/

		break;

		/***********************************Y************************************/
	case 'y':
	case 'Y':
		//   fseeko(fin,1024+(k*head->nx*head->ny+head->nx*slcN)*psize,SEEK_SET);
		/* fseeko(fin,1024+slcN*head->nx*psize,SEEK_SET);
		 for(k=0;k<head->nz;k++)
		 {

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
		 fread(slcdata+k*head->nx,psize,head->nx,fin);


		 break;

		 }//end switch
		 fseeko(fin,head->nx*(head->ny-1)*psize,SEEK_CUR);
		 }//end for*/
		break;

		/***********************************Z************************************/
	case 'z':
	case 'Z':
		offset = head->nx * head->ny;
		offset *= (start * psize);
		offset += (1024);
		fseeko(fin, offset, SEEK_SET);
		//fseeko(fin,1024+slcN*head->nx*head->ny*psize,SEEK_SET);

		switch (head->mode) {
		case MRC_MODE_BYTE:
			for (i = 0; i < head->nx * head->ny * (end - start); i++) {
				fread(&buf_byte, psize, 1, fin);
				mrc_data_block[i] = (float) buf_byte;
			}
			break;

		case MRC_MODE_SHORT:
		case MRC_MODE_USHORT:
			for (i = 0; i < head->nx * head->ny * (end - start); i++) {
				fread(&buf_short, psize, 1, fin);
				mrc_data_block[i] = (float) buf_short;
			}
			break;

		case MRC_MODE_FLOAT:
			fread(mrc_data_block, psize, head->nx * head->ny * (end - start),
					fin);
			break;
		}

		break;

	}
	return 0;
}

/*****************************************************************************************************/

int mrc_read_all(FILE *fin, MrcHeader *head, float *mrc_data_all) {
	int headSize = 1024;
//check the mrc file to make sure the size is exact in register with the head
	switch (head->mode) {
	case MRC_MODE_BYTE:
		/*if (get_file_size(fin) - 2718
				!= head->nx * head->ny * head->nz * sizeof(char)) {
			printf("1.Error with Function 'mrc_read_all()'! File size erro!");
		}*/
		headSize = get_file_size(fin)
						- head->nx * head->ny * head->nz * sizeof(char);
		break;

	case MRC_MODE_SHORT:
	case MRC_MODE_USHORT:
		/*if (get_file_size(fin) - 2718
				!= head->nx * head->ny * head->nz * sizeof(short)) {
			printf("2.Error with Function 'mrc_read_all()'! File size erro!");
		}*/
		headSize = get_file_size(fin)
								- head->nx * head->ny * head->nz * sizeof(short);
		break;

	case MRC_MODE_FLOAT:
		/*if(get_file_size(fin) - 2718 != head->nx*head->ny*head->nz*sizeof(float))
		 {
		 printf("3.Error with Function 'mrc_read_all()'! File size erro!");
		 }
		 */
		headSize = get_file_size(fin)
				- head->nx * head->ny * head->nz * sizeof(float);
		//printf("%d",headSize);
		break;

	default:
		printf("4.Error with Function 'mrc_read_all()'! File type unknown!");

		break;
	}

	long i;
	unsigned char buf_byte;
	short buf_short;
	short buf_ushort;

	fseek(fin, (headSize), SEEK_SET);

	switch (head->mode) {
	case MRC_MODE_BYTE:

		for (i = 0; i < head->nx * head->ny * head->nz; i++) {
			fread(&buf_byte, sizeof(char), 1, fin);
			mrc_data_all[i] = (float) buf_byte;
		}
		break;

	case MRC_MODE_SHORT:
		for (i = 0; i < head->nx * head->ny * head->nz; i++) {
			fread(&buf_short, sizeof(short), 1, fin);
			mrc_data_all[i] = (float) buf_short;
		}
		break;

	case MRC_MODE_USHORT:

		for (i = 0; i < head->nx * head->ny * head->nz; i++) {
			fread(&buf_ushort, sizeof(short), 1, fin);
			mrc_data_all[i] = (float) buf_ushort;
		}
		break;

	case MRC_MODE_FLOAT:

		if ((fread(mrc_data_all, head->nx * head->ny * head->nz * sizeof(float),
				1, fin) == 0)) {
			printf(
					"Error with Function 'mrc_read_all()'! Reading file failed!");
			return -1;
		}
		break;

	default:
		printf("Error with Function 'mrc_read_all()'! File type unknown!");
		break;

	}

	return 0;
}

/*******************************************************************************************/
int mrc_write_slice(FILE *fout, MrcHeader *head, int slcN, char axis,
		float *slcdata) {
	int psize;
	if (head->mode == MRC_MODE_FLOAT)
		psize = sizeof(float);
	else {
		head->mode = MRC_MODE_FLOAT;
		printf("outfile headmode is error!\n");
		return false;
	}

	int i, k;
	off_t offset;

	switch (axis) {

	/***********************************X************************************/
	case 'x':
	case 'X':

		fseeko(fout, 1024 + slcN * psize, SEEK_SET);

		for (i = 0; i < head->ny * head->nz; i++) {
			fwrite(slcdata + i, psize, 1, fout);
			fseeko(fout, (head->nx - 1) * psize, SEEK_CUR);
		}
		break;

		/***********************************Y************************************/
	case 'y':
	case 'Y':
		fseeko(fout, 1024 + slcN * head->nx * psize, SEEK_SET);

		for (k = 0; k < head->nz; k++) {
			fwrite(slcdata + k * head->nx, psize, head->nx, fout);
			fseeko(fout, head->nx * (head->ny - 1) * psize, SEEK_CUR);
		}
		break;

		/***********************************Z************************************/
	case 'z':
	case 'Z': //problem
		//fseeko(fout,1024+slcN*head->nx*head->ny*psize,SEEK_SET);
		offset = head->nx * head->ny;
		offset *= (slcN * psize);
		offset += 1024;
		fseeko(fout, offset, SEEK_SET);
		fwrite(slcdata, psize, head->nx * head->ny, fout);

		break;

	}
	return 0;
}

/*******************************************************************************************/
int mrc_add_sliceN(FILE *fout, MrcHeader *headout, float *slcdata, int slcN) {
	off_t length;
	//fseeko(fout,1024+sizeof(float)*headout->nx*headout->ny*slcN,SEEK_SET);
	length = headout->nx * headout->ny;
	length *= (sizeof(float) * slcN);
	length += 1024;
	fseeko(fout, length, SEEK_SET);

	fwrite(slcdata, sizeof(float), headout->nx * headout->ny, fout);
	return 0;
}

/*******************************************************************************************/
int mrc_add_slice(FILE *fout, MrcHeader *headout, float *slcdata) {

	fseeko(fout, 0, SEEK_END);
	fwrite(slcdata, sizeof(float), headout->nx * headout->ny, fout);
	return 0;
}

/*****************************************************************************************************/

int mrc_write_all(FILE *fout, MrcHeader *head, int Z_start, int Z_end,
		void *mrc_data_all) {
	int psize;

	switch (head->mode)

	{
	case MRC_MODE_BYTE:
		psize = sizeof(char);

		break;

	case MRC_MODE_SHORT:
		psize = sizeof(short);

		break;

	case MRC_MODE_FLOAT:
		psize = sizeof(float);

		break;
	}

	off_t offset;
	offset = head->nx * head->ny;
	offset *= (Z_start * psize);
	offset += 1024;
	fseeko(fout, offset, SEEK_SET);

	if ((fwrite(mrc_data_all, psize, head->nx * head->ny * (Z_end - Z_start),
			fout) == 0)) {
		printf("Error with Function 'mrc_read_all()'! Reading file failed!");
		return false;
	}

	return true;
}

/*******************************************************************************************/
/*int mrc_flipyz(char *inf,char *outf,int myid,int mypro)
 {
 printf("\nBegin flipping:");
 MrcHeader  *inhead,*outhead;
 inhead=(MrcHeader *)malloc(sizeof(MrcHeader));
 outhead=(MrcHeader *)malloc(sizeof(MrcHeader));

 FILE *fin,*fout;
 if((fin=fopen(inf,"r"))==NULL)
 {
 printf("\nCannot open file strike any key exit!");
 return false;
 }

 if((fout=fopen(outf,"w+"))==NULL)
 {
 printf("\nCannot open file strike any key exit!");
 return false;
 }

 mrc_read_head(fin,inhead);
 mrc_read_head(fin,outhead);

 outhead->nx=inhead->nx;
 outhead->ny=inhead->nz;
 outhead->nz=inhead->ny;
 outhead->mode=MRC_MODE_FLOAT;
 mrc_write_head(fout,outhead);
 printf("inhead.ny is %d\n",inhead->ny);

 int start_process;   //the start slice of reproject per process
 int end_process  ;   //the end slice of reproject per process
 start_process=myid*inhead->ny/mypro;
 end_process  =(myid+1)*inhead->ny/mypro;

 int sliceYZ=0;
 int global_sliceYZ=0;

 float *buf;
 buf=(float *)malloc(sizeof(float)*inhead->nx*inhead->nz);
 int j;

 for(j=start_process;j<end_process;j++)
 {
 mrc_read_slice(fin,inhead,j,'y',buf);
 //mrc_add_slice(fout,outhead,buf);
 mrc_add_sliceN(fout,outhead,buf,j);
 printf("%d of %d slice flipping finished  of process %d\n",j+1,inhead->ny,myid);
 sliceYZ++;
 }

 free (buf);
 printf("sliceYZ number in mrc_flipyz is %d in process %d\n",sliceYZ,myid);

 fclose(fin);
 fclose(fout);
 free (inhead);
 free (outhead);
 MPI_Reduce(&sliceYZ,&global_sliceYZ,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
 if(!myid)
 {
 printf("%d slice is flipped!\n",global_sliceYZ);
 printf("\nflipping finished!\n");
 }
 return 0;
 }

 */

/*******************************************************************************************/
int mrc_upsidedown(char *inf, char *outf) {
	MrcHeader *inhead, *outhead;
	inhead = (MrcHeader *) malloc(sizeof(MrcHeader));
	outhead = (MrcHeader *) malloc(sizeof(MrcHeader));

	FILE *fin, *fout;
	if ((fin = fopen(inf, "r")) == NULL) {
		printf("\nCannot open file strike any key exit!");
	}

	if ((fout = fopen(outf, "w+")) == NULL) {
		printf("\nCannot open file strike any key exit!");
	}

	mrc_read_head(fin, inhead);
	mrc_read_head(fin, outhead);

	outhead->mode = MRC_MODE_FLOAT;
	mrc_write_head(fout, outhead);

	float *buf;
	buf = (float *) malloc(sizeof(float) * inhead->nx * inhead->ny);
	int j;
	for (j = 0; j < inhead->nz; j++) {
		mrc_read_slice(fin, inhead, inhead->nz - 1 - j, 'z', buf);
		mrc_add_slice(fout, outhead, buf);
	}
	free(buf);
	free(inhead);
	free(outhead);
	fclose(fin);
	fclose(fout);
	return 0;
}

/****************************************************************************************/
/*
 void mrc_rotx(char *inf,char *outf,float ang,int thick)
 {
 printf("\nBegin rotating:");
 FILE *fin,*fout;
 if((fin=fopen64(inf,"r"))==NULL)
 {
 printf("\nCannot open file strike any key exit!");
 }

 MrcHeader  *inhead,*outhead;
 inhead=(MrcHeader *)malloc(sizeof(MrcHeader));
 outhead=(MrcHeader *)malloc(sizeof(MrcHeader));
 mrc_read_head(fin,inhead);
 mrc_read_head(fin,outhead);

 float *indata,*outdata;
 indata=(float *)malloc(sizeof(float)*inhead->ny*inhead->nz);
 outdata=(float *)malloc(sizeof(float)*inhead->ny*thick);
 int yp,zp,yp0,zp0;
 int y,z,y0,z0;
 yp0=inhead->ny/2;
 zp0=inhead->nz/2;
 y0=yp0;
 z0=thick/2;


 outhead->nz=thick;

 mrc_new(outf,outhead);

 if((fout=fopen64(outf,"a+"))==NULL)
 {
 printf("\nCannot open file strike any key exit!");
 }
 int i,j,k,r;
 for(i=0;i<inhead->nx;i++)
 {
 mrc_read_slice(fin,i,'x',indata);
 for(r=0;r<inhead->ny*thick;i++)
 {
 y=r%thick; z=r/thick;
 yp=int((y-y0)*cos(ang)-(z-z0)*sin(ang)+yp0);
 zp=int((y-y0)*sin(ang)+(z-z0)*cos(ang)+zp0);
 j=zp*inhead->ny+yp;
 if(j>=0&&j<inhead->ny*inhead->nz)outdata[r]=indata[j];
 else outdata[r]=0;
 }
 mrc_write_slcX(fout,i,outdata);
 printf("\n%d of %d slice finished");
 }
 fclose(fout);
 printf("\nRotating finished!");

 }
 */

/*int main(int argc, char *argv[])
 {
 mrc_flipyz(argv[1],argv[2]);
 }*/

/****************************************************************************************/
/*int main(int argc,char *argv[])
 {
 char *inf="tempflip.rec";
 char *outf="cavfinal.rec";
 float ang=rad(11.62);
 int thick=100;
 printf("rotating:\n");
 FILE *fin,*fout;
 if((fin=fopen64(inf,"r"))==NULL)
 {
 printf("\nCannot open file strike any key exit!");
 }

 MrcHeader  *inhead,*outhead;
 inhead=(MrcHeader *)malloc(sizeof(MrcHeader));
 outhead=(MrcHeader *)malloc(sizeof(MrcHeader));
 mrc_read_head(fin,inhead);
 mrc_read_head(fin,outhead);

 float *indata,*outdata;
 indata=(float *)malloc(sizeof(float)*inhead->ny*inhead->nz);
 //outdata=(float *)malloc(sizeof(float)*inhead->ny*inhead->nz);
 outdata=(float *)malloc(sizeof(float)*inhead->ny*thick);
 int yp,zp,yp0,zp0;
 int y,z,y0,z0;
 yp0=inhead->ny/2;
 zp0=inhead->nz/2;
 y0=yp0;
 //z0=inhead->nz/2;
 z0=thick/2;

 // outhead->nx=inhead->ny;
 outhead->nz=thick;

 // outhead->ny=inhead->nz;

 mrc_new(outf,outhead);

 if((fout=fopen64(outf,"r+"))==NULL)
 {
 printf("\nCannot open file strike any key exit!");
 }
 int i,j,k,r;
 printf("ok1");
 for(i=0;i<inhead->nx;i++)
 //for(i=0;i<3;i++)
 {
 mrc_read_slice(fin,i,'x',indata);
 for(r=0;r<inhead->ny*thick;r++)
 //for(r=0;r<inhead->ny*inhead->nz;r++)
 { //j=r+10*inhead->ny;
 //j=r;
 y=r%inhead->ny; z=r/inhead->ny;
 yp=int((y-y0)*cos(ang)-(z-z0)*sin(ang)+yp0);
 zp=int((y-y0)*sin(ang)+(z-z0)*cos(ang)+zp0);
 j=zp*inhead->ny+yp;
 if(j>=0&&j<inhead->ny*inhead->nz)outdata[r]=indata[j];
 else outdata[r]=0;
 }
 mrc_write_slcX(fout,i,outdata);
 printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
 printf("%4d of %4d slice finished",i,inhead->nx);

 }

 fclose(fout);
 printf("\nRotating finished!\n");
 }*/
