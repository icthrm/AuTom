#include "ProManage.h"
#include <iostream>
#include<QtGlobal>

using namespace std;

CProInstance* CProManage::m_proInstance;
bool CProManage::m_haveProcess;
CSettings* CProManage::m_settings;

string JudgeKey(int state);

string JudgeKey(int state)
{
    switch(state){
	case UNFINISH:
	    return "UNFINISH";
	case FINISH:
	    return "FINISH";
	case FAIL:
	    return "FAIL";
    }
}

CProInstance::CProInstance()
{
    Pro_flow=Null;
    Preprocess_state = UNFINISH;
    Cosali_state = UNFINISH;
    Fineali_state  = UNFINISH;
    Germetry_state =  UNFINISH;
    Restruct_state  = UNFINISH;
    Postprocess_state = UNFINISH;
    
    Preprocess_state_based= UNFINISH;
    Align_state_based= UNFINISH;
    Germetry_state_based= UNFINISH;
    Restruct_state_based= UNFINISH;
    Postprocess_state_based= UNFINISH;
    
    Process_dir = "NULL";
    Pro_dir = "NULL";
    Mrc_dir  = "NULL";
    Angle_dir  = "NULL";
    Preali_dir = "NULL";
    Pre_xf_dir = "NULL";
    Param_dir = "NULL";
    Fin_xf_dir = "NULL";
    Fin_xg_dir = "NULL";
    Newtlt_dir = "NULL";
    Fin_mrc_dir = "NULL";
    Res_mrc_dir = "NULL";
    Pre_mrc_dir = "NULL";
    Post_mrc_dir = "NULL";
    Proerro_mrc_dir = "NULL";
    Ali_mrc_dir="NULL";
}

void CProInstance::Clear()
{
    Pro_flow=Null;
    Preprocess_state = UNFINISH;
    Cosali_state = UNFINISH;
    Fineali_state  = UNFINISH;
    Germetry_state =  UNFINISH;
    Restruct_state  = UNFINISH;
    Postprocess_state = UNFINISH;
    
    Preprocess_state_based= UNFINISH;
    Align_state_based= UNFINISH;
    Germetry_state_based= UNFINISH;
    Restruct_state_based= UNFINISH;
    Postprocess_state_based= UNFINISH;
    
    Process_dir = "NULL";
    Pro_dir = "NULL";
    Mrc_dir  = "NULL";
    Angle_dir  = "NULL";
    Preali_dir = "NULL";
    Pre_xf_dir = "NULL";
    Param_dir = "NULL";
    Fin_xf_dir = "NULL";
    Fin_xg_dir = "NULL";
    Newtlt_dir = "NULL";
    Fin_mrc_dir = "NULL";
    Res_mrc_dir = "NULL";
    Pre_mrc_dir = "NULL";
    Post_mrc_dir = "NULL";
    Proerro_mrc_dir = "NULL";
    Ali_mrc_dir="NULL";
    syslog_file="NULL";
}

string CProInstance::GetDir(int type)
{
    switch(type){
	case PROCESS_DIR:
	    return Process_dir;
	case PRO_DIR:
	    return Pro_dir;
	case MRC_DIR:
	    return Mrc_dir;
	case ANGLE_DIR:
	    return Angle_dir;
	case PREALI_DIR:
	    return Preali_dir;
	case PRE_XF_DIR:
	    return Pre_xf_dir;
	case PARAM_DIR:
	    return Param_dir;
	case FIN_XF_DIR:
	    return Fin_xf_dir;
	case FIN_XG_DIR:
	    return Fin_xg_dir;
	case NEWTLT_DIR:
	    return Newtlt_dir;
	case FIN_MRC_DIR:
	    return Fin_mrc_dir;
	case RES_MRC_DIR:
	  return Res_mrc_dir;
	case PRE_MRC_DIR:
	  return Pre_mrc_dir;
	case POST_MRC_DIR:
	  return Post_mrc_dir;
	case PROERRO_MRC_DIR:
	  return Proerro_mrc_dir;
	case ALI_MRC_DIR:
	    return Ali_mrc_dir;
      	case SYSLOG_FILE:
	   return syslog_file;
    }
}

string CProInstance::GetPath(int type)
{
  
}

