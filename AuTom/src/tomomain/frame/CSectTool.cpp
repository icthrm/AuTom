#include "CSectTool.h"
#include <cstring>
#include "core/msg_map.h"
#include "core/msg.h"
#include "custom/custom.h"
#include "geometry/calibrate.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "widget/Ctag.h"
#include "promanage/ProManage.h"
#include <string>
#include <stdlib.h>
/** CSectTool*/
CSectTool::CSectTool(ItemModel& im, QWidget* parent, Qt::WindowFlags f):_data(im), QWidget(parent, f)
{
//     QRect rect = QApplication::desktop()->availableGeometry();
//     skeleton_w = rect.width()*0.3;
//     skeleton_h = rect.height()*0.3;
    skeleton_w = 160;
    skeleton_h = 100;
    
    _view.Mirror(_data.Img());
    
    QGridLayout* l = new QGridLayout();
    
    m_scene = new CSkeleton(this);
    m_scene->SetText(QString("  %1 index: %2").arg(_data.Name()).arg(_data.Index()), _view.MirrorImage(), skeleton_w, skeleton_h, CSkeleton::Zoom);
    l->addWidget(m_scene, 0, 0);
    
    scene_copy = *(m_scene->Pixmap());
    
    m_slice[0] = new CSkeleton(this);
    m_slice[0]->SetText(tr("  slice1"), NULL, skeleton_w, skeleton_h, CSkeleton::Zoom);
    l->addWidget(m_slice[0], 0, 1);
    
    m_slice[1] = new CSkeleton(this);
    m_slice[1]->SetText(tr("  slice2"), NULL, skeleton_w, skeleton_h, CSkeleton::Zoom);
    l->addWidget(m_slice[1], 1, 0);
    
    m_slice[2] = new CSkeleton(this);
    m_slice[2]->SetText(tr("  slice3"), NULL, skeleton_w, skeleton_h, CSkeleton::Zoom);
    
    l->addWidget(m_slice[2], 1, 1);
    l->setMargin(0);
    setLayout(l);
    
    for(int i = 0; i < 3; i++){
	loc[i] = CProManage::getSettings()->geo_location[i];
	slice_dv[i].state = CSectTool::null;
	slice_dv[i].lines[0] = QLineF(0.15, 0.1, 0.85, 0.1);
	slice_dv[i].lines[1] = QLineF(0.15, 0.9, 0.85, 0.9);
    }
    
    connect(m_scene, SIGNAL(pressed()), this, SLOT(ShowSectView()));
    connect(m_slice[0], SIGNAL(pressed()), this, SLOT(ShowSlice1()));
    connect(m_slice[1], SIGNAL(pressed()), this, SLOT(ShowSlice2()));
    connect(m_slice[2], SIGNAL(pressed()), this, SLOT(ShowSlice3()));
    
    m_sectviewf = NULL;
}

CSectTool::~CSectTool()
{
  
}

void CSectTool::LoadSlice(int idx, const char* slice_name)
{
    if(! slice_dv[idx]._data.Open(slice_name)) return;	//changed by chice, origin: slice_dv[idx]._data.Open(slice_name);
    slice_dv[idx]._view.Mirror(slice_dv[idx]._data.Img());
    
    m_slice[idx]->SetText(tr(slice_name), slice_dv[idx]._view.MirrorImage(), skeleton_w, skeleton_h, CSkeleton::Zoom);
    slice_dv[idx].s_copy = *(m_slice[idx]->Pixmap());
    slice_dv[idx].state = CSectTool::init;
}

void CSectTool::ShowSectView()
{
    m_sectviewf = new CSectViewFrame(_data, loc, this);
    
    m_sectviewf->setWindowModality(Qt::ApplicationModal);
    m_sectviewf->setAttribute(Qt::WA_DeleteOnClose);
    m_sectviewf->show();
    connect(m_sectviewf, SIGNAL(destroyed(QObject*)), this, SLOT(UpdateScene()));
    
    
}

