#ifndef OPTS_H__
#define OPTS_H__

#include <iostream>
#include <sstream>
#include <cassert>
#include <getopt.h>
#include "exception.h"
#include <stdlib.h>
#include <stdio.h>
#include<string>
using namespace std;

typedef struct {
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
} options;

void usageDual();
void usageSig();

void printOpt(options* opt);
void initOpt(options* opt);
int GetOptsDual(int argc, char **argv, options* opts_);

int GetOptsSig(int argc, char **argv, options* opts_);
#endif
