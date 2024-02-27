#include "PostProcessWin.h"
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
using namespace std;

PostProcessWin::PostProcessWin(QWidget* parent, Qt::WindowFlags f): QWidget(parent, f)
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
    
    connect(m_Input , SIGNAL(clicked(bool)) , this , SLOT(ShowInput()));
    connect(m_Output , SIGNAL(clicked(bool)) , this , SLOT(ShowOutput()));
    connect(m_startButton , SIGNAL(clicked(bool)) , this , SLOT(StartPostProcess()));
}

PostProcessWin::~PostProcessWin()
{

}

void PostProcessWin::CreateImageFrame()
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

void PostProcessWin::CreateButton()
{
  m_ButtonFrame = new QFrame(m_mainWidget);
    m_ButtonFrame->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    m_timesLabel = new QLabel("Filter Times :  " ,m_ButtonFrame);
    m_spinBox = new QSpinBox(m_ButtonFrame);
    m_spinBox->setValue(1);
//     m_spinBox->setMaximumWidth(100);;
    m_startButton = new QPushButton("start" , m_ButtonFrame);
    m_startButton->setMaximumWidth(100);
//     m_checkbox = new QCheckBox("Calculate Rotation" , m_ButtonFrame);
//     m_checkbox->setCheckState(Qt::Checked);
//     m_checkbox->setEnabled(false);
    
    QGridLayout* l = new QGridLayout();
    l->addWidget(m_timesLabel , 0 ,0 , 1 , 1);
    l->addWidget(m_spinBox , 0 , 1 , 1 ,1);
    l->addWidget(m_startButton ,0 ,5 , 1 ,1);
//     l->addWidget(m_checkbox , 1 , 0 , 1 ,1);
//     delete l;
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

void PostProcessWin::SetInputImage()
{ 
  if(CProManage::GetInstance()->Exite("RES_MRC_DIR")){
	m_inputImage = CProManage::GetInstance()->GetDir(RES_MRC_DIR).data();
  }
  
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

void PostProcessWin::SetOutputImage()
{
  m_outputImage = CProManage::GetInstance()->GetDir(POST_MRC_DIR).data();
    
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
	  QMessageBox::warning(this , tr("Set Output Image") , tr("Filter file not exist!")  ,QMessageBox::Ok);
	}
    }
    else{     
      m_Output->setEnabled(false);
    }
}

void PostProcessWin::ShowInput()
{
    VolumeMainWindow* win = VolumeMainWindow::ViewMrc(m_inputImage);

//  util::MrcStack* images = new util::MrcStack;
//  images->Open(m_inputImage.toLatin1().data());
//   XyzWindow *foo = new XyzWindow(images);
//    foo->setAttribute(Qt::WA_DeleteOnClose);
//        foo->show();
//    delete images;
}

void PostProcessWin::ShowOutput()
{
    VolumeMainWindow* win = VolumeMainWindow::ViewMrc(m_outputImage);

//  util::MrcStack* images = new util::MrcStack;
//  images->Open(m_outputImage.toLatin1().data());
//   XyzWindow *foo = new XyzWindow(images);
//    foo->setAttribute(Qt::WA_DeleteOnClose);
//        foo->show();
//    delete images;
}

void PostProcessWin::StartPostProcess()
{
  emit Start();    
    m_ls->setCurrentWidget(m_progressbar);
     if(m_outputImage != "NULL"){
    QFile file(m_outputImage);
    if(file.exists()){
      file.remove();
    }
    if(CProManage::GetInstance()->GetMethod()=="BasedMethod"){
	CProManage::GetInstance()->SetState(POSTPROCESS_STATE_BASED,UNFINISH);
    }
    else{
	CProManage::GetInstance()->SetState(POSTPROCESS_STATE , UNFINISH);
    }
    
    CProManage::GetInstance()->SetDir(POST_MRC_DIR , "NULL");
    CProManage::ReWriteProFile();
    SetOutputImage();
  }
    
    string inputimage = m_inputImage.toStdString();
    int round = m_spinBox->value();
    stringstream ss;
    string str;
    ss<<round;
    ss>>str;
    string outputimage = CProManage::GetInstance()->GetDir(PROCESS_DIR) + "_filter.mrc";
    m_outputImage = outputimage.data();
    
    QString instruction = QString("\n%1\n%2\n%3").arg(inputimage.data()).arg(outputimage.data()).arg(round);
    outputimage = CProManage::GetInstance()->GetDir(PROCESS_DIR) + "_filter_"+str+".mrc";//!!
    m_outputImage = outputimage.data();
    qDebug()<<"instruction : "<<instruction;
    msg_map::Message* msg = MMTApp::NewMessage(POSTPROCESS_MSG , this , instruction.toLatin1().data());
    MMTApp::PostMessage(msg);
}

void PostProcessWin::customEvent(QEvent* e)
{
  if(e->type() == msg_map::QSequelType){
	msg_map::CSequelEvent* sequel = static_cast<msg_map::CSequelEvent*>(e);
	if(sequel->msg_id == POSTPROCESS_MSG){
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
		      QFile file(m_outputImage);
		      if(file.exists()){
			qDebug()<<"remove";
			file.remove();
		      }
		      if(CProManage::GetInstance()->GetMethod()=="BasedMethod"){
			  CProManage::AddItem("POSTPROCESS_STATE_BASED" , "FAIL");   
		      }
		      else{
			  CProManage::AddItem("POSTPROCESS_STATE" , "FAIL");     
		      }
			emit RunError();
		    }
		    else if(sequel->data == (void*)1){
		                                                                                                       
		      QFileInfo file(m_outputImage); //!!
			if(file.exists()){
			  CProManage::AddItem("POST_MRC_DIR" , m_outputImage.toLatin1().data());SetOutputImage();
			  if(CProManage::GetInstance()->GetMethod()=="BasedMethod"){
			      CProManage::AddItem("POSTPROCESS_STATE_BASED" , "FINISH");
			      emit Finish("PostProcessingBased");
			  }
			  else{
			      CProManage::AddItem("POSTPROCESS_STATE" , "FINISH");       
			      emit Finish("PostProcessing");
			  }
			  
			}
			else{
			  QFile file(m_outputImage);
			  if(file.exists()){
			    file.remove();
			  }
			  if(CProManage::GetInstance()->GetMethod()=="BasedMethod"){
			      CProManage::AddItem("POSTPROCESS_STATE_BASED" , "FAIL");    
			  }
			  else{
			      CProManage::AddItem("POSTPROCESS_STATE" , "FAIL");    
			  }
			  emit RunError();
			}			
		    }
		}break;
	    }
	}
    }
  QObject::customEvent(e);
}


#include "PostProcessWin.moc"
