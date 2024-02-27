#ifndef CINIT_H
#define CINIT_H
#include "env/env.h"
#include "string.h"
#include <string>
#include <iostream>
using namespace std;

class Cinit
{
private:
    Cinit() {}
    ~Cinit() {}
public:
    static bool  InitSet();
    static void  SetWorkSpace(const char* cwd);
};

#endif