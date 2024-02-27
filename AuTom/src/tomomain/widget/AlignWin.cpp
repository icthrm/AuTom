#include "AlignWin.h"
#include "promanage/ProManage.h"
#include "env/env.h"
#include <cstring>
#include "core/msg_map.h"
#include "core/msg.h"
#include "custom/custom.h"
#include "ImageViewWindow/slicemain/slicemainwindow.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include"Ctag.h"
using namespace std;

AlignWin::AlignWin(QWidget* parent, Qt::WindowFlags f): QWidget(parent, f)
{
    inputSaver = InputSaver::getInstance();
  
    m_mainWidget = new QWidget(this);
    CreateImageFrame();
    CreateButton();
    QGridLayout *l = new QGridLayout();
    l->addWidget(m_imageFram , 0 , 0 ,4 ,0);
    l->addWidget(m_SetFram , 5 , 0);
    m_mainWidget->setLayout(l);
    //     delete l;
    m_mainWidget->setSizePolicy(QSizePolicy::Fixed , QSizePolicy::Fixed);
    QGridLayout *l0 = new QGridLayout();
    l0->addWidget(m_mainWidget);
    this->setLayout(l0);
    //     delete l0;

    QPalette palette;
    m_mainWidget->setAutoFillBackground(true);
    palette.setColor(QPalette::Background , QColor(200, 200, 200));
    m_mainWidget->setPalette(palette);

    palette.setColor(QPalette::Background , QColor(189, 189, 189));
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    SetInputImage();
    SetOutputImage();


    connect(m_Input , SIGNAL(clicked(bool)) , this , SLOT(ShowInput()));
    connect(m_Output , SIGNAL(clicked(bool)) , this , SLOT(ShowOutput()));
    connect(m_startButton1 , SIGNAL(clicked(bool)) , this , SLOT(StartMarkerAuto()));
}

AlignWin::~AlignWin(){
  inputSaver->alignment_checkbox_saved = m_checkbox->checkState();
  inputSaver->alignment_AutoLineEdit_saved = m_AutoLineEdit->text();
  inputSaver->alignment_fast_checkbox_saved = m_fast_checkbox->checkState();
  inputSaver->serialize();
}

void AlignWin::CreateImageFrame()
{
    m_imageFram = new QFrame(m_mainWidget);
    m_imageFram->setFrameStyle(QFrame::Panel|QFrame::Raised|QFrame::HLine);
    QGridLayout *l = new QGridLayout();
    m_Input = new CSkeleton(m_imageFram);
    m_Output = new CSkeleton(m_imageFram);
    l->addWidget(m_Input , 0 ,0);
    l->addWidget(m_Output , 0 ,1);
    m_imageFram->setLayout(l);
    m_Input->SetText(tr("Input") , NULL , 170 , 150 ,CSkeleton::FullFill);
    m_Output->SetText(tr("Output") , NULL , 170 , 150 ,CSkeleton::FullFill);
}

void AlignWin::CreateButton()
{
    m_ButtonFrame = new QFrame(m_mainWidget);
    m_ButtonFrame->setFrameStyle(QFrame::Panel|QFrame::Sunken);

    m_ButtonFrame_Alogrithm= new QFrame(m_ButtonFrame);
    m_buttonFrame_sub = new QFrame(m_ButtonFrame);
    m_buttonFrame_sub1 = new QFrame(m_ButtonFrame);

    m_startButton1 = new QPushButton("Start" , m_buttonFrame_sub);
    m_startButton1->setToolTip(ToolTips::ali_btn_start);
    QGridLayout* l0 = new QGridLayout();
    l0->setMargin(0);
    m_ButtonFrame_Alogrithm->setLayout(l0);

    //m__buttonFrame_sub1
    QGridLayout* l1 = new QGridLayout();
    m_AutoLabel=new QLabel(tr("  Fiducial Diameter (px): ") , m_ButtonFrame_Alogrithm);
    m_AutoLineEdit= new QLineEdit(inputSaver->alignment_AutoLineEdit_saved,m_ButtonFrame_Alogrithm);
    m_fast_checkbox=new QCheckBox("Fast mode");
    m_fast_checkbox->setCheckState(inputSaver->alignment_fast_checkbox_saved);
    m_fast_checkbox->setToolTip(ToolTips::ali_ckb_fast);
    //input validity xx.xx or -1
    QRegExp rx("^[1-9]\\d+(\\.\\d+)?$ |-1");//非负浮点数 或 -1
    m_AutoLineEdit->setValidator(new QRegExpValidator(rx, this));
    m_AutoLineEdit->setToolTip(ToolTips::ali_edt_r);
    m_checkbox = new QCheckBox("Erase the fiducial markers" , m_ButtonFrame);
    m_checkbox->setCheckState(inputSaver->alignment_checkbox_saved);
    m_checkbox->setToolTip(ToolTips::ali_lbl_erase);
    l1->addWidget(m_checkbox,0,0,1,6);
    l1->addWidget(m_AutoLabel,1,0,1,1);
    l1->addWidget(m_AutoLineEdit,1,1,1,3);
    l1->addWidget(m_fast_checkbox,1,4,1,2);
    l1->addWidget(m_startButton1,2,1,1,4);
    l1->setMargin(0);
    m_buttonFrame_sub1->setLayout(l1);

    m_button_ls = new QStackedLayout;

    m_button_ls->addWidget(m_buttonFrame_sub1);
    m_button_ls->setMargin(0);
    m_buttonFrame_sub->setLayout(m_button_ls);
    m_button_ls->setCurrentWidget(m_buttonFrame_sub1);

    QGridLayout* l = new QGridLayout();
    l->addWidget(m_ButtonFrame_Alogrithm , 0 , 0);
    l->addWidget(m_buttonFrame_sub , 1 , 0);
    //     l->setMargin(0);
    m_ButtonFrame->setLayout(l);

    m_progressbar = new CProgressBar(m_mainWidget);
    m_ls = new QStackedLayout;
    //     ls->setStackingMode(QStackedLayout::StackAll);
    m_ls->addWidget(m_ButtonFrame);
    m_ls->addWidget(m_progressbar);
    m_ls->setCurrentWidget(m_ButtonFrame);
    //     m_ls->setCurrentWidget(m_progressbar);
    m_SetFram = new QFrame(m_mainWidget);
    m_SetFram->setLayout(m_ls);

}


