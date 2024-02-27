#ifndef MSG_MAP__
#define MSG_MAP__

#include "msg.h"
#include "thread.h"
#include "proxy.h"

namespace msg_map {

class SyncACC {
    friend class MessageMapThreadApplication;
private:
    pthread_mutex_t mutex;
    unsigned long acc_n;
private:
    SyncACC() {
        pthread_mutex_init(&mutex, NULL);
        acc_n = 1;
    }

    ~SyncACC() {
        pthread_mutex_destroy(&mutex);
    }
public:
    unsigned long Accum() {
        unsigned long i;
        pthread_mutex_lock(&mutex);
        i = acc_n++;
        pthread_mutex_unlock(&mutex);
        return i;
    }

    unsigned long Val() {
        unsigned long i;
        pthread_mutex_lock(&mutex);
        i = acc_n;
        pthread_mutex_unlock(&mutex);
        return i;
    }
};

class MessageMapThreadApplication {
private:
    static MessageMapThread theMMT;
    static SyncACC theACC;
    static Proxy theProxy;

private:
    MessageMapThreadApplication() {}
    ~MessageMapThreadApplication() {}

public:
    static MessageMapThread& GetMMT();
    static msg_map::Message* NewMessage(int id, void* src, const char* ins, void* extra = NULL);
    static void SetMMTDefaultASyncFunc(FeedBackFunc func);
    static void PostMessage(Message* msg);
    static int  AbortCurrentProc();
    static void DeclareCancelable();
    static void Execute(const char* cmd, const char* log, const char* mode);
    static bool ExecuteCmd(const char* cmd , FILE** fpin);
	static void ExecuteCmdAsync(const char* cmd);
    //@ wls
    static void Echo(unsigned long rate);
    static void Echo(const char* message, int size);
    static void EchoResult(int state);
    static int  CloseExecutePFile(FILE* fpin);
    
};

}

typedef msg_map::MessageMapThreadApplication MMTApp;

#endif
