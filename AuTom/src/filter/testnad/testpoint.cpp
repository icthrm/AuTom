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
	
	Filter* fl=new Filter;
	fl->readmrc(input,1);
	int count=0,x,y,z;
	unsigned long i;
	float* pt=fl->d.f;
	for(z=0,i=0;z<fl->depth();z++)
		for(y=0;y<fl->height();y++)
			for(x=0;x<fl->width();x++,i++)
			{
				if(pt[i]<=-5000)
				{
					count++;
					pt[i]=-5000;
				}
					
			}
			
	fl->update_statistic();
	fl->printhead();
	fl->update_header();
	fl->writemrc(output);
	
	cout<<"the number of point <-5000: "<<count<<endl;
			
			
			
	
}