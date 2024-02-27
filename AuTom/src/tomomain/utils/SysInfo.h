#ifndef SYSINFO_H
#define SYSINFO_H

#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>  
#include <unistd.h>

class SysInfo{
public:
  SysInfo(){}
  virtual ~SysInfo(){}
  static const int getLCpuNum();
  static const int getGpuNum();
  
private:
  static std::string getShellResult(const std::string & cmd) ;
};


#endif//SYSINFO_H