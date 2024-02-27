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

	int pos=ouf.find(".");
	string temp=ouf.substr(0,pos);
	string name=ouf.substr(pos);

	time_t timep;
	time(&timep);
	printf("%s",asctime(gmtime(&timep)));
	Vector3<int> ksize(3,3,3);
	Filter* fl=new Filter;
	cout<<"d:"<<fl->d.f<<endl;
	fl->readmrc(input,1);
 	fl->change_type(2);
	fl->printhead();
	
	Filter* fv=new Filter(fl->d.f,fl->width(),fl->height(),fl->depth());
	for(int i=0;i<num;i++)
	{
		fv->update_statistic();
		fv->printhead();
		//fv->nad3d(fv->width(),fv->height(),fv->depth(),fv->d.f);	
		fv->nad3d();
	}

	const char* output=ouf.c_str();
	fl->d.f=fv->d.f;
	fl->update_statistic();
 	fl->writemrc(output);
	fv->close();
	fl->close();
	time(&timep);
	printf("%s",asctime(gmtime(&timep)));
	cout<<"The program end!\n";

}
