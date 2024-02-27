#include "RestructWin.h"
#include "promanage/ProManage.h"
#include "env/env.h"
#include <cstring>
#include "core/msg_map.h"
#include "core/msg.h"
#include "custom/custom.h"
#include "ImageViewWindow/slicemain/slicemainwindow.h"
#include "ImageViewWindow/volumemain/volumemainwindow.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <pthread.h>
#include"CTFSettingsDialog.h"
#include"Ctag.h"
#include <QSpacerItem>
#include <QIntValidator>

using namespace std;

RestructWin::RestructWin(QWidget* parent, Qt::WindowFlags f): QWidget(parent, f)
{
  
    inputSaver = InputSaver::getInstance();
  
    m_mainWidget = new QWidget(this);
    CreateCTFCorrectionFrame();
    CreateImageFrame();
    CreateButton();

    QGridLayout *l = new QGridLayout();
    l->addWidget(m_ctf_frame , 0, 0);
    l->addWidget(m_imageFram , 1 , 0 );
    l->addWidget(m_SetFram , 2 , 0);
//     l->addWidget(m_SelectFrame);
    m_mainWidget->setLayout(l);
    m_mainWidget->resize(250 , 50);

    m_mainWidget->setSizePolicy(QSizePolicy::Expanding , QSizePolicy::Fixed);

    QGridLayout *l0 = new QGridLayout();
    l0->addWidget(m_mainWidget);
    this->setLayout(l0);

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
    connect(m_start , SIGNAL(clicked(bool)) , this , SLOT(StartforRestruct()));
}

RestructWin::~RestructWin(){
  inputSaver->reconsruction_startModel_Combobox_saved = m_startModel_Combobox->currentIndex();
  inputSaver->reconsruction_version_Combobox_saved = m_version_Combobox->currentIndex();
  inputSaver->reconsruction_alogrithm_Combobox_saved = m_alogrithm_Combobox->currentIndex();
  inputSaver->reconsruction_threadNum_LineEdit_saved = m_threadNum_LineEdit->text();
  inputSaver->reconsruction_ParameterLineEdit_saved = m_ParameterLineEdit->text();
  inputSaver->reconsruction_IterativenumbersLineEdit_saved = m_IterativenumbersLineEdit->text();
  inputSaver->reconsruction_Method_ComboBox_saved = m_Method_ComboBox->currentIndex();
  inputSaver->reconsruction_SliceLineEdit1_saved = m_SliceLineEdit1->text();
  inputSaver->reconsruction_SliceLineEdit2_saved = m_SliceLineEdit2->text();
  inputSaver->reconsruction_ICONIterLineEdit1_saved = m_ICONIterLineEdit1->text();
  inputSaver->reconsruction_ICONIterLineEdit2_saved = m_ICONIterLineEdit2->text();
  inputSaver->reconsruction_ICONIterLineEdit3_saved = m_ICONIterLineEdit3->text();
  inputSaver->reconsruction_ThresholdLineEdit_saved = m_ThresholdLineEdit->text();
  inputSaver->reconsruction_radiusLineEdit_saved = m_radiusLineEdit->text();
  inputSaver->reconsruction_dataTypeBox_saved = m_dataTypeBox->currentIndex();
  inputSaver->reconsruction_bin_lineEdit_saved = m_bin_lineEdit->text();
  inputSaver->reconsruction_Mrcfile_Lineedit_saved = m_Mrcfile_Lineedit->text();
  inputSaver->serialize();
}

void RestructWin::CreateCTFCorrectionFrame()
{
    m_ctf_frame = new QFrame(m_mainWidget);
    m_ctf_frame->setFrameStyle(QFrame::Panel|QFrame::Raised);
    QGridLayout* l = new QGridLayout();
    m_ctf_lable = new QLabel("CTF Correction");
    m_ctf_enable_checkbox = new QCheckBox("Enable CTF Correction");
    m_ctf_settings_btn = new QPushButton("Start CTF determination and correction");
    m_ctf_settings_btn->setEnabled(false);
    QSpacerItem* spaceitem=new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Expanding);	
//     QHBoxLayout* hl=new QHBoxLayout();
//     hl->addwi
    l->addWidget(m_ctf_lable,0,0);
    l->addWidget(m_ctf_enable_checkbox,1,0);
    l->addWidget(m_ctf_settings_btn,1,1);
    l->addItem(spaceitem,1,2,1,3);
    m_ctf_frame -> setLayout(l);
    m_ctf_dialog = new CTFSettingsDialog();
    connect(m_ctf_settings_btn, SIGNAL(clicked(bool)),this ,SLOT(ShowCTFCorrectionSettings()));
    connect(m_ctf_enable_checkbox,SIGNAL(stateChanged(int)),this,SLOT(EnableCTFCorrection()));
}


void RestructWin::CreateImageFrame()
{
    m_imageFram = new QFrame(m_mainWidget);
    m_imageFram->setFrameStyle(QFrame::Panel|QFrame::Raised|QFrame::HLine);
    QGridLayout *l = new QGridLayout();
    m_Input = new CSkeleton(m_imageFram);
    m_Output = new CSkeleton(m_imageFram);
    l->addWidget(m_Input , 0 ,0);
    l->addWidget(m_Output , 0 ,1);
    l->setMargin(0);
    m_imageFram->setLayout(l);
    m_Input->SetText(tr("Input") , NULL , 170 , 150 ,CSkeleton::FullFill);
    m_Output->SetText(tr("Output") , NULL , 170 , 150 ,CSkeleton::FullFill);
}


