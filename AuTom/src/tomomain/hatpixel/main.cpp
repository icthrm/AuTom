#include <iostream>
#include <sstream>
#include <cassert>
#include <linux/limits.h>

extern "C" {
#include <getopt.h>
}

using namespace std;

int main(int argc, char **argv) {
    cout<<"Start hat_pixel"<<endl;
    if(argc < 3){
	cout<<"paramete error!"<<endl;
    }
    
    char* m_input;
    char* m_output;
    float cutoff;
    
    int opt = getopt(argc , argv , "i:c:o:");
    while(opt != -1){
	switch(opt){
	    case 'i':
		m_input = optarg;
		cout<<"input = "<<m_input<<endl;
		break;
	    case 'c':
	    {
		std::istringstream iss(optarg);
		iss>>cutoff;
		cout<<"cutoff = "<<cutoff<<endl;
		break;
	    }
	    case 'o':
		m_output = optarg;
		cout<<"output = "<<m_output<<endl;
		break;
	}
	opt = getopt( argc, argv, "i:c:o:" );
    }
    cout<<"end"<<endl;
}