int CProInstance::GetState(int type)
{
        switch(type){
	    case PREPROCESS_STATE:
		return Preprocess_state;
	    case COSALI_STATE:
		return Cosali_state;
	    case FINEALI_STATE:
		return Fineali_state;
	    case GERMETRY_STATE:
		return Germetry_state;
	    case RESTRUCT_STATE:
		return Restruct_state;
	    case POSTPROCESS_STATE:
	      return Postprocess_state;
	      
	    case PREPROCESS_STATE_BASED:
		return Preprocess_state_based;
	    case ALIGN_STATE_BASED:
		return Align_state_based;
	    case GERMETRY_STATE_BASED:
		return Germetry_state_based;
	    case RESTRUCT_STATE_BASED:
		return Restruct_state_based;
	    case POSTPROCESS_STATE_BASED:
		return Postprocess_state_based;
	      
    }
}

void CProInstance::SetDir(int type, string data)
{
    switch(type){
         case SYSLOG_FILE:
	     syslog_file=data;
	     break;
	case PROCESS_DIR:
	    Process_dir = data;
	    break;
	case PRO_DIR:
	    Pro_dir = data;
	    break;
	case MRC_DIR:
	    Mrc_dir = data;
	    break;
	case ANGLE_DIR:
	    Angle_dir = data;
	    break;
	case PREALI_DIR:
	    Preali_dir = data;
	    break;
	case PRE_XF_DIR:
	    Pre_xf_dir = data;
	     break;
	case PARAM_DIR:
	    Param_dir = data;
	    break;
	case FIN_XF_DIR:
	    Fin_xf_dir = data;
	    break;
	case FIN_XG_DIR:
	    Fin_xg_dir = data;
	    break;
	case NEWTLT_DIR:
	    Newtlt_dir = data;
	    break;
	case FIN_MRC_DIR:
	    Fin_mrc_dir = data;
	    break;
	case RES_MRC_DIR:
	  Res_mrc_dir = data;
	   break;
	case PRE_MRC_DIR:
	  Pre_mrc_dir = data;
	   break;
	case POST_MRC_DIR:
	  Post_mrc_dir = data;
	   break;
	case PROERRO_MRC_DIR:
	  Proerro_mrc_dir = data;
	   break;
	case ALI_MRC_DIR:
	    Ali_mrc_dir=data;
	     break;
     
	default:break;
    }
}

void CProInstance::SetState(int type, int state)
{
    switch(type){
	    case PREPROCESS_STATE:
		Preprocess_state = state;
		break;
	    case COSALI_STATE:
		Cosali_state = state;
		break;
	    case FINEALI_STATE:
		Fineali_state = state;
		break;
	    case GERMETRY_STATE:
		Germetry_state = state;
		break;
	    case RESTRUCT_STATE:
		Restruct_state = state;
		break;
	    case POSTPROCESS_STATE:
	      Postprocess_state = state;
	      break;
	      
	    case PREPROCESS_STATE_BASED:
		Preprocess_state_based = state;
		break;
	    case ALIGN_STATE_BASED:
		Align_state_based= state;
		break;
	    case GERMETRY_STATE_BASED:
		Germetry_state_based = state;
		break;
	    case RESTRUCT_STATE_BASED:
		Restruct_state_based = state;
		break;
	    case POSTPROCESS_STATE_BASED:
		Postprocess_state_based = state;
		break;
    }
}


void CProInstance::Print()
{
    cout<<"---------------Print Instance----------------"<<endl;
    //added by liuce
    
      qDebug("Process_dir:%s\n\
     Pro_dir:%s\n\
     Mrc_dir:%s\n\
     Angle_dir:%s\n\
     Preali_dir:%s\n\
     Pre_xf_dir:%s\n\
     Param_dir:%s\n\
     Fin_xf_dir:%s\n\
     Fin_xg_dir:%s\n\
     Newtlt_dir:%s\n\
     Fin_mrc_dir:%s\n\
     Res_mrc_dir:%s\n\
     Pre_mrc_dir:%s\n\
     Post_mrc_dir:%s\n\
     Proerro_mrc_dir:%s\n\
     Ali_mrc_dir:%s\n\
     syslog_file:%s\n",
     Process_dir.data(),
     Pro_dir.data(),
     Mrc_dir.data(),
     Angle_dir.data(),
     Preali_dir.data(),
     Pre_xf_dir.data(),
     Param_dir.data(),
     Fin_xf_dir.data(),
     Fin_xg_dir.data(),
     Newtlt_dir.data(),
     Fin_mrc_dir.data(),
     Res_mrc_dir.data(),
     Pre_mrc_dir.data(),
     Post_mrc_dir.data(),
     Proerro_mrc_dir.data(),
     Ali_mrc_dir.data(),
	     syslog_file.data()
      
      );
    
    
    cout<<"-------------------------------------------"<<endl;
}

