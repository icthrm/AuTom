#include "getopt.h"
extern "C"
{
#include "nadc/nad.h"
};
#include "nad/Filter.h"
#include <omp.h>

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
      fl->readmrc(input,1);  
      fl->change_type(2);
      nad3d(fl->width(),fl->height(),fl->depth(),fl->d.f,num);
      fl->mrc_update_head();
      fl->writemrc(output);
      fl->colse();
      
      
      
}