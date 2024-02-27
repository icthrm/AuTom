#include "FineAlignWin.h"
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

FineAlignWin::FineAlignWin(QWidget* parent, Qt::WindowFlags f): QWidget(parent, f)
{
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

    m_alogrithm = Markless;

    connect(m_Input , SIGNAL(clicked(bool)) , this , SLOT(ShowInput()));
    connect(m_Output , SIGNAL(clicked(bool)) , this , SLOT(ShowOutput()));
    connect(m_startButton1 , SIGNAL(clicked(bool)) , this , SLOT(StartMarklessAlign()));

}

void FineAlignWin::CreateImageFrame()
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

void FineAlignWin::CreateButton()
{
    m_ButtonFrame = new QFrame(m_mainWidget);
    m_ButtonFrame->setFrameStyle(QFrame::Panel|QFrame::Sunken);

    m_ButtonFrame_Alogrithm= new QFrame(m_ButtonFrame);
    m_buttonFrame_sub = new QFrame(m_ButtonFrame);
    m_buttonFrame_sub1 = new QFrame(m_ButtonFrame);
    m_buttonFrame_sub2 = new QFrame(m_ButtonFrame);
    m_buttonFrame_sub3 = new QFrame(m_ButtonFrame);


    m_startButton1 = new QPushButton("Start" , m_buttonFrame_sub);
    m_startButton1->setToolTip(ToolTips::fali_btn_start);
    QGridLayout* l0 = new QGridLayout();
    l0->setMargin(0);
    m_ButtonFrame_Alogrithm->setLayout(l0);

    //m__buttonFrame_sub1
    QGridLayout* l1 = new QGridLayout();
    l1->addWidget(m_startButton1);
    l1->setMargin(0);
    m_buttonFrame_sub1->setLayout(l1);

    QGridLayout* l2 = new QGridLayout();
    l2->setMargin(0);
    m_buttonFrame_sub2->setLayout(l2);

    QGridLayout* l3 = new QGridLayout();
    l3->setMargin(0);
    m_buttonFrame_sub3->setLayout(l3);

    m_button_ls = new QStackedLayout;

    m_button_ls->addWidget(m_buttonFrame_sub1);
    m_button_ls->addWidget(m_buttonFrame_sub2);
    m_button_ls->addWidget(m_buttonFrame_sub3);
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

void FineAlignWin::SetButton(QString txt)
{
    if(txt == "RAPTOR")
        m_alogrithm = RAPTOR;
    if(txt == "Align without fiducial markers")
        m_alogrithm = Markless;
    if(txt == "Align with fiducial")
        m_alogrithm = MarkerAuto;
    switch(m_alogrithm) {
    case Markless:
        m_button_ls->setCurrentWidget(m_buttonFrame_sub1);
        break;
    case RAPTOR:
        m_button_ls->setCurrentWidget(m_buttonFrame_sub2);
        break;
    case MarkerAuto:
        m_button_ls->setCurrentWidget(m_buttonFrame_sub3);
        break;
    }
}

void FineAlignWin::SetInputImage()
{
    if(CProManage::GetInstance()->Exite("PREALI_DIR")) {
        m_inputImage = CProManage::GetInstance()->GetDir(PREALI_DIR).data();
// 	m_Input->SetText(tr("CosAligned Image") , NULL , 170 , 150 ,CSkeleton::FullFill);
    }
    else if(CProManage::GetInstance()->Exite("PRE_MRC_DIR")) {
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

void FineAlignWin::SetOutputImage()
{
    cout<<"Set Output image"<<endl;
    m_outputImage = CProManage::GetInstance()->GetDir(FIN_MRC_DIR).data();
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

void FineAlignWin::ShowInput()
{
//    ImageViewWindow* win = ImageViewWindow::ViewMrc(m_inputImage);
//    SliceMainWindow* win =SliceMainWindow::ViewMrc(m_inputImage);
    QString instruction = QString("\n%1").arg(m_inputImage);
    qDebug() << instruction << endl;
    msg_map::Message* msg = MMTApp::NewMessage(IMOD_MSG , this , instruction.toLatin1().data());
    MMTApp::PostMessage(msg);
}

void FineAlignWin::ShowOutput()
{
//     QString  m_outputImage = CProManage::GetInstance()->GetDir(FIN_MRC_DIR).data();
//    ImageViewWindow* win = ImageViewWindow::ViewMrc(m_outputImage);
 //    SliceMainWindow* win =SliceMainWindow::ViewMrc(m_outputImage);
    QString instruction = QString("\n%1").arg(m_outputImage);
    qDebug() << instruction << endl;
    msg_map::Message* msg = MMTApp::NewMessage(IMOD_MSG , this , instruction.toLatin1().data());
    MMTApp::PostMessage(msg);
}

void FineAlignWin::StartMarklessAlign()
{
    emit Start();
    m_ls->setCurrentWidget(m_progressbar);
    if(m_outputImage != "NULL") {
        QFile file(m_outputImage);
        if(file.exists()) {
            file.remove();
        }
        CProManage::GetInstance()->SetState(FINEALI_STATE , UNFINISH);
        CProManage::GetInstance()->SetDir(FIN_MRC_DIR , "NULL");
        CProManage::ReWriteProFile();
        SetOutputImage();
    }
    string inputimage = CProManage::GetInstance()->GetDir(MRC_DIR);
    string inputpreali = m_inputImage.toLatin1().data();
    string inputprexf = "2d_image.xf";
    struct stat info;
//    if(stat(inputprexf.c_str(), &info) == -1) {
//        cout<<inputprexf<<" does not exist!"<<endl;
//        emit Log("2d_image.xf does not exist!\n");
//        emit RunError();
//        m_ls->setCurrentWidget(m_ButtonFrame);
//        return;
//    }
    string inputangle = CProManage::GetInstance()->GetDir(ANGLE_DIR);
    string outputxf = CProManage::GetInstance()->GetDir(PROCESS_DIR) + "_fin.xf";
    string outputmrc = CProManage::GetInstance()->GetDir(PROCESS_DIR) + "_fin.mrc";
    string newangle = CProManage::GetInstance()->GetDir(PROCESS_DIR) + "_new.tlt";
    string outputxg =  CProManage::GetInstance()->GetDir(PROCESS_DIR) + "_fin.xg";

    m_outputImage = outputmrc.data();
    // CProManage::AddItem("FIN_MRC_DIR" , outputmrc);


    QString instruction = QString("\n%1\n%2\n%3\n%4\n%5\n%6\n%7\n%8").arg(inputpreali.data()).arg(inputangle.data())
                          .arg(outputxf.data()).arg(newangle.data()).arg(inputprexf.data()).arg(outputxg.data())
                          .arg(inputimage.data()).arg(outputmrc.data());
    qDebug()<<"instruction: "<<instruction;
    msg_map::Message* msg = MMTApp::NewMessage(FINE_ALIGN_MSG, this, instruction.toLatin1().data());
    MMTApp::PostMessage(msg);
}
// }



void FineAlignWin::customEvent(QEvent* e)
{
    if(e->type() == msg_map::QSequelType) {
        msg_map::CSequelEvent* sequel = static_cast<msg_map::CSequelEvent*>(e);
        if(sequel->msg_id == FINE_ALIGN_MSG) {
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
                    QFile file(m_outputImage);
                    if(file.exists()) {
                        file.remove();
                    }
                    CProManage::AddItem("FINEALI_STATE" , "FAIL");
                    emit RunError();
                }
                else if(sequel->data == (void*)1) {
                    QFileInfo file(m_outputImage);
                    if(file.exists()) {
                        CProManage::AddItem("FIN_MRC_DIR" , m_outputImage.toLatin1().data());
                        CProManage::AddItem("FINEALI_STATE" , "FINISH");

                        string outputxf = CProManage::GetInstance()->GetDir(PROCESS_DIR) + "_fin.xf";
                        string newangle = CProManage::GetInstance()->GetDir(PROCESS_DIR) + "_new.tlt";
                        string outputxg =  CProManage::GetInstance()->GetDir(PROCESS_DIR) + "_fin.xg";

                        CProInstance* instance=CProManage::GetInstance();
                        
                        instance->SetItem("FIN_XF_DIR",outputxf);
                        instance->SetItem("NEWTLT_DIR",newangle);
                        instance->SetItem("FIN_XG_DIR",outputxg);
                        CProManage::ReWriteProFile();
                        SetOutputImage();
                        emit Finish("Fine Align");
                    }
                    else {
                        QFile file(m_outputImage);
                        if(file.exists()) {
                            file.remove();
                        }
                        CProManage::AddItem("FINEALI_STATE" , "FAIL");
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

void FineAlignWin::ViewFidMode(bool )
{
    string imagefile;
    string modefile;
    QFileInfo file0(m_inputImage);
    imagefile = m_inputImage.toStdString();
    modefile = "raptor/IMOD/"+file0.baseName().toStdString()+".fid.txt";

    QFile f1(imagefile.data());
    QFile f2(modefile.data());
    if(f1.exists()&&f2.exists()) {

//        util::MrcStack* images = new util::MrcStack;
//        images->Open(imagefile.c_str());
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
#include "FineAlignWin.moc"