bool CProInstance::SetItem(const std::string& key, const std::string& value)
{
      if(key == "SYSLOG_FILE") {
	  syslog_file = value;
	  return true;
	}
    if(key=="PRO_FLOW"){
	if(value=="BasedMethod") Pro_flow=BasedMethod;
	else if(value=="FreeMethod") Pro_flow=FreeMethod;
	else Pro_flow=Null;
	return true;
    }
    if(key == "PROCESS_DIR"){
	Process_dir = value;
	return true;
    }
    if(key == "PRO_DIR"){
	Pro_dir = value;
	return true;
    }
    if(key == "MRC_DIR"){
	Mrc_dir = value;
	return true;
    }
    if(key == "ANGLE_DIR"){
	Angle_dir = value;
	return true;
    }
    if(key == "PREALI_DIR"){
	Preali_dir = value;
	return true;
    }
    if(key == "PRE_XF_DIR"){
	Pre_xf_dir = value;
	return true;
    }
    if(key == "PARAM_DIR"){
	Param_dir = value;
	return true;
    }
    //…��?
    if(key == "FIN_XF_DIR"){
	Fin_xf_dir = value;
	return true;
    }
    if(key == "FIN_XG_DIR"){
	Fin_xg_dir = value;
	return true;
    }
    if(key == "NEWTLT_DIR"){
	Newtlt_dir = value;
	return true;
    }
    if(key == "FIN_MRC_DIR"){
	Fin_mrc_dir = value;
	return true;
    }
    if(key == "RES_MRC_DIR"){
      Res_mrc_dir = value;
      return true;
    }
    if(key == "POST_MRC_DIR"){
      Post_mrc_dir = value;
      	return true;
    }
    if(key == "PROERRO_MRC_DIR"){
      Proerro_mrc_dir = value;
      	return true;
    }
    if(key == "ALI_MRC_DIR"){
	Ali_mrc_dir = value;
	return true;
    }
    if(key == "PREPROCESS_STATE"){
	Preprocess_state = JudgeState(value);
	return true;
    }
    if(key == "COSALI_STATE"){
	Cosali_state = JudgeState(value);
	return true;
    }
    if(key == "FINEALI_STATE"){
	Fineali_state = JudgeState(value);
	return true;
    }
    if(key == "GERMETRY_STATE"){
	Germetry_state = JudgeState(value);
	return true;
    }
    if(key == "RESTRUCT_STATE"){
	Restruct_state = JudgeState(value);
	return true;
    }
    if(key == "POSTPROCESS_STATE"){
      Postprocess_state = JudgeState(value);
      return true;
    }
    
    if(key == "PREPROCESS_STATE_BASED"){
	Preprocess_state_based = JudgeState(value);
	return true;
    }
    if(key == "ALIGN_STATE_BASED"){
	Align_state_based = JudgeState(value);
	return true;
    }
    if(key == "GERMETRY_STATE_BASED"){
	Germetry_state_based = JudgeState(value);
	return true;
    }
    if(key == "RESTRUCT_STATE_BASED"){
	Restruct_state_based = JudgeState(value);
	return true;
    }
    if(key == "POSTPROCESS_STATE_BASED"){
	Postprocess_state_based = JudgeState(value);
	return true;
    }
    
    
    if(key == "PRE_MRC_DIR"){
      Pre_mrc_dir = value;
      return true;
    }
    //…��?
    return false;
}

int CProInstance::JudgeState(string str)
{
    if(str == "UNFINISH") return UNFINISH;
    if(str == "FINISH") return FINISH;
    if(str == "FAIL") return FAIL;
}


