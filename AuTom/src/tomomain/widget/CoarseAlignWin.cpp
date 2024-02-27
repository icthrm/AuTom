#include "CoarseAlignWin.h"
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

CoarseAlignWin::CoarseAlignWin(QWidget* parent): QWidget(parent)
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
    connect(m_startButton , SIGNAL(clicked(bool)) , this , SLOT(StartCoarseAlign()));
}

CoarseAlignWin::~CoarseAlignWin()
{
  inputSaver->coarseAlignment_checkbox_saved = m_checkbox->checkState();
  inputSaver->coarseAlignment_spinBox_saved = m_spinBox->value();
  inputSaver->serialize();
}

void CoarseAlignWin::CreateImageFrame()
{
    m_imageFram = new QFrame(m_mainWidget);
    m_imageFram->setFrameStyle(QFrame::Panel|QFrame::Raised|QFrame::HLine);
    QGridLayout *l = new QGridLayout();
    m_Input = new CSkeleton(m_imageFram);
    m_Output = new CSkeleton(m_imageFram);
    
    l->addWidget(m_Input , 0 ,0);
    l->addWidget(m_Output , 0 ,1);
    m_imageFram->setLayout(l);
//     delete l;
    m_Input->SetText(tr("Input") , NULL , 170 , 150 ,CSkeleton::FullFill);
   m_Output->SetText(tr("Output") , NULL , 170 , 150 ,CSkeleton::FullFill);
}