void CSectTool::UpdateScene()
{

    *(m_scene->Pixmap()) = scene_copy;
    QPixmap* pix = m_scene->Pixmap();
    QPainter* painter = new QPainter(pix);
    int width = pix->width();
    int height = pix->height();
    
    painter->setPen(QPen(QColor(0, 255, 0), 1, Qt::DashDotLine, Qt::RoundCap));
    for(int i = 0; i < 3; i++){
	if(loc[i] >= 0 && loc[i] <= 1){
	    painter->drawLine(0, loc[i]*height, width, loc[i]*height);
        CProManage::getSettings()->geo_location[i]=loc[i];
	}
    }
    
    m_scene->Update();
    delete painter;
}

void CSectTool::UpdateSliceSkeleton()
{
    *(m_slice[cur_slice_idx]->Pixmap()) = slice_dv[cur_slice_idx].s_copy;
    QPixmap* pix = m_slice[cur_slice_idx]->Pixmap();
    QPainter* painter = new QPainter(pix);
    int width = pix->width();
    int height = pix->height();
    
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(QColor(0, 255, 0), 1, Qt::SolidLine, Qt::RoundCap));
    for(int i = 0; i < 2; i++){
	painter->drawLine(slice_dv[cur_slice_idx].lines[i].x1()*width, 
		slice_dv[cur_slice_idx].lines[i].y1()*height, slice_dv[cur_slice_idx].lines[i].x2()*width, slice_dv[cur_slice_idx].lines[i].y2()*height);
    }
    
    slice_dv[cur_slice_idx].state = written;
    
    m_slice[cur_slice_idx]->Update();
    delete painter;
}

void CSectTool::ShowSlice1()
{
    ShowSlice(0);
}

void CSectTool::ShowSlice2()
{
    ShowSlice(1);
}

void CSectTool::ShowSlice3()
{
    ShowSlice(2);
}

void CSectTool::ShowSlice(int idx)
{
    if(slice_dv[idx].state == null){
	return;
    }
    
    cur_slice_idx = idx;
//     cout<<"## cur_slice_idx ="<< idx<<"##########################"<<endl;
      
    
    
    m_sliceviewf = new CSliceViewFrame(slice_dv[idx]._data, slice_dv[idx].lines, this);
    m_sliceviewf->setWindowModality(Qt::ApplicationModal);
    m_sliceviewf->setAttribute(Qt::WA_DeleteOnClose);
    
    connect(m_sliceviewf, SIGNAL(destroyed(QObject*)), this, SLOT(UpdateSliceSkeleton()));
    
    m_sliceviewf->show();    
}

bool CSectTool::IsCalculable()
{
    return slice_dv[0].state == written && slice_dv[1].state == written && slice_dv[2].state == written;
}

QLineF CSectTool::GetDemarcater(int idx, int lidx)
{
    return slice_dv[idx].lines[lidx];
}

void CSectTool::GetYLoc(float* yloc)
{
    memcpy(yloc, loc, sizeof(float)*3);
}
/*
void CSectTool::setSlicesEnable(bool enable){
  for(int i = 0; i < 3; i++){
    m_slice[i]->setEnabled(enable);
  }
}*/

/** CSectToolFrame*/
//==============================================================================================================