bool CProInstance::Exite(const std::string& key)
{
  if(key == "SYSLOG_FILE") {
	  if(syslog_file == "NULL")
	      return false;
	  else return true;
	}
   if(key == "PROCESS_DIR"){
	if(Process_dir == "NULL")
	return false;
	else return true;
    }
    if(key == "PRO_DIR"){
	if(Pro_dir == "NULL")
	return false;
	else return true;
    }
    if(key == "MRC_DIR"){
	if(Mrc_dir == "NULL")
	return false;
	else return true;
    }
    if(key == "ANGLE_DIR"){
	if(Angle_dir == "NULL")
	return false;
	else return true;
    }
    if(key == "PREALI_DIR"){
	if(Preali_dir == "NULL")
	return false;
	else return true;
    }
    if(key == "PRE_XF_DIR"){
	if(Pre_xf_dir == "NULL")
	return false;
	else return true;
    }
    if(key == "PARAM_DIR"){
	if(Param_dir == "NULL")
	return false;
	else return true;
    }
    //…��?
    if(key == "FIN_XF_DIR"){
	if(Fin_xf_dir == "NULL")
	return false;
	else return true;
    }
    if(key == "FIN_XG_DIR"){
	if(Fin_xg_dir == "NULL")
	return false;
	else return true;
    }
    if(key == "NEWTLT_DIR"){
	if(Newtlt_dir == "NULL")
	return false;
	else return true;
    }
    if(key == "FIN_MRC_DIR"){
	if(Fin_mrc_dir == "NULL")
	return false;
	else return true;
    }
    if(key == "RES_MRC_DIR"){
	if(Res_mrc_dir == "NULL")
	return false;
	else return true;
    }
     if(key == "PRE_MRC_DIR"){
       cout<<"PRE_MRC_DIR"<<endl;
	if(Pre_mrc_dir == "NULL"){
	  cout<<"not exit"<<endl;
	  return false;
	}
	else return true;
    }
    if(key == "POST_MRC_DIR"){
	if(Post_mrc_dir == "NULL")
	return false;
	else return true;
    }
    if(key == "PROERRO_MRC_DIR"){
	if(Proerro_mrc_dir == "NULL")
	return false;
	else return true;
    }
    if(key == "ALI_MRC_DIR"){
	if(Ali_mrc_dir == "NULL")
	    return false;
	else return true;
    }
    if(key == "PREPROCESS_STATE"){
	if(Preprocess_state == UNFINISH)
	return false;
	else return true;
    }
    if(key == "COSALI_STATE"){
	if(Cosali_state == UNFINISH)
	return false;
	else return true;
    }
    if(key == "FINEALI_STATE"){
	if(Fineali_state == UNFINISH)
	return false;
	else return true;
    }
    if(key == "GERMETRY_STATE"){
	if(Germetry_state == UNFINISH)
	return false;
	else return true;
    }
    if(key == "RESTRUCT_STATE"){
	if(Restruct_state == UNFINISH)
	return false;
	else return true;
    }
    if(key == "POSTPROCESS_STATE"){
	if(Postprocess_state == UNFINISH)
	return false;
	else return true;
    }
    
    if(key == "PREPROCESS_STATE_BASED"){
	if(Preprocess_state_based == UNFINISH)
	    return false;
	else return true;
    }
    if(key == "ALIGN_STATE_BASED"){
	if(Align_state_based == UNFINISH)
	    return false;
	else return true;
    }
    if(key == "GERMETRY_STATE_BASED"){
	if(Germetry_state_based == UNFINISH)
	    return false;
	else return true;
    }
    if(key == "RESTRUCT_STATE_BASED"){
	if(Restruct_state_based == UNFINISH)
	    return false;
	else return true;
    }
    if(key == "POSTPROCESS_STATE_BASED"){
	if(Postprocess_state_based == UNFINISH)
	    return false;
	else return true;
    }
}

bool CProInstance::Finished(int step)
{
     switch(step){
	    case PREPROCESS_STATE:
		if(Preprocess_state == FINISH)
		    return true;
		else return false;
	    case COSALI_STATE:
		if(Cosali_state == FINISH)
		    return true;
		else return false;
	    case FINEALI_STATE:
		if(Fineali_state == FINISH)
		    return true;
		else return false;
	    case GERMETRY_STATE:
		if(Germetry_state == FINISH)
		    return true;
		else return false;
	    case RESTRUCT_STATE:
		if(Restruct_state == FINISH)
		    return true;
		else return false;
	    case POSTPROCESS_STATE:
		if(Postprocess_state == FINISH)
		    return true;
		else return false;
		
	    case PREPROCESS_STATE_BASED:
		if(Preprocess_state_based == FINISH)
		    return true;
		else return false;
	    case ALIGN_STATE_BASED:
		if(Align_state_based == FINISH)
		    return true;
		else return false;
	    case GERMETRY_STATE_BASED:
		if(Germetry_state_based == FINISH)
		    return true;
		else return false;
	    case RESTRUCT_STATE_BASED:
		if(Restruct_state_based == FINISH)
		    return true;
		else return false;
	    case POSTPROCESS_STATE_BASED:
		if(Postprocess_state_based == FINISH)
		    return true;
		else return false;
		
    }
}


