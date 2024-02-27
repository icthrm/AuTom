#include "profunc.h"
#include "string.h"
#include <sstream>
#include <fstream>
#include <string>
#include <cstdio>
#include <stdio.h>
#include <sys/stat.h>
#include<sys/types.h>
#include<dirent.h>
#include <stdlib.h>
#include <errno.h>
#include <regex.h>
#include "promanage/ProManage.h"
#include "core/proxy.h"
#ifndef BUF_MAX
#define BUF_MAX				4096
#endif


bool writeLog(const char* cmd)
{
    string logfile=CProManage::GetInstance()->GetDir(SYSLOG_FILE);
// 	cout<<"logfile dir:"<<logfile<<endl;
    fstream writelog;
    writelog.open(logfile.c_str(),ios::app|ios::out);
    writelog<<cmd<<std::endl;
    writelog.close();

    return true;
}
int RunCmd(const char* cmd)
{
    writeLog(cmd);
    FILE* fpin;
    char line[BUF_MAX];
    int returnValue=-1;
    if(MMTApp::ExecuteCmd(cmd , &fpin)) {
        printf("cmd = %s\n",cmd);
    }

    while(fgets(line, BUF_MAX, fpin) != NULL) {
        MMTApp::Echo(line , BUF_MAX);
    }

    if((returnValue = MMTApp::CloseExecutePFile(fpin)) != 0) {
        printf("child process returned a non-zero value!\n");
    }
    return returnValue;
}

void* profunc::OnTestMsg(msg_map::Message* msg)
{
    std::cout<<"Receive instruction: "<<msg->instruction<<std::endl;
    char* inst = new char[1024];
    memcpy(inst, msg->instruction, 1024);
    return (void*)inst;
}

void profunc::TestMsgFeedBack(void* data, msg_map::INFO* dst)
{
}
void* profunc::OnBuildModelMsg(msg_map::Message* msg)
{
//     std::istringstream ss(msg->instruction);
    
//     std::string mrcname, anglename;
//     bool to_rec[3];
//     float loc[3];
//     std::string rec_name[3];
//     int thickness;
//     std::string process_name;
//     std::string log_file;
// 
//     ss>>mrcname>>anglename;
//     for(int i = 0; i < 3; i++) {
//         ss>>to_rec[i];
//     }
// 
//     for(int i = 0; i < 3; i++) {
//         ss>>loc[i];
//     }
// 
//     ss>>thickness;
// 
//     for(int i = 0; i < 3; i++) {
//         ss>>rec_name[i];
//     }
//     char line[5];
// 
//     for(int i = 0; i < 3; i++) {
//         std::stringstream cmd;
//         if(to_rec[i]) {
//             cmd<<"tomosect"<<" -i "<<mrcname<<" -a "<<anglename<<" -t "<<thickness<<" -y "<<float(loc[i])<<" -o "<<rec_name[i];
//             printf("cmd = %s\n",cmd.str().c_str());
//             MMTApp::Execute(cmd.str().c_str(), "tomoset.log", "a");
//             MMTApp::Echo(i);
//             writeLog(cmd.str().c_str());
//         }
//     }
    return new int(RunCmd(msg->instruction));
   
}

void* profunc::OnBuildModelMsg_modify(msg_map::Message* msg)
{

}

