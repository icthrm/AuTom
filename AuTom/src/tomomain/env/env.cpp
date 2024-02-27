#include "env.h"
#include <climits>
#include <iostream>
#include <stdlib.h>

std::string CEnv::exe_bin_dir;
std::string CEnv::abs_path_tmp;
std::string CEnv::init_work_path;

const char* CEnv::CurrentWorkDirectory()
{
    static char path[PATH_MAX];
    //获取当前工作目录
    getcwd(path, sizeof(char)*PATH_MAX);
    return path;
}

const char* CEnv::CurrentExeAbsolutePath()
{
    static char path[PATH_MAX];
    int count = readlink( "/proc/self/exe", path, PATH_MAX);
    if (count < 0 || count >= PATH_MAX){
	std::cerr<<"Get exe absolute path error"<<std::endl;
	return NULL;
    }
    path[count] = '\0';
    return path;
}

const char* CEnv::ExeBinDirectory()
{
    return exe_bin_dir.c_str();
}

void CEnv::SetCurrentWorkDirectory(const char* cwd)
{
//    std::cout<<"set current  work dir"<<std::endl;
    chdir(cwd);
}

void CEnv::SetExeBinDirectory(const char* ebd)
{
    exe_bin_dir = std::string(ebd);
}

/** @brief set the exe bin directory to the directory path of current exe */
void CEnv::SetDefaultExeBinDirectory()
{
    std::string absolute_path(CurrentExeAbsolutePath());
    int end = absolute_path.rfind('/');
    exe_bin_dir = absolute_path.substr(0, end+1);
    AppendSearchPath(exe_bin_dir.c_str());
}

void CEnv::AppendSearchPath(const char* path)
{
    char* ptr = getenv("PATH");
    std::string exepath(ptr);
    setenv("PATH", exepath.append(":").append(path).c_str(), 1);
}

const char* CEnv::SearchPath()
{
    char* ptr = getenv("PATH");
    return ptr;
}


const char* CEnv::Abs(const char* filename)
{
    abs_path_tmp = exe_bin_dir;
    return abs_path_tmp.append(filename).c_str();
}

const char* CEnv::InitWorkDirectory()
{
    return init_work_path.c_str();
// 	return abs_path_tmp.c_str();
}

void CEnv::SetInitWorkDirectory(const char* cwd)
{
  
    init_work_path = std::string(cwd);
}