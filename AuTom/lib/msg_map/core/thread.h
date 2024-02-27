#ifndef THREAD_H__
#define THREAD_H__

#include <queue>
#include <pthread.h>
#include "msg.h"
#include <sys/stat.h>
#include <string>

namespace msg_map {

// extern msg_bind theMaps[];

template<class T>
class SyncQueue
{
private:
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    std::queue<T> queue_;
public:
    SyncQueue() {
        pthread_mutex_init(&mutex, NULL);
        pthread_cond_init(&cond, NULL);
    }

    ~SyncQueue() {
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&cond);
    }

public:
    void EnQueue(const T& m) {
        pthread_mutex_lock(&mutex);
        queue_.push(m);
        pthread_mutex_unlock(&mutex);
    }

    void Pop(T* m) {
        pthread_mutex_lock(&mutex);
        *m = queue_.front();
        queue_.pop();
        pthread_mutex_unlock(&mutex);
    }

    void Front(T* m) {
        pthread_mutex_lock(&mutex);
        *m = queue_.front();
        pthread_mutex_unlock(&mutex);
    }

    void SyncFetch(T* m) {
        pthread_mutex_lock(&mutex);
        while(queue_.size() == 0) {
            pthread_cond_wait(&cond, &mutex);
        }
        *m = queue_.front();
        queue_.pop();
        pthread_mutex_unlock(&mutex);
    }

    void SyncEnqueue(const T& m) {
        EnQueue(m);
        pthread_cond_signal(&cond);
    }
};

typedef SyncQueue<Message*> MsgQueue;

struct ASyncfBack {
    __info info;
    pthread_t tid;
    void* rslt;
};

typedef SyncQueue<ASyncfBack> AfbQueue;

class MessageMapThread
{
    friend class MessageMapThreadApplication;

    DECLARE_MESSAGE_MAP()
private:
    MsgQueue msg_queue;
    AfbQueue afb_queue;
    pthread_t main_tid, async_recv_tid;
    FeedBackFunc default_async_rec;

    pthread_mutex_t mutex;
private:
    struct ASyncData {
        ProFunc profunc;
        __msg msg;
        AfbQueue* afb_q_ptr;
    };

    struct SyncData {
        ProFunc profunc;
        __msg* msg;
    };

    struct CurrentInfo {
        __info info;
        pthread_t current_tid;
    } curr_info;

private:
    static void* MainFunction(void* arg);
    static void* ASyncChildFunction(void* arg);
    static void* SyncChildFunction(void* arg);
    static void* ASyncRecoverFunction(void* arg);
    static void DefaultASyncRecovery(void* data, INFO* info);

private:
    MessageMapThread()
    {
        default_async_rec = DefaultASyncRecovery;
    }

    ~MessageMapThread() {}

    void SetDefaultASyncRecovery(FeedBackFunc func);

public:
    int Run();
    void PostMessage(Message* msg);
    void Echo(unsigned long rate);
    void Echo(const char* message, int size = 1024);
    void EchoResult(int state);
    int AbortCurrentProc();
    int Stop();
};

}

typedef msg_map::MessageMapThread MMT;

#endif