void* profunc::OnHatPixel(msg_map::Message* msg)
{
    std::cout<<"Start OnHatPixel"<<std::endl;
    std::istringstream ss(msg->instruction);
    std::string input , output , logfile;
    float cutoff;
    ss>>input>>output>>cutoff>>logfile;
    std::stringstream cmd;
    cmd<<"hat_pixel"<<" -i "<<input<<" -c "<<cutoff<<" -o "<<output;
    std::cout<<"cmd = "<<cmd.str().c_str()<<std::endl;
    //MMTApp::Execute(cmd.str().c_str() , logfile.c_str() , "a");

    writeLog(cmd.str().c_str());
    FILE* fpin , * fout;
    char line[BUF_MAX];

    if(MMTApp::ExecuteCmd(cmd.str().c_str() , &fpin)) {
        printf("in file = ");
    }

    if((fout = fopen(logfile.c_str(), "a")) == NULL) {
        printf("Open log<%s> error\n", logfile.c_str());
    }
    else printf("open log\n");
    while(fgets(line, BUF_MAX, fpin) != NULL) {
        fputs(line, fout);
        MMTApp::Echo(line , BUF_MAX);
    }
    if(  MMTApp::CloseExecutePFile(fpin) != 0 ) {
        printf("child process returned a non-zero value!\n");
    }
    fclose(fout);
    MMTApp::EchoResult(1);
}

void* profunc::OnRotateMrc(msg_map::Message* msg)
{
    std::cout<<"Start rotate mrc"<<std::endl;

    std::istringstream ss(msg->instruction);

    std::string inputimage , outputimage;
    float rotate_angle;

    ss>>inputimage>>outputimage>>rotate_angle;

    rotate_angle = 0-rotate_angle;

    std::stringstream cmd;

    cmd<<"newstack ";
    cmd<<"-input "<<inputimage;
    cmd<<" -output "<<outputimage;
    cmd<<" -rotate "<<rotate_angle;

    FILE* fpin , * fout ,*fp;
    char line[BUF_MAX];

    RunCmd(cmd.str().c_str());

    MMTApp::EchoResult(1);

}