void RestructWin::CreateButton()
{
    m_ButtonFrame = new QFrame(m_mainWidget);
    m_ButtonFrame->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    m_start = new QPushButton(tr("Start") , m_ButtonFrame);
    m_start->setToolTip(ToolTips::rec_btn_start);
    m_threadNum_Lable= new QLabel(tr("Thread Num"),m_ButtonFrame);
    m_threadNum_Lable->setToolTip(ToolTips::rec_lbl_thnum);
    m_threadNum_LineEdit =new QLineEdit(m_ButtonFrame);
    
    //const int lCpuNum = SysInfo::getLCpuNum();
    QIntValidator* threadnumValidator =new QIntValidator(1,24);
    m_threadNum_LineEdit->setValidator(threadnumValidator);
    
    //m_threadNum_LineEdit->setText(QString::number(lCpuNum));

    QGridLayout* l = new QGridLayout();

    m_startModel_Label = new QLabel(tr("Start Model") , m_ButtonFrame);
    m_startModel_Label->setToolTip(ToolTips::rec_lbl_model);
    m_startModel_Combobox = new QComboBox(m_ButtonFrame);
    m_startModel_Combobox->insertItem(0 ,tr("Single Axis"));
    //m_startModel_Combobox->insertItem(1 ,tr("Multiple Axes"));
    m_startModel_Combobox->setItemData(1,   0, Qt::UserRole - 1);
    m_startModel_Combobox->setCurrentIndex(inputSaver->reconsruction_startModel_Combobox_saved);
    m_startModel_Combobox->setMinimumWidth(120);


    m_version_Label = new QLabel(tr("Version") , m_ButtonFrame);
    m_version_Label->setMaximumWidth(50);
    m_version_Label->setToolTip(ToolTips::rec_lbl_version);
    m_version_Combobox = new QComboBox(m_ButtonFrame);
    m_version_Combobox->setMinimumWidth(60);
    m_version_Combobox->setObjectName("version");
    m_version_Combobox->insertItem(0 , "MPI");
    m_version_Combobox->insertItem(1 , "CUDA");
    m_version_Combobox->setCurrentIndex(inputSaver->reconsruction_version_Combobox_saved);

    m_alogrithm_Label = new QLabel(tr("Algorithm") , m_ButtonFrame);
    m_alogrithm_Label->setToolTip(ToolTips::rec_lbl_algo);
    m_alogrithm_Combobox = new QComboBox(m_ButtonFrame);
    m_alogrithm_Combobox->setMinimumWidth(80);
    m_alogrithm_Combobox->insertItem(0 , tr("BPT"));
    m_alogrithm_Combobox->insertItem(1 , tr("FBP"));
    m_alogrithm_Combobox->insertItem(2 , tr("SART"));
    //m_alogrithm_Combobox->insertItem(3 , tr("ASART"));
    m_alogrithm_Combobox->insertItem(4,  tr("SIRT"));
    //m_alogrithm_Combobox->insertItem(5,  tr("ASIRT"));
    m_alogrithm_Combobox->insertItem(6,  tr("ICON"));
    m_alogrithm_Combobox->setCurrentIndex(inputSaver->reconsruction_alogrithm_Combobox_saved);


    m_ParameterLabel = new QLabel(tr("Parameter : "),m_ButtonFrame);
    m_ParameterLabel->setToolTip(ToolTips::rec_lbl_para);
    m_ParameterLineEdit = new QLineEdit(inputSaver->reconsruction_ParameterLineEdit_saved, m_ButtonFrame);
    QRegExp rx4("^(-?\\d+)(\\.\\d+)?$");//æµ®ç¹æ?
    m_ParameterLineEdit ->setValidator(new QRegExpValidator(rx4, this));
    m_ParameterLineEdit ->setMaximumWidth(100);
    m_ParameterLineEdit->setMinimumWidth(80);

    m_IterativenumbersLabel = new QLabel(tr(" Iterative Num : "),m_ButtonFrame);
    m_IterativenumbersLabel->setToolTip(ToolTips::rec_lbl_iter);
    // m_IterativenumbersLabel->setWordWrap(true); m_IterativenumbersLabel->setAlignment(Qt::AlignTop);

    m_IterativenumbersLineEdit = new QLineEdit(inputSaver->reconsruction_IterativenumbersLineEdit_saved, m_ButtonFrame);
    QRegExp rx5("^\\d+$");//éèŽæŽæ°
    m_IterativenumbersLineEdit ->setValidator(new QRegExpValidator(rx5, this));
    m_IterativenumbersLineEdit->setMaximumWidth(100);
    m_IterativenumbersLineEdit->setMinimumWidth(80);

    m_Mrcfile_Label = new QLabel(tr("Mrc File"),m_ButtonFrame);
    m_Mrcfile_Label->setToolTip(ToolTips::rec_lbl_file);
    m_Mrcfile_Action = new QAction(QIcon(":/images/files.png"),tr("files"),this);
    m_Mrcfile_Lineedit = new QLineEdit(m_ButtonFrame);
    m_Mrcfile_Button = new QToolButton(m_ButtonFrame);
    m_Mrcfile_Button->setDefaultAction(m_Mrcfile_Action);
    m_Mrcfile_Button->setIconSize(QSize(20 , 20 ));
    QSpacerItem *space= new QSpacerItem(100,0, QSizePolicy::Minimum, QSizePolicy::Fixed);
//     l->addItem(space,0,0,1,2);
    l->addWidget(m_startModel_Label,0,0);
    l->addWidget(m_startModel_Combobox,0,1,1,2);
    l->addWidget(m_version_Label,0,3);
    l->addWidget(m_version_Combobox,0,4,1,2);
    l->addWidget(m_alogrithm_Label,0,6);
    l->addWidget(m_alogrithm_Combobox,0,7,1,1);
    space = new QSpacerItem(100,0, QSizePolicy::Minimum, QSizePolicy::Fixed);
//     l->addItem(space,0,6,1,2);
    space = new QSpacerItem(100,0, QSizePolicy::Minimum, QSizePolicy::Fixed);
    l->addWidget(m_ParameterLabel,1,0);
    l->addWidget(m_ParameterLineEdit,1,1,1,1);
    l->addWidget(m_IterativenumbersLabel,1,2,2,1);
    l->addWidget(m_IterativenumbersLineEdit,1,3,1,1);
    l->addWidget(m_Mrcfile_Label,1,4);
    l->addWidget(m_Mrcfile_Lineedit,1,5,1,2);
    l->addWidget(m_Mrcfile_Button,1,7);
    space = new QSpacerItem(100,0, QSizePolicy::Minimum, QSizePolicy::Fixed);
    l->addWidget(m_threadNum_Lable,3,0);
    l->addWidget(m_threadNum_LineEdit,3,1,1,2);
    l->addWidget(m_start,3,3,1,2);
    m_ParameterLabel->hide();
    m_ParameterLineEdit->hide();
    m_IterativenumbersLabel->hide();
    m_IterativenumbersLineEdit->hide();
    m_Mrcfile_Label->hide();
    m_Mrcfile_Lineedit->hide();
    m_Mrcfile_Button->hide();
    m_Method_Label = new QLabel(tr("Method"),m_ButtonFrame);
    m_Method_Label->setToolTip(ToolTips::rec_lbl_method);
    m_Method_ComboBox = new QComboBox(m_ButtonFrame);
    m_Method_ComboBox->insertItem(0,tr("SheppLogan"));
    m_Method_ComboBox->insertItem(1,tr("RamLak"));
    m_Method_ComboBox->setCurrentIndex(inputSaver->reconsruction_Method_ComboBox_saved);

    l->addWidget(m_Method_Label,1,0);
    l->addWidget(m_Method_ComboBox,1,1,1,2);

    //ICON
    m_Slice_Label = new QLabel(tr("Slice :"),m_ButtonFrame);
    m_Slice_Label->setToolTip(ToolTips::rec_lbl_slice);
    m_ICONIteration_Label = new QLabel(tr("Iteration :"),m_ButtonFrame);
    m_ICONIteration_Label->setToolTip(ToolTips::rec_lbl_iteration);
    m_Threshold_Label = new QLabel(tr("Threshold :"),m_ButtonFrame);
    m_Threshold_Label->setToolTip(ToolTips::rec_lbl_thresh);
    m_radiusLabel = new QLabel(tr("radius :"),m_ButtonFrame);
    m_radiusLabel->setToolTip(ToolTips::rec_lbl_rad);
    m_dataTypeLabel = new QLabel(tr("dataType :"),m_ButtonFrame);
    m_dataTypeLabel->setToolTip(ToolTips::rec_lbl_datatype);
    m_bin_label = new QLabel(tr("bin: "),m_ButtonFrame);
    m_SliceLineEdit1 = new QLineEdit(inputSaver->reconsruction_SliceLineEdit1_saved,m_ButtonFrame);
    m_SliceLineEdit2 = new QLineEdit(inputSaver->reconsruction_SliceLineEdit2_saved,m_ButtonFrame);
    m_ICONIterLineEdit1 = new QLineEdit(inputSaver->reconsruction_ICONIterLineEdit1_saved,m_ButtonFrame);
    m_ICONIterLineEdit2 = new QLineEdit(inputSaver->reconsruction_ICONIterLineEdit2_saved,m_ButtonFrame);
    m_ICONIterLineEdit3 = new QLineEdit(inputSaver->reconsruction_ICONIterLineEdit3_saved,m_ButtonFrame);
    m_radiusLineEdit = new QLineEdit(m_ButtonFrame);
    m_bin_lineEdit = new QLineEdit(inputSaver->reconsruction_bin_lineEdit_saved, m_ButtonFrame);

    m_dataTypeBox = new QComboBox(m_ButtonFrame);
    m_dataTypeBox->insertItem(0,"cryo-ET or plastic embedded ET");
    m_dataTypeBox->insertItem(1,"negative stained  ET");
    m_dataTypeBox->setCurrentIndex(inputSaver->reconsruction_dataTypeBox_saved);

    QRegExp rx1("^\\d+$");
    m_SliceLineEdit1->setValidator(new QRegExpValidator(rx1, this));
    m_SliceLineEdit2->setValidator(new QRegExpValidator(rx1, this));
    m_ICONIterLineEdit1->setValidator(new QRegExpValidator(rx1, this));
    m_ICONIterLineEdit2->setValidator(new QRegExpValidator(rx1, this));
    m_ICONIterLineEdit3->setValidator(new QRegExpValidator(rx1, this));
    m_radiusLineEdit->setValidator(new QRegExpValidator(rx1,this));
    m_radiusLineEdit->setText(inputSaver->reconsruction_radiusLineEdit_saved);

    m_bin_lineEdit->setValidator(new QRegExpValidator(rx1,this));
    m_ThresholdLineEdit = new QLineEdit(inputSaver->reconsruction_ThresholdLineEdit_saved, m_ButtonFrame);
    QRegExp rx2("^(-?\\d+)(\\.\\d+)?$");
    m_ThresholdLineEdit->setValidator(new QRegExpValidator(rx2, this));

    /*
        m_ICONIterLineEdit1->setMaximumWidth(60);
        m_ICONIterLineEdit1->setMinimumWidth(50);
         m_ICONIterLineEdit2->setMaximumWidth(60);
        m_ICONIterLineEdit2->setMinimumWidth(50);
         m_ICONIterLineEdit3->setMaximumWidth(60);
        m_ICONIterLineEdit3->setMinimumWidth(50);
        m_SliceLineEdit1->setMaximumWidth(40);
        m_SliceLineEdit1->setMinimumWidth(30);
        m_SliceLineEdit2->setMaximumWidth(40);
        m_SliceLineEdit2->setMinimumWidth(30);*/

    l->addWidget(m_Slice_Label,1,0);
    l->addWidget(m_SliceLineEdit1 ,1,1);
    l->addWidget(m_SliceLineEdit2 ,1,2);
    l->addWidget(m_ICONIteration_Label,2,0,1,2);
    l->addWidget(m_ICONIterLineEdit1,2,1);
    l->addWidget(m_ICONIterLineEdit2,2,2);
    l->addWidget(m_ICONIterLineEdit3,2,3);
    l->addWidget(m_radiusLabel,2,4);
    l->addWidget(m_radiusLineEdit,2,5);
    l->addWidget(m_dataTypeLabel,2,6);
    l->addWidget(m_dataTypeBox,2,7);

    l->addWidget(m_Threshold_Label,1,3);
    l->addWidget(m_ThresholdLineEdit,1,4);
    l->addWidget(m_bin_label,1,5);
    l->addWidget(m_bin_lineEdit,1,6);
    m_Slice_Label->hide();
    m_SliceLineEdit1->hide();
    m_SliceLineEdit2->hide();
    m_ICONIteration_Label->hide();
    m_ICONIterLineEdit1->hide();
    m_ICONIterLineEdit2->hide();
    m_ICONIterLineEdit3->hide();
    m_Threshold_Label->hide();
    m_ThresholdLineEdit->hide();
    m_radiusLabel->hide();
    m_radiusLineEdit->hide();
    m_dataTypeBox->hide();
    m_dataTypeLabel->hide();
    m_bin_label->hide();
    m_bin_lineEdit->hide();

    /*
     l ->addWidget(m_versio*n_Label , 0 , 0 );
     l->addWidget(m_version_Combobox , 0 ,1 );
     l->addWidget(m_LightPath_Label , 1 , 0);
     l->addWidget(m_LightPath_Combobox , 1 ,1 );
     l->addWidget(m_startModel_Label , 2 ,0);
     l->addWidget(m_startModel_Combobox , 2, 1 );
     l->addWidget(m_restructNumber_Lable , 3 ,0 );
     l->addWidget(m_restructNumber_LineEdit , 3 ,1);
     l->addWidget(m_factor_Label , 4 , 0 );
     l->addWidget(m_factor_LineEdit , 4 ,1 );
     l->addWidget(m_start , 5 ,1 , 1,1);
     */
    m_ButtonFrame->setLayout(l);
    //     m_ButtonFrame->resize(50 ,60);;

    m_progressbar = new CProgressBar(m_mainWidget);
    m_ls = new QStackedLayout;
    //     ls->setStackingMode(QStackedLayout::StackAll);
    m_ls->addWidget(m_ButtonFrame);
    m_ls->addWidget(m_progressbar);

    m_ls->setCurrentWidget(m_ButtonFrame);
    //     m_ls->setCurrentWidget(m_progressbar);
    m_SetFram = new QFrame(m_mainWidget);
    m_SetFram->setLayout(m_ls);
    
    setDefaultThreadNum();
    m_threadNum_LineEdit->setText(inputSaver->reconsruction_threadNum_LineEdit_saved);
    
   m_alogrithm_Combobox->setCurrentIndex(0);
    
    
    SetParVisible();
    ChangeAlgorithm2();
    
    connect(m_Mrcfile_Action , SIGNAL(triggered(bool)) , this , SLOT(ChooseFile()));
    connect(m_startModel_Combobox,SIGNAL(currentIndexChanged(QString)),this,SLOT(ChangeAlgorithm()));
    connect(m_version_Combobox,SIGNAL(currentIndexChanged(QString)),this,SLOT(ChangeAlgorithm2()));
    connect(m_alogrithm_Combobox,SIGNAL(currentIndexChanged(QString)),this,SLOT(SetParVisible()));
}


