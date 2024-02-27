#include "recon_gpu.h"
#include "../main/function.h"
#include "../main/printcontrol.h"
#define GPU
#ifdef GPU
#include "atomrecon.cuh"
#endif
#define MILLION 1000000
//#define GPU


/**********************************************************************************/

int ATOM_GPU(options *opt, int myid, int mypro)
{

   const char *inf = opt->input.c_str();
   const char *tilt_f = opt->tiltangle.c_str();
   const char *outf = opt->output.c_str();


   MPI_File fin, fout;

   MPI_File_open(MPI_COMM_WORLD, const_cast<char*>(inf), MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &fin);
   MPI_File_open(MPI_COMM_WORLD, const_cast<char*>(outf), MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &fout);

   MrcHeader *inhead, *outhead;

   inhead=(MrcHeader *)malloc(sizeof(MrcHeader));
   outhead=(MrcHeader *)malloc(sizeof(MrcHeader));

   if (myid==0)
	   mrc_read_head_MPI(fin, inhead);
   MPI_Bcast(inhead, 1024, MPI_CHAR, 0, MPI_COMM_WORLD);

   mrc_init_head(outhead);
   outhead->nx=inhead->nx;
   outhead->ny=inhead->ny;
   outhead->nz=opt->thickness;

   if(myid==0)
	   mrc_write_head(fout,outhead);

   Volume vol;
   vol.X=outhead->nx;
   vol.Y=outhead->ny;
   vol.Z=outhead->nz;
   vol.Xstart=0;
   vol.Xend=vol.Xstart+vol.X;
   vol.Ystart=0;
   vol.Yend=vol.Ystart+vol.Y;
   vol.Zstart=0;
   vol.Zend=vol.Zstart+vol.Z;
   vol.Ycount=0;//init

   if (myid ==0)
	   PRINT_DEBUG_MSG("vol.Xend is %d, vol.Yend is %d, vol.Zend is %d\n",vol.Xend, vol.Yend, vol.Zend);

   Projection prj;
   prj.X=vol.X;
   prj.Y=vol.Y;
   prj.AngN=inhead->nz;

   FILE *f_tilt;
   f_tilt=fopen(tilt_f,"r");


   float *tilt_angle;
   if((tilt_angle=(float *)malloc(sizeof(float)*prj.AngN))==NULL)
   {
       printf("Can't malloc memery for 'tilt_angle'!");
       return FALSE;
   }
   memset(tilt_angle, 0 , sizeof(float)*prj.AngN);
   read_coef(tilt_angle, f_tilt, myid);
/*   */

   int i;

	float *cosang = (float *) malloc(sizeof(float) * prj.AngN);
	float *sinang = (float *) malloc(sizeof(float) * prj.AngN);

	for (i = 0; i < prj.AngN; i++) {
		tilt_angle[i] += opt->offset;
		float ang = D2R(-tilt_angle[i]);
		cosang[i] = cos(ang);
		sinang[i] = sin(ang);
	}




/************************malloc memory for prj, slc, slc->data****************************************/
   Slide *slid;

   if((slid=(Slide *)malloc(sizeof(Slide)))==NULL)
   {
       printf("Can't malloc memery for 'slid'!");
       return FALSE;
   }

   slid->X=vol.X;
   slid->x0=vol.X/2;

   if(opt->xaxistilt>=0)
       slid->Z=(int)(vol.Z+tan(D2R(opt->xaxistilt))*vol.Y);
   else slid->Z=(int)(vol.Z+tan(D2R(-opt->xaxistilt))*vol.Y);
   slid->z0=slid->Z/2+opt->zshift;

   int  pixel_num, line_num;
   long long  pixel_num_per;
   line_num=vol.X*prj.AngN;
   pixel_num=vol.X*vol.Z;

   if(PRINTINFO){
   PRINT_DEBUG_MSG("slid->X is %d, slid->Z is %d, pixel_num is %d, line_allnum is %d\n",slid->X,slid->Z,pixel_num, line_num);
   PRINT_DEBUG_MSG("slid->x0 is %d, slid->z0 is %d\n",slid->x0,slid->z0);
   }




//for Iterative method
   float *prj_calc, *prj_calc_num;
   prj_calc  = (float *)malloc(sizeof(float)*line_num); 
   memset(prj_calc, 0 , sizeof(float)*line_num);

   prj_calc_num  = (float *)malloc(sizeof(float)*line_num); 
   memset(prj_calc_num, 0 , sizeof(float)*line_num);

//for output file
   Slice *slc;
   slc=(Slice *)malloc(sizeof(Slice));


/**********************decide Z_minS,Z_minE,Z_maxS,Z_maxE  ***********************************************************/
   int Z_minS,Z_minE,Z_maxS,Z_maxE;

   if(opt->xaxistilt>=0)
   {
       Z_minS=0;
       Z_maxS=vol.Z;
       Z_minE=(int)(tan(D2R(opt->xaxistilt))*vol.Y);
       Z_maxE=Z_minE+vol.Z;
   }

   if(opt->xaxistilt<0)
   {
       Z_minS=(int)(tan(D2R(-opt->xaxistilt))*vol.Y);
       Z_maxS=vol.Z+Z_minS;
       Z_minE=0;
       Z_maxE=vol.Z;
   }
   if(PRINTINFO)
   PRINT_DEBUG_MSG("Z_minS is %d,Z_maxS is %d,Z_minE is %d,Z_maxE is %d\n",Z_minS,Z_maxS,Z_minE,Z_maxE);

  /**********************reconstruction begin for each slice***********************************************************/
        long long volY_per;
        int volY_add=vol.Y%mypro;
        int Y_start;   //the start slice of reproject per process
        int Y_end  ;   //the end slice of reproject per process

        if(myid<volY_add)
        {
            volY_per=vol.Y/mypro+1;
            Y_start=volY_per*myid+vol.Ystart;
            Y_end=volY_per*(myid+1)+vol.Ystart;
        }
        else
        {
            volY_per=vol.Y/mypro;
            Y_start=volY_per*myid+volY_add+vol.Ystart;
            Y_end=volY_per*(myid+1)+volY_add+vol.Ystart;
        }
        if(PRINTINFO)
        PRINT_DEBUG_MSG("Y_start is %d, volY_per is %lld, Y_end is %d in process %d\n",Y_start,volY_per,Y_end,myid);


        vol.volY_per=volY_per;
        vol.Ystart_per=Y_start;
        vol.Yend_per=Y_end;

        pixel_num_per=vol.X*vol.Z*volY_per;





    int j;
	//for input file
#define slicenum1 3

	int slicenum = 1;
	float *prj_real;


	slc->min = (int *) malloc(sizeof(int) * slicenum);
	slc->max = (int *) malloc(sizeof(int) * slicenum);


        if(opt->method=="BPT")
        {

#define BPT


            initGPU(myid);

            BPT_hostmalloc(&prj_real,sizeof(float) * line_num * slicenum,&(slc->data),sizeof(float) * pixel_num * slicenum);
            ImageStruct dev_img;
            dev_img.prj=prj;
            dev_img.slid=*slid;
            //random_angle(tilt_angle,prj.AngN);
            dev_img.vol=vol;
            //dev_img.slc=*slc;

            initMalloc(dev_img,line_num,prj.AngN,pixel_num,slicenum);

            transferToDevtilt(dev_img.cosang,cosang,sizeof(float)*prj.AngN);
            transferToDevtilt(dev_img.sinang,sinang,sizeof(float)*prj.AngN);

            for(j=Y_start;j<Y_end;j=j+slicenum)
            {
            	int offset=0;
            	//not consider % divide
            	//start = clock();
            	for(int k=j;k<j+slicenum;k++)
            	{
            		mrc_read_slice_y(fin, inhead, k, 'y', prj_real+offset);
            		//mrc_read_slice(fin, inhead, k, 'y', prj_real+offset);
            		slc->min[k-j]=slid->X*((k*(Z_minE-Z_minS))/vol.Y+Z_minS);
            		offset+=line_num;

            	}

            	//printf("slice %d \n",j);
                prj.Y=prj.AngN; //one projection image along Y-axis is prj.X*prj.AngN

                dev_img.prj.Y=prj.Y;
                initDevVol(dev_img.slc.data,pixel_num*sizeof(float)*slicenum);
                initDevMem(dev_img.slc.c,pixel_num*sizeof(float)*slicenum);
                initDevMem(dev_img.slc.s,pixel_num*sizeof(float)*slicenum);

                transferToDevPro(dev_img.prj_real, prj_real,line_num*sizeof(float)*slicenum);


               transferConst(slc->min,sizeof(int)*slicenum);

               Bpt_Slice_backproj(pixel_num, prj.AngN, volY_per, dev_img.prj_real,dev_img.slc, dev_img.slid,slicenum,line_num);
               Bpt_Slice_divide(pixel_num,dev_img.slc,slicenum);
               transferToHost(dev_img.slc.data,slc->data,pixel_num*sizeof(float)*slicenum);

               offset=0;


           	   for(int k=j;k<j+slicenum;k++)
               {
           		 mrc_write_slice(fout, outhead, k, 'y', slc->data+offset);
                 offset+=pixel_num;
              }
            }


        freeDevProjection(dev_img.prj_real);
        freeDevSlc(dev_img.slc.data) ;
        freeDevMem_float(dev_img.slc.c) ;
        freeDevMem_float(dev_img.slc.s);
        freeDevMem_int(dev_img.slc.min);


        }// end BPT
        else if(opt->method=="FBP")
        {
#define FBP

            initGPU(myid);

            BPT_hostmalloc(&prj_real,sizeof(float) * line_num * slicenum,&(slc->data),sizeof(float) * pixel_num * slicenum);
            ImageStruct dev_img;
            dev_img.prj=prj;
            dev_img.slid=*slid;
            //random_angle(tilt_angle,prj.AngN);
		dev_img.vol = vol;
		//dev_img.slc=*slc;
		int filtlength = 251;
		const char *filter = opt->filter.c_str();
		int row_pad = 2;  //x-padded
		int col_pad = 2;  //y-padded
		int symmetrize_2D_flag = 1;
            initMalloc(dev_img,line_num,prj.AngN,pixel_num,slicenum);

            transferToDevtilt(dev_img.cosang,cosang,sizeof(float)*prj.AngN);
            transferToDevtilt(dev_img.sinang,sinang,sizeof(float)*prj.AngN);


            prj.Y=prj.AngN; //one projection image along Y-axis is prj.X*prj.AngN
            dev_img.prj.Y=prj.Y;
            for(j=Y_start;j<Y_end;j=j+slicenum)
            {
            	int offset=0;
            	//not consider % divide
            	//start = clock();
            	for(int k=j;k<j+slicenum;k++)
            	{
            		mrc_read_slice_y(fin, inhead, k, 'y', prj_real+offset);

            	    filter_prj_sym(prj_real+offset, filter, filtlength, prj, row_pad, col_pad, symmetrize_2D_flag,j, myid);
            		slc->min[k-j]=slid->X*((k*(Z_minE-Z_minS))/vol.Y+Z_minS);
            		offset+=line_num;
            	}


                initDevVol(dev_img.slc.data,pixel_num*sizeof(float)*slicenum);
                initDevMem(dev_img.slc.c,pixel_num*sizeof(float)*slicenum);
                initDevMem(dev_img.slc.s,pixel_num*sizeof(float)*slicenum);

                transferToDevPro(dev_img.prj_real, prj_real,line_num*sizeof(float)*slicenum);


               transferConst(slc->min,sizeof(int)*slicenum);

               Bpt_Slice_backproj(pixel_num, prj.AngN, volY_per, dev_img.prj_real,dev_img.slc, dev_img.slid,slicenum,line_num);
               Bpt_Slice_divide(pixel_num,dev_img.slc,slicenum);
               transferToHost(dev_img.slc.data,slc->data,pixel_num*sizeof(float)*slicenum);

               offset=0;

           	   for(int k=j;k<j+slicenum;k++)
               {
           		 mrc_write_slice(fout, outhead, k, 'y', slc->data+offset);
                 offset+=pixel_num;
              }
            }


        freeDevMem_float(dev_img.cosang);
        freeDevMem_float(dev_img.sinang);
        freeDevProjection(dev_img.prj_real);
        freeDevSlc(dev_img.slc.data) ;
        freeDevMem_float(dev_img.slc.c) ;
        freeDevMem_float(dev_img.slc.s);
        freeDevMem_int(dev_img.slc.min);

        }
        else if (opt->method=="SIRT")
        {
#define SIRT

      
            initGPU(myid);
            BPT_hostmalloc(&prj_real,sizeof(float) * line_num * slicenum,&(slc->data),sizeof(float) * pixel_num * slicenum);
            ImageStruct dev_img;
            dev_img.prj=prj;
            dev_img.slid=*slid;
            dev_img.vol=vol;

            SIRT_initMalloc(dev_img,line_num,prj.AngN,pixel_num,slicenum);
            transferToDevtilt(dev_img.cosang,cosang,sizeof(float)*prj.AngN);
            transferToDevtilt(dev_img.sinang,sinang,sizeof(float)*prj.AngN);


            float SIRT_ITR_STEP;
            SIRT_ITR_STEP=opt->step;
            const char *initialf = opt->initial.c_str();
           	MPI_File  f_initial;

		if (strcmp(initialf, "initial") == 0) {

			for (j = Y_start; j < Y_end; j = j + slicenum) {
				int offset = 0;
				//not consider % divide
				for (int k = j; k < j + slicenum; k++) {
					mrc_read_slice_y(fin, inhead, k, 'y', prj_real + offset);
					slc->min[k - j] = slid->X
							* ((k * (Z_minE - Z_minS)) / vol.Y + Z_minS);
					offset += line_num;
				}

				initDevVol(dev_img.slc.data,pixel_num * sizeof(float) * slicenum);
				initDevMem(dev_img.slc.c, pixel_num * sizeof(float) * slicenum);
				initDevMem(dev_img.slc.s, pixel_num * sizeof(float) * slicenum);

				transferToDevPro(dev_img.prj_real, prj_real,
						line_num * sizeof(float) * slicenum);
	         
			    transferConst(slc->min,sizeof(int)*slicenum);
				Bpt_Slice_backproj(pixel_num, prj.AngN, volY_per,
						dev_img.prj_real, dev_img.slc, dev_img.slid, slicenum,
						line_num);
				Bpt_Slice_divide(pixel_num, dev_img.slc, slicenum);
				//printf("Iteration %d has done in BPT %d\n", i, j);
				int i; //itertive number
				for (i = 0; i < opt->iter_num; i++) {

					initDevfloat(dev_img.prj_calc,
							line_num * sizeof(float) * slicenum);
					initDevfloat(dev_img.prj_calc_num,
							line_num * sizeof(float) * slicenum);
					Cu_SIRT_update_slice(dev_img.prj_real, dev_img.prj_calc,
							dev_img.prj_calc_num, dev_img.slc.data, prj.AngN,
							dev_img.slc, dev_img.slid, pixel_num, prj.X,
							SIRT_ITR_STEP,slicenum);
					//
				}

				transferToHost(dev_img.slc.data, slc->data,
						pixel_num * sizeof(float) * slicenum);

				offset = 0;
				for (int k = j; k < j + slicenum; k++) {
					mrc_write_slice(fout, outhead, k, 'y',slc->data + offset);
					offset += pixel_num;
				}

				printf("Iteration %d has done in Y_slice %d\n", i, j);
			}


            }//end if
            else
            {
            	MPI_File_open(MPI_COMM_WORLD, const_cast<char*>(initialf), MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &f_initial);

			for(j=Y_start;j<Y_end;j=j+slicenum)
			{

				int offset=0;
				int offsetslc=0;
				for(int k=j;k<j+slicenum;k++)
				{
					mrc_read_slice_y(fin, inhead, k, 'y', prj_real+offset);
					mrc_read_slice_y(f_initial, outhead, k, 'Y', slc->data+offsetslc);//////////dsfdfdas
					slc->min[k-j]=slid->X*((k*(Z_minE-Z_minS))/vol.Y+Z_minS);
					slc->max[k-j]=slid->X*((k*(Z_maxE-Z_maxS))/vol.Y+Z_maxS);
					offset+=line_num;
					offsetslc+=pixel_num;
				}

				transferToDevPro(dev_img.slc.data, slc->data,pixel_num*sizeof(float)*slicenum);
				transferToDevPro(dev_img.prj_real, prj_real,line_num*sizeof(float)*slicenum);
				transferIntToDev(dev_img.slc.min, slc->min,sizeof(int)*slicenum);
             
				//transferToDevVol(dev_img.slc.data, temp,pixel_num*sizeof(float)*slicenum);
				dev_img.prj.Y=prj.Y;
                int i;//itertive number
				for(i=0;i<opt->iter_num;i++)
				{

					initDevfloat(dev_img.prj_calc, line_num*sizeof(float)*slicenum);
					initDevfloat(dev_img.prj_calc_num,line_num*sizeof(float)*slicenum);
					Cu_SIRT_update_slice(dev_img.prj_real, dev_img.prj_calc,
							dev_img.prj_calc_num, dev_img.slc.data, prj.AngN, dev_img.slc, dev_img.slid,
							pixel_num,prj.X,SIRT_ITR_STEP,slicenum);
					
                 }

				transferToHost(dev_img.slc.data, slc->data,pixel_num*sizeof(float)*slicenum);
				printf("%d slice done \n ",j);
				offsetslc=0;
				for(int k=j;k<j+slicenum;k++)
				{
					mrc_write_slice(fout, outhead, k, 'y', slc->data+offsetslc);
					offsetslc+=pixel_num;
				}



				} //end for
			
            }//end else


        freeDevProjection(dev_img.prj_real) ;
        freeDevSlc(dev_img.slc.data) ;
        freeDevMem_float(dev_img.slc.c) ;
        freeDevMem_float(dev_img.slc.s) ;
        freeDevMem_int(dev_img.slc.min);
        freeDevMem_int(dev_img.slc.max);

        }// end if SIRT
        else if (opt->method=="SART")
        {

#define SART

        initGPU(myid);

		int *numsequence = (int *) malloc(sizeof(int) * prj.AngN);
		memset(numsequence, 0, sizeof(sizeof(int) * prj.AngN));
		random_angle(tilt_angle, prj.AngN, numsequence);
	    BPT_hostmalloc(&prj_real,sizeof(float) * line_num * slicenum,&(slc->data),sizeof(float) * pixel_num * slicenum);


		ImageStruct dev_img;
		dev_img.prj = prj;
		dev_img.slid = *slid;
		dev_img.vol = vol;

		SART_initMalloc(dev_img, line_num, prj.AngN, pixel_num, slicenum);
        transferToDevtilt(dev_img.cosang,cosang,sizeof(float)*prj.AngN);
        transferToDevtilt(dev_img.sinang,sinang,sizeof(float)*prj.AngN);

		float SART_ITR_STEP;
		SART_ITR_STEP = opt->step;

		float* prj_real_temp = (float *) malloc(sizeof(float) * line_num * slicenum);
		const char *initialf = opt->initial.c_str();

		if (strcmp(initialf, "initial") == 0) {

			for (j = Y_start; j < Y_end; j = j + slicenum) {
				int offset = 0;

				//not consider % divide
				for (int k = j; k < j + slicenum; k++) {
					mrc_read_slice_y(fin, inhead, k, 'y', prj_real + offset);
					for (int i = 0; i < prj.AngN; i++) {
						for (int j = 0; j < vol.X; j++)
							prj_real_temp[i * vol.X + j + offset] =prj_real[numsequence[i] * vol.X + j + offset];
					}
					slc->min[k - j] = slid->X* ((k * (Z_minE - Z_minS)) / vol.Y + Z_minS);
					offset += line_num;
				}

				initDevVol(dev_img.slc.data,pixel_num * sizeof(float) * slicenum);
				initDevMem(dev_img.slc.c, pixel_num * sizeof(float) * slicenum);
				initDevMem(dev_img.slc.s, pixel_num * sizeof(float) * slicenum);

				transferToDevPro(dev_img.prj_real, prj_real_temp,line_num * sizeof(float) * slicenum);

				transferConst(slc->min,sizeof(int)*slicenum);
				Bpt_Slice_backproj(pixel_num, prj.AngN, volY_per,
						dev_img.prj_real, dev_img.slc, dev_img.slid, slicenum,
						line_num);
				Bpt_Slice_divide(pixel_num, dev_img.slc, slicenum);
				int i; //itertive number
				for (i = 0; i < opt->iter_num; i++) {

					initDevfloat(dev_img.prj_calc,
							line_num * slicenum * sizeof(float));
					initDevfloat(dev_img.prj_calc_num,
							line_num * slicenum * sizeof(float));
					Cu_SART_update_slice(dev_img.prj_real, dev_img.prj_calc,
							dev_img.prj_calc_num, dev_img.slc.data, prj.AngN,
							dev_img.slc, dev_img.slid, pixel_num, dev_img.prj.X,
							SART_ITR_STEP, slicenum, j);
				}

				transferToHost(dev_img.slc.data, slc->data,
						pixel_num * slicenum * sizeof(float));
				offset = 0;
				for (int k = j; k < j + slicenum; k++) {
					mrc_write_slice(fout, outhead, k, 'y',
							slc->data + offset);
					offset += pixel_num;
				}
			}

		} else {

			MPI_File f_initial;
			MPI_File_open(MPI_COMM_WORLD, const_cast<char*>(initialf),
					MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &f_initial);

			for (j = Y_start; j < Y_end; j = j + slicenum) {
				int offset = 0;
				int offsetslc = 0;

				for (int k = j; k < j + slicenum; k++) {
					mrc_read_slice_y(fin, inhead, k, 'y', prj_real + offset);
					for (int i = 0; i < prj.AngN; i++) {
						for (int j = 0; j < vol.X; j++)
							prj_real_temp[i * vol.X + j + offset] =
									prj_real[numsequence[i] * vol.X + j + offset];
					}
					mrc_read_slice_y(f_initial, outhead, k, 'Y',
							slc->data + offsetslc);
					slc->min[k - j] = slid->X
							* ((k * (Z_minE - Z_minS)) / vol.Y + Z_minS);

					offset += line_num;
					offsetslc += pixel_num;
				}

				transferToDevPro(dev_img.prj_real, prj_real_temp,
						line_num * slicenum * sizeof(float));
				transferToDevVol(dev_img.slc.data, slc->data,
						pixel_num * sizeof(float) * slicenum);
				transferIntToDev(dev_img.slc.min, slc->min,
						sizeof(int) * slicenum);

				dev_img.prj.Y = prj.Y;

				int i; //itertive number
				for (i = 0; i < opt->iter_num; i++) {

					initDevfloat(dev_img.prj_calc,
							line_num * slicenum * sizeof(float));
					initDevfloat(dev_img.prj_calc_num,
							line_num * slicenum * sizeof(float));
					Cu_SART_update_slice(dev_img.prj_real, dev_img.prj_calc,
							dev_img.prj_calc_num, dev_img.slc.data, prj.AngN,
							dev_img.slc, dev_img.slid, pixel_num, dev_img.prj.X,
							SART_ITR_STEP, slicenum, j);
				}

				transferToHost(dev_img.slc.data, slc->data,
						pixel_num * slicenum * sizeof(float));

				offsetslc = 0;
				for (int k = j; k < j + slicenum; k++) {
					mrc_write_slice(fout, outhead, k, 'y',
							slc->data + offsetslc);
					offsetslc += pixel_num;
				}

			}
		}

        freeDevMem_float(dev_img.tilt_angle);
        freeDevProjection(dev_img.prj_real);
        freeDevSlc(dev_img.slc.data) ;
        freeDevMem_float(dev_img.slc.c) ;
        freeDevMem_float(dev_img.slc.s);
        freeDevMem_int(dev_img.slc.min);
		free(numsequence);
	} // end if SART





    MPI_Barrier(MPI_COMM_WORLD);

	mrc_update_head_MPI(fout, myid, mypro);

	//free(slc);
	fclose(f_tilt);
	free(slid);
	free(inhead);
	inhead = NULL;

	free(outhead);
	outhead = NULL;

	free(prj_calc);
	free(prj_calc_num);
	free(tilt_angle);

	MPI_File_close(&fin);
	MPI_File_close(&fout);


   return TRUE;
}
    /*
                for(int index=0;index<pixel_num;index++){
                	int z=index/vol.X;
                	int y=j-Y_start;
                	int x=index%vol.X;
                	temp[index]=slc->data[x+y*vol.X+z*vol.X*(volY_per)];
                }*/
