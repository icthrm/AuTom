#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include "nad/Filter.h"
#include "nad/mrcheader.h"
#include "nad/img_util.h"
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
	const char* outf=ouf.c_str();
	
// 	ofstream ftime;
 	string twrite=outf;
 	twrite=twrite+"_time.txt";
// 	ftime.open(twrite.c_str());
 	time_t timep;
 	time(&timep);
//  	ftime<<"filter begin time:"<<asctime(gmtime(&timep))<<"    "<<endl;
// 	printf("%s",asctime(gmtime(&timep)));

	int pos=ouf.find(".");
	string temp=ouf.substr(0,pos);
	string type=ouf.substr(pos);

	Vector3<int> ksize(3,3,3);
	Filter* fl=new Filter;
	float* kernel;

	fl->readmrc(input,1);  
  	fl->change_type(2);
	
	kernel=fl->gaussian_kernel(ksize,sigma,1);  
	fl->convolve(kernel,fl->d.f,fl->width(),fl->height(),fl->depth());
	
	FILE* ft;
	ft=fopen(twrite.c_str(),"w");
	fprintf(ft,"nad 开始时间：");
	printctime(ft);
	
	for(int i=0; i<num; i++)
	{
		time_t nadbegin;
		time(&nadbegin);
		//命名
		stringstream ss;
		int k=i+1;
		ss<<k;
		string index=ss.str();
		
 		temp+="_"+index+type;
		cout<<"temp:"<<temp<<endl;
		
		fl->nad3d(fl->width(),fl->height(),fl->depth(),fl->d.f);
		const char* output=temp.c_str();
		
 		fl->mrc_update_head();
		if(i!=num-1)
			fl->writemrc(output);		

// 		pos=temp.find(".");
// 		temp=temp.substr(0,pos);
		pos=temp.find("_");
		temp=temp.substr(0,pos);
		time_t nadend;
		time(&nadend);
		cout<<"the "<<i+1<<" time nad end"<<endl;
		fprintf(ft,"第%d次滤波用时：",i+1);
		printutime(ft,nadbegin,nadend);
	}
	fprintf(ft,"nad 结束时间：");
	printctime(ft);
	fclose(ft);
	fl->mrc_update_head();
	fl->writemrc(outf);
	fl->colse();

	printf("%s",asctime(gmtime(&timep)));
	cout<<"The program end!\n";

}
