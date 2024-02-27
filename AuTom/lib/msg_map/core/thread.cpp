#include "thread.h"
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "proc_state.h"

// msg_map::MessageMapThread msg_map::MessageMapThread::theMMT = MessageMapThread();

void* msg_map::MessageMapThread::MainFunction(void* arg)
{
    MessageMapThread* theMMThrd = (MessageMapThread*)arg;
    
    const msg_bind* theMaps = theMMThrd->theMaps;
    MsgQueue& queue = theMMThrd->msg_queue;
    
    void* pro_rslt;
    
    while(true){
	__msg* new_msg;
	
	queue.SyncFetch(&new_msg);

	for(int i = 0; true; i++){
	    if(theMaps[i].id == DEFAULT_MSG){
		break;
	    }
	    if(theMaps[i].id == new_msg->id){
		switch(theMaps[i].sync){
		case ASYNC:{
		    int err; pthread_t tid;
		    ASyncData* asData = new ASyncData();
		    asData->profunc = theMaps[i].profunc;
		    asData->msg = *new_msg;
		    asData->afb_q_ptr = &(theMMThrd->afb_queue);
		    if((err = pthread_create(&tid, NULL, MessageMapThread::ASyncChildFunction, (void*)asData)) != 0){
			std::cerr<<"create ASyncChildFunction Thread failed!"<<std::endl;
			return 0;
		    }
		    }break;
		    
		case SYNC:{
		    int err; pthread_t tid;
		    SyncData sData;
		    sData.msg = new_msg;
		    sData.profunc = theMaps[i].profunc;
		    if((err = pthread_create(&tid, NULL, MessageMapThread::SyncChildFunction, (void*)&sData)) != 0){
			std::cerr<<"create SyncChildFunction Thread failed!\n"<<std::endl;
			return 0;
		    }		    
		    theMMThrd->curr_info.info = __INFO(*new_msg);
		    theMMThrd->curr_info.current_tid = tid;
		    pthread_join(tid, &pro_rslt);
		    new_msg->state = msg_map::RESULT;
		    
		    if(pro_rslt == PTHREAD_CANCELED){			
			ASyncfBack asfbk;
			asfbk.info = __INFO(*new_msg);
			asfbk.rslt = PTHREAD_CANCELED;
			asfbk.tid = tid;
			theMMThrd->afb_queue.SyncEnqueue(asfbk);
		    }
		    else if(theMaps[i].feedbackfunc == DEFAULT_FBF){
			(*(theMMThrd->default_async_rec))(pro_rslt, &__INFO(*new_msg));
		    }
		    else if(theMaps[i].feedbackfunc){
			(*(theMaps[i].feedbackfunc))(pro_rslt, &__INFO(*new_msg));
		    }
		    
		    }break;
		    
		default:
		    break;
		}
		
		break;
	    }
	}
	
	delete new_msg;
    }
    
    return (void*)NULL;
}

void* msg_map::MessageMapThread::ASyncChildFunction(void* arg)
{
    ASyncData* asData = (ASyncData*)arg;
    void* rslt = (*(asData->profunc))(&asData->msg);
    ASyncfBack asfbk;
    asfbk.info = __INFO(asData->msg);
    asfbk.info.state = msg_map::RESULT;
    asfbk.rslt = rslt;
    asfbk.tid = pthread_self();
    
    asData->afb_q_ptr->SyncEnqueue(asfbk);
    delete asData;
    
    return (void*)NULL;
}

void* msg_map::MessageMapThread::SyncChildFunction(void* arg)
{
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    SyncData* sData = (SyncData*)arg;
    void* rslt = (*(sData->profunc))(sData->msg);
    return rslt;
}