void* profunc::OnCoarseAlign(msg_map::Message* msg)
{
    std::cout<<"Start CoarseAlign function"<<std::endl;

    std::istringstream ss(msg->instruction);
//     , align_param , rotation_param;
    std::string inputimage , outputimage ,processname;
    int round , rotation;

    ss>>inputimage>>outputimage>>round>>rotation>>processname;
//     std::cout<<"ss : "<<inputimage<<" "<<outputimage<<" "<<round<<" "<<rotation;

    FILE* fpin , * fout ,*fp;
    char line[BUF_MAX];

    //看temp是否存在，不存在就创��?
    if(opendir("tmp") == NULL) {
        std::cout<<"tmp not exit"<<std::endl;
        if(mkdir("tmp" , S_IRWXU) == 0) {
            std::cout<<"creat tmp"<<std::endl;
        }
        else {
            std::cout<<"error"<<std::endl;
            MMTApp::EchoResult(-1);
            return (void*)-1;
        }
    }
    else {
        system("rm -f ./tmp/*");
    }

    strcpy(line ,  "turn 1\n");
    MMTApp::Echo(line , BUF_MAX);
    if(rotation) {
        strcpy(line ,  "xyshift\n");
        MMTApp::Echo(line , BUF_MAX);

        std::ostringstream xyshift_param ;
        std::string out = "./tmp/shift1.dat";
        xyshift_param<<"cr_xyshift";
        xyshift_param<<" -i "<<inputimage;
        xyshift_param<<" -o "<<out;

        RunCmd(xyshift_param.str().c_str());

        strcpy(line ,  "align\n");
        MMTApp::Echo(line , BUF_MAX);
        std::ostringstream align_param;
        align_param<<"par2xf";
        align_param<<" -s "<<out;
        align_param<<" -o "<<"param.par -I";

        RunCmd(align_param.str().c_str());

        std::ostringstream newstack_param;
        newstack_param<<"mrcstack";
        newstack_param<<" -x "<<"param.par ";
        newstack_param<<" -i "<<inputimage;
        newstack_param<<" -o "<<outputimage;

        RunCmd(newstack_param.str().c_str());

        strcpy(line ,  "rotation\n");
        MMTApp::Echo(line , BUF_MAX);
        std::ostringstream rotation_param;
        std::string shiftmrc = "./tmp/"+ processname + ".shift1.proj.mrc";
        std::string tmp = "./tmp/" + processname + ".shift1.proj.trp.st";
        std::string rotdata = "./tmp/rot1.dat";
        std::string fft = "./tmp/" + processname + ".fft1.mrc";
        rotation_param<<"cr_rotation";
        rotation_param<<" -p "<<shiftmrc;
        rotation_param<<" -t "<<tmp;
        rotation_param<<" -o "<<rotdata;
        rotation_param<<" -f "<<fft;
        rotation_param<<" -s "<<outputimage;

        RunCmd(rotation_param.str().c_str());

        strcpy(line ,  "align2\n");
        MMTApp::Echo(line , BUF_MAX);
        align_param.str("");
        align_param<<"par2xf";
        align_param<<" -s "<<out;
        align_param<<" -r "<<rotdata;
        align_param<<" -o "<<"param.par -I";

        RunCmd(align_param.str().c_str());

        RunCmd(newstack_param.str().c_str());
    }
    else {
        strcpy(line ,  "xyshift\n");
        MMTApp::Echo(line , BUF_MAX);

        std::ostringstream xyshift_param ;
        std::string out = "./tmp/shift1.dat";
        xyshift_param<<"cr_xyshift";
        xyshift_param<<" -i "<<inputimage;
        xyshift_param<<" -o "<<out;

        RunCmd(xyshift_param.str().c_str());

        strcpy(line ,  "align\n");
        MMTApp::Echo(line , BUF_MAX);
        std::ostringstream align_param;
        align_param<<"par2xf";
        align_param<<" -s "<<out;
        align_param<<" -o "<<"param.par -I";

        RunCmd(align_param.str().c_str());

        std::ostringstream newstack_param;
        newstack_param<<"mrcstack";
        newstack_param<<" -x "<<"param.par ";
        newstack_param<<" -i "<<inputimage;
        newstack_param<<" -o "<<outputimage;

        RunCmd(newstack_param.str().c_str());
    }

    int i = 2;
    std::ostringstream _line;
    while( i <= round) {
        _line.str("");
        _line<<"turn "<<i;
        strcpy(line ,  _line.str().c_str());
        MMTApp::Echo(line , BUF_MAX);

        if(rotation) {
            strcpy(line ,  "xyshift\n");
            MMTApp::Echo(line , BUF_MAX);

            std::ostringstream xyshift_param ;
            std::ostringstream outdat;
            std::ostringstream rotdat;
            std::ostringstream inshift;
            outdat<<"./tmp/shift"<<i<<".dat";
            rotdat<<"./tmp/rot"<<i-1<<".dat";
            inshift<<"./tmp/shift"<<i-1<<".dat";
            xyshift_param<<"cr_xyshift";
            xyshift_param<<" -i "<<outputimage;
            xyshift_param<<" -o "<<outdat.str();;
            xyshift_param<<" -r "<<rotdat.str();;
            xyshift_param<<" -s "<<inshift.str();;

            RunCmd(xyshift_param.str().c_str());

            strcpy(line ,  "align\n");
            MMTApp::Echo(line , BUF_MAX);
            std::ostringstream align_param;
            align_param<<"par2xf";
            align_param<<" -s "<<outdat.str();;
            align_param<<" -r "<<rotdat.str();
            align_param<<" -o "<<"param.par -I";

            RunCmd(align_param.str().c_str());

            std::ostringstream newstack_param;
            newstack_param<<"mrcstack ";
            newstack_param<<" -x "<<"param.par ";
            newstack_param<<" -i "<<inputimage;
            newstack_param<<" -o "<<outputimage;

            RunCmd(newstack_param.str().c_str());

            strcpy(line ,  "rotation\n");
            MMTApp::Echo(line , BUF_MAX);
            std::ostringstream rotation_param;
            std::string shiftmrc = "./tmp/"+ processname + ".shift1.proj.mrc";
            std::string tmp = "./tmp/" + processname + ".shift1.proj.trp.st";
            std::ostringstream rotout;
            rotout<<"./tmp/rot"<<i<<".dat";
            std::string fft = "./tmp/" + processname + ".fft1.mrc";
            rotation_param<<"cr_rotation";
            rotation_param<<" -p "<<shiftmrc;
            rotation_param<<" -t "<<tmp;
            rotation_param<<" -o "<<rotout.str();;
            rotation_param<<" -f  "<<fft;
            rotation_param<<" -r "<<rotdat.str();
            rotation_param<<" -s "<<outputimage;

            RunCmd(rotation_param.str().c_str());

            strcpy(line ,  "align2\n");
            MMTApp::Echo(line , BUF_MAX);
            align_param.str("");
            align_param<<"par2xf";
            align_param<<" -s "<<outdat.str();;
            align_param<<" -r "<<rotout.str();;
            align_param<<" -o "<<"param.par -I";

            RunCmd(align_param.str().c_str());

            RunCmd(newstack_param.str().c_str());
        }
        else {
            strcpy(line ,  "xyshift\n");
            MMTApp::Echo(line , BUF_MAX);

            std::ostringstream xyshift_param ;
            std::ostringstream outdat;
            std::ostringstream inshift;
            outdat<<"./tmp/shift"<<i<<".dat";
            inshift<<"./tmp/shift"<<i-1<<".dat";
            xyshift_param<<"cr_xyshift";
            xyshift_param<<" -i "<<outputimage;
            xyshift_param<<" -o "<<outdat.str();;
            xyshift_param<<" -s "<<inshift.str();;

            RunCmd(xyshift_param.str().c_str());

            strcpy(line ,  "align\n");
            MMTApp::Echo(line , BUF_MAX);
            std::ostringstream align_param;
            align_param<<"par2xf";
            align_param<<" -s "<<outdat.str();;
            align_param<<" -o "<<"param.par -I";

            RunCmd(align_param.str().c_str());

            std::ostringstream newstack_param;
            newstack_param<<"mrcstack";
            newstack_param<<" -x "<<"param.par ";
            newstack_param<<" -i "<<inputimage;
            newstack_param<<" -o "<<outputimage;

            RunCmd(newstack_param.str().c_str());
        }
        i++;
    }

    std::string str;
    std::ostringstream shift1;
    std::ostringstream shift2;

    shift1<<"./tmp/shift"<<round<<".dat";
    shift2<<"./shift"<<round<<".dat";

    str = "cp "+ shift1.str() + " .";
    system(str.c_str());
    str = "mv "+shift2.str() + " shift.dat";
    system(str.c_str());

    if(rotation) {
        std::ostringstream rot1;
        std::ostringstream rot2;
        rot1<<"./tmp/rot"<<round<<".dat";
        rot2<<"./rot"<<round<<".dat";

        str = "cp "+ rot1.str() + " .\n";
        std::cout<<"str : "<<str<<std::endl;
        system(str.c_str());
        str = "mv "+ rot2.str() + "  rotation.dat";
        system(str.c_str());
    }

    if(rotation) {
        std::ostringstream align_param;
        align_param<<"par2xf";
        align_param<<" -s "<<"shift.dat";
        align_param<<" -r "<<"rotation.dat";
        align_param<<" -o "<<"param.par";

        RunCmd(align_param.str().c_str());

        align_param.str("");
        align_param<<"par2xf";
        align_param<<" -s "<<"shift.dat";
        align_param<<" -r "<<"rotation.dat";
        align_param<<" -o "<<" 2d_image.xf --IMOD";
        RunCmd(align_param.str().c_str());
    }
    else {
        std::ostringstream align_param;
        align_param<<"par2xf";
        align_param<<" -s "<<"shift.dat";
        align_param<<" -o "<<"param.par";

        RunCmd(align_param.str().c_str());

        align_param.str("");
        align_param<<"par2xf";
        align_param<<" -s "<<"shift.dat";
        align_param<<" -o "<<" 2d_image.xf --IMOD";
        RunCmd(align_param.str().c_str());
    }
    MMTApp::EchoResult(1);
}


