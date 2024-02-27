#ifndef PROFUNC_H__
#define PROFUNC_H__

#include <iostream>
#include "core/msg_map.h"
#include "env/env.h"
//#include <QtGui>
#include <QtWidgets>

/** typedef void* (*ProFunc)(__msg*);		*/

/** typedef void (*FeedBackFunc)(void* data, INFO* dst);	*/

namespace profunc{

void* OnTestMsg(msg_map::Message* msg);

void* OnBuildModelMsg(msg_map::Message* msg);
void* OnBuildModelMsg_modify(msg_map::Message* msg);

void TestMsgFeedBack(void* data, msg_map::INFO* dst);

void* OnHatPixel(msg_map::Message* msg);

void* OnCoarseAlign(msg_map::Message* msg);

void* OnFineAlign(msg_map::Message* msg);

void* OnRestruct(msg_map::Message* msg);

void* OnNormalize(msg_map::Message* msg);//+

void* OnRAPTOR(msg_map::Message* msg);

void* OnEvaluate(msg_map::Message* msg);

void* OnPostProcess(msg_map::Message* msg);

void* OnEvaluateNCC(msg_map::Message* msg);

void* OnRotateMrc(msg_map::Message* msg);

/*
void* OnDETECT(msg_map::Message* msg);

void* OnMatch(msg_map::Message* msg);
*/


void* OnAlign(msg_map::Message* msg);

void* onImodFid(msg_map::Message* msg);

void* onImodOpen(msg_map::Message* msg);

void* onEvaluateTraj(msg_map::Message* msg);

void* onCTFCorrection(msg_map::Message* msg);

void* OnGeoDeterm(msg_map::Message* msg);
}

#endif
