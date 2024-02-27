#ifndef OPTS_H__
#define OPTS_H__

#include <iostream>
#include <sstream>
#include <cassert>
#include <cstdio>
extern "C" {
#include <getopt.h>
}

struct options {
    std::string input;
    std::string addMrc;
};

inline int GetOpts(int argc, char **argv, options* opts_){

    static struct option longopts[] = {
        { "help",            no_argument,            NULL,              'h' },
        { "input",    	     required_argument,      NULL,              'i' },
		{ "add",    	     required_argument,      NULL,              'a' },
        { NULL,              0,                      NULL,               0  }
    };
	
    if(argc != 5 && argc!=2){
		printf("[-i input mrc file1 ][-a input mrc file2  ]\n This program will add all images from file2 to the end of file1.\n");
		return -1;
    }
    
    int ch;
    while ((ch = getopt_long(argc, argv, "hi:a:", longopts, NULL))!= -1) {
        switch (ch) {

        case '?':
            printf("Invalid option '%s'.", argv[optind-1]);
            return -1;

        case ':':
            printf("Missing option argument for '%s'.", argv[optind-1]);
            return -1;

        case 'h':
            printf("[-i input mrc file1 ][-a input mrc file2  ]\n This program will add all images from file2 to the end of file1.\n");
			return 0;

        case 'i':
        {
            std::istringstream iss(optarg);
            iss >> opts_->input;
            if (iss.fail()){
                printf("Invalid argument '%s'.", optarg);
				return -1;
			}
        }
        break;
		
        case 'a':
        {
            std::istringstream iss(optarg);
            iss >> opts_->addMrc;
            if (iss.fail()){
                printf("Invalid argument '%s'.", optarg);
                return -1;
            }
        }
        break;
		
        case 0: 
            break;

        default:
            assert(false);
        }
    }
    return 1;
}

#endif

