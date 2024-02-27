#ifndef MSG_H__
#define MSG_H__

#include <vector>

#define INS_MAX			4096

namespace msg_map{
    
struct __msg;

typedef __msg  Message;

enum receipt_enum{RESULT, RATE, MESSAGE};

struct __info{
    int id;
    unsigned long timestamp;
    receipt_enum state;
    void* src;
};

typedef __info INFO;

typedef void* (*ProFunc)(__msg*);

typedef void (*FeedBackFunc)(void* data, INFO* dst);

enum sync_enum{ASYNC, SYNC};

struct __msg{
    union{
      //same struct ,why union ?  added by liuce
    struct{
	int id;
	unsigned long timestamp;
	receipt_enum state;
	void* src;
    };
    __info info;
    }; 
    char instruction[INS_MAX];
    void* extra;
};

struct msg_bind{
    int id;
    ProFunc profunc;
    FeedBackFunc feedbackfunc;
    sync_enum sync;
};

class MessageMapThread;

#define __INFO(msg)		(msg).info

#define DEFAULT_MSG				0
#define DEFAULT_MSG_BIND			{DEFAULT_MSG, NULL, NULL, msg_map::ASYNC}

#define TERMINATE_MSG				1	
#define TEST_MSG				2

#define DECLARE_MESSAGE_MAP()					\
    static msg_bind theMaps[];					\
    static const msg_bind* GetMessageMaps(){ return theMaps;}

#define BEGIN_MESSAGE_MAP()		msg_map::msg_bind msg_map::MessageMapThread::theMaps[] = {
#define END_MESSAGE_MAP()	DEFAULT_MSG_BIND};

#define REG_MSG_FUNC(id, profunc, feedbackfunc, sync)		\
    {id, profunc, feedbackfunc, sync},

#define REG_MSG_FUNC_ASYNC(id, profunc)				\
    {id, profunc, NULL, msg_map::ASYNC},


#define DEFAULT_FBF			(void (*)(void*, msg_map::INFO*)) -11
    
#define DECLARE_CANCEL_ABLE		pthread_testcancel();

#ifndef PTHREAD_CANCELED
#define PTHREAD_CANCELED 		(void*)-1
#endif
   
#define PIPE_ENABLE			(void*)-2
// #define 
    
}

#endif
