#include <iostream>
#include "getopt.h"
#include "nad/Filter.h"
#include "nad/Vector3.h"

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

      Vector3<int> ksize(3,3,3);
      Filter* fl=new Filter;
      float* kernel;
      fl->readmrc(input,1);
//       fl->printhead();
      fl->change_type(2);
      int tag;
      kernel=fl->gaussian_kernel(ksize,1,1);   
      
      tag=fl->convolve(kernel,fl->d.f,fl->width(),fl->height(),fl->depth());
      if(tag)
      {
	    cout<<"convolve error!\n";
	    return false;
      }
      
      tag=fl->update_statistic();
      if(tag)
      {
	    cout<<"update_statistic error!\n";
	    return false;
      }
      fl->update_statistic();
	fl->update_header();
      tag=fl->writemrc(output);
      if(tag)
      {
	    cout<<"writemrc error!\n";
	    return false;
      }      
      fl->close();
      cout<<"The program end!\n";
}