CSectToolFrame::CSectToolFrame(ItemModel& im, QWidget* parent, Qt::WindowFlags flags): QMainWindow(parent, flags), _data(im)
{   
    inputSaver = InputSaver::getInstance();
    
    setWindowTitle("section view");
    m_secttool = new CSectTool(_data);
    
    m_mainWidget = new QWidget(this);
    CreateImageFrame();
    CreateButton();
    CreateInputFrame();//初始化下方输入区域
    CreateAutogFrame();
  
  
  
    m_input_checkbox = new QCheckBox(tr("input parameters directly"));//勾选则显示输入区
    m_input_checkbox->setCheckState(inputSaver->geometry_input_checkbox_saved);
    connect(m_input_checkbox,SIGNAL(stateChanged(int)),this,SLOT(ChangeinputMethod()));
    
     
    m_autog_checkbox = new QCheckBox(tr("automatic boundary detection"));//勾选则显示输入区
    m_autog_checkbox->setCheckState(inputSaver->geometry_input_checkbox_saved);
    connect(m_autog_checkbox,SIGNAL(stateChanged(int)),this,SLOT(ChangeinputMethod_auto()));
    
    
    QGridLayout *l = new QGridLayout();
    l->addWidget(m_imageFram , 0 , 0 );
    l->addWidget(m_ButtonFrame , 1 , 0);
    l->addWidget(m_input_checkbox, 2, 0);
    l->addWidget(m_autog_checkbox, 3, 0);
    l->addWidget(m_InputFrame, 4, 0);
    l->addWidget(m_SetFram, 5, 0); 
    
    
    l->setMargin(0);
    l->setSpacing(0);
    
    m_mainWidget->setLayout(l);
     m_mainWidget->setSizePolicy(QSizePolicy::Fixed , QSizePolicy::Fixed);

   this->resize(250 , 250);
    
    QWidget* q = new QWidget(this);
    setCentralWidget(q);
    
    QGridLayout *l0 = new QGridLayout();
    l0->addWidget(m_mainWidget);
    l0->setMargin(3);
   q->setLayout(l0);
    
    QPalette palette;
    m_mainWidget->setAutoFillBackground(true);
    palette.setColor(QPalette::Background , QColor(200, 200, 200));
    m_mainWidget->setPalette(palette);
    
    palette.setColor(QPalette::Background , QColor(189, 189, 189));
    this->setAutoFillBackground(true);
    this->setPalette(palette);
    
//     QWidget* q = new QWidget(this);
//     QGridLayout* l = new QGridLayout();
//     q->setLayout(l);
//     l->addWidget(m_secttool);    
//     setCentralWidget(q);  
    
//     m_topbar = new QToolBar("Control");
//     addToolBar(Qt::TopToolBarArea, m_topbar);
//     m_topbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
//     m_topbar->setMovable(false);
//     
//     SetTopToolBar(m_topbar);
//    
//     QRect rect = QApplication::desktop()->availableGeometry();
//     this->move((rect.width()-this->width())/3, (rect.height()-this->height())/3);
//      ChangeinputMethod();
        setImageAndButtonFrameEnable(true);
        setInputFrameEnable(false);
        setAutogFrameEnable(false);
    
}

CSectToolFrame::~CSectToolFrame()
{
  inputSaver->geometry_input_checkbox_saved = m_input_checkbox->checkState();
  inputSaver->geometry_thickness_saved = m_thickness->text();
  inputSaver->geometry_zshift_saved = m_zshift->text();
  inputSaver->geometry_offset_saved = m_offset->text();
  inputSaver->geometry_pitch_saved = m_pitch->text();
  inputSaver->serialize();
}

void CSectToolFrame::CreateImageFrame()
{
  m_imageFram = new QFrame(m_mainWidget);
   m_imageFram->setFrameStyle(QFrame::Panel|QFrame::Raised|QFrame::HLine);
    QGridLayout *l = new QGridLayout();
    l->addWidget(m_secttool);
    l->setMargin(5);
//     m_secttool->resize(320 , 200);
    m_imageFram->setLayout(l);
//     m_imageFram->resize(320 ,200);
}

