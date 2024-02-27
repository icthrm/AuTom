#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <unistd.h>
#include "filter/nad/Filter.h"
using namespace std;

int main(int argc,char** argv)
{
    cout<<"begin histogram normalize!\n"<<"this program have 3 step:\n";
 	float			avg, std;				// Average and standard deviation for output
 	int 			norm_type;					// Default Gaussian normalization	
	string 		input;
    string 			output;
    int opt = getopt(argc , argv , "i:a:s:t:o:");
    while(opt != -1)
	{
	  switch(opt)
	  {
		  case 'i':
			input = optarg;
			break;
		  case 'a':
		  {
			std::istringstream is1(optarg);
			is1>>avg;
			break;
		  }
		  case 's':
		  {
			std::istringstream is2(optarg);
			is2>>std;
			break;
		  }
		  case 't':
		  {
			std::istringstream is3(optarg);
			is3>>norm_type;
			break;
		  }
		  case 'o':
		  {
			std::istringstream is4(optarg);
			is4>>output;
			break;
		  }
	  }
	  opt = getopt( argc, argv, "i:a:s:t:o:" );
    }
	const char* h_input;
	const char* h_output;
	h_input=(const char*)input.c_str();
	h_output=(const char*)output.c_str();
	Filter* fl=new Filter;
	fl->readmrc(h_input,1);
	cout<<"(1)read file successfuly!\n";
	fl->img_mass_normalize(avg,std,1);
	cout<<"(2)normalize successfuly!\n";
	fl->update_header();
	fl->writemrc(h_output);
	cout<<"(3)write file successfuly\n"<<"histogram normalize end!\n";
	fl->printhead();
 	//p = read_img(h_input, 1, -1);	
 	//img_mass_normalize(p, avg, std, norm_type);
	//p->change_type(Short);
 	//write_img(h_output, p);
	
	
}
