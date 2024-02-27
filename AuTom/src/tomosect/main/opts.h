#ifndef OPTS_H__
#define OPTS_H__

#include <iostream>
#include <sstream>
#include <cassert>
#include <linux/limits.h>
#include "util/exception.h"
extern "C" {
#include <getopt.h>
}

struct opts {
    char input[PATH_MAX];
    char inputangle[PATH_MAX];
    int thickness;
    char output[PATH_MAX];
    float y;
};

inline int GetOpts(int argc, char **argv, opts* opts_){

    static struct option longopts[] = {
        { "help",            no_argument,            NULL,              'h' },
        { "output",          required_argument,      NULL,              'o' },
        { "input",    	     required_argument,      NULL,              'i' },
        { "angles",           required_argument,      NULL,              'a' },
	{ "thickness",       required_argument,      NULL,              't' },
	{ "yfractile",        required_argument,      NULL,              'y' },
        { NULL,              0,                      NULL,               0  }
    };
    
    if(argc != 11  && argc >= 3 || (argc == 2 && argv[1][0] != '-' && argv[1][1] != 'h') || argc == 1){
	EX_TRACE("[-i INPUT FILENAME][-a ANGLE FILENAME][-t THICKNESS][-y Y FRACTILE][-o OUTPUT FILENAME]\n");
	return -1;
    }
    
    int ch;
    while ((ch = getopt_long(argc, argv, "ho:i:a:t:y:o:", longopts, NULL))!= -1) {
        switch (ch) {

        case '?':
            return -1;

        case ':':
            EX_TRACE("Missing option argument for '%s'.", argv[optind-1]);
            return -1;

        case 'h':
            EX_TRACE("  [--input(-i) INPUT FILENAME]\n  [--angles(-a) ANGLE FILENAME]\n  [--thickness(-t) THICKNESS]\n  [--yfractile(-y) Y FRACTILE]\n  [--output(-o) OUTPUT FILENAME]\n");
            return 0;

        case 'i':
        {
            std::istringstream iss(optarg);
            iss >> opts_->input;
            if (iss.fail())
                EX_TRACE("Invalid argument '%s'.", optarg);
        }
        break;

        case 'a': 
        {
            std::istringstream iss(optarg);
            iss >> opts_->inputangle;
            if (iss.fail())
                EX_TRACE("Invalid argument '%s'.", optarg);
        }
        break;

        case 'o':
        {
            std::istringstream iss(optarg);
            iss >> opts_->output;
            if (iss.fail())
                EX_TRACE("Invalid argument '%s'.", optarg);
        }
        break;

	case 'y':
        {
            std::istringstream iss(optarg);
            iss >> opts_->y;
            if (iss.fail())
                EX_TRACE("Invalid argument '%s'.", optarg);
        }
        break;
	
        case 't':
        {
            std::istringstream iss(optarg);
            iss >> opts_->thickness;
            if (iss.fail())
                EX_TRACE("Invalid argument '%s'.", optarg);
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