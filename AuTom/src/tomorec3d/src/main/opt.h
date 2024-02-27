#ifndef OPTS_H__
#define OPTS_H__

#include <limits>
#include <iostream>
#include <sstream>
#include <cassert>
#include <getopt.h>
#include "exception.h"
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <mpi.h>
using namespace std;
#define PRINTERROR id==0
#define PRINTINFO myid==0
typedef struct {
	int gpu_used;
	int gpu_process;
	int mpi_process;
	string tiltangle;
	string xtiltangle;
	string input;
	string output;
	string initial;
	float xaxistilt;
	float zshift;
	int thickness;
	float offset;
	string filter;
	string method;
	int iter_num;
	float step;
	int rotate;
} options;

void usageDual();
void usageSig();

void printOpt(options* opt);
void initOpt(options* opt);
int GetOptsDual(int argc, char **argv, options* opts_);

int GetOptsSig(int argc, char **argv, options* opts_,int id);
#endif
