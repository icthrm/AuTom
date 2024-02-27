#include "msg_map.h"
#include "string.h"
#include "stdio.h"

msg_map::MessageMapThread msg_map::MessageMapThreadApplication::theMMT;
msg_map::SyncACC msg_map::MessageMapThreadApplication::theACC;
msg_map::Proxy msg_map::MessageMapThreadApplication::theProxy;

msg_map::MessageMapThread& msg_map::MessageMapThreadApplication::GetMMT()
{
    return theMMT;
}

msg_map::Message* msg_map::MessageMapThreadApplication::NewMessage(int id, void* src, const char* ins, void* extra)
{
    msg_map::Message* msg = new msg_map::Message();
    msg->id = id;
    msg->timestamp = theACC.Accum();
    msg->src = src;
    memcpy(msg->instruction, ins, sizeof(char)*INS_MAX);
    msg->extra = extra;
    
    return msg;
}

void msg_map::MessageMapThreadApplication::SetMMTDefaultASyncFunc(msg_map::FeedBackFunc func)
{
    theMMT.SetDefaultASyncRecovery(func);
}

void msg_map::MessageMapThreadApplication::PostMessage(msg_map::Message* msg)
{
    theMMT.PostMessage(msg);
}

int msg_map::MessageMapThreadApplication::AbortCurrentProc()
{
	theMMT.AbortCurrentProc();
	theProxy.KillCurrentProc();
    return 1;
}

void msg_map::MessageMapThreadApplication::DeclareCancelable()
{
    DECLARE_CANCEL_ABLE;
}

void msg_map::MessageMapThreadApplication::Execute(const char* cmd, const char* log, const char* mode)
{
    theProxy.Execute(cmd, log, mode);
}

//@ wls
bool msg_map::MessageMapThreadApplication::ExecuteCmd(const char* cmd, FILE** fpin)
{
    return theProxy.ExecuteCmd(cmd , fpin);
}

void msg_map::MessageMapThreadApplication::ExecuteCmdAsync(const char* cmd)
{
	theProxy.ExecuteCmdASync(cmd);
}

void msg_map::MessageMapThreadApplication::Echo(unsigned long rate)
{
    theMMT.Echo(rate);
}

void msg_map::MessageMapThreadApplication::Echo(const char* message, int size)
{
    theMMT.Echo(message, size);
}

void msg_map::MessageMapThreadApplication::EchoResult(int state)
{
    theMMT.EchoResult(state);
}

int msg_map::MessageMapThreadApplication::CloseExecutePFile(FILE* fpin)
{
    return theProxy.CloseExecutePFile(fpin);
}

