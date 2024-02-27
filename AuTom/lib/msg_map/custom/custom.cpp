#include "custom.h"

#include <QTimer>
#include <iostream>


void msg_map::QtASyncRecovery(void* data, msg_map::INFO* info)
{
    CSequelEvent* sequ = new CSequelEvent(data, info->id, info->timestamp, info->state);
    QCoreApplication::postEvent((QObject*)info->src, sequ);
}

void msg_map::QtSyncProcessEvent(long unsigned int _elapse, msg_map::INFO* info)
{
    CTimeElapseEvent* t_elps = new CTimeElapseEvent(_elapse, info->id, info->timestamp);
    QCoreApplication::postEvent((QObject*)info->src, t_elps);
}

#include "custom.moc"
