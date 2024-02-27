#include "proxy.h"
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <cstring>
#include <stdlib.h>  
#include "proc_state.h"
#include "msg_map.h""
void msg_map::Proxy::Execute(const char* cmd, const char* log, const char* mode)
{
#ifndef BUF_MAX
#define BUF_MAX				1024
#endif
    printf("start Execute!\n");
    FILE* fpin, * fout;
    char line[BUF_MAX];

    if((fout = fopen(log, mode)) == NULL) {
        printf("Open log<%s> error\n", log);
        return;
    }
    printf("open log\n");

    fputs("\n#", fout);
    fputs(cmd, fout);

    if((fpin = POpen(cmd, "r")) == NULL) {
        printf("Popen error!\n");
        return;
    }

    while(fgets(line, BUF_MAX, fpin) != NULL) {
        fputs(line, fout);

    }

    if( MMTApp::CloseExecutePFile(fpin) == -1) {
        printf("Pclose error!\n");
    }
    fclose(fout);

#undef BUF_MAX
}

//@ wls
bool msg_map::Proxy::ExecuteCmd(const char* cmd , FILE** fpin)
{
    printf("start ExecuteCmd!\n");
    if((*fpin = POpen(cmd, "r")) == NULL) {
        printf("Popen error!\n");
        return false;
    }
    return true;
}

bool msg_map::Proxy::ExecuteCmdASync(const char* cmd)
{
	pid_t pid;
	if((pid = fork()) < 0){
        return false;   /* errno set by fork() */
    }
    else if(pid == 0){                           /* child */
        execl("/bin/sh", "sh", "-c", cmd, (char *)0);
        _exit(127);
    }
    return true;
}

FILE* msg_map::Proxy::POpen(const char* cmdstring, const char* type)
{
    int i;
    int pfd[2];
    pid_t pid;
    FILE *fp;

    /* only allow "r" or "w" */
    if((type[0] != 'r' && type[0] != 'w') || type[1] != 0){
        errno = EINVAL;     /* required by POSIX */
        return(NULL);
    }

    if(pipe(pfd) < 0){
		return(NULL);   /* errno set by pipe() */
	}

    if((pid = fork()) < 0){
        return(NULL);   /* errno set by fork() */
    }
    else if(pid == 0){                           /* child */
        if(*type == 'r'){
            close(pfd[0]);
            if(pfd[1] != STDOUT_FILENO){
                dup2(pfd[1], STDOUT_FILENO);
                close(pfd[1]);
            }
        }
        else{
            close(pfd[1]);
            if(pfd[0] != STDIN_FILENO){
                dup2(pfd[0], STDIN_FILENO);
                close(pfd[0]);
            }
        }

        /* close all descriptors in childpid[] */
        for (i = 0; i < maxfd; i++){
            if (childpid[i] > 0){
                close(i);
			}
		}

        execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);
        _exit(127);
    }

    /* parent continues... */
    if (*type == 'r') {
        close(pfd[1]);
        if ((fp = fdopen(pfd[0], type)) == NULL)
            return(NULL);
    } else {
        close(pfd[0]);
        if ((fp = fdopen(pfd[1], type)) == NULL)
            return(NULL);
    }

    curr_pid = pid;
    childpid[fileno(fp)] = pid; /* remember child pid for this fd */
    return(fp);
}

int msg_map::Proxy::PClose(FILE* fp)
{
    int     fd, stat;
    pid_t   pid;

    if (childpid == NULL) {
        errno = EINVAL;
        return(-1);     /* popen() has never been called */
    }

    fd = fileno(fp);
    if ((pid = childpid[fd]) == 0) {
        errno = EINVAL;
        return(-1);     /* fp wasn't opened by popen() */
    }

    childpid[fd] = 0;
    if (fclose(fp) == EOF)
        return(-1);

    while (waitpid(pid, &stat, 0) < 0)
        if (errno != EINTR)
            return(-1); /* error other than EINTR from waitpid() */

    return(stat);   /* return child's termination status */
}

void msg_map::Proxy::KillChildProcTree(pid_t pid)
{
    DIR* pDir;
    struct dirent* ent;
    int i=0;
    char childpath[1024];
	procinfo st;
	
    pDir=opendir("/proc");
    memset(childpath, 0, sizeof(char)*1024);

    while((ent = readdir(pDir)) != NULL){
        if(ent->d_type & DT_DIR){
            if ((ent->d_name[0] >= '0') && (ent->d_name[0] <= '9')){
				sprintf(childpath, "/proc/%s/stat", ent->d_name);
				int cpid = atoi(ent->d_name);
				if(get_proc_info(cpid, &st) == 0){
					if(st.ppid == pid){
						KillChildProcTree(cpid);
					}
				}
			}
        }
    }
    kill(pid, SIGKILL);
}

int msg_map::Proxy::CloseExecutePFile(FILE* fp)
{
	return PClose(fp);
}

void msg_map::Proxy::KillCurrentProc()
{
	KillChildProcTree(curr_pid);
	pid_t pid = waitpid(curr_pid, NULL, 0);
	printf("Killed Process pid = %d\n", pid);
}