void AlignWin::SetInputImage()
{
    /*   if(CProManage::GetInstance()->Exite("PREALI_DIR")){
    m_inputImage = CProManage::GetInstance()->GetDir(PREALI_DIR).data();
    // 	m_Input->SetText(tr("CosAligned Image") , NULL , 170 , 150 ,CSkeleton::FullFill);
       }
       else*/
    string filename=CProManage::GetInstance()->GetDir(PROCESS_DIR)+"_norm.st";
    fstream pfile;
    pfile.open(filename.c_str(), ios_base::in);
    if(CProManage::GetInstance()->Exite("PRE_MRC_DIR") &&(pfile)) {
        m_inputImage = CProManage::GetInstance()->GetDir(PRE_MRC_DIR).data();
    }
    else {
        m_inputImage = CProManage::GetInstance()->GetDir(MRC_DIR).data();
        // 	m_Input->SetText(tr("Raw Image") , NULL , 170 , 150 ,CSkeleton::FullFill);
    }

    ItemModel item;
    ViewData view;
    if(item.Open(m_inputImage.toLatin1().data() , CProManage::GetInstance()->GetDir(ANGLE_DIR).c_str()))
    {
        view.Mirror(item.Img());

        m_Input->SetImage(view.MirrorImage() , 170 , 150);
    }
    else {
        m_Input->setEnabled(false);
        QMessageBox::warning(this , tr("Output Image") , tr("Can't open input file!")  ,QMessageBox::Ok);
    }
}

void AlignWin::SetOutputImage()
{
    cout<<"Set Output image"<<endl;
    m_outputImage = CProManage::GetInstance()->GetDir(ALI_MRC_DIR).data();
    qDebug()<<m_outputImage;
    if(m_outputImage != "NULL") {
        ItemModel item;
        ViewData view;
        if(item.Open(m_outputImage.toLatin1().data() , CProManage::GetInstance()->GetDir(ANGLE_DIR).c_str()))
        {
            view.Mirror(item.Img());
            m_Output->setEnabled(true);
            m_Output->SetImage(view.MirrorImage() , 170 , 150);
        }
        else {
            m_Output->setEnabled(false);
            QMessageBox::warning(this , tr("Set Output Image") , tr("Coarse align file not exist!")  ,QMessageBox::Ok);
        }
    }
    else {
        m_Output->setEnabled(false);
    }
    cout<<"end Set Output image"<<endl;
}

void AlignWin::ShowInput()
{
//    ImageViewWindow* win = ImageViewWindow::ViewMrc(m_inputImage);
      SliceMainWindow* win = SliceMainWindow::ViewMrc(m_inputImage);

}

void AlignWin::ShowOutput()
{
      SliceMainWindow* win = SliceMainWindow::ViewMrc(m_outputImage);
//    ImageViewWindow* win = ImageViewWindow::ViewMrc(m_outputImage);
}

