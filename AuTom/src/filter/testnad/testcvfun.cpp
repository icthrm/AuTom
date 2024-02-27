#include <iostream>
#include <cv.hpp>
#include "getopt.h"
#include "nad/Filter.h"

using namespace std;

int main(int argc,char** argv)
{
      string inf,ouf;
      int num;
      int opt = getopt(argc , argv , "i:n:o:");
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
	}
	opt = getopt( argc, argv, "i:n:o:" );
      }
      const char* input=inf.c_str();
      const char* output=ouf.c_str();
      
      Filter* fl=new Filter;
      
      fl->readmrc(input,0);
      fl->mrc_write_head(output);
      
      int depth=fl->depth();
      for(int i=0;i<20;i++)
      {
	   IplImage* img=fl->GetIplImage(i);
	   fl->statistic(img);	   
	   cvReleaseImage(&img);	   
      }
      fl->inclose();
      fl->outclose();
}