void* profunc::OnRAPTOR(msg_map::Message* msg)
{
    std::cout<<"start RAPTOR"<<std::endl;
    std::istringstream ss(msg->instruction);
    std::string execPath , path , input , output , diameter , markers;
    ss>>execPath>>path>>input>>output>>diameter>>markers;
    std::stringstream cmd;

    cmd<<execPath<<"RAPTOR";
    cmd<<" -execPath "<<execPath;
    cmd<<" -path "<<path;
    cmd<<" -input "<<input;
    cmd<<" -output "<<output;
    cmd<<" -diameter "<<diameter;
    cmd<<" -markers "<<markers;
//   cmd<<" -verb 1 ";
    cmd<<" 2>&1 | tee raptor.log";

    RunCmd(cmd.str().c_str());

    MMTApp::EchoResult(1);
}

void* profunc::OnFineAlign(msg_map::Message* msg)
{
    std::istringstream ss(msg->instruction);
    std::string  inputpreali , inputangle , outputxf , newangle ,
        inputprexf , outputxg , inputimage , outputmrc;
    ss>>inputpreali>>inputangle>>outputxf>>newangle>>inputprexf>>outputxg>>inputimage>>outputmrc;
    std::stringstream align_param;

    align_param<<"atomalign";
    align_param<<" -i "<<inputpreali;
    align_param<<" -a "<<inputangle;
    align_param<<" -o "<<outputxf;
    align_param<<" -n "<<newangle;
 
    writeLog(align_param.str().c_str());
    FILE* fpin , * fout;
    char line[BUF_MAX];

    RunCmd(align_param.str().c_str());
    

//    std::stringstream xfproduct_param;
////     xfproduct_param<<"xfp";
////     xfproduct_param<<" -in1 "<<inputprexf<<" -in2 "<<outputxf<<" -output "<<outputxg;
//    xfproduct_param<<"xfp";
//    xfproduct_param<<" "<<inputprexf<<" "<<outputxf<<" "<<outputxg;
//    writeLog( xfproduct_param.str().c_str());

//    RunCmd(xfproduct_param.str().c_str());

    
    std::stringstream newstack_param;

    newstack_param<<"mrcstack";
     newstack_param<<" -i "<<inputimage<<" -o "<<outputmrc<<"  -x "<<outputxf;
    
    /*
    newstack_param<<"newstack";
    newstack_param<<" -input "<<inputimage<<" -output "<<outputmrc<<"  -xform "<<outputxg;*/

    writeLog(newstack_param.str().c_str());
    
    RunCmd(newstack_param.str().c_str());
    
    MMTApp::EchoResult(1);
}