void* msg_map::MessageMapThread::ASyncRecoverFunction(void* arg)
{
    MessageMapThread* theMMThrd = (MessageMapThread*)arg;
    
    const msg_bind* theMaps = theMMThrd->theMaps;
    AfbQueue& afb_queue = theMMThrd->afb_queue;
    FeedBackFunc default_async_rec = theMMThrd->default_async_rec;
    
    while(true){
	ASyncfBack new_afb;
	
	afb_queue.SyncFetch(&new_afb);

	for(int i = 0; true; i++){
	    if(theMaps[i].id == DEFAULT_MSG){
		break;
	    }
	    
	    if(theMaps[i].id == new_afb.info.id){
		if(theMaps[i].feedbackfunc == DEFAULT_FBF){
		    (*default_async_rec)(new_afb.rslt, &new_afb.info);
		}
		else{
		  //what if feedbackfunc is time-consuming,or feedbackfunc contains a forerver loop? --added by liuce
		    (*(theMaps[i].feedbackfunc))(new_afb.rslt, &new_afb.info);
		}
	    }
	}
    }
    
    return (void*)NULL;    
}

int msg_map::MessageMapThread::AbortCurrentProc()
{
    int err;
    pthread_mutex_lock(&mutex);
	pthread_detach(curr_info.current_tid);
    err = pthread_cancel(curr_info.current_tid);
    pthread_mutex_unlock(&mutex);
    
    return err;
}

void ClearProcTree(pid_t pid)
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
						ClearProcTree(cpid);
					}
				}
			}
        }
    }
    kill(pid, SIGKILL);
}

static void ClearProcess()
{
	ClearProcTree(getpid());
}

int msg_map::MessageMapThread::Run()
{
    pthread_mutex_lock(&mutex);
    int err1, err2;
    if((err1 = pthread_create(&main_tid, NULL, MessageMapThread::MainFunction, (void*)this)) != 0){
	std::cerr<<"create MainFunction Thread failed!"<<std::endl;
	return 0;
    }
    std::cout<<"Message Map: MainFunction started(tid = "<<main_tid<<")"<<std::endl;
    if((err2 = pthread_create(&async_recv_tid, NULL, MessageMapThread::ASyncRecoverFunction, (void*)this)) != 0){
	std::cerr<<"create ASyncRecoverFunction Thread failed!"<<std::endl;
	return 0;
    }
    std::cout<<"Message Map: ASyncRecoverFunction started(tid = "<<async_recv_tid<<")"<<std::endl;
   
    pthread_mutex_unlock(&mutex);
    
// 	if(atexit(ClearProcess) == 0){
// 		std::cout<<"Register ClearProcess() with Exit"<<std::endl;
// 	}
// 	else{
// 		std::cout<<"Cannot register ClearProcess() with Exit"<<std::endl;
// 	}	
    return 1;
}

int msg_map::MessageMapThread::Stop()
{
    pthread_mutex_lock(&mutex);
    pthread_cancel(main_tid);
    pthread_cancel(async_recv_tid);
    pthread_mutex_unlock(&mutex);
}

void msg_map::MessageMapThread::PostMessage(msg_map::Message* msg)
{
    msg_queue.SyncEnqueue(msg);
}

void msg_map::MessageMapThread::DefaultASyncRecovery(void* data, msg_map::INFO* info)
{
    
}

void msg_map::MessageMapThread::SetDefaultASyncRecovery(FeedBackFunc func)
{
    pthread_mutex_lock(&mutex);
    default_async_rec = func;
    pthread_mutex_unlock(&mutex);
}

void msg_map::MessageMapThread::Echo(unsigned long rate)
{
    ASyncfBack asfbk;
    asfbk.info = curr_info.info;
    asfbk.info.state = msg_map::RATE;
    asfbk.rslt = (void*)rate;
    asfbk.tid = curr_info.current_tid;
    afb_queue.SyncEnqueue(asfbk);
}

void msg_map::MessageMapThread::Echo(const char* message, int size)
{
    ASyncfBack asfbk;
    asfbk.info = curr_info.info;
    asfbk.info.state = msg_map::MESSAGE;
    char* msg = new char[size];
    memcpy(msg, message, sizeof(char)*size);
    asfbk.rslt = (void*)msg;
    asfbk.tid = curr_info.current_tid;
    afb_queue.SyncEnqueue(asfbk);   
}

void msg_map::MessageMapThread::EchoResult(int state)
{
    ASyncfBack asfbk;
    asfbk.info = curr_info.info;
    asfbk.info.state = msg_map::RESULT;
    asfbk.rslt = (void*)state;
    asfbk.tid = curr_info.current_tid;
    afb_queue.SyncEnqueue(asfbk);
}
