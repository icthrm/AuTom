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

	fl->readmrc(input,1);  
	fl->change_type(2);
	fl->gaissian_convolve3d(ksize,sigma);      
	
	for(int i=0; i<num; i++)
	{
	//命名
	stringstream ss;
	int k=i+1;
	ss<<k;
	string index=ss.str();
	temp+="_"+index+name;
	fl->nad3d(fl->width(),fl->height(),fl->depth(),fl->d.f);

	const char* output=temp.c_str();
	fl->mrc_update_head();
	fl->writemrc(output);

	pos=temp.find(".");
	temp=temp.substr(0,pos);
	cout<<"the "<<i+1<<" time nad end"<<endl;
	}
	
// 	const char* output=ouf.c_str();
// 	fl->mrc_update_head();
// 	fl->writemrc(output);
	fl->colse();
	time(&timep);
	printf("%s",asctime(gmtime(&timep)));
	cout<<"The program end!\n";

}
