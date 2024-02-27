#include <iostream>

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
}