void* profunc::OnRestruct(msg_map::Message* msg)
{
    std::cout<<"Start Reconstruct function"<<std::endl;
    std::istringstream cmd(msg->instruction);
    std::cout<<"cmd = "<<cmd.str().c_str()<<std::endl;
    int* returnValue=new int(RunCmd(cmd.str().c_str()));
    /*
    //     FILE* fpin , * fout;
    //     char line[BUF_MAX];
    //
    //     std::istringstream cmd(msg->instruction);
    //     std::cout<<"cmd = "<<cmd.str().c_str()<<std::endl;
    //
    //     if(MMTApp::ExecuteCmd(cmd.str().c_str() , &fpin)){
    // 	printf("in file = ");
    //     }
    //
    //      while(fgets(line, BUF_MAX, fpin) != NULL){
    // 	MMTApp::Echo(line , BUF_MAX);
    //     }
    //
    //     if(pclose(fpin) == -1){
    // 	printf("Pclose error!\n");
    //     }
    */
    return returnValue;
}

void* profunc::OnEvaluate(msg_map::Message* msg)
{
    std::cout<<"Start OnEvaluate function"<<std::endl;
    std::istringstream ss(msg->instruction);
    std::string recmrc , finmrc , angle , outputrepro ,outputerror , paramfile;
    int thickness;
    float zshift, offset , pitch;
    ss>>recmrc>>finmrc>>angle>>outputrepro>>outputerror>>paramfile;

    FILE *f_geo;
    char *tmp;

    if((f_geo=fopen(paramfile.data(),"rb"))==NULL)
    {

        printf("\nCannot open file '%s' strike any key exit!",paramfile.data());
        MMTApp::EchoResult(-1);
    }
//	for(i=0; i < GEO_LINES; i++)
    {

        int cflags=REG_EXTENDED|REG_NEWLINE|REG_ICASE;
        unsigned long PosCur,PosBegin,PosEnd;
        size_t fileLen;
        PosCur=ftell(f_geo);
        fseek(f_geo,0,SEEK_SET);
        PosBegin=ftell(f_geo);
        fseek(f_geo,0,SEEK_END);
        PosEnd=ftell(f_geo);
        fseek(f_geo,PosCur,SEEK_SET);

        fileLen=PosEnd-PosBegin;

        char *buf=(char*)malloc(fileLen+1);
        fread(buf,fileLen,1,f_geo);
        buf[fileLen]=0;
        printf("buf is: \n%s",buf);
        regex_t reg;
        regmatch_t mat[2];
        regcomp(&reg,"^[ \t]*THICKNESS[ \t]+([-0-9.]+)[ \t]*$",cflags);
        int status;
        status=regexec(&reg,buf,2,mat,0);
        if(status!=0)
        {
            printf("\nCan not found Param thickness! Error file!\n");
            MMTApp::EchoResult(-1);
        }
        int len =mat[1].rm_eo-mat[1].rm_so;
        char *num=(char*)malloc(len+1);
        memcpy(num,&buf[mat[1].rm_so],len);
        num[len]=0;
        thickness=atoi(num);
        free(num);
        regfree(&reg);

        regcomp(&reg,"^[ \t]*ZSHIFT[ \t]+([-0-9.]+)[ \t]*$",cflags);
        status=regexec(&reg,buf,2,mat,0);
        if(status!=0)
        {
            printf("\nCan not found Param zshift! Error file!\n");
            MMTApp::EchoResult(-1);
        }
        len =mat[1].rm_eo-mat[1].rm_so;
        num=(char*)malloc(len+1);
        memcpy(num,&buf[mat[1].rm_so],len);
        num[len]=0;
        zshift=atof(num);
        free(num);
        regfree(&reg);

        regcomp(&reg,"^[ \t]*OFFSET[ \t]+([-0-9.]+)[ \t]*$",cflags);
        status=regexec(&reg,buf,2,mat,0);
        if(status!=0)
        {
            printf("\nCan not found Param zshift! Error file!\n");
            MMTApp::EchoResult(-1);
        }
        len =mat[1].rm_eo-mat[1].rm_so;
        num=(char*)malloc(len+1);
        memcpy(num,&buf[mat[1].rm_so],len);
        num[len]=0;
        offset=atof(num);
        free(num);
        regfree(&reg);


        regcomp(&reg,"^[ \t]*PITCH[ \t]+([-0-9.]+)[ \t]*$",cflags);
        status=regexec(&reg,buf,2,mat,0);
        if(status!=0)
        {
            printf("\nCan not found Param zshift! Error file!\n");
            MMTApp::EchoResult(-1);
        }
        len =mat[1].rm_eo-mat[1].rm_so;
        num=(char*)malloc(len+1);
        memcpy(num,&buf[mat[1].rm_so],len);
        num[len]=0;
        pitch=atof(num);
        free(num);
        regfree(&reg);


        free(buf);

    }

    fclose(f_geo);

    std::stringstream cmd;
    cmd<<"ProjectionErrorStraight ";
    cmd<<" "<<recmrc;
    cmd<<" "<<finmrc;
    cmd<<" "<<angle;
    cmd<<" "<<outputrepro;
    cmd<<" "<<outputerror;
    cmd<<" -t "<<thickness;
    cmd<<" -z "<<zshift;
    cmd<<" -p "<<offset;
    cmd<<" -f "<<pitch;
    writeLog(cmd.str().c_str());
    FILE* fpin;
    char line[BUF_MAX];

    RunCmd(cmd.str().c_str());

    int* caller=static_cast<int*>(msg->extra);
    MMTApp::EchoResult(*caller);
    delete caller;

}

