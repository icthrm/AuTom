#ifndef PROXY_H__
#define PROXY_H__

#include <queue>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>  
#include <sys/wait.h>
#include <malloc.h>
#include "thread.h"

namespace msg_map{

struct data{
    char* line;
};

class Proxy
{
friend class MessageMapThreadApplication;
private:
	pid_t* childpid;
	long maxfd;
	pid_t curr_pid;
	
private:
    Proxy(){maxfd = sysconf(_SC_OPEN_MAX);  childpid = (pid_t*)calloc(maxfd, sizeof(pid_t));}
    ~Proxy(){free(childpid);}
    
private:
	FILE* POpen(const char* cmdstring, const char* type);
	int PClose(FILE* fp);
	void KillChildProcTree(pid_t pid);		//kill the child process = pid and its process tree
	
public:
    void Execute(const char* cmd, const char* log, const char* mode = "w");
    //@ wls
    bool ExecuteCmd(const char* cmd , FILE** fpin);
	bool ExecuteCmdASync(const char* cmd);
	void KillCurrentProc();
	int CloseExecutePFile(FILE* fp);
}; 
    
}

#endif
