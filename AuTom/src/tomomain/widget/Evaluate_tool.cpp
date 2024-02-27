#include "Evaluate_tool.h"
#include "promanage/ProManage.h"
#include "env/env.h"
#include <cstring>
#include "core/msg_map.h"
#include "core/msg.h"
#include "custom/custom.h"
#include "ImageViewWindow/slicemain/slicemainwindow.h"
#include "mrcimg/GraphUtils.h"
#include "vector"
#include "EvaluateWin.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <fstream>
#include <string>
#include <qmessagebox.h>
#include<Qt>
#define RETURN_ON_FILE_NOT_EXIST(filename) {\
    QFile file(filename); \
    if(!file.exists()){\
    QMessageBox::warning(this,tr("warning"),tr( string("missing file:").append(#filename).c_str()),QMessageBox::Ok);\
    emit end();\
    return ;    \
    }\
}

#define RETURN_ON_DIR_NOT_EXIST(dirname ) {\
    QDir dir(dirname); \
    if(!dir.exists()){\
    QMessageBox::warning(this,tr("warning"),tr( string("missing dir :").append(#dirname).c_str()),QMessageBox::Ok);\
    emit end();\
    return ;    \
    }\
}


using namespace std;

Evaluate_tool::Evaluate_tool(QWidget* parent, Qt::WindowFlags f): QWidget(parent, f)
{
    m_frame = new QFrame(this);
    m_frame->setFrameStyle(QFrame::StyledPanel|QFrame::Plain);
    m_tool_lable = new QLabel(tr("   Evaluation of results   ") , this);
    QPalette pal ;
    pal.setColor(QPalette::Background, QColor(0x00,0xff,0x00,0x00));
    m_tool_lable->setPalette(pal);
    QFont ft;
//   ft.setPointSize(18);
    ft.setBold(true);
    ft.setUnderline(true);
    m_tool_lable->setFont(ft);

    m_lable =new QLabel(tr("method") , this);
    m_methed_combobox =new QComboBox(this);
    m_methed_combobox->insertItem(0 , "Projection Error");
    m_methed_combobox->insertItem(1 , "NCC");
    //added by liuce !!  
     m_methed_combobox->insertItem(2,"Trajplot");  
  
    
    m_start_button = new QPushButton("evaluate" ,this);
    m_ButtonFrame = new QFrame(this);

    QGridLayout *l = new QGridLayout();
    l->addWidget(m_lable , 0 ,0);
    l->addWidget(m_methed_combobox , 0 ,1);
    l->addWidget(m_start_button , 1 ,1);
    l->setMargin(0);
    m_ButtonFrame->setLayout(l);

    m_progressbar = new CProgressBar(this);
    m_progressbar->setWidth(180);
    m_ls = new QStackedLayout();
    m_ls->addWidget(m_ButtonFrame);
    m_ls->addWidget(m_progressbar);
    m_ls->setCurrentWidget(m_ButtonFrame);

    m_SetFram = new QFrame(this);
    m_SetFram->setLayout(m_ls);

    QGridLayout *l1 = new QGridLayout();
    l1->addWidget(m_frame);
    l1->setMargin(0);
    this->setLayout(l1);

    QGridLayout *l0 = new QGridLayout();
    l0->addWidget(m_tool_lable , 0 , 0 , 1 ,2);
    l0->addWidget(m_SetFram , 1 , 0);
//   l0->addWidget(m_lable , 1 , 0);
//   l0->addWidget(m_methed_combobox , 1 , 1);
//   l0->addWidget(m_start_button , 2 , 1);
    m_frame->setLayout(l0);
    this->setFixedHeight(110);

    connect(m_start_button , SIGNAL(clicked(bool)) , this , SLOT(StartEvaluate()));
}

Evaluate_tool::~Evaluate_tool()
{

}

CTrajPlotWin::CTrajPlotWin() {

    this->setAttribute(Qt::WA_DeleteOnClose);

    m_instruction=new QLabel(tr("choose the plane"),this);
    m_okBtn=new QPushButton(tr("start"),this);
    m_cancelBtn=new QPushButton(tr("cancel"),this);
    //m_killBtn=new QPushButton(tr("kill"),this);
    m_modeBox=new QComboBox(this);
    m_rawOrAligned_label=new QLabel(tr("raw or aligned"),this);
    m_rawOrAligned_combobox=new QComboBox(this);
    m_progressbar=new CProgressBar(this);
    m_stack=new QStackedLayout(this);
    
    QGridLayout* layout0=new QGridLayout();
    layout0->addWidget(m_instruction,0,0);
    layout0->addWidget(m_modeBox,0,1);
    layout0->addWidget(m_rawOrAligned_label,1,0);
    layout0->addWidget(m_rawOrAligned_combobox,1,1);
    layout0->addWidget(m_cancelBtn,2,0);
    layout0->addWidget(m_okBtn,2,1);
    frame0=new QFrame(this);
    frame0->setLayout(layout0);

    QGridLayout* layout1=new QGridLayout();
    layout1->addWidget(m_progressbar,0,0,2,3);
    //layout1->addWidget(m_killBtn,1,2);
    frame1=new QFrame(this);
    frame1->setLayout(layout1);

    m_stack->addWidget(frame0);
    m_stack->addWidget(frame1);
    m_stack->setCurrentWidget(frame0);


    m_modeBox->insertItem(0,"xy");
    m_modeBox->insertItem(1,"yz");
    m_modeBox->insertItem(2,"xz");
    
    m_rawOrAligned_combobox->insertItem(0,"raw");
    m_rawOrAligned_combobox->insertItem(1,"aligned");
    
    connect(m_okBtn,SIGNAL(clicked(bool)),this,SLOT(startProcess()));
    connect(m_cancelBtn,SIGNAL(clicked(bool)),this,SLOT(close()));


}

CTrajPlotWin::~CTrajPlotWin() {



}

void Evaluate_tool::StartEvaluate()
{

    m_method = m_methed_combobox->currentIndex();
    switch(m_method) {
    case 0:
        StartProjectionEvaluate(1);
        break;
    case 1:
        StartNCCEvaluate();
        break;
    case 2:
        m_trajplotWin=new CTrajPlotWin();
        m_trajplotWin->setWindowModality(Qt::ApplicationModal);
        m_trajplotWin->setWindowFlags(Qt::WindowStaysOnTopHint);
        m_trajplotWin->show();
        m_trajplotWin->setAttribute(Qt::WA_DeleteOnClose);
        //StartTrajplotEvaluate();
        connect(m_trajplotWin,SIGNAL(start(QString,int)),this,SLOT(StartTrajplotEvaluate(QString,int)),Qt::UniqueConnection);

        break;
    }
}

void Evaluate_tool::ShowResult()
{
    EvaluateWin* e = new EvaluateWin();
    e->setAttribute(Qt::WA_DeleteOnClose);
    QDesktopWidget *pDesk = QApplication::desktop();
    e->move((pDesk->width() - e->width()) / 2, (pDesk->height() - e->height()) / 2);
    e->show();
}

void Evaluate_tool::ShowNCCResult()
{
    qDebug()<<"show ncc result";
    vector <float> ncc;
    ifstream in("ncc_out.txt");
    float f;
    while(in.good()&&!in.eof()) {
        in>>f;
        cout<<f<<endl;
        ncc.push_back(f);
    }
    in.close();
    //showFloatGraph("NCC result" ,&ncc[0] , ncc.size());
    showFloatGraphWithXYLabel("NCC result" ,ncc.data() , ncc.size(), "No. of projection", "NCC value", 0);
}

/*
    The parameter is used to specify the caller.
    It has two possible value:1 and 2.Value 1 means that its caller is StartEvaluate().Value 2 is StartNCCEvaluate(), which means user hasn't finish ProjectionError before starting NCCEvaluate.So the caller StartNCCEvaluate() must do it for user before the actual ncc evaluate.


 */
void Evaluate_tool::StartProjectionEvaluate(int caller)
{
    qDebug()<<"StartProjectionEvaluate";

    string recmrc , finmrc , angle , outputrepro ,outputerror , paramfile;
    //判断文件是否都存在
    recmrc = CProManage::GetInstance()->GetDir(RES_MRC_DIR).data();
    if(recmrc == "NULL") {
        QMessageBox::warning(this , tr("Warning") , tr("you haven't complate reconstruction!")  ,QMessageBox::Ok);
        emit end();
        return;
    }
    else {
        QFile file(recmrc.data());
        if(!file.exists()) {
            QMessageBox::warning(this , tr("Warning") , tr("reconstruction mrc file not exist!")  ,QMessageBox::Ok);
            emit end();
            return;
        }
    }

    if(CProManage::GetInstance()->Exite("FIN_MRC_DIR")) {
        finmrc = CProManage::GetInstance()->GetDir(FIN_MRC_DIR);
        angle = CProManage::GetInstance()->GetDir(PROCESS_DIR) + "_new.tlt";
    }
    else if(CProManage::GetInstance()->Exite("ALI_MRC_DIR")) {
        finmrc = CProManage::GetInstance()->GetDir(ALI_MRC_DIR);
        angle = CProManage::GetInstance()->GetDir(ANGLE_DIR);
    }
    else { // if(CProManage::GetInstance()->Exite("MRC_DIR"))
        finmrc = CProManage::GetInstance()->GetDir(MRC_DIR);
        angle = CProManage::GetInstance()->GetDir(ANGLE_DIR);
    }

    QFile file(finmrc.data());
    if(!file.exists()) {
        QMessageBox::warning(this , tr("Warning") , tr("input mrc file not exist!")  ,QMessageBox::Ok);
        emit end();
        return;
    }
    QFile file1(angle.data());
    if(!file1.exists()) {
        QMessageBox::warning(this , tr("Warning") , tr("input angle file not exist!")  ,QMessageBox::Ok);
        emit end();
        return;
    }

    paramfile = "geometry.par";
    QFile file2(paramfile.data());
    if(!file2.exists()) {
        QMessageBox::warning(this , tr("Warning") , tr("geometry file not exist!")  ,QMessageBox::Ok);
        emit end();
        return;
    }

    QFile file3("output_reprojection.mrc");
    if(file3.exists()) {
        qDebug()<<"remove";
        file3.remove();
    }
    QFile file4("output_projectionError.mrc");
    if(file4.exists()) {
        qDebug()<<"remove";
        file4.remove();
    }
    outputrepro = "output_reprojection.mrc";
    outputerror = "output_projectionError.mrc";

    m_ls->setCurrentWidget(m_progressbar);
    

    QString instruction = QString("\n%1\n%2\n%3\n%4\n%5\n%6").arg(recmrc.data()).arg(finmrc.data()).arg(angle.data())
                          .arg(outputrepro.data()).arg(outputerror.data()).arg(paramfile.data());
    qDebug()<<"instruction : "<<instruction;
    int * extra =new int(caller);

    msg_map::Message* msg = MMTApp::NewMessage(EVALUATE_MSG , this , instruction.toLatin1().data(),extra);
    MMTApp::PostMessage(msg);
    emit end();
}

void Evaluate_tool::StartNCCEvaluate()
{
    qDebug()<<"StartNCC";
    string recmrc, inputimg , inputrepro , outtxt;
    recmrc = CProManage::GetInstance()->GetDir(RES_MRC_DIR).data();
    if(recmrc == "NULL") {
        QMessageBox::warning(this , tr("Warning") , tr("you haven't complate reconstruction!")  ,QMessageBox::Ok);
        emit end();
        return;
    }

    inputrepro = "output_reprojection.mrc";
    QFile f(inputrepro.data());
    if(!f.exists()) {
        StartProjectionEvaluate(2);
    }

    if(!f.exists()) {
        emit end();
        return;
    }
    if(CProManage::GetInstance()->Exite("FIN_MRC_DIR")) {
        inputimg = CProManage::GetInstance()->GetDir(FIN_MRC_DIR);
    }
    else if(CProManage::GetInstance()->Exite("ALI_MRC_DIR")) {
        inputimg = CProManage::GetInstance()->GetDir(ALI_MRC_DIR);
    }
    else {//if(CProManage::GetInstance()->Exite("MRC_DIR"))
        inputimg = CProManage::GetInstance()->GetDir(MRC_DIR);
    }
    QFile file(inputimg.data());
    if(!file.exists()) {
        QMessageBox::warning(this , tr("Warning") , tr("input mrc file not exist!")  ,QMessageBox::Ok);
        emit end();
        return;
    }

    outtxt = "ncc_out.txt";
    m_ls->setCurrentWidget(m_progressbar);

    QString instruction = QString("\n%1\n%2\n%3").arg(inputimg.data()).arg(inputrepro.data()).arg(outtxt.data());
    msg_map::Message* msg = MMTApp::NewMessage(EVALUATE_NCC_MSG , this , instruction.toLatin1().data());
    MMTApp::PostMessage(msg);
    emit end();
}
//added by liuce
void Evaluate_tool::StartTrajplotEvaluate(QString message,int rawOrAligned)
{
    
    connect(m_trajplotWin->m_progressbar, SIGNAL(Kill_Signal()) , this , SLOT(kill()));
    qDebug()<<"start Trajplot\n";
    //this tool is not avaliable before alignment
     if(CProManage::GetInstance()->GetMethod()!="BasedMethod") {
       QMessageBox::warning(this,tr("Warning"),tr("Trajplot is only avaliable for markerbased workflow!"),QMessageBox::Ok);
        emit end();
        return ;
     }
    if(!CProManage::GetInstance()->Finished(ALIGN_STATE_BASED)) {
        QMessageBox::warning(this,tr("Warning"),tr("alignment not finished!"),QMessageBox::Ok);
        emit end();
	//m_trajplotWin->m_stack->setCurrentWidget(m_trajplotWin->frame0);
        return ;
    }

    m_trajplotWin->m_stack->setCurrentIndex(1);
    CProInstance* instance=CProManage::GetInstance();
    instance->Print();

    string mrcFileName=instance->GetDir(MRC_DIR);
    string tltFileName=instance->GetDir(NEWTLT_DIR);
    string xfFileName=instance->GetDir(FIN_XF_DIR);
    string matchFolderName="./matches";


    RETURN_ON_FILE_NOT_EXIST(mrcFileName.c_str());//automatically convert const char* into QString
    RETURN_ON_FILE_NOT_EXIST(tltFileName.c_str());
    RETURN_ON_FILE_NOT_EXIST(xfFileName.c_str());
    RETURN_ON_DIR_NOT_EXIST(matchFolderName.c_str());

    string mode=string(message.toLatin1().constData());
    string displayedFile="";
    if(rawOrAligned==0){//raw
        displayedFile="init.fid.txt";
    }else{
        displayedFile="fin.fid.txt";
    }
    QString instrution=QString(" traj -i ").append(mrcFileName.c_str()).append(" -a ").append(tltFileName.c_str()).append(" --xf ").append(xfFileName.c_str()).append("; trajplot -i  ").append(displayedFile.c_str()).append(" -m ").append(mode.c_str());
    msg_map::Message* msg=MMTApp::NewMessage(EVALUATE_TRAJ_MSG,this,instrution.toLatin1().data());
    MMTApp::PostMessage(msg);
    emit end();
}

void Evaluate_tool::kill()
{
     m_trajplotWin->m_stack->setCurrentIndex(0);
}


void CTrajPlotWin::startProcess()
{
    //m_stack->setCurrentIndex(1);
    
    emit(start(m_modeBox->currentText(),m_rawOrAligned_combobox->currentIndex()));
}

void Evaluate_tool::customEvent(QEvent* e)
{
    if(e->type() == msg_map::QSequelType) {
        msg_map::CSequelEvent* sequel=0;
        if( 0==(sequel = dynamic_cast<msg_map::CSequelEvent*>(e))) {
            return;

        }
        if(sequel->msg_id == EVALUATE_MSG) {
            switch(sequel->state) {
            case msg_map::MESSAGE: {
                QString message = (char *)sequel->data;
                emit Log(message);
                delete [] (char*)sequel->data;
            }
            break;
            case msg_map::RATE: {
                int rate = (unsigned long)sequel->data;
                //?????
            }
            break;
            case msg_map::RESULT: {
                m_ls->setCurrentWidget(m_ButtonFrame);

                if(sequel->data == (void*)-1) {
                    QMessageBox::warning(this , tr("Warning") , tr("Evaluate Error!")  ,QMessageBox::Ok);
                }
                else if(sequel->data == (void*)1) {
                    //the caller is StartEvaluate();
                    QFileInfo file("output_reprojection.mrc");
                    QFile file1("output_projectionError.mrc");
                    if(file.exists()&&file1.exists()) {
                        ShowResult();
                    }
                    else {
                        QMessageBox::warning(this , tr("Warning") , tr("Evaluate Error!")  ,QMessageBox::Ok);
                    }
                } else if(sequel->data == (void*)2) {
                    //the caller is StartNCCEvaluate()
                    StartNCCEvaluate();

                }
                //  else{
                //    QFile file("output_reprojection.mrc");
                //    if(file.exists()){
                //      qDebug()<<"remove";
                //      file.remove();
                //    }
                //    QFile file1("output_projectionError.mrc");
                //    if(file1.exists()){
                //     qDebug()<<"remove";
                //      file.remove();
                //    }
                //    QMessageBox::warning(this , tr("Warning") , tr("Evaluate Error!")  ,QMessageBox::Ok);
                // }
            }
            break;
            }
        }
        else if(sequel->msg_id == EVALUATE_NCC_MSG) {
            switch(sequel->state) {
            case msg_map::MESSAGE: {
                QString message = (char *)sequel->data;
                emit Log(message);
                delete [] (char*)sequel->data;
            }
            break;
            case msg_map::RATE: {
                int rate = (unsigned long)sequel->data;
            }
            break;
            case msg_map::RESULT: {
                m_ls->setCurrentWidget(m_ButtonFrame);
                int state = (unsigned long)sequel->data;
                if(sequel->data == (void*)-1) {
                    QMessageBox::warning(this , tr("Warning") , tr("Evaluate Error!")  ,QMessageBox::Ok);
                }
                else if(sequel->data == (void*)1) {
                    QFileInfo file("ncc_out.txt");
                    if(file.exists()) {
                        ShowNCCResult();
                    }
                    else {
                        qDebug()<<"else";
                        QMessageBox::warning(this , tr("Warning") , tr("Evaluate Error!")  ,QMessageBox::Ok);
                    }
                }
// 	  else{
// 	    qDebug()<<"else1";
// 	    QMessageBox::warning(this , tr("Warning") , tr("Evaluate Error!")  ,QMessageBox::Ok);
// 	  }
            }
            break;
            }
        }
        else if(sequel->msg_id == EVALUATE_TRAJ_MSG) {
            switch(sequel->state) {
            case msg_map::MESSAGE: {
                QString message = (char *)sequel->data;
                emit Log(message);
                delete [] (char*)sequel->data;
                break;
            }
            case msg_map::RESULT: {

                void* data=sequel->data;
                if(data==PTHREAD_CANCELED) {
                    m_trajplotWin->m_stack->setCurrentIndex(0);
                    

                }
                else if(*(int*)data == 0) {
                    string mode = m_trajplotWin->m_modeBox->currentText().toStdString();

                    QString pngName=QString("traj%1.png").arg(mode.c_str());
                    
                    
                    delete m_trajplotWin;
                    m_trajplotWin=0;

                    //system(QString("eog ").append(pngName).toLatin1().constData());
                    QWidget* imageWidget=new QWidget();
                    QLabel* imageLabel=new QLabel(imageWidget);
                    QImage* image=new QImage();
                    image->load(pngName);
                    if(image->isNull()) {
                        QMessageBox::information(this,tr("fail"),tr("fail to open image"));
                        delete imageWidget;
                        delete image;
                    }
                    int height=image->height();
                    int width=image->width();

                    imageLabel->setPixmap(QPixmap::fromImage(*image));
                    imageLabel->setScaledContents(true);
                    imageLabel->setFixedSize(width,height);
                    imageWidget->setAttribute(Qt::WA_DeleteOnClose);
                    imageWidget->show();

                    delete (int*)data;

                }else{
                    QMessageBox::information(this,tr("fail"),tr("fail to run traj "));
                    delete (int*)data;
                    
                }
                break;
            }

            }
        }
    }
    QObject::customEvent(e);
}



#include "Evaluate_tool.moc"
