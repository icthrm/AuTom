#ifndef ENV_H__
#define ENV_H__

#include <string>
#include <unistd.h>
#include <fcntl.h>

class CEnv
{
private:
    static std::string exe_bin_dir;
    static std::string abs_path_tmp;
    static std::string init_work_path;

private:
    CEnv(){}
    ~CEnv(){}
    
public:
    static void SetCurrentWorkDirectory(const char* cwd);
    static const char* CurrentWorkDirectory();
    static void SetExeBinDirectory(const char* ebd);
    static void SetDefaultExeBinDirectory();
    static const char* CurrentExeAbsolutePath();
    static const char* ExeBinDirectory();
    static void AppendSearchPath(const char* path);
    static const char* SearchPath();
    static const char* Abs(const char* filename);
    static void SetInitWorkDirectory(const char* cwd);
    static const char* InitWorkDirectory();
};

#endif