void RestructWin::SetInputImage()
{
    if(CProManage::GetInstance()->Exite("FIN_MRC_DIR")) {
        m_inputImage = CProManage::GetInstance()->GetDir(FIN_MRC_DIR).data();
    }
    else if(CProManage::GetInstance()->Exite("ALI_MRC_DIR")) {
        m_inputImage = CProManage::GetInstance()->GetDir(ALI_MRC_DIR).data();
    }
    else if(CProManage::GetInstance()->Exite("PREALI_DIR")) {
        m_inputImage = CProManage::GetInstance()->GetDir(PREALI_DIR).data();
    }
//     else if(CProManage::GetInstance()->Exite("PRE_MRC_DIR")){
// 	m_inputImage = CProManage::GetInstance()->GetDir(PRE_MRC_DIR).data();
//     }
//     else {   //(CProManage::GetInstance()->Exite("MRC_DIR"))
// 	m_inputImage = CProManage::GetInstance()->GetDir(MRC_DIR).data();
//     }
//
//
    qDebug()<<"setinputimage = "<<m_inputImage;
    if(m_inputImage != "NULL") {
        QFileInfo file(m_inputImage);
        if(file.exists()) {
            ItemModel item;
            ViewData view;
            if(item.Open(m_inputImage.toLatin1().data() , CProManage::GetInstance()->GetDir(ANGLE_DIR).c_str()))
            {
                cout<<m_inputImage.toLatin1().data()<<endl;
                view.Mirror(item.Img());
                m_Input->SetImage(view.MirrorImage() , 170 , 150);
            }
            else {
                m_Input->setEnabled(false);
                QMessageBox::warning(this , tr("Output Image") , tr("Can't open input file!")  ,QMessageBox::Ok);
            }
        }
        else {
            QMessageBox::warning(this , tr("Set Intput Image") , tr("MRC file not exist!")  ,QMessageBox::Ok);
        }
    }
    else {
        QMessageBox::warning(this , tr("Set Intput Image") , tr("MRC file not exist!")  ,QMessageBox::Ok);
    }
}

void RestructWin::SetOutputImage()
{
    cout<<"Set Output image"<<endl;
    m_outputImage = CProManage::GetInstance()->GetDir(RES_MRC_DIR).data();
    qDebug()<<m_outputImage;
    if(m_outputImage != "NULL") {
        QFileInfo file(m_outputImage);
        if(file.exists()) {
            ItemModel item;
            ViewData view;
            if( item.Open(m_outputImage.toLatin1().data() , CProManage::GetInstance()->GetDir(ANGLE_DIR).c_str()))
            {
                view.Mirror(item.Img());
                m_Output->setEnabled(true);
                m_Output->SetImage(view.MirrorImage() , 170 , 150);
            }
            else {
                m_Output->setEnabled(false);
                QMessageBox::warning(this , tr("Output Image") , tr("Can't open reconstructed file!")  ,QMessageBox::Ok);
                QFile file(m_outputImage);
                file.remove();
                CProManage::GetInstance()->SetState(RESTRUCT_STATE , UNFINISH);
                CProManage::GetInstance()->SetDir(RES_MRC_DIR , "NULL");
                CProManage::ReWriteProFile();
            }
        }
        else {
            // 	qDebug()<<"else";
            m_Output->setEnabled(false);
            QMessageBox::warning(this , tr("Set Output Image") , tr("Reconstructed file not exist!")  ,QMessageBox::Ok);
            // 	  qDebug()<<"end else";
        }
    }
    else {
        m_Output->setEnabled(false);
    }
}