void CSectToolFrame::CreateButton()
{
  m_ButtonFrame = new QFrame(m_mainWidget);
  m_ButtonFrame->setFrameStyle(QFrame::Panel|QFrame::Sunken);
  m_Set = new QPushButton(tr("Set Thickness") , m_ButtonFrame);
  m_Set->setToolTip(ToolTips::geoi_btn_thick);
  m_Set->setIcon(QIcon(":/setting.png"));
  m_Calibrate = new QPushButton(tr("Calculate") , m_ButtonFrame);
  m_Calibrate->setIcon(QIcon(":/build.png"));
  m_Calibrate->setToolTip(ToolTips::geo_btn_calcu);
  QGridLayout* l = new QGridLayout();
  l->addWidget(m_Set , 0 , 0);;
  l->addWidget(m_Calibrate , 0 ,2);
  l->setMargin(0);
  m_ButtonFrame->setLayout(l);

   connect(m_Set , SIGNAL(clicked(bool)) , this , SLOT(SetSetWindow()));
   connect(m_Calibrate , SIGNAL(clicked(bool)) , this  , SLOT(DoCalibration()));
}
void CSectToolFrame::CreateAutogFrame()
{ 
   m_autogFrame = new QFrame(m_mainWidget);
   m_autogFrame->setFrameStyle(QFrame::Panel|QFrame::Sunken);
   m_autogdeterm = new QPushButton(tr("AutoBoundary&Calculate") , m_autogFrame);
   m_dataTypeBox = new QComboBox(m_autogFrame);
   m_dataTypeBox->insertItem(0,"cryo-ET or plastic embedded ET");
   m_dataTypeBox->insertItem(1,"negative stained  ET");
   m_dataTypeBox->setCurrentIndex(inputSaver->reconsruction_dataTypeBox_saved);
   
   QGridLayout* ll = new QGridLayout();
   ll->addWidget(m_dataTypeBox , 0 , 0);;
   ll->addWidget(m_autogdeterm , 0 ,2);
   ll->setMargin(0);
   m_autogFrame->setLayout(ll);
    connect(m_autogdeterm , SIGNAL(clicked(bool)) , this , SLOT(AutoG()));
    
    
    m_progressbar = new CProgressBar(m_mainWidget);
    m_ls = new QStackedLayout;
    m_ls->addWidget(m_autogFrame);
    m_ls->addWidget(m_progressbar);
    m_ls->setCurrentWidget(m_autogFrame);
    m_SetFram = new QFrame(m_mainWidget);
    m_SetFram->setLayout(m_ls);
   }
   
void CSectToolFrame::CreateInputFrame(){

  m_InputFrame = new QFrame(m_mainWidget);
  m_InputFrame->setFrameStyle(QFrame::Panel|QFrame::Sunken);
  //m_InputFrame->setFixedSize(200, 200);
  //m_InputFrame->setAlignment(Qt::AlignCenter);
  
  m_thickness = new QLineEdit(m_InputFrame);
  m_zshift = new QLineEdit(m_InputFrame);
  m_offset = new QLineEdit(m_InputFrame);
  m_pitch = new QLineEdit(m_InputFrame);
  
  QDoubleValidator * doubleValidator1 = new QDoubleValidator(0.0, z_o_p_range, z_o_p_decimals, this);
  doubleValidator1->setNotation(QDoubleValidator::StandardNotation);
  QDoubleValidator * doubleValidator2 = new QDoubleValidator(-z_o_p_range, z_o_p_range, z_o_p_decimals, this);
  doubleValidator2->setNotation(QDoubleValidator::StandardNotation);
  
  m_thickness->setValidator(doubleValidator1);
  m_zshift->setValidator(doubleValidator2);
  m_offset->setValidator(doubleValidator2);	
  m_pitch->setValidator(doubleValidator2);
  
  m_thickness->setText(inputSaver->geometry_thickness_saved);
  m_zshift->setText(inputSaver->geometry_zshift_saved);
  m_offset->setText(inputSaver->geometry_offset_saved);
  m_pitch->setText(inputSaver->geometry_pitch_saved);
  
  m_thickness_label = new QLabel(tr("thickness: "), m_InputFrame);
  m_zshift_label = new QLabel(tr("zshift: "), m_InputFrame);
  m_offset_label = new QLabel(tr("offset: "), m_InputFrame);
  m_pitch_label = new QLabel(tr("pitch: "), m_InputFrame);
  
  m_inputFinish = new QPushButton(tr("Done"), m_InputFrame);
  connect(m_inputFinish, SIGNAL(clicked(bool)), this, SLOT(inputProcess()));
  
  QGridLayout * layout = new QGridLayout();
  layout->addWidget(new QLabel("                   "), 0, 0);//美观
  layout->addWidget(m_thickness_label, 1, 1);
  layout->addWidget(m_thickness, 1, 2);
  layout->addWidget(m_zshift_label, 2, 1);
  layout->addWidget(m_zshift, 2, 2);
  layout->addWidget(m_offset_label, 3, 1);
  layout->addWidget(m_offset, 3, 2);
  layout->addWidget(m_pitch_label, 4, 1);
  layout->addWidget(m_pitch, 4, 2);
  layout->addWidget(m_inputFinish, 5, 3);
  m_InputFrame->setLayout(layout);

}