void* profunc::OnPostProcess(msg_map::Message* msg)
{
    std::cout<<"Start PostProcess"<<std::endl;
    std::istringstream ss(msg->instruction);
    std::string input , output;
    int n;
    ss>>input>>output>>n;
    std::stringstream cmd;
    cmd<<"testnad"<<" -i "<<input<<" -o "<<output<<" -n "<<n;
    std::cout<<"cmd = "<<cmd.str().c_str()<<std::endl;

    writeLog(cmd.str().c_str());
    FILE* fpin;
    char line[BUF_MAX];

    RunCmd(cmd.str().c_str());

    MMTApp::EchoResult(1);

}

void* profunc::OnEvaluateNCC(msg_map::Message* msg)
{
    std::cout<<"Start EvaluateNCC"<<std::endl;
    std::istringstream ss(msg->instruction);
    std::string inputimg , inputrepro , outtxt;
    ss>>inputimg>>inputrepro>>outtxt;
    std::stringstream cmd;
    cmd<<"normcc"<<" -i "<<inputimg<<" -p "<<inputrepro<<" -o "<<outtxt;
    std::cout<<"cmd = "<<cmd.str().c_str()<<std::endl;

    writeLog(cmd.str().c_str());
    FILE* fpin;
    char line[BUF_MAX];
    
    RunCmd(cmd.str().c_str());

    MMTApp::EchoResult(1);
}