void RestructWin::ShowInput()
{
//    ImageViewWindow* win = ImageViewWindow::ViewMrc(m_inputImage);
//     SliceMainWindow* win =SliceMainWindow::ViewMrc(m_inputImage);
    QString instruction = QString("\n%1").arg(m_inputImage);
    qDebug() << instruction << endl;
    msg_map::Message* msg = MMTApp::NewMessage(IMOD_MSG , this , instruction.toLatin1().data());
    MMTApp::PostMessage(msg);
}
void RestructWin::allhide(){
       m_Method_Label->hide();
        m_Method_ComboBox->hide();
        
        m_ParameterLabel->hide();
        m_ParameterLineEdit->hide();
        m_IterativenumbersLabel->hide();
        m_IterativenumbersLineEdit->hide();
        m_Mrcfile_Label->hide();
        m_Mrcfile_Lineedit->hide();
        m_Mrcfile_Button->hide();
        m_Slice_Label->hide();
        m_SliceLineEdit1->hide();
        m_SliceLineEdit2->hide();
        m_ICONIteration_Label->hide();
        m_ICONIterLineEdit1->hide();
        m_ICONIterLineEdit2->hide();
        m_ICONIterLineEdit3->hide();
        m_Threshold_Label->hide();
        m_ThresholdLineEdit->hide();
        m_radiusLabel->hide();
        m_radiusLineEdit->hide();
        m_dataTypeBox->hide();
        m_dataTypeLabel->hide();
        m_bin_label->hide();
        m_bin_lineEdit->hide();
}
void RestructWin::SetParVisible()
{
    allhide();
    if(m_alogrithm_Combobox->currentText()=="BPT")//bpt
    {}
    else if(m_alogrithm_Combobox->currentText()=="FBP")//fbp
    {  m_Method_Label->show();
       m_Method_ComboBox->show();
    }
    else if (m_alogrithm_Combobox->currentText()=="ICON")
    {
                m_Slice_Label->show();
                m_SliceLineEdit1->show();
                m_SliceLineEdit2->show();
                m_ICONIteration_Label->show();
                m_ICONIterLineEdit1->show();
                m_ICONIterLineEdit2->show();
                m_ICONIterLineEdit3->show();
                m_Threshold_Label->show();
                m_ThresholdLineEdit->show();
                m_radiusLabel->show();
                m_radiusLineEdit->show();
                m_dataTypeBox->show();
                m_dataTypeLabel->show();
                m_bin_label->show();
                m_bin_lineEdit->show();
    }
    else //sirt.sart..
    {
            m_ParameterLabel->show();
            m_ParameterLineEdit->show();
            m_IterativenumbersLabel->show();
            m_IterativenumbersLineEdit->show();
            m_Mrcfile_Label->show();
            m_Mrcfile_Lineedit->show();
            m_Mrcfile_Button->show();
         
    }
   
    
//     if(m_alogrithm_Combobox->currentIndex()==0||m_alogrithm_Combobox->currentIndex()==1||m_alogrithm_Combobox->currentText()=="ICON") {
//         m_ParameterLabel->hide();
//         m_ParameterLineEdit->hide();
//         m_IterativenumbersLabel->hide();
//         m_IterativenumbersLineEdit->hide();
//         m_Mrcfile_Label->hide();
//         m_Mrcfile_Lineedit->hide();
//         m_Mrcfile_Button->hide();
//         m_Slice_Label->hide();
//         m_SliceLineEdit1->hide();
//         m_SliceLineEdit2->hide();
//         m_ICONIteration_Label->hide();
//         m_ICONIterLineEdit1->hide();
//         m_ICONIterLineEdit2->hide();
//         m_ICONIterLineEdit3->hide();
//         m_Threshold_Label->hide();
//         m_ThresholdLineEdit->hide();
//         m_radiusLabel->hide();
//         m_radiusLineEdit->hide();
//         m_dataTypeBox->hide();
//         m_dataTypeLabel->hide();
//         m_bin_label->hide();
//         m_bin_lineEdit->hide();
//          
//         
//         if(m_alogrithm_Combobox->currentIndex()==1) //fbp
//         {   m_Method_Label->show();
//             m_Method_ComboBox->show();
//         }
//         else//bpt
//         {   m_Method_Label->hide();
//             m_Method_ComboBox->hide();
//             if(m_alogrithm_Combobox->currentText()=="ICON")
//             {
//                 m_Slice_Label->show();
//                 m_SliceLineEdit1->show();
//                 m_SliceLineEdit2->show();
//                 m_ICONIteration_Label->show();
//                 m_ICONIterLineEdit1->show();
//                 m_ICONIterLineEdit2->show();
//                 m_ICONIterLineEdit3->show();
//                 m_Threshold_Label->show();
//                 m_ThresholdLineEdit->show();
//                 m_radiusLabel->show();
//                 m_radiusLineEdit->show();
//                 m_dataTypeBox->show();
//                 m_dataTypeLabel->show();
//                 m_bin_label->show();
//                 m_bin_lineEdit->show();
//             }
//         }
//     }
//     else { //sirt.sart.............
//         if(m_ParameterLabel->isHidden()) {
//             m_ParameterLabel->show();
//             m_ParameterLineEdit->show();
//             m_IterativenumbersLabel->show();
//             m_IterativenumbersLineEdit->show();
//             m_Mrcfile_Label->show();
//             m_Mrcfile_Lineedit->show();
//             m_Mrcfile_Button->show();
//             m_Method_Label->hide();
//             m_Method_ComboBox->hide();
// 
//             m_Slice_Label->hide();
//             m_SliceLineEdit1->hide();
//             m_SliceLineEdit2->hide();
//             m_ICONIteration_Label->hide();
//             m_ICONIterLineEdit1->hide();
//             m_ICONIterLineEdit2->hide();
//             m_ICONIterLineEdit3->hide();
//             m_Threshold_Label->hide();
//             m_ThresholdLineEdit->hide();
//             m_radiusLabel->hide();
//             m_radiusLineEdit->hide();
//             m_dataTypeBox->hide();
//             m_dataTypeLabel->hide();
//             m_bin_label->hide();
//             m_bin_lineEdit->hide();
// 	    
//             string filename=CProManage::GetInstance()->GetDir(PROCESS_DIR)+"_fbp.mrc";
//             fstream pfile;
//             pfile.open(filename.c_str(), ios_base::in);
//             if(pfile) {
//                 m_Mrcfile_Lineedit->setText(QString::fromStdString(filename));
//             }
//             else {
//                 m_Mrcfile_Lineedit->clear();
//             }
//         }
//         else {
//             //m_ParameterLineEdit->clear();
//             //m_IterativenumbersLineEdit->clear();
//         }
// 
//     }

    }
