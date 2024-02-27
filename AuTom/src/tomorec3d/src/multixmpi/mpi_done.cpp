

/**********************************************************************************main.c*/

#include "recon.h"

/*#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include<math.h>
#include<string.h>

#include <time.h>
#include <mpi.h>
#include <omp.h>*/

/**********************************************************************************/
/**********************************************************************************/
//using namespace std;

int main(int argc, char *argv[])
{


 //   opt=(options *)malloc(sizeof(options));

   	int id;  //process id number
	int p; //number of process
//	int namelen;
//	char processor_name[MPI_MAX_PROCESSOR_NAME];
//
    MPI::Init(argc,argv);

    p=MPI::COMM_WORLD.Get_size();

    id=MPI::COMM_WORLD.Get_rank();

/*	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Get_processor_name(processor_name, &namelen);*/

   PRINT_DEBUG_MSG("rank=%d nr_process=%d \n", id, p);
   
 
   options* opt;     
   opt= new options;
   initOpt(opt);
   GetOptsSig(argc, argv, opt);
   if (id==0)
       printOpt(opt);
   ATOM(opt, id, p);


   MPI_Barrier(MPI_COMM_WORLD);

  // free(opt);

   delete opt;
 //  MPI_Finalize();                 //parallel finish
   MPI::Finalize();

    return 0;
}