//+
void* profunc::OnNormalize(msg_map::Message* msg)
{
    std::cout<<"Start histogram normalize function"<<std::endl;

    std::istringstream ss(msg->instruction);
    std::string input , output;
    int ntype;
    float avg,std;
    ss>>input>>avg>>std>>ntype>>output;
    std::stringstream cmd;
    if(ntype == 2) { //ICONPreprocess
        cmd<<"ICONPreProcess"<<" -i "<<input<<" -o "<<output;

    } else {
        cmd<<"norm2d"<<" -i "<<input<<" -a "<<avg<<" -s "<<std<<" -t "<<ntype<<" -o "<<output;

    }
    std::cout<<"cmd = "<<cmd.str().c_str()<<std::endl;

    writeLog(cmd.str().c_str());
    FILE* fpin;
    char line[BUF_MAX];

    if(MMTApp::ExecuteCmd(cmd.str().c_str(), &fpin))
    {
        //printf("in file = %s",input);
        std::cout<<"in file="<<input<<"\n";
    }
    //std::cout<<"1\n";
    while(fgets(line, BUF_MAX, fpin) != NULL)
    {
        //std::cout<<"line:"<<line<<"\n";
        MMTApp::Echo(line , BUF_MAX);
    }
    //std::cout<<"2\n";
    if(MMTApp::CloseExecutePFile(fpin) != 0) {
        printf("ERROR:Childprocess returned a non-zero value.\n");
        return (void*)-1;
    }
    //std::cout<<"3\n";

    MMTApp::EchoResult(1);
    //std::cout<<"endprofunc\n";
}
/*
void* profunc::OnDETECT(msg_map::Message* msg)
{
    std::cout<<"Start OnMarkerAuto function"<<std::endl;

    std::string input , output;
    int diameter , markers;
    std::istringstream ss(msg->instruction);

    ss>>input>>diameter>>output;
    std::stringstream cmd;
    cmd<<"detect ";
    cmd<<" -i "<<input;
    cmd<<" -d "<<diameter;
    cmd<<" -o "<<output;

    RunCmd(cmd.str().c_str());

    MMTApp::EchoResult(1);

}

void* profunc::OnMatch(msg_map::Message* msg)
{
    std::cout<<"Start OnMatch function"<<std::endl;
    std::string input ,mrc , output_path , input_angle;
    std::istringstream ss(msg->instruction);
    ss>>input>>mrc>>output_path>>input_angle;
    std::stringstream cmd;
    cmd<<"match ";
    cmd<<" -i "<<input;
    cmd<<" -m "<<mrc;
    cmd<<" -p "<<output_path;
    cmd<<" -a "<<input_angle;

    std::cout<<cmd.str().c_str()<<std::endl;

    RunCmd(cmd.str().c_str());

    MMTApp::EchoResult(1);

}*/

