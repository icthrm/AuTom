

/**********************************************************************************main.c*/

#include "../singlexmpi/recon.h"
#include "../singlexcuda/recon_gpu.h"
#include "opt.h"
/*#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include<math.h>
#include<string.h>

#include <time.h>
#include <mpi.h>
#include <omp.h>*/

//using namespace std;

int main(int argc, char *argv[])
{

	//   opt=(options *)malloc(sizeof(options));

	int id;  //process id number
	int p; //number of process
//	int namelen;
//	char processor_name[MPI_MAX_PROCESSOR_NAME];
/*	clock_t st, en;
	 st = clock();*/
//
	MPI::Init(argc, argv);
	p = MPI::COMM_WORLD.Get_size();

	id = MPI::COMM_WORLD.Get_rank();

	printf("rank=%d nr_process=%d \n", id, p);
    if(id == 0)
        printf("Version: 1.0.0\n");
	clock_t start , finish;
	MPI_Barrier(MPI_COMM_WORLD);

	start = clock();

	options* opt;
	opt = new options;
	initOpt(opt);
	int status = GetOptsSig(argc, argv, opt, id);



	if (status == 1) {
		if (PRINTERROR) {
				printOpt(opt);
		}
		
		if(opt->gpu_used==1)
		{
      		ATOM_GPU(opt, id, p);
		}
		else
		{
			ATOM_MPI(opt, id, p);
		}
	}

	MPI_Barrier(MPI_COMM_WORLD );
	//if(PRINTERROR){
	finish = clock();
	printf("Elapsed time is %f seconds\n",double(finish-start)/CLOCKS_PER_SEC);

//	}
	//  MPI_Finalize();                 //parallel finish
	MPI::Finalize();
/*	en = clock();
	printf("copy1 finished, time %lf\n",(float) (en - st) / CLOCKS_PER_SEC);*/
	delete opt;
	return 0;
}