void RestructWin::ChangeAlgorithm()
{
  
    if(m_startModel_Combobox->currentIndex()==1) { //multiple axis
        if(m_alogrithm_Combobox->itemText(6)=="ICON")
            m_alogrithm_Combobox->removeItem(6);
        m_alogrithm_Combobox->removeItem(5);
        m_alogrithm_Combobox->removeItem(4);
    }
    else { //single axis
        m_alogrithm_Combobox->clear();
        m_alogrithm_Combobox->insertItem(0 , tr("BPT"));
	m_alogrithm_Combobox->insertItem(1 , tr("FBP"));
	m_alogrithm_Combobox->insertItem(2 , tr("SART"));
        m_alogrithm_Combobox->insertItem(4, tr("SIRT"));
//         m_alogrithm_Combobox->insertItem(5, tr("ASIRT"));
        if(m_version_Combobox->currentIndex()==0)//mpi
            m_alogrithm_Combobox->insertItem(6, tr("ICON"));
    }
}
void RestructWin::ChangeAlgorithm2()
{  
    m_alogrithm_Combobox->clear();
    if(m_startModel_Combobox->currentIndex()==0)//single
    { 
      
      if(m_version_Combobox->currentIndex()==0)//mpi
      {
	m_alogrithm_Combobox->insertItem(0 , tr("BPT"));
	m_alogrithm_Combobox->insertItem(1 , tr("FBP"));
	m_alogrithm_Combobox->insertItem(2 , tr("SART"));
	m_alogrithm_Combobox->insertItem(4,  tr("SIRT"));
	if(m_version_Combobox->currentIndex()==0) m_alogrithm_Combobox->insertItem(6,tr("ICON"));
      }else
      {
        m_alogrithm_Combobox->insertItem(0 , tr("BPT"));
	m_alogrithm_Combobox->insertItem(1 , tr("FBP"));
	m_alogrithm_Combobox->insertItem(2 , tr("SART"));
	m_alogrithm_Combobox->insertItem(4,  tr("SIRT"));
      }
    
//         if(m_version_Combobox->currentIndex()==0)
//             m_alogrithm_Combobox->insertItem(6, tr("ICON"));
//         else{
//             int index=m_alogrithm_Combobox->findText("ICON",Qt::MatchCaseSensitive);
//             if(index!=-1)
//                 m_alogrithm_Combobox->removeItem(index);
//         }
    }
    setDefaultThreadNum();
}

void RestructWin::ChooseFile()
{

    QString filename=QFileDialog::getOpenFileName(this ,tr("open"), ".",tr("MRC files (*.mrc *.st)"));
    int start ,end;
    start = filename.lastIndexOf('/');
    end = filename.length();
    QString name = filename.mid(start+1 , end-start-1);
    m_Mrcfile_Lineedit->setText(name);
}

void RestructWin::ShowOutput()
{
    qDebug()<<"show output:"<<m_outputImage;
    printf("show output: %s\n" , m_outputImage.toLatin1().data());

//    VolumeMainWindow* win = VolumeMainWindow::ViewMrc(m_outputImage);
    QString instruction = QString("\n%1").arg(m_outputImage);
    qDebug() << instruction << endl;
    msg_map::Message* msg = MMTApp::NewMessage(IMOD_MSG , this , instruction.toLatin1().data());
    MMTApp::PostMessage(msg);
//    util::MrcStack* images = new util::MrcStack;
//    images->Open(m_outputImage.toLatin1().data());
//    XyzWindow *foo = new XyzWindow(images);
//    foo->setAttribute(Qt::WA_DeleteOnClose);
//    foo->show();
//    delete images;
}

// void RestructWin::StartRestruct()
// {
//     emit Start();
//     m_ls->setCurrentWidget(m_progressbar);
//     if(m_outputImage != "NULL"){
// 	QFile file(m_outputImage);
// 	if(file.exists()){
// 	    file.remove();
// 	}
// 	CProManage::GetInstance()->SetState(RESTRUCT_STATE , UNFINISH);
// 	CProManage::GetInstance()->SetDir(RES_MRC_DIR , "NULL");
// 	CProManage::ReWriteProFile();
// 	SetOutputImage();
//     }
//
//     QString version;
//     QString Haved_input_path;
//     QString path_Light;
//     QString Start_Model;
//     QString restruct_method;
//     QString iter_num;
//     QString step;
//     QString path_geo;
//     QString path_angle;
//     QString part_restruct;
//     QString isWeight;
//     QString pixc_correction;
//     QString tilt_correction;
//
//     version = m_version_Combobox->currentText();
//
//     string output= CProManage::GetInstance()->GetDir(PROCESS_DIR)+"_res.st";
//     m_outputImage = output.data();
//
//     Haved_input_path = m_startModel_Combobox->currentText();
//
//     path_Light = "PATH_MODE_II";
//
//     Start_Model = "";
//     if(Start_Model == tr("Zero"))
// 	Start_Model = "ATOM_MODEL_ZERO";
//     else if(Start_Model == tr("Random"))
// 	Start_Model = "ATOM_MODEL_RANDOM";
//     else if(Start_Model == tr("Weighted Back Project"))
// 	Start_Model = "ATOM_MODEL_WBP";
//     else if(Start_Model == tr("Reconstructed Image Already Exists"))
// 	Start_Model = "ATOM_MODEL_EXIST";
//     else
// 	Start_Model = "ATOM_MODEL_BP";
//
//     restruct_method = m_alogrithm_Combobox->currentText();
//
//     iter_num = m_restructNumber_LineEdit->text();
//
//     step = m_factor_LineEdit->text();
//
//     path_geo = "geometry.par";
//
//     if(CProManage::GetInstance()->Exite("FIN_MRC_DIR")){
// 	string str = CProManage::GetInstance()->GetDir(PROCESS_DIR) + "_new.tlt";
// 	path_angle = str.data();
//     }
//     else
// 	path_angle = CProManage::GetInstance()->GetDir(ANGLE_DIR).data();
//
//     part_restruct = "ALL";
//
//     isWeight = "0";
//
//     pixc_correction = "1";
//
//     tilt_correction = "0";
//
//     QStringList cmdlist;
//     cmdlist.append(" -i ");
//     cmdlist.append(m_inputImage);
//     cmdlist.append(" -o ");
//     cmdlist.append(m_outputImage);
//     cmdlist.append(" -m ");
//     cmdlist.append(restruct_method);
//     cmdlist.append(" -n ");
//     cmdlist.append(iter_num);
//     cmdlist.append(" -t ");
//     cmdlist.append(step);
//     cmdlist.append(" -p ");
//     cmdlist.append(path_Light);
//     cmdlist.append(" -s ");
//     cmdlist.append(Start_Model);
//     cmdlist.append(" -g ");
//     cmdlist.append(path_geo);
//     cmdlist.append(" -a ");
//     cmdlist.append(path_angle);
//
//     cmdlist.append(" -P ");
//     cmdlist.append(part_restruct);
//     cmdlist.append(" -w ");
//     cmdlist.append(isWeight);
//     cmdlist.append(" -c ");
//     cmdlist.append(pixc_correction);
//     cmdlist.append(" -r ");
//     cmdlist.append(tilt_correction);
//     cmdlist.append(" -e ");
//     cmdlist.append(Haved_input_path);
//
//     QFile file("restruct.sh");
//     if (!file.open(QIODevice::WriteOnly | QIODevice::Text))/*æ å¿éèŠçæ¯åŠå·²ææä»¶*/
// 	return;
//
//     QTextStream out(&file);
//     out<<"#!/bin/sh"<<endl;
//     if(version=="CPU"&&Start_Model!="ATOM_MODEL_EXIST")
//     {
// 	out <<"recon_proc_cpu";
// 	for(int i=0;i<cmdlist.count()-2;++i)
// 	    out<<cmdlist.at(i);
//     }
//     if(version=="CPU"&&Start_Model=="ATOM_MODEL_EXIST")
//     {
// 	out <<"recon_proc_cpu";
// 	for(int i=0;i<cmdlist.count();++i)
// 	    out<<cmdlist.at(i);
//     }
//
//     if(version=="GPU"&&Start_Model!="ATOM_MODEL_EXIST")
//     {
// 	out <<"recon_proc_gpu";
// 	for(int i=0;i<cmdlist.count()-2;++i)
// 	    out<<cmdlist.at(i);
//     }
//     if(version=="GPU"&&Start_Model=="ATOM_MODEL_EXIST")
//     {
// 	out <<"recon_proc_gpu";
// 	for(int i=0;i<cmdlist.count();++i)
// 	    out<<cmdlist.at(i);
//     }
//     out<<" 2>&1 | tee restruct.log";
//     out<<endl;
//     file.close();
//
//     msg_map::Message* msg = MMTApp::NewMessage(RESTRUCT_MSG, this, "sh restruct.sh");
//     MMTApp::PostMessage(msg);
// }



