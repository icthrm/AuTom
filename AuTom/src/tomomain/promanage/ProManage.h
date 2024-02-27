#ifndef PROMANAGE_H
#define PROMANAGE_H
#include <string>
#include <iostream>
//#include <QtGui> ##!!
#include "env/env.h"
#include <sys/stat.h>
#include <fstream>
#include <sstream>
#include <tr1/tuple>
using namespace std;

#define FINISH 0
#define FAIL 1
#define UNFINISH 2

#define PREPROCESS_STATE 5
#define COSALI_STATE 6
#define FINEALI_STATE 7
#define GERMETRY_STATE 8
#define RESTRUCT_STATE 9
#define POSTPROCESS_STATE 23

#define PROCESS_DIR 10
#define PRO_DIR 11 
#define MRC_DIR 12
#define ANGLE_DIR 13
#define PREALI_DIR 14
#define PRE_XF_DIR 15
#define PARAM_DIR 16
#define FIN_XF_DIR 17
#define FIN_XG_DIR 18
#define NEWTLT_DIR 19
#define FIN_MRC_DIR 20
#define RES_MRC_DIR 21
#define PRE_MRC_DIR 22
#define POST_MRC_DIR 24
#define PROERRO_MRC_DIR 25
#define ALI_MRC_DIR 31
#define SYSLOG_FILE 32

#define PREPROCESS_STATE_BASED 26
#define ALIGN_STATE_BASED 27
#define GERMETRY_STATE_BASED 28
#define RESTRUCT_STATE_BASED 29
#define POSTPROCESS_STATE_BASED 30

#define BasedMethod 100
#define FreeMethod 101
#define Null 102

string JudgeKey(int state);
bool IsSpace(char c);
void Trim(string & str);

class CProInstance
{
private:
    int Preprocess_state;
    int Cosali_state;
    int Fineali_state;
    int Germetry_state;
    int Restruct_state;
    int Postprocess_state;
    
    int Preprocess_state_based;
    int Align_state_based;
    int Germetry_state_based;
    int Restruct_state_based;
    int Postprocess_state_based;
    
    int Pro_flow;
    string Process_dir;
    string Pro_dir;
    string Mrc_dir;
    string Angle_dir;
    string Preali_dir;
    string Pre_xf_dir;
    string Param_dir;
    string Fin_xf_dir;
    string Fin_xg_dir;
    string Newtlt_dir;
    string Fin_mrc_dir;
    string Res_mrc_dir;
    string Pre_mrc_dir;
    string Post_mrc_dir;
    string Proerro_mrc_dir;
    string Ali_mrc_dir;	
    string syslog_file;//##!!
public:
    CProInstance();
    ~CProInstance(){}
public:
    string GetDir(int type);
    int GetState(int type);
    void SetDir(int type , string data ); 
    void SetState(int type , int state);
    bool SetItem(const string& key , const string& value);
    void Print();
    bool Exite(const string& key );
    bool Finished(int step);
    string GetPath(int type);
    void Clear();
    void SetMethod(int method);
    void ChangeMethod();
    string GetMethod();
private:
    int JudgeState(string str);

    
};
class CSettings{
    /*
      The CSettings does not store any data in harddrive, which means all the data in CSettings will be lost when users close the Autom and renew when Autom starts again. The lifespan of CSettings is from Autom-open to Autom-close.
      The class CSettings is added to the project when the main features are almost finished and it is added because I find that Autom is still lack of such static class whose lifespan is from Autom-open to Autom-close. However, due to the present of the class is so late, the class is still unfinished and needs improvements in the future. 
     */   
public:
    float geo_location[3];//used in geometry
    CSettings();  


};

class CProManage
{
private:
    static CSettings* m_settings;
    static CProInstance *m_proInstance;
    static bool m_haveProcess;
private:
    CProManage() {}
    ~CProManage(){}
public:
    static void InitInstance(){
        m_proInstance = new CProInstance();
        m_settings= new CSettings();
        
    }
    //static void DestoryInstance(){delete m_proInstance;}
    static bool CreatProcess(const string& name, const string& MRC, const string& rawtlt);
    static bool LoadProcess(const string &path);
    static CProInstance* GetInstance(){return m_proInstance;}
    static void SetHaveProcess(bool b){m_haveProcess = b;}
    static void Clear();
    static void AddItem(const string& key , const string& value);
    static void ReWriteProFile();
    static CSettings* getSettings(){return m_settings;}
private:
    static bool AnalyseLine(const string& line , string& key , string&  value );
};

#endif
