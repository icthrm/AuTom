#ifndef QSIGNALGENERATOR_H__
#define QSIGNALGENERATOR_H__

#include <QtCore/QObject>
#include <QtCore/QEvent>
#include <QtCore/QCoreApplication>

#include "core/msg.h"

namespace msg_map{

enum Type{QSequelType = QEvent::User+1, TimeElapseType = QEvent::User+2, CustomType};
    
class CSequelEvent:public QEvent
{
public:
    void* data;
    int msg_id;
    unsigned long timestamp;
    receipt_enum state;
    
public:
    CSequelEvent(void* _data, int _id, int _timestamp, receipt_enum _state):QEvent(QEvent::Type(QSequelType)), data(_data), msg_id(_id), timestamp(_timestamp), state(_state){}
    ~CSequelEvent(){}
};

class CTimeElapseEvent:public QEvent
{
public:
    unsigned long elapse;
    int msg_id;
    unsigned long timestamp;
    
public:
    CTimeElapseEvent(unsigned long _elapse, int _id, int _timestamp):QEvent(QEvent::Type(QSequelType)), elapse(_elapse), msg_id(_id), timestamp(_timestamp){}
    ~CTimeElapseEvent(){}
};

void QtASyncRecovery(void* data, msg_map::INFO* info);

void QtSyncProcessEvent(unsigned long _elapse, msg_map::INFO* info);

}

#endif 
