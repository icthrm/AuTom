#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include "filter/nad/Filter.h"
#include "filter/nad/mrcheader.h"
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

	int pos=ouf.find(".");
	string temp=ouf.substr(0,pos);
	string name=ouf.substr(pos);

	time_t timep;
	time(&timep);
	printf("%s",asctime(gmtime(&timep)));
	Filter* fl=new Filter;
	fl->readmrc(input,1);  
	fl->change_type(2);   
	
	fl->corr_coefficient(input,output,num);
	
	time(&timep);
	printf("%s",asctime(gmtime(&timep)));
	cout<<"The program end!\n";

}