int RestructWin::StartforRestruct()
{
    if(m_threadNum_LineEdit->text().isEmpty()) {
        QMessageBox::warning(this , tr("Warning") , tr("Parameter cannot be null!")  ,QMessageBox::Ok);
        return false;
    }
    if(!m_Mrcfile_Label->isHidden() &&
            (   m_IterativenumbersLineEdit->text().isEmpty()
                ||m_ParameterLineEdit->text().isEmpty()

            )
      )   {
        QMessageBox::warning(this , tr("Warning") , tr("Parameter cannot be null!")  ,QMessageBox::Ok);
        return false;
    }
    if(!m_Slice_Label->isHidden()&& (m_SliceLineEdit1->text().isEmpty()||m_SliceLineEdit2->text().isEmpty()||
                                     m_ICONIterLineEdit1->text().isEmpty()||m_ICONIterLineEdit2->text().isEmpty()||m_ICONIterLineEdit3->text().isEmpty()||
                                     m_ThresholdLineEdit->text().isEmpty() ) )   {
        QMessageBox::warning(this , tr("Warning") , tr("Parameter cannot be null!")  ,QMessageBox::Ok);
        return false;
    }

    QString Algorithm = m_alogrithm_Combobox->currentText();
    if(Algorithm=="ICON")
    {
        DeleteDirectory("crossValidation");
        DeleteDirectory("reconstruction");
    }
    StartRestruct();
}
int RestructWin::StartRestruct()
{

    emit Start();

    string path=CEnv::ExeBinDirectory();
    m_ls->setCurrentWidget(m_progressbar);
    if(m_outputImage != "NULL") {
        QFile file(m_outputImage);
        if(file.exists()) {
            file.remove();
        }
        CProManage::GetInstance()->SetState(RESTRUCT_STATE , UNFINISH);
        CProManage::GetInstance()->SetDir(RES_MRC_DIR , "NULL");
        CProManage::ReWriteProFile();
        SetOutputImage();
    }
    QString StartModel= m_startModel_Combobox->currentText();
    QString Algorithm = m_alogrithm_Combobox->currentText();
    QString Version = m_version_Combobox->currentText();
    qDebug()<<StartModel<<Algorithm;
    if(StartModel=="Single Axis") {
        string version;
        if(Version == "MPI") {
            path+="tomorec3d";
            version+="-G 0";
        }
        else {
            path+="tomorec3d";
            version+="-G 1";
        }
        if(Algorithm=="FBP"||Algorithm=="BPT") {

            string inputfile1,inputfile2,offset,pitch,zshift,thickness,outputfile,threadnum,method;
            inputfile1=CProManage::GetInstance()->GetDir(PROCESS_DIR)+"_fin.mrc";
            inputfile2=CProManage::GetInstance()->GetDir(PROCESS_DIR)+"_new.tlt";
            if(Algorithm=="FBP")
                outputfile=CProManage::GetInstance()->GetDir(PROCESS_DIR)+"_fbp.mrc";
            else
                outputfile=CProManage::GetInstance()->GetDir(PROCESS_DIR)+"_bpt.mrc";
            m_outputImage=outputfile.data();
            ifstream in;
            string filename="geometry.par";
            in.open(filename.c_str());
            string st;
            for(int i=0;; i++) {
                in>>st;
                if(st=="THICKNESS") {
                    in>>st;
                    if(isNum(st)) thickness=st;
                }
                if(st=="ZSHIFT") {
                    in>>st;
                    if(isNum(st)) zshift=st;
                }
                if(st=="OFFSET") {
                    in>>st;
                    if(isNum(st)) offset=st;
                }
                if(st=="PITCH") {
                    in>>st;
                    if(isNum(st)) pitch=st;
                }
                if(in.eof()) break;
            }

            threadnum=m_threadNum_LineEdit->text().toStdString();

            if(Algorithm=="FBP")
                method=Algorithm.toStdString()+","+m_Method_ComboBox->currentText().toStdString();
            else
                method=Algorithm.toStdString();


            // 		#time mpiexec -n 2 /home/wanxiaohua/mpi_fbp_nocuda/SingleCuda -i BBa_fin.mrc -t BBa_new.tlt
// 		-g 0.29,1.16,0,256 -o BBa_fbp.mrc -m FBP,SheppLogan
            QString instruction =
                QString("%1 %2 %3 %4 %5 %6 %7 %8 %9,%10,%11,%12 %13 %14 %15 %16 %17")
                .arg("time mpiexec -n").arg(threadnum.data()).arg(path.data())
                .arg("-i").arg(inputfile1.data()).arg("-t").arg(inputfile2.data())
                .arg("-g").arg(offset.data()).arg(pitch.data()).arg(zshift.data()).arg(thickness.data())
                .arg("-o").arg(outputfile.data())
                .arg("-m").arg(method.data()).arg(version.data());


            qDebug()<<"instruction : "<<instruction;
            msg_map::Message* msg = MMTApp::NewMessage(RESTRUCT_MSG , this , instruction.toLatin1().data());
            MMTApp::PostMessage(msg);
        }
        else if(Algorithm=="ICON")
        {
            string inputfile1,inputfile2,Iteration1,Iteration2,Iteration3,threshold,threadnum,path2,outputfile ;
            int slice1=0;
            int slice2=0;
            QString path=CEnv::ExeBinDirectory();
            QString ICONPath=path+"./ICON";
            QString ICONPreprocessPath=path+"./ICONPreProcess";
            QString ICONMaskPath=path+"./ICONMask";

            QString radius,dataType,bin;
            path2=CEnv::CurrentWorkDirectory();
            QString processDir=CProManage::GetInstance()->GetDir(PROCESS_DIR).data();
            inputfile1=CProManage::GetInstance()->GetDir(PROCESS_DIR)+"_fin.mrc";
            inputfile2=CProManage::GetInstance()->GetDir(PROCESS_DIR)+"_new.tlt";
            
            bin=m_bin_lineEdit->text();
            int binNumber=bin.toInt();
            slice1=m_SliceLineEdit1->text().toInt();
            slice2=m_SliceLineEdit2->text().toInt();
            slice2=(slice2-slice1+1)/binNumber+slice1-1;

            Iteration1=m_ICONIterLineEdit1->text().toStdString();
            Iteration2=m_ICONIterLineEdit2->text().toStdString();
            Iteration3=m_ICONIterLineEdit3->text().toStdString();
            threshold=m_ThresholdLineEdit->text().toStdString();

            radius=m_radiusLineEdit->text();
            dataType=QString::number(m_dataTypeBox->currentIndex()+1);
            outputfile=path2+"/reconstruction/mid00000.mrc";
            threadnum=m_threadNum_LineEdit->text().toStdString();
            
            //read geometry from file 
            QString thickness = "";
            QString ZShift = 0;
            QString offset;
            QString pitch;
            ifstream in;
            string filename="geometry.par";
            in.open(filename.c_str());
            string st;
            for(int i=0;; i++) {
                in>>st;

                if(st=="THICKNESS") {
                    in>>st;
                    if(isNum(st)) {
                        double thicknessNumber=QString(st.data()).toDouble();
                        thicknessNumber/=binNumber;
                        thickness=QString::number(thicknessNumber);
                    }
                }
                if(st == "ZSHIFT") {
                    in>>st;
                    if(isNum(st)) {
                        double zshiftNumber=QString(st.data()).toDouble();
                        zshiftNumber/=binNumber;
                        ZShift=QString::number(zshiftNumber);
                    }

                }
                if(st=="OFFSET") {
                    in>>st;
                    if(isNum(st)) offset=st.data();
                }
                if(st=="PITCH") {
                    in>>st;
                    if(isNum(st)) pitch=st.data();
                }
                if(in.eof()) break;
            }
            
            
            //generate cmd 
            QString ICONPreprocessCmd = "";
            QString ICONPreprocessOutputFileName=processDir+"_preprocessTemp.mrc";
            ICONPreprocessCmd.append(ICONPreprocessPath)
                            .append(" -i ").append(inputfile1.data())
                            .append(" -o ").append(ICONPreprocessOutputFileName)
                            .append(" -bin ").append(bin);
                            
            // ./ICON -input BBa_fin.mrc -tiltfile BBa_new.tlt -outputPath . -slice 0,63 -ICONIteration 1,7,2 -thr 0
            QString ICONCmd =
                QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11,%12 %13 %14,%15,%16 %17 %18 %19 %20")
                .arg("time mpiexec -n").arg(threadnum.data())
                .arg(ICONPath)
                .arg("-input").arg(ICONPreprocessOutputFileName)
                .arg("-tiltfile").arg(inputfile2.data())
                .arg("-outputPath").arg(path2.data())
                .arg("-slice")
                .arg(QString::number(slice1).toStdString().data())
                .arg(QString::number(slice2).toStdString().data())
                .arg("-ICONIteration")
                .arg(Iteration1.data())
                .arg(Iteration2.data())
                .arg(Iteration3.data())
                .arg("-thr").arg(threshold.data())
                .arg(" -dataType ").arg(dataType)
                .append(" -toff ").append(offset);
                ;

            
            m_outputImage="./masked_ICONreconstruction.mrc";

            QString ICONMaskCmd = "";
            if(radius.toInt() <=0) {
                //user wants to ignore radius,so use parameter crossVfrc and fullRecfrc
                ICONMaskCmd
                .append(ICONMaskPath)
                .append(" -inputPath ")
                .append("./reconstruction")
                .append(" -tiltfile ").append(inputfile2.data())
                .append(" -output ")
                .append(m_outputImage)
                .append(" -slice ")
                .append(QString::number(slice1).toStdString().data()).append(",").append(QString::number(slice2).toStdString().data())
                .append(" -thickness ").append(thickness)
                .append(" -crossVfrc ")
                .append("./crossValidation/crossV.frc")
                .append(" -fullRecfrc ")
                .append("./crossValidation/fullRec.frc")
                .append(" -z ").append(ZShift)
                .append(" -pang ").append(pitch)
                .append(" -toff ").append(offset);
            } else {
                // radius greater than zero,so use parameter radius and ignore crossVfrc and fullRecfrc
                ICONMaskCmd
                .append(ICONMaskPath)
                .append(" -inputPath ").append("./reconstruction")
                .append(" -tiltfile ").append(inputfile2.data())
                .append(" -output ").append(m_outputImage)
                .append(" -slice ")
                .append(QString::number(slice1).toStdString().data()).append(",").append(QString::number(slice2).toStdString().data())
                .append(" -thickness ").append(thickness)
                .append(" -radius ").append(radius)
                .append(" -z ").append(ZShift)
                .append(" -pang ").append(pitch);
            }
            QString instruction=ICONPreprocessCmd+";\n"+ICONCmd +";\n"+ICONMaskCmd;//!

            qDebug()<<"instruction : "<<instruction;
            msg_map::Message* msg3 = MMTApp::NewMessage(RESTRUCT_MSG, this , instruction.toLatin1().data());
            MMTApp::PostMessage(msg3);

        }
        else {
            string inputfile1,inputfile2,offset,pitch,zshift,thickness,outputfile,threadnum,iterativenum,parameter,mrcfile;
            inputfile1=CProManage::GetInstance()->GetDir(PROCESS_DIR)+"_fin.mrc";
            inputfile2=CProManage::GetInstance()->GetDir(PROCESS_DIR)+"_new.tlt";
            outputfile=CProManage::GetInstance()->GetDir(PROCESS_DIR) +"_"+Algorithm.toStdString()+".mrc";
            m_outputImage=outputfile.data();
            ifstream in;
            string filename="geometry.par";
            in.open(filename.c_str());
            string st;
            for(int i=0;; i++) {
                in>>st;
                if(st=="THICKNESS") {
                    in>>st;
                    if(isNum(st)) thickness=st;
                }
                if(st=="ZSHIFT") {
                    in>>st;
                    if(isNum(st)) zshift=st;
                }
                if(st=="OFFSET") {
                    in>>st;
                    if(isNum(st)) offset=st;
                }
                if(st=="PITCH") {
                    in>>st;
                    if(isNum(st)) pitch=st;
                }
                if(in.eof()) break;
            }
            threadnum=m_threadNum_LineEdit->text().toStdString();
            iterativenum=m_IterativenumbersLineEdit->text().toStdString();
            parameter=m_ParameterLineEdit->text().toStdString();
            mrcfile=m_Mrcfile_Lineedit->text().toStdString();

            QString tempMrcfile=QString(mrcfile.data());
            QString instruction;

            if(tempMrcfile.remove(QChar(' ')).isEmpty()) {
                instruction =
                    QString("%1 %2 %3 %4 %5 %6 %7 %8 %9,%10,%11,%12 %13 %14 %15 %16,%17,%18 %19")
                    .arg("time mpiexec -n").arg(threadnum.data()).arg(path.data())
                    .arg("-i").arg(inputfile1.data()).arg("-t").arg(inputfile2.data())
                    .arg("-g").arg(offset.data()).arg(pitch.data()).arg(zshift.data()).arg(thickness.data())
                    .arg("-o").arg(outputfile.data())
                    .arg("-m").arg(Algorithm).arg(iterativenum.data()).arg(parameter.data()).arg(version.data());

            } else {
                instruction =
                    QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11,%12,%13,%14 %15 %16 %17 %18,%19,%20 %21")
                    .arg("time mpiexec -n").arg(threadnum.data()).arg(path.data())
                    .arg("-i").arg(inputfile1.data()).arg("-n").arg(mrcfile.data()).arg("-t").arg(inputfile2.data())
                    .arg("-g").arg(offset.data()).arg(pitch.data()).arg(zshift.data()).arg(thickness.data())
                    .arg("-o").arg(outputfile.data())
                    .arg("-m").arg(Algorithm).arg(iterativenum.data()).arg(parameter.data()).arg(version.data());
            }


            // 	    time mpiexec -n 24 ../single_tilt/mpi/SingleMpi -M 24 -i BBa_fin.mrc -n BBa_bpt.mrc -t BBa_new.tlt
            // 	    -g -0.3,2,20,400 -o BBa_ASIRT1.mrc  -m ASIRT,1,0.2


            qDebug()<<"instruction : "<<instruction;
            msg_map::Message* msg = MMTApp::NewMessage(RESTRUCT_MSG , this , instruction.toLatin1().data());
            MMTApp::PostMessage(msg);
        }
    }
    else { //Multi Axis

        if(Version == "MPI") path+="MultiMpi";
        else path+="MultiCuda";

        if(Algorithm=="FBP"||Algorithm=="BPT") {

            string inputfile1,inputfile2,inputfile3,offset,pitch,zshift,thickness,outputfile,threadnum,method;
            inputfile1=CProManage::GetInstance()->GetDir(PROCESS_DIR)+"_fin.mrc";
            inputfile2=CProManage::GetInstance()->GetDir(PROCESS_DIR)+"_new.tlt";
            inputfile3="xtiltangle.txt";
            if(Algorithm=="FBP")
                outputfile=CProManage::GetInstance()->GetDir(PROCESS_DIR)+"_fbp.mrc";
            else
                outputfile=CProManage::GetInstance()->GetDir(PROCESS_DIR)+"_bpt.mrc";
            m_outputImage=outputfile.data();
            ifstream in;
            string filename="geometry.par";
            in.open(filename.c_str());
            string st;
            for(int i=0;; i++) {
                in>>st;
                if(st=="THICKNESS") {
                    in>>st;
                    if(isNum(st)) thickness=st;
                }
                if(st=="ZSHIFT") {
                    in>>st;
                    if(isNum(st)) zshift=st;
                }
                if(st=="OFFSET") {
                    in>>st;
                    if(isNum(st)) offset=st;
                }
                if(st=="PITCH") {
                    in>>st;
                    if(isNum(st)) pitch=st;
                }
                if(in.eof()) break;
            }

            threadnum=m_threadNum_LineEdit->text().toStdString();

            if(Algorithm=="FBP")
                method=Algorithm.toStdString()+","+m_Method_ComboBox->currentText().toStdString();
            else
                method=Algorithm.toStdString();

            // 		#time mpiexec -n 24 /home/wanxiaohua/mpi_fbp_nocuda/MultiMpi  -i BBa_fin.mrc -t BBa_new.tlt
            // 		-g 0.29,1.16,0,256 -o BBa_fbp.mrc -G 2 -m FBP,SheppLogan
            QString instruction =
                QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11,%12,%13,%14 %15 %16 %17 %18")
                .arg("time mpiexec -n").arg(threadnum.data()).arg(path.data())
                .arg("-i").arg(inputfile1.data()).arg("-t").arg(inputfile2.data()).arg("-x").arg(inputfile3.data())
                .arg("-g").arg(offset.data()).arg(pitch.data()).arg(zshift.data()).arg(thickness.data())
                .arg("-o").arg(outputfile.data())
                .arg("-m").arg(method.data());


            qDebug()<<"instruction : "<<instruction;
            msg_map::Message* msg = MMTApp::NewMessage(RESTRUCT_MSG , this , instruction.toLatin1().data());
            MMTApp::PostMessage(msg);
        }
        else {

            string inputfile1,inputfile2,inputfile3,offset,pitch,zshift,thickness,outputfile,threadnum,iterativenum,parameter,mrcfile;
            inputfile1=CProManage::GetInstance()->GetDir(PROCESS_DIR)+"_fin.mrc";
            inputfile2=CProManage::GetInstance()->GetDir(PROCESS_DIR)+"_new.tlt";
            inputfile3="xtiltangle.txt";
            outputfile=CProManage::GetInstance()->GetDir(PROCESS_DIR) +"_"+Algorithm.toStdString()+".mrc";
            m_outputImage=outputfile.data();
            ifstream in;
            string filename="geometry.par";
            in.open(filename.c_str());
            string st;
            for(int i=0;; i++) {
                in>>st;
                if(st=="THICKNESS") {
                    in>>st;
                    if(isNum(st)) thickness=st;
                }
                if(st=="ZSHIFT") {
                    in>>st;
                    if(isNum(st)) zshift=st;
                }
                if(st=="OFFSET") {
                    in>>st;
                    if(isNum(st)) offset=st;
                }
                if(st=="PITCH") {
                    in>>st;
                    if(isNum(st)) pitch=st;
                }
                if(in.eof()) break;
            }

            threadnum=m_threadNum_LineEdit->text().toStdString();
            iterativenum=m_IterativenumbersLineEdit->text().toStdString();
            parameter=m_ParameterLineEdit->text().toStdString();
            mrcfile=m_Mrcfile_Lineedit->text().toStdString();

            // 	    time mpiexec -n 24 ../single_tilt/mpi/Recon_mpi -M 24 -i BBa_fin.mrc -n BBa_bpt.mrc -t BBa_new.tlt
            // 	    -g -0.3,2,20,400 -o BBa_ASIRT1.mrc -G 2 -m ASIRT,1,0.2
            QString instruction =
                QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11 %12 %13,%14,%15,%16 %17 %18 %19 %20 %21,%22")
                .arg("time mpiexec -n").arg(threadnum.data()).arg(path.data())
                .arg("-i").arg(inputfile1.data()).arg("-n").arg(mrcfile.data()).arg("-t").arg(inputfile2.data()).arg("-x").arg(inputfile3.data())
                .arg("-g").arg(offset.data()).arg(pitch.data()).arg(zshift.data()).arg(thickness.data())
                .arg("-o").arg(outputfile.data())
                .arg("-m").arg(Algorithm).arg(iterativenum.data()).arg(parameter.data());


            qDebug()<<"instruction : "<<instruction;
            msg_map::Message* msg = MMTApp::NewMessage(RESTRUCT_MSG , this , instruction.toLatin1().data());
            MMTApp::PostMessage(msg);
        }
    }
    return true;
}