void AlignWin::StartMarkerAuto()
{
    if(m_AutoLineEdit->text().isEmpty()) {
        m_AutoLineEdit->setText("-1");
        QMessageBox::warning(
            this ,
            tr("Warning") ,
            tr("The diameter value can be automatic detected, if the user set 'Fiducial Diameter (px):' to -1 or leave it blank.")  ,
            QMessageBox::Ok);
    }
    emit Start();
    string inputmrc,RawTlt,OutputTlt,OutputXf,Rotate,D,outputst,outputmrc;
    int checktype;
    if(m_checkbox->isChecked())  checktype =1 ;
    else checktype=0;
    //inputmrc=CProManage::GetInstance()->GetDir(MRC_DIR);
    inputmrc= m_inputImage.toStdString();
    RawTlt=CProManage::GetInstance()->GetDir(ANGLE_DIR);
    OutputTlt=CProManage::GetInstance()->GetDir(PROCESS_DIR)+"_new.tlt";
    OutputXf=CProManage::GetInstance()->GetDir(PROCESS_DIR)+"_ali.xf";
    Rotate="0";
    D=m_AutoLineEdit->text().toStdString();
    outputst=CProManage::GetInstance()->GetDir(PROCESS_DIR)+"_mask.st";
    outputmrc=CProManage::GetInstance()->GetDir(PROCESS_DIR)+"_fin.mrc";

    QString instruction = QString("\n%1\n%2\n%3\n%4\n%5\n%6\n%7\n%8\n%9\n%10").arg(inputmrc.data()).arg(RawTlt.data()).arg(OutputTlt.data()).arg(OutputXf.data()).arg(Rotate.data()).
                          arg(D.data()).arg(outputst.data()).arg(outputmrc.data()).arg(checktype).arg(m_fast_checkbox->isChecked()?" -t ":"");
//     printf("instruction %s\n",instruction.toStdString().data());
    m_ls->setCurrentWidget(m_progressbar);
    msg_map::Message* msg = MMTApp::NewMessage(ALIGN_MSG , this , instruction.toLatin1().data());
    MMTApp::PostMessage(msg);
}

void AlignWin::customEvent(QEvent* e)
{
    if(e->type() == msg_map::QSequelType) {
        msg_map::CSequelEvent* sequel = static_cast<msg_map::CSequelEvent*>(e);
        if(sequel->msg_id == ALIGN_MSG) {
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
                string fin_img = CProManage::GetInstance()->GetDir(PROCESS_DIR) + "_fin.mrc";

//                     OutputTlt=CProManage::GetInstance()->GetDir(PROCESS_DIR)+"_new.tlt";
//     OutputXf=CProManage::GetInstance()->GetDir(PROCESS_DIR)+"_ali.xf";
//     Rotate="0";
//     D=m_AutoLineEdit->text().toStdString();
//     outputst=CProManage::GetInstance()->GetDir(PROCESS_DIR)+"_mask.st";
//     outputmrc=CProManage::GetInstance()->GetDir(PROCESS_DIR)+"_fin.mrc";


                m_outputImage = QString("%1").arg(fin_img.data());
                if(sequel->data == (void*)-1) {
                    QFile file(m_outputImage);
                    if(file.exists()) {
                        file.remove();
                    }
                    CProManage::AddItem("ALIGN_STATE_BASED" , "FAIL");
                    emit RunError();
                }
                else if(sequel->data == (void*)1) {
                    QFileInfo file(m_outputImage);
                    if(file.exists()) {
                        CProManage::AddItem("ALI_MRC_DIR" , m_outputImage.toLatin1().data());
                        CProManage::AddItem("ALIGN_STATE_BASED" , "FINISH");
                        SetOutputImage();

                        string OutputTlt=CProManage::GetInstance()->GetDir(PROCESS_DIR)+"_new.tlt";
                        string OutputXf=CProManage::GetInstance()->GetDir(PROCESS_DIR)+"_ali.xf";
//     outputst=CProManage::GetInstance()->GetDir(PROCESS_DIR)+"_mask.st";
                        CProInstance * instance=CProManage::GetInstance();
                        instance->SetItem("NEWTLT_DIR",OutputTlt);
                        instance->SetItem("FIN_XF_DIR",OutputXf);
                        CProManage::ReWriteProFile();
                        emit Finish("AlignBased");
                    }
                    else {
                        QFile file(m_outputImage);
                        if(file.exists()) {
                            file.remove();
                        }
                        CProManage::AddItem("ALIGN_STATE_BASED" , "FAIL");
                        emit RunError();
                    }
                }
            }
            break;
            }
        }
    }

    QObject::customEvent(e);
}


void AlignWin::ViewFidMode(bool )
{
    string imagefile;
    string modefile;
    QFileInfo file0(m_inputImage);
    imagefile = m_inputImage.toStdString();
    modefile = "raptor/IMOD/"+file0.baseName().toStdString()+".fid.txt";

    QFile f1(imagefile.data());
    QFile f2(modefile.data());
    if(f1.exists()&&f2.exists()) {
        util::MrcStack* images = new util::MrcStack;
        images->Open(imagefile.c_str());
//        util::FiducialStack fidsk;
//        fidsk.ReadFidsByFile(modefile.c_str());

//        ImageViewWindow *win = new ImageViewWindow();
//        win->LoadModeImage(images , &fidsk);
//        win->show();
    }
    else {
        QMessageBox::warning(this , tr("Warning") , tr("The is no Fidical model!")  ,QMessageBox::Ok);
    }
}
#include "AlignWin.moc"