//---------------------CProManage-------------------------

bool IsSpace(char c)
{
    if (' ' == c || '\t' == c)
        return true;
    return false;
}

void Trim(string & str)
{
    if (str.empty()) {
        return;
    }
    int i, start_pos, end_pos;
    for (i = 0; i < str.size(); ++i) {
        if (!IsSpace(str[i])) {
            break;
        }
    }
    if (i == str.size()) { // 全部是空白字符串
        str = "";
        return;
    }
    
    start_pos = i;
    
    for (i = str.size() - 1; i >= 0; --i) {
        if (!IsSpace(str[i])) {
            break;
        }
    }
    end_pos = i;
    
    str = str.substr(start_pos, end_pos - start_pos + 1);
}

bool CProManage::AnalyseLine(const string& line, string& key, string& value)
{
  cout<<"analys line:"<<line<<endl;
    int pos;
    if((pos = line.find('=')) == -1)
	return false;
    key = line.substr( 0 , pos);
    value = line.substr(pos+1 ,line.size() - (pos + 1));
    Trim(key);
    if (key.empty()) {
        return false;
    }
    Trim(value);
    return true;
}


bool CProManage::CreatProcess(const string& name, const string& MRC, const string& rawtlt)
{
    cout<<"Create Process"<<endl;
    
    GetInstance()->Clear();
    //needtobechanged
    m_proInstance->SetMethod(Null);
    m_proInstance->SetDir(PROCESS_DIR , name );
    m_proInstance->SetDir(MRC_DIR , MRC );
    m_proInstance->SetDir(ANGLE_DIR , rawtlt);
    m_proInstance->SetDir(PRO_DIR , name +".pro");
    m_proInstance->SetDir(SYSLOG_FILE , name +".log");//##!!
//     m_proInstance->Print();
    
    //改工作路��?
    string currentWorkDir = CEnv::InitWorkDirectory();
    currentWorkDir+="/"+CProManage::GetInstance()->GetDir(PROCESS_DIR);
    CEnv::SetCurrentWorkDirectory(currentWorkDir.data());
    
    //create .pro
    fstream pfile;
    pfile.open(m_proInstance->GetDir(PRO_DIR).c_str() , ios::out);
    if(!pfile) return false;
    pfile.close();
           
    ofstream logfile;
    logfile.open(m_proInstance->GetDir(SYSLOG_FILE).c_str()); 
    logfile.close();

    pfile.open(m_proInstance->GetDir(PRO_DIR).c_str() , ios::out);
    if(pfile){
	pfile<<"PRO_FLOW ="<<m_proInstance->GetMethod()<<endl;
	pfile<<"PROCESS_DIR = "<<name<<endl;
	pfile<<"PRO_DIR = "<<m_proInstance->GetDir(PRO_DIR)<<endl;
	pfile<<"MRC_DIR = "<<MRC<<endl;
	pfile<<"ANGLE_DIR = "<<rawtlt<<endl;
	pfile<<"SYSLOG_FILE = "<<m_proInstance->GetDir(SYSLOG_FILE)<<endl;
	pfile.close();
	//	//
	
	return true;
    }
    else{
	pfile.close();
	return false;
    }

    
}

bool CProManage::LoadProcess(const string& path)
{
    cout<<"Start Load Process path :"<<path<<endl;
    
    //要先清空之前的工��?
    GetInstance()->Clear();
    
    ifstream pfile;
    pfile.open(path.c_str());
    if(pfile){
	string line, key, value;
	while(getline(pfile , line)){
	    if(AnalyseLine(line , key , value)){
		m_proInstance->SetItem(key ,value);
	    }
	}
	pfile.close();
    }
    else{
	pfile.close();
	return false;
    }
    SetHaveProcess(true);
    
//     string currentWorkDir = CEnv::CurrentWorkDirectory();
//     currentWorkDir+="/"+CProManage::GetInstance()->GetDir(PROCESS_DIR);
    string currentWorkDir = path.substr(0 , path.length() - CProManage::GetInstance()->GetDir(PRO_DIR).length()-1);
    cout<<"set currentWorkDir : "<<currentWorkDir<<endl;
    CEnv::SetCurrentWorkDirectory(currentWorkDir.data());
      
    return true;
}

