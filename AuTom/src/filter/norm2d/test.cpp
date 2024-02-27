#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <unistd.h>
#include "nad/Filter.h"
using namespace std;

int main(int argc,char** argv)
{
	Filter* fl=new Filter;
	fl->readmrc("/home/zzy/epikfile/toto_norm.mrc",1);
	fl->writemrc("/home/zzy/epikfile/toto_me.mrc");
// 	fl->printhead();
}
