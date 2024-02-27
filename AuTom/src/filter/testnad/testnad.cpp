#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include "nad/Filter.h"
#include "nad/mrcheader.h"
#include <sstream>
#include <string>
#include <time.h>
using namespace std;

int main(int argc,char** argv)
{
	string inf,ouf;
	int num;
	float sigma;
	int opt = getopt(argc , argv , "i:o:n:s:");
	while(opt != -1)
	{
	switch(opt)
	{
	      case 'i':
	      {
		  inf = optarg;
		  break;
	      }
	      case 'n':
	      {
		  std::istringstream is(optarg);
		  is>>num;
		  break;
	      }
	      case 'o':
	      {
		  std::istringstream is(optarg);
		  is>>ouf;
		  break;
	      }
	      case 's':
	      {
		  std::istringstream is(optarg);
		  is>>sigma;
		  break;
	      }
	}
	opt = getopt( argc, argv, "i:o:n:s:" );
	}
	const char* input=inf.c_str();
	const char* output=ouf.c_str();

	Vector3<int> ksize(3,3,3);
	float* kernel;
	Filter* fl=new Filter;

	fl->readmrc(input,1);  
	fl->change_type(2);
	kernel=fl->gaussian_kernel(ksize,0.5,1);  
	fl->convolve(kernel,fl->d.f,fl->width(),fl->height(),fl->depth());
	
	for(int i=0; i<num; i++)
	{
		fl->nad3d();
		//fl->nad3d(fl->width(),fl->height(),fl->depth(),fl->d.f);
		cout<<"the "<<i+1<<" time nad end"<<endl;
	}
	fl->writemrc(output);
	fl->close();
	delete fl;
	//cout<<"The program end!\n";

}