void CProManage::AddItem(const std::string& key, const std::string& value)
{
    cout<<"Start Add Item"<<" key :"<<key<<" value = "<<value<<endl;
    fstream pfile;   
    if(m_proInstance->Exite(key)){
	cout<<"exite key!"<<endl;
	m_proInstance->SetItem(key ,value);
// 	m_proInstance->Print();
	ReWriteProFile();
    }
    else{
// 	cout<<"else"<<endl;
	cout<<m_proInstance->GetDir(PRO_DIR)<<endl;
	pfile.open(m_proInstance->GetDir(PRO_DIR).c_str() , ios::app|ios::out);	
	if(pfile){
	pfile<<key<<" = "<<value<<endl;
	}
	m_proInstance->SetItem(key , value);
	pfile.close();
// 	cout<<"close"<<endl;
 
    }
 ReWriteProFile();	 
  
}

void CProManage::ReWriteProFile()
{
    cout<<"start rewrite profile"<<endl;
    fstream pfile;
    pfile.open(m_proInstance->GetDir(PRO_DIR).c_str() , ios::out|ios::trunc);
	 cout<<"start rewrite profile"<<endl;
	 pfile<<"PRO_FLOW= "<<m_proInstance->GetMethod()<<endl;
    	pfile<<"PROCESS_DIR = "<<m_proInstance->GetDir(PROCESS_DIR)<<endl;
	pfile<<"PRO_DIR = "<<m_proInstance->GetDir(PRO_DIR)<<endl;
	pfile<<"MRC_DIR = "<<m_proInstance->GetDir(MRC_DIR)<<endl;
	pfile<<"ANGLE_DIR = "<<m_proInstance->GetDir(ANGLE_DIR)<<endl;
	pfile<<"SYSLOG_FILE = "<<m_proInstance->GetDir(SYSLOG_FILE)<<endl;//#!!
	if(m_proInstance->Exite("PRE_MRC_DIR")){
	    pfile<<"PRE_MRC_DIR = "<<m_proInstance->GetDir(PRE_MRC_DIR)<<endl;
	}
        if(m_proInstance->Exite("PREALI_DIR")){
	    pfile<<"PREALI_DIR = "<<m_proInstance->GetDir(PREALI_DIR)<<endl;
	}
	if(m_proInstance->Exite("PRE_XF_DIR")){
	    pfile<<"PRE_XF_DIR = "<<m_proInstance->GetDir(PRE_XF_DIR)<<endl;
	}
	if(m_proInstance->Exite("PARAM_DIR")){
	    pfile<<"PARAM_DIR = "<<m_proInstance->GetDir(PARAM_DIR)<<endl;
	}
	if(m_proInstance->Exite("FIN_XF_DIR")){
	    pfile<<"FIN_XF_DIR = "<<m_proInstance->GetDir(FIN_XF_DIR)<<endl;
	}
	if(m_proInstance->Exite("FIN_XG_DIR")){
	    pfile<<"FIN_XG_DIR = "<<m_proInstance->GetDir(FIN_XG_DIR)<<endl;
	}
	if(m_proInstance->Exite("FIN_MRC_DIR")){
	    pfile<<"FIN_MRC_DIR = "<<m_proInstance->GetDir(FIN_MRC_DIR)<<endl;
	}
	if(m_proInstance->Exite("NEWTLT_DIR")){
	    pfile<<"NEWTLT_DIR = "<<m_proInstance->GetDir(NEWTLT_DIR)<<endl;
	}
	if(m_proInstance->Exite("RES_MRC_DIR")){
	    pfile<<"RES_MRC_DIR = "<<m_proInstance->GetDir(RES_MRC_DIR)<<endl;
	}
	if(m_proInstance->Exite("POST_MRC_DIR")){
	    pfile<<"POST_MRC_DIR = "<<m_proInstance->GetDir(POST_MRC_DIR)<<endl;
	}
	if(m_proInstance->Exite("ALI_MRC_DIR")){
	    pfile<<"ALI_MRC_DIR = "<<m_proInstance->GetDir(ALI_MRC_DIR)<<endl;
	}
	if(m_proInstance->Exite("PROERRO_MRC_DIR")){
	    pfile<<"PROERRO_MRC_DIR = "<<m_proInstance->GetDir(PROERRO_MRC_DIR)<<endl;
	}
	if(m_proInstance->Exite("PREPROCESS_STATE")){
	    pfile<<"PREPROCESS_STATE = "<<JudgeKey(m_proInstance->GetState(PREPROCESS_STATE))<<endl;
	}
	if(m_proInstance->Exite("COSALI_STATE")){
	    pfile<<"COSALI_STATE = "<<JudgeKey(m_proInstance->GetState(COSALI_STATE))<<endl;
	}
	if(m_proInstance->Exite("FINEALI_STATE")){
	    pfile<<"FINEALI_STATE = "<<JudgeKey(m_proInstance->GetState(FINEALI_STATE))<<endl;
	}
	if(m_proInstance->Exite("GERMETRY_STATE")){
	    pfile<<"GERMETRY_STATE = "<<JudgeKey(m_proInstance->GetState(GERMETRY_STATE))<<endl;
	}
	if(m_proInstance->Exite("RESTRUCT_STATE")){
	    pfile<<"RESTRUCT_STATE = "<<JudgeKey(m_proInstance->GetState(RESTRUCT_STATE))<<endl;
	}
	if(m_proInstance->Exite("POSTPROCESS_STATE")){
	    pfile<<"POSTPROCESS_STATE = "<<JudgeKey(m_proInstance->GetState(POSTPROCESS_STATE))<<endl;
	}	
    
	
	
	if(m_proInstance->Exite("PREPROCESS_STATE_BASED")){
	    pfile<<"PREPROCESS_STATE_BASED = "<<JudgeKey(m_proInstance->GetState(PREPROCESS_STATE_BASED))<<endl;
	}
	if(m_proInstance->Exite("ALIGN_STATE_BASED")){
	    pfile<<"ALIGN_STATE_BASED = "<<JudgeKey(m_proInstance->GetState(ALIGN_STATE_BASED))<<endl;
	}
	if(m_proInstance->Exite("GERMETRY_STATE_BASED")){
	    pfile<<"GERMETRY_STATE_BASED = "<<JudgeKey(m_proInstance->GetState(GERMETRY_STATE_BASED))<<endl;
	}
	if(m_proInstance->Exite("RESTRUCT_STATE_BASED")){
	    pfile<<"RESTRUCT_STATE_BASED = "<<JudgeKey(m_proInstance->GetState(RESTRUCT_STATE_BASED))<<endl;
	}
	if(m_proInstance->Exite("POSTPROCESS_STATE_BASED")){
	    pfile<<"POSTPROCESS_STATE_BASED = "<<JudgeKey(m_proInstance->GetState(POSTPROCESS_STATE_BASED))<<endl;
	}
    pfile.close();
    cout<<"close"<<endl;
}