void CSectToolFrame::setImageAndButtonFrameEnable(bool enable){
  m_imageFram->setEnabled(enable);
  //m_secttool->setSlicesEnable(enable);
  m_ButtonFrame->setEnabled(enable);
}

void CSectToolFrame::setInputFrameEnable(bool enable){
  m_InputFrame->setVisible(enable);
}

void CSectToolFrame::setAutogFrameEnable(bool enable){
//   m_autogFrame->setVisible(enable);
  m_SetFram->setVisible(enable);
}

void CSectToolFrame::SetTopToolBar(QToolBar* toolbar)
{    
    calib = new QAction(QIcon(":/build.png"), tr("Calculate"), this);
    calib->setShortcut(tr("Ctrl+C"));
    
    auto_demar = new QAction(QIcon(":/auto.png"), tr("Auto"), this);
    auto_demar->setShortcut(tr("Ctrl+A"));
    auto_demar->setDisabled(true);
    
    to_set = new QAction(QIcon(":/setting.png"), tr("Setting"), this);
    to_set->setShortcut(tr("Ctrl+S"));
    
    toolbar->addAction(calib);
    toolbar->addAction(auto_demar);
    toolbar->addAction(to_set);
    
    connect(calib, SIGNAL(triggered()), this, SLOT(DoCalibration()));
    connect(auto_demar, SIGNAL(triggered()), this, SLOT(AutoDemarcate()));
    connect(to_set, SIGNAL(triggered(bool)),this, SLOT(SetSetWindow()));
}

const QLineF& CSectToolFrame::FullLine(const QLineF& itm)
{
    static QLineF l;
    l = QLineF(0, itm.y1()-itm.x1()*(itm.y2()-itm.y1())/(itm.x2()-itm.x1()), 1, itm.y1()+(1-itm.x1())*(itm.y2()-itm.y1())/(itm.x2()-itm.x1()));
    
    return l;
}

void CSectToolFrame::AutoG() 
{ 
  QString path=CEnv::ExeBinDirectory();
  QString instruction= path+"./AutoGDeterm";
   
  string inputfile1,inputfile2; 
 
  
  QFileInfo file(m_outputpar); 
  m_outputpar="auto_geometry.par";
   
  emit Start();
  QString processDir=CProManage::GetInstance()->GetDir(PROCESS_DIR).data();
  inputfile1=CProManage::GetInstance()->GetDir(PROCESS_DIR)+"_fin.mrc";
  inputfile2=CProManage::GetInstance()->GetDir(PROCESS_DIR)+"_new.tlt";
  QString dataType=QString::number(m_dataTypeBox->currentIndex()+1) ;         
  
  instruction.append(" -input ").append(inputfile1.data())
     .append(" -tiltfile ").append(inputfile2.data())
     .append(" -output auto_geometry.par")
     .append(" -dataType ").append(dataType);
      
     
 qDebug()<<"instruction : "<<instruction;
 
//  writeFile_geo();
 
  m_ls->setCurrentWidget(m_progressbar);
  msg_map::Message* msg = MMTApp::NewMessage(AUTOGDETERM_MSG, this, instruction.toLatin1().data());
  MMTApp::PostMessage(msg); 
 
}

