#include "Cinit.h"
#include "string.h"
#include "env/env.h"
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

bool Cinit::InitSet()
{
    cout<<"Inite Set"<<endl;
    CEnv::SetDefaultExeBinDirectory();
    cout<<"Exe Bin Directory :"<<CEnv::ExeBinDirectory()<<endl;
    ifstream in(CEnv::Abs("setting.ini"));
    string cwdir;
    
     //先看文件Setting是否存在，不存在说明是第一次打开程序，
    //那么先进行默认配置 然后弹出设置工作空间的窗口
    if(!in.good()){
	ofstream out(CEnv::Abs("setting.ini"));
	out<<"."<<endl;
	out.close();
	//然后弹出设置工作空间的窗口
	return false;
    }
    //存在，则读取文件内容进行配置
    else{
	 in>>cwdir;
	 //改变工作路径为setting文件中读取到的
        CEnv::SetCurrentWorkDirectory(cwdir.c_str());
        //设置工作路径
        CEnv::SetInitWorkDirectory(cwdir.c_str());
	// cout<<"###InitWorkDirectory :"<<CEnv::InitWorkDirectory()<<endl;
        cout<<"CurrentWorkDirectory :"<<CEnv::CurrentWorkDirectory()<<endl;
        return true;
    }   
    
}

void  Cinit::SetWorkSpace(const char* cwd)
{
    cout<<"cwd = "<<cwd<<endl;
    ofstream out(CEnv::Abs("setting.ini"));
    out<<cwd<<endl;
    out.close();
    CEnv::SetCurrentWorkDirectory(cwd);
}