void* profunc::OnAlign(msg_map::Message* msg)
{
    std::cout<<"Start OnAlign function"<<std::endl;
    std::string inputmrc,RawTlt,OutputTlt,OutputXf,Rotate,D,outputst,outputmrc,fastmode;
    int checktype;
    std::istringstream ss(msg->instruction);
    ss>>inputmrc>>RawTlt>>OutputTlt>>OutputXf>>Rotate>>D>>outputst>>outputmrc>>checktype>>fastmode;
    std::stringstream MarkerAuto;
    MarkerAuto<<"markerauto ";
    MarkerAuto<<" -i "<<inputmrc;
    MarkerAuto<<" -a "<<RawTlt;
    MarkerAuto<<" -n "<<OutputTlt;
    MarkerAuto<<" -o "<<OutputXf;
    MarkerAuto<<" -r "<<Rotate;
    MarkerAuto<<" -d "<<D;
    MarkerAuto<<" "<<fastmode;
    std:: cout<<MarkerAuto.str().c_str()<<std::endl;
    // -i BBa.st -a BBa.rawtlt -n BBa_new.tlt -o BBa_fin.xf -r -12.5  -d 8
    RunCmd(MarkerAuto.str().c_str());

    if(checktype)
    {
        //markererase -i $inputmrc  -o $head1"_mask.st" -f fids.txt -a addfids.txt
        std::stringstream MarkerErase;
        MarkerErase<<"markererase ";
        MarkerErase<<" -i "<<inputmrc;
        MarkerErase<<" -o "<<outputst;
        MarkerErase<<" -f fids.txt ";
        MarkerErase<<"-a addfids.txt ";
        RunCmd(MarkerErase.str().c_str());
    }

    //newstack -input $head1"_mask.st" -xf $head1"_fin.xf" -output $outputmrc
    std::stringstream Newstack;
    Newstack<<"mrcstack ";
    if(checktype)  {
          Newstack<<" -i "<<outputst;
        
    }
    else  {
        Newstack<<" -i "<<inputmrc;
        
    }
    Newstack<<" -x "<<OutputXf;
    Newstack<<" -o "<<outputmrc;
    RunCmd(Newstack.str().c_str());
    MMTApp::EchoResult(1);
}

void* profunc::onImodOpen(msg_map::Message* msg)
{
    istringstream ss(msg->instruction);
    string filename;
    ss>>filename;
    stringstream cmd;
    cmd<<"imod "<<filename;
  //  std::cout<<"###"<<cmd.str().c_str()<<std::endl;

    RunCmd(cmd.str().c_str());
}

void* profunc::onEvaluateTraj(msg_map::Message* msg)
{

    int* returnValue=new int( RunCmd(msg->instruction));
    //todo ????wrong return value
    return returnValue;

}

void* profunc::onCTFCorrection(msg_map::Message* msg){
    istringstream ss(msg->instruction);
    string shell;
    ss>>shell;
    string cmd="sh "+shell;
    return new int(RunCmd(cmd.c_str()));
    
}

void* profunc::OnGeoDeterm(msg_map::Message* msg){
    
    std::cout<<"Start OnGeoDeterm function"<<std::endl;
    std::istringstream cmd(msg->instruction);
    std::cout<<"cmd = "<<cmd.str().c_str()<<std::endl;
    int* returnValue=new int(RunCmd(cmd.str().c_str()));
    return returnValue;
}
#undef BUF_MAX