void CSectToolFrame::DoCalibration()
{

  if(!m_secttool->IsCalculable()){
      QMessageBox::warning(this, "Warning", "No model to calibrate!");
      return;
  }
    float yloc[3];
    float lz[12];
    m_secttool->GetYLoc(yloc);
    for(int i = 0; i < 3; i++){
	for(int j = 0; j < 2; j++){
	    QLineF l = FullLine(m_secttool->GetDemarcater(i, j));
	    lz[i*4+j*2] = l.y1();
	    lz[i*4+j*2+1] = l.y2();
	}
    }

//     ss<<"y axis: ";
//     for(int i = 0; i < 3; i++){
// ss<<yloc[i]<<" ";
//     }
//     ss<<std::endl;
//     ss<<"z axis: ";
//     for(int i = 0; i < 12; i++){
// ss<<lz[i]<<" ";
//     }
//     ss<<std::endl;
    
    float z_shift, offset, thickness, pitch_angle;
    Calibrate(yloc, lz, CDoc::GetSectSetting().Thickness(), CDoc::GetItemModel().Width(), CDoc::GetItemModel().Height(), &z_shift, &offset, &thickness, &pitch_angle);
//    std::stringstream ss;
//    ss<<"THICKNESS "<<thickness<<std::endl;
//    ss<<"ZSHIFT "<<z_shift<<std::endl;
//    ss<<"OFFSET "<<offset*180/M_PI<<std::endl;
//    ss<<"PITCH "<<pitch_angle*180/M_PI<<std::endl;
//    std::ofstream out("geometry.par");
//    out<<ss.str();
//    out.close();
//    QMessageBox::information(this, "Result", ss.str().c_str());
    writeFile(z_shift, offset, thickness, pitch_angle, 0);
    
//    if(CProManage::GetInstance()->GetMethod()=="BasedMethod"){
//	CProManage::AddItem("GERMETRY_STATE_BASED" , "FINISH");
//	emit Finish("GeometryBased");
//   }
//    else{
//	CProManage::AddItem("GERMETRY_STATE" , "FINISH");
//	emit Finish("Geometry");
//    }
    finishGeometry();
}

void CSectToolFrame::writeFile(float z_shift, float offset, float thickness, float pitch_angle, int inputmethod){//写入geometry.par文件
  std::stringstream ss;
  ss<<"THICKNESS "<<fixed<<setprecision(6)<<thickness<<std::endl;
  ss<<"ZSHIFT "<<fixed<<setprecision(6)<<z_shift<<std::endl;
 
  if(0 == inputmethod){
    ss<<"OFFSET "<<offset*180/M_PI<<std::endl;   //hudu->du
    ss<<"PITCH "<<pitch_angle*180/M_PI<<std::endl;
  }
  else{ //inputmethod= 1,inputs
    ss<<"OFFSET "<<fixed<<setprecision(6)<<offset<<std::endl; //du
    ss<<"PITCH "<<fixed<<setprecision(6)<<pitch_angle<<std::endl;
  }
  std::ofstream out("geometry.par");
  out<<ss.str();
  out.close();
  QMessageBox::information(this, "Result", ss.str().c_str());
}

void CSectToolFrame::writeFile_geo(){ 
   
   string z_shift, offset,  thickness, pitch_angle; 
   string filename="auto_geometry.par"; 
   ifstream in;
   in.open(filename.c_str());
   string st; 
            for(int i=0;; i++) {
                in>>st;
                if(st=="thickness") {
                    in>>st;in>>st;
                    if(isNum(st)) thickness=st ;  
                }
                if(st=="zshift") {
                    in>>st;in>>st;
                    if(isNum(st)) z_shift=st ;
                }
                if(st=="tilt") {
                   in>>st; in>>st; in>>st; in>>st; 
                   if(isNum(st))   offset=st ; 
                }
                if(st=="angle") {
                    in>>st;in>>st;
                    if(isNum(st)) pitch_angle=st;
                }
                if(in.eof()) break;
            }
        writeFile(QString(z_shift.data()).toDouble(), QString(offset.data()).toDouble(), QString(thickness.data()).toDouble(),QString(pitch_angle.data()).toDouble(), 1);
        
        QFile file(m_outputpar);
        if(file.exists())   file.remove();
        
        }