void CProManage::Clear()
{
    
}
void CProInstance::ChangeMethod()
{
    Preali_dir = "NULL";
    Pre_xf_dir = "NULL";
    Param_dir = "NULL";
    Fin_xf_dir = "NULL";
    Fin_xg_dir = "NULL";
    Newtlt_dir = "NULL";
    Fin_mrc_dir = "NULL";
    Res_mrc_dir = "NULL";
    Pre_mrc_dir = "NULL";
    Post_mrc_dir = "NULL";
    Proerro_mrc_dir = "NULL";
    
    if(Pro_flow==BasedMethod){
	Pro_flow=FreeMethod;
    }
    else Pro_flow=BasedMethod;
    
    Preprocess_state = UNFINISH;
    Cosali_state = UNFINISH;
    Fineali_state  = UNFINISH;
    Germetry_state =  UNFINISH;
    Restruct_state  = UNFINISH;
    Postprocess_state = UNFINISH;
    
    Preprocess_state_based= UNFINISH;
    Align_state_based= UNFINISH;
    Germetry_state_based= UNFINISH;
    Restruct_state_based= UNFINISH;
    Postprocess_state_based= UNFINISH;

}
void CProInstance::SetMethod(int method)
{
    Pro_flow=method;
}
string CProInstance::GetMethod()
{
    if(Pro_flow==BasedMethod) return "BasedMethod";
    else if(Pro_flow==FreeMethod)return "FreeMethod";
    else return "NULL";
}
CSettings::CSettings(){
    for(int i=0;i<3;i++)
        geo_location[i]=-1;
}