void CoarseAlignWin::CreateButton()
{
    m_ButtonFrame = new QFrame(m_mainWidget);
    m_ButtonFrame->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    m_timesLabel = new QLabel("Iteration Times :  " ,m_ButtonFrame);
    m_spinBox = new QSpinBox(m_ButtonFrame);
    m_spinBox->setMinimum(1);
    m_spinBox->setValue(inputSaver->coarseAlignment_spinBox_saved);
    m_spinBox->setToolTip(ToolTips::cali_lbl_iter);
//     m_spinBox->setMaximumWidth(100);;
    m_startButton = new QPushButton("start" , m_ButtonFrame);
    m_startButton->setMaximumWidth(100);
    m_startButton->setToolTip(ToolTips::cali_btn_start);
    m_checkbox = new QCheckBox("Calculate Rotation" , m_ButtonFrame);
    m_checkbox->setCheckState(inputSaver->coarseAlignment_checkbox_saved);
    m_checkbox->setToolTip(ToolTips::cali_lbl_rot);
//     m_checkbox->setEnabled(false);
    
    QGridLayout* l = new QGridLayout();
    l->addWidget(m_timesLabel , 1 ,0 , 1 , 1);
    l->addWidget(m_spinBox , 1 , 1 , 1 ,1);
    l->addWidget(m_startButton ,1 ,5 , 1 ,1);
    l->addWidget(m_checkbox , 0 , 0 , 1 ,1);
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

void CoarseAlignWin::SetInputImage()
{  
     string filename=CProManage::GetInstance()->GetDir(PROCESS_DIR)+"_norm.st";
     fstream pfile;
     pfile.open(filename.c_str(), ios_base::in);
    if(CProManage::GetInstance()->Exite("PRE_MRC_DIR") &&(pfile)){
	m_inputImage = CProManage::GetInstance()->GetDir(PRE_MRC_DIR).data();
    }
    else 
      m_inputImage = CProManage::GetInstance()->GetDir(MRC_DIR).data();
    qDebug()<<"setinputimage = "<<m_inputImage;
    if(m_inputImage != "NULL"){
      QFileInfo file(m_inputImage);
      if(file.exists()){
	  ItemModel item;
	  ViewData view;
	  cout<<"item Open: "<<m_inputImage.toLatin1().data()<<" "<<CProManage::GetInstance()->GetDir(ANGLE_DIR).c_str()<<endl;
	 if( item.Open(m_inputImage.toLatin1().data() , CProManage::GetInstance()->GetDir(ANGLE_DIR).c_str()))
	 {
	  view.Mirror(item.Img());
	
	   m_Input->SetImage(view.MirrorImage() , 170 , 150);
	 }
	 else{
	   m_Input->setEnabled(false);
	   QMessageBox::warning(this , tr("Output Image") , tr("Can't open input file!")  ,QMessageBox::Ok);
	 }
      }
      else{
	 QMessageBox::warning(this , tr("Set Intput Image") , tr("MRC file not exist!")  ,QMessageBox::Ok);
      }
    }
    else{
      QMessageBox::warning(this , tr("Set Intput Image") , tr("MRC file not exist!")  ,QMessageBox::Ok);
//        m_Input->SetText(tr("Raw Image") , NULL , 170 , 150 ,CSkeleton::FullFill);
    }   
}

void CoarseAlignWin::SetOutputImage()
{
    cout<<"Set Output image"<<endl;
    m_outputImage = CProManage::GetInstance()->GetDir(PREALI_DIR).data();
    
    qDebug()<<m_outputImage;
    if(m_outputImage != "NULL"){
	QFileInfo file(m_outputImage);
	if(file.exists()){
	  ItemModel item;
	  ViewData view;
	 if( item.Open(m_outputImage.toLatin1().data() , CProManage::GetInstance()->GetDir(ANGLE_DIR).c_str()))
	 {
	    view.Mirror(item.Img());
	    m_Output->SetImage(view.MirrorImage() , 170 , 150);
	    m_Output->setEnabled(true);
	 }
	 else{
	   m_Output->setEnabled(false);
	   QMessageBox::warning(this , tr("Output Image") , tr("Can't open output file!")  ,QMessageBox::Ok);
	 }
	}
	else{
	  m_Output->setEnabled(false);
	  QMessageBox::warning(this , tr("Set Output Image") , tr("Coarse align file not exist!")  ,QMessageBox::Ok);
	}
    }
    else{     
      m_Output->setEnabled(false);
    }
}


void CoarseAlignWin::ShowInput()
{
//    ImageViewWindow* win = ImageViewWindow::ViewMrc(m_inputImage);
      SliceMainWindow* win = SliceMainWindow::ViewMrc(m_inputImage);
}

void CoarseAlignWin::ShowOutput()
{
//     QString  m_outputImage = CProManage::GetInstance()->GetDir(PREALI_DIR).data();
//    ImageViewWindow* win = ImageViewWindow::ViewMrc(m_outputImage);
     SliceMainWindow* win = SliceMainWindow::ViewMrc(m_outputImage);
}


void CoarseAlignWin::StartCoarseAlign()
{
       emit Start();    
    m_ls->setCurrentWidget(m_progressbar);
     if(m_outputImage != "NULL"){
    QFile file(m_outputImage);
    if(file.exists()){
      file.remove();
    }
    CProManage::GetInstance()->SetState(COSALI_STATE , UNFINISH);
    CProManage::GetInstance()->SetDir(PREALI_DIR , "NULL");
    CProManage::ReWriteProFile();
    SetOutputImage();
  }
    
    string inputimage = m_inputImage.toStdString();
    string processname = CProManage::GetInstance()->GetDir(PROCESS_DIR);
    int round = m_spinBox->value();
    string outputimage = CProManage::GetInstance()->GetDir(PROCESS_DIR) + "_CosAli.st";
    int rotation = 0;
    if(m_checkbox->checkState()){
      rotation =1;
    }
    
    m_preali_dir = outputimage.data();
    
    QString instruction = QString("\n%1\n%2\n%3\n%4\n%5").arg(inputimage.data()).arg(outputimage.data()).arg(round).arg(rotation).arg(processname.data());
    
    qDebug()<<"instruction : "<<instruction;
    msg_map::Message* msg = MMTApp::NewMessage(COARSE_ALIGN_MSG , this , instruction.toLatin1().data());
    MMTApp::PostMessage(msg);
}

void CoarseAlignWin::customEvent(QEvent* e )
{
           if(e->type() == msg_map::QSequelType){
	msg_map::CSequelEvent* sequel = static_cast<msg_map::CSequelEvent*>(e);
	if(sequel->msg_id == COARSE_ALIGN_MSG){
	    switch(sequel->state){
		case msg_map::MESSAGE:{
			QString message = (char *)sequel->data;
			emit Log(message);
			delete [] (char*)sequel->data;
		}break;
		case msg_map::RATE:{
		    int rate = (unsigned long)sequel->data;		
		}break;
		case msg_map::RESULT:{
		  m_ls->setCurrentWidget(m_ButtonFrame);
		    int state = (unsigned long)sequel->data;
		    if(sequel->data == (void*)-1){
		      QFile file(m_preali_dir);
		      if(file.exists()){
			qDebug()<<"remove";
			file.remove();
		      }
			CProManage::AddItem("COSALI_STATE" , "FAIL");
			emit RunError();
		    }
		    else if(sequel->data == (void*)1){
		        QFileInfo file(m_preali_dir);
			if(file.exists()){
			  CProManage::AddItem("PREALI_DIR" , m_preali_dir.toLatin1().data());
			  CProManage::AddItem("COSALI_STATE" , "FINISH");
			  SetOutputImage();
			  emit Finish("Coarse Align");
			}
			else{
			  QFile file(m_preali_dir);
			  if(file.exists()){
			    file.remove();
			  }
			  CProManage::AddItem("COSALI_STATE" , "FAIL");
			  emit RunError();
			}			
		    }
		}break;
	    }
	}
    }
    
    QObject::customEvent(e);
}

#include "CoarseAlignWin.moc"