void CSectToolFrame::finishGeometry(){//完成geometry步骤
  if(CProManage::GetInstance()->GetMethod()=="BasedMethod"){
	CProManage::AddItem("GERMETRY_STATE_BASED" , "FINISH");
	emit Finish("GeometryBased");
    }
    else{
	CProManage::AddItem("GERMETRY_STATE" , "FINISH");
	emit Finish("Geometry");
    }
}

void CSectToolFrame::AutoDemarcate()
{
//     msg_map::Message* msg = MMTApp::NewMessage(BUILD_MODEL_MSG, this, instruction.toLatin1().data());
//     MMTApp::PostMessage(msg);
}

void CSectToolFrame::StopAutoDemarcate()
{
//     MMTApp::AbortCurrentProc();
}
 

void CSectToolFrame::closeEvent(QCloseEvent* e)
{
    QWidget::closeEvent(e);
}

//@ wang lian shan
void CSectToolFrame::Setting(QAbstractButton* button)//若thickness>=400则设置thickness值，否则弹出警告窗口
{
    if(button == buttonBox->button(QDialogButtonBox::Ok)){
	if(spinBox->value() >= 400){
	    CDoc::GetSectSetting().SetThickness(spinBox->value());
	    m_setwindow->close();
	}
	else{
	    QMessageBox::StandardButton rb = QMessageBox::warning(m_setwindow , tr("Warning!"), tr("The thickness is less than 400\n"),  QMessageBox::Cancel|QMessageBox::Ok);
	    if(rb == QMessageBox::Ok){
		CDoc::GetSectSetting().SetThickness(spinBox->value());
		m_setwindow->close();
	    }
	    else if(rb == QMessageBox::Cancel){
		spinBox->setValue(CDoc::GetSectSetting().Thickness());
	    }
	}
    }
    else{
	m_setwindow->close();
    }
    
}

void CSectToolFrame::ChangeinputMethod(){//改变输入方法（图像/直接输入/auto）
  bool isChecked = m_input_checkbox->isChecked();
  bool isChecked2 = m_autog_checkbox->isChecked();
  if(isChecked){
  m_autog_checkbox->setCheckState(Qt::Unchecked);
  setImageAndButtonFrameEnable(!isChecked);
  setInputFrameEnable(isChecked);
  setAutogFrameEnable(!isChecked);
  }
  else if(isChecked2==false)//none of them checked
  { 
 
   setImageAndButtonFrameEnable(true);
   setInputFrameEnable(false);
   setAutogFrameEnable(false);
  }
  //if(isChecked) inputmethod = 1;
  //else inputmethod = 0;
}

void CSectToolFrame::ChangeinputMethod_auto(){//改变输入方法（图像/直接输入/auto）
  bool isChecked = m_autog_checkbox->isChecked();
  bool isChecked2 = m_input_checkbox->isChecked();
  if(isChecked){
  m_input_checkbox->setCheckState(Qt::Unchecked);
  setImageAndButtonFrameEnable(!isChecked);
  setInputFrameEnable(!isChecked);
  setAutogFrameEnable(isChecked);
  }
  else if(isChecked2==false)//none of them checked
   {
   setImageAndButtonFrameEnable(true);
   setInputFrameEnable(false);
   setAutogFrameEnable(false);
  }
}

void CSectToolFrame::inputProcess(){//处理输入
  float thickness = m_thickness->text().toDouble();
  float z_shift = m_zshift->text().toDouble();
  float offset = m_offset->text().toDouble();
  float pitch_angle = m_pitch->text().toDouble();
  writeFile(z_shift, offset, thickness, pitch_angle, 1);
  finishGeometry();
    
}