bool RestructWin::DeleteDirectory(const QString &path)
{
    if (path.isEmpty())
        return false;

    QDir dir(path);
    if(!dir.exists())
        return true;

    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QFileInfoList fileList = dir.entryInfoList();
    foreach (QFileInfo fi, fileList)
    {
        if (fi.isFile())
            fi.dir().remove(fi.fileName());
        else
            DeleteDirectory(fi.absoluteFilePath());
    }
    return dir.rmpath(dir.absolutePath());
}
void RestructWin::customEvent(QEvent* e)
{

    if(e->type() == msg_map::QSequelType) {
        msg_map::CSequelEvent* sequel = static_cast<msg_map::CSequelEvent*>(e);
        if(sequel->msg_id == RESTRUCT_MSG) {
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
                int* state =static_cast<int*>(sequel->data);
                if(state == PTHREAD_CANCELED) {
                    QFile file(m_outputImage);
                    if(file.exists()) {
                        file.remove();
                    }
                    if(CProManage::GetInstance()->GetMethod()=="BasedMethod") {
                        CProManage::AddItem("RESTRUCT_STATE_BASED" , "FAIL");
                    }
                    else {
                        CProManage::AddItem("RESTRUCT_STATE" , "FAIL");
                    }

                    emit RunError();
                }
                else if(*state == 0 ) {
                    //the child process return zero
                    qDebug()<<"1";
                    QFileInfo file(m_outputImage);
                    if(file.exists()) {
//                         QString Algorithm = m_alogrithm_Combobox->currentText();
//                         if(Algorithm=="ICON")  {
//                             cout<<"start  ICONMask function\n"<<endl;
//                             ICONMask();
//
//                         }


                        if(CProManage::GetInstance()->GetMethod()=="BasedMethod") {
                            CProManage::AddItem("RES_MRC_DIR" , m_outputImage.toLatin1().data());
                            CProManage::AddItem("RESTRUCT_STATE_BASED" , "FINISH");
                            SetOutputImage();
                            emit Finish("RestructBased");
                        }
                        else {
                            CProManage::AddItem("RES_MRC_DIR" , m_outputImage.toLatin1().data());
                            CProManage::AddItem("RESTRUCT_STATE" , "FINISH");
                            SetOutputImage();
                            emit Finish("Restruct");
                        }

                    }
                    else {
                        QFile file(m_outputImage);
                        if(file.exists()) {
                            file.remove();
                        }
                        if(CProManage::GetInstance()->GetMethod()=="BasedMethod") {
                            CProManage::AddItem("RESTRUCT_STATE_BASED" , "FAIL");
                            emit RunError();
                        }
                        else {
                            CProManage::AddItem("RESTRUCT_STATE" , "FAIL");
                            emit RunError();
                        }

                    }
                    delete state;
                } else {
                    // the child process return a non-zero value
                    if(CProManage::GetInstance()->GetMethod()=="BasedMethod") {
                        CProManage::AddItem("RESTRUCT_STATE_BASED" , "FAIL");
                        emit RunError();
                    }
                    else {
                        CProManage::AddItem("RESTRUCT_STATE" , "FAIL");
                        emit RunError();
                    }
                    delete state;
                }
            }
            break;
            }
        }
    } else {
        QObject::customEvent(e);
    }



}
int RestructWin::isNum(string s)
{
    for(int i=0; i<s.length(); i++) {
        if((s.at(i)>'9'||s.at(i)<'0')&&s.at(i)!='.'&&s.at(i)!='-') return 0;
    }
    return 1;
}
void RestructWin::ShowCTFCorrectionSettings()
{
    m_ctf_dialog->exec();
}
void RestructWin::EnableCTFCorrection(){
    bool isChecked = m_ctf_enable_checkbox->isChecked();
    m_ctf_settings_btn->setEnabled(isChecked);
}

void RestructWin::setDefaultThreadNum(){
  
  int recommandedValue = 0;
   if(0 == m_version_Combobox->currentIndex()){
      recommandedValue = SysInfo::getLCpuNum();
  }
  else{
      recommandedValue = SysInfo::getGpuNum();
  }
  m_threadNum_LineEdit->setText(QString::number(recommandedValue));
  
}

#include "RestructWin.moc"