//@ wang lian shan
void CSectToolFrame::SetSetWindow()//显示设置Thickness窗口
{
    m_setwindow = new QDialog;
    m_setwindow->resize(260,100);
    m_setwindow->setFixedSize(260,100);

    QLabel *label;
 
    m_setwindow->setWindowTitle(tr("Set Thickness"));;
    
     buttonBox = new QDialogButtonBox(m_setwindow);
     buttonBox->setGeometry(QRect(40, 60, 170, 32));
     buttonBox->setOrientation(Qt::Horizontal);
     buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    
    label = new QLabel(m_setwindow);
    label->setText(tr("Thickness: "));
    label->setGeometry(QRect(32,22,90,17));
    spinBox = new QSpinBox(m_setwindow);
    spinBox->setGeometry(QRect(130, 20, 120, 27));
    spinBox->setRange(0,10000);
    spinBox->setValue(CDoc::GetSectSetting().Thickness());
    
    connect(buttonBox , SIGNAL(clicked(QAbstractButton*)) , this , SLOT(Setting(QAbstractButton*)));
    m_setwindow->setAttribute(Qt::WA_DeleteOnClose);//设置关闭窗口时delete
    m_setwindow->show();
}

int CSectToolFrame::isNum(string s)
{
    for(int i=0; i<s.length(); i++) {
        if((s.at(i)>'9'||s.at(i)<'0')&&s.at(i)!='.'&&s.at(i)!='-') return 0;
    }
    return 1;
}

void CSectToolFrame::customEvent(QEvent* e)
{ 
  if(e->type() == msg_map::QSequelType) { 
        msg_map::CSequelEvent* sequel = static_cast<msg_map::CSequelEvent*>(e);
        if(sequel->msg_id == AUTOGDETERM_MSG) { 
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
                m_ls->setCurrentWidget(m_autogFrame);
                int* state =static_cast<int*>(sequel->data);
               
                if(state == PTHREAD_CANCELED) {                   
                    QFile file(m_outputpar);
                    if(file.exists()) {
                        file.remove();
                    }
                    if(CProManage::GetInstance()->GetMethod()=="BasedMethod") {
                        CProManage::AddItem("GERMETRY_STATE_BASED" , "FAIL");
                    }
                    else {
                        CProManage::AddItem("GERMETRY_STATE" , "FAIL");
                    }

                    emit RunError(); 
                }
                else if(*state == 0) {      
                    QFileInfo file(m_outputpar);
                    if(file.exists()) {                           
                        if(CProManage::GetInstance()->GetMethod()=="BasedMethod") {
                        CProManage::AddItem("GERMETRY_STATE_BASED" , "FINISH"); 
                        emit Finish("GeometryBased"); 
                        
                        writeFile_geo();
                        }
                        else {
                        CProManage::AddItem("GERMETRY_STATE" , "FINISH");  
                        emit Finish("Geometry");
                         writeFile_geo();
                        }          
                        
                    }
                    else {      
                        QFile file(m_outputpar);
                        if(file.exists()) {  
                            file.remove();
                        }
                      
                      if(CProManage::GetInstance()->GetMethod()=="BasedMethod") {
                        CProManage::AddItem("GERMETRY_STATE_BASED" , "FAIL"); 
                         }
                    else { CProManage::AddItem("GERMETRY_STATE" , "FAIL");
                        CProManage::AddItem("GERMETRY_STATE" , "FAIL");
                         }
                        emit RunError(); 
                    }
                delete state;
                }
                else{          
                 
                 // the child process return a non-zero value
                    if(CProManage::GetInstance()->GetMethod()=="BasedMethod") {
                        CProManage::AddItem("GERMETRY_STATE_BASED" , "FAIL");
                        emit RunError();
                    }
                    else {
                        CProManage::AddItem("GERMETRY_STATE" , "FAIL");
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
#include "CSectTool.moc"
