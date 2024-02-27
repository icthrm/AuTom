#include "XyzWindow.h"
#include "b3dgfx.h"

#include "mrcimg/img_util.h"
#include "widget/LoadImgWin.h"
#include "func.h"
//#include <QtGui>
#include <QtWidgets>
#include <math.h>
// #include <GL/glaux.h>

#define BM_WIDTH 16
#define BM_HEIGHT 16
#define XYZ_BSIZE 6
#define XYZ_GSIZE 6
#define ALL_BORDER (2 * XYZ_BSIZE + XYZ_GSIZE)
#define FRAM_BORDER 3
#define GRAB_LENGTH 7
#define GRAB_WIDTH 3
#define XYZ_TOGGLE_RESOL 0
#define XYZ_TOGGLE_LOCKED 1
#define MAX_SLIDER_WIDTH 100
#define MIN_SLIDER_WIDTH 20
#define NOTNEW -999999999

XyzWindow::XyzWindow(util::MrcStack* vi ,QWidget* parent, Qt::WindowFlags flags ): QMainWindow(parent, flags)
{
  //inite Tool Line
  qDebug()<<"New XyzWindow";
  setAttribute(Qt::WA_DeleteOnClose);
  setAttribute(Qt::WA_AlwaysShowToolTips);
  setAnimated(false);
  mVi   = vi;
  nx = mVi->Width();
  ny = mVi->Height();
  nz = mVi->Size();
//   qDebug()<<"here";
  cx = 0;
  cy = 0;
  cz = nz/2;
  mZoom = 1;
  mx = nx*mZoom;
  my = ny*mZoom;
  mz = nz*mZoom;
  mXorigin1 = mz + XYZ_BSIZE + XYZ_GSIZE + 2*FRAM_BORDER;
  mYorigin1 = XYZ_BSIZE;
  mXorigin2 = XYZ_BSIZE;
  mYorigin2 = mz + XYZ_BSIZE + XYZ_GSIZE + 2*FRAM_BORDER;
  mWidth = mz+mx+ALL_BORDER+4*FRAM_BORDER;
  mHeight = mz + my + ALL_BORDER +40+4*FRAM_BORDER+20;
  
  //对于过大图像，要定义一个新mzoom值
  if(mHeight > 700 || mWidth > 900){
    int w = mWidth;
    int h = mHeight;
    float d = 0.25;
    while(1){
      if(mZoom>d)
      mZoom -= d;
      else{
	d/=5;
	mZoom-=d;
      }
      mHeight = h*mZoom;
      mWidth = w*mZoom;
      qDebug()<<"window size : "<<mWidth<<"*"<<mHeight;
      if(mHeight <= 900 && mWidth <=1100)
	break;
    }
    mx = nx*mZoom;
  my = ny*mZoom;
  mz = nz*mZoom;
  mXorigin1 = mz + XYZ_BSIZE + XYZ_GSIZE + 2*FRAM_BORDER;
  mYorigin1 = XYZ_BSIZE;
  mXorigin2 = XYZ_BSIZE;
  mYorigin2 = mz + XYZ_BSIZE + XYZ_GSIZE + 2*FRAM_BORDER;
  mWidth = mz+mx+ALL_BORDER+4*FRAM_BORDER;
  mHeight = mz + my + ALL_BORDER +40+4*FRAM_BORDER+20;
  }
  
  m_OpenZoom = mZoom;
  m_RelativeZoom = mZoom/m_OpenZoom;
  wx = mx;
  wy = my;
  wz = mz;
  
  qDebug()<<"mzoom : "<<mZoom;
  resize(mWidth , mHeight);
   mFdatayz = new unsigned char[nz*ny];
  mFdataxz = new unsigned char[nx*nz];
  mFdataxy = new unsigned char[nx*ny];
//   mCurView = -1;
  m_imginfo = new ImgInfo(this);
  m_imginfo->setAttribute(Qt::WA_DeleteOnClose);
  
  LoadImageData();
  CreateActions();
  CreateToolBars();  
  CreateStatusBar();
  CreateXYZView();
  
  m_XSlider->setRange(0 , nx-1);
  m_XSlider->setValue(cx);
  m_YSlider->setRange(0,ny-1);
  m_YSlider->setValue(cy);
  m_ZSlider->setRange(0,nz-1);
  m_ZSlider->setValue(cz);
  qDebug()<<"here";
  connect(m_imginfo , SIGNAL(destroyed(QObject*)) , this , SLOT(close()));
    connect(m_imginfo , SIGNAL(Adjust()) , this , SLOT(On_Adjust()));
    
    QRect rect = QApplication::desktop()->availableGeometry();
  move((rect.width()-mWidth)/2 , (rect.height()-mHeight)/2);
  show();
}

XyzWindow::~XyzWindow()
{
  qDebug()<<"~XyzWindow";
  if(mFdataxz){
    delete [] mFdataxz;
  }
  if(mFdatayz)
    delete [] mFdatayz;
  if(mFdataxy)
    delete [] mFdataxy;
  for(int i = 0;i<nz ; i++)
    delete []  mData[i];
  delete [] mData;
}

void XyzWindow::CreateActions()
{
  m_EnlargeAction = new QAction(QIcon(":/images/zoomin.png"),tr("放大"),this);
  connect(m_EnlargeAction,SIGNAL(triggered()) , this , SLOT(zoomUp()));
  
  m_ShrinkAction = new QAction(QIcon(":/images/zoomout.png") , tr("缩小") , this);
  connect(m_ShrinkAction , SIGNAL(triggered()) , this , SLOT(zoomDown()));
}

void XyzWindow::CreateToolBars()
{
  m_EnlargeTool = addToolBar("放大");
  m_EnlargeTool->addAction(m_EnlargeAction);
  m_EnlargeTool->setMovable(false);
  
  m_ShrinkTool = addToolBar("缩小");
  m_ShrinkTool->addAction(m_ShrinkAction);
  m_ShrinkTool->setMovable(false);
  
  m_SizeLineEdit = new QLineEdit;
  m_SizeLineEdit->setReadOnly(true);
  m_SizeLineEdit->setMaximumWidth(70);
  m_SizeLineEdit->setText(QString::number(mZoom));
  m_ShrinkTool->addWidget(m_SizeLineEdit);
  
  m_XLable = new QLabel(tr("  X"));
  QFont ft;
  ft.setPointSize(12);
  m_XLable->setFont(ft);
  m_ShrinkTool->addWidget(m_XLable);
  m_XSlider = new QSlider(Qt::Horizontal);
  m_XSlider->setMaximumWidth(200);
  m_ShrinkTool->addWidget(m_XSlider);
  
  m_YLable = new QLabel(tr("  Y"));
  m_YLable->setFont(ft);
  m_ShrinkTool->addWidget(m_YLable);
  m_YSlider = new QSlider(Qt::Horizontal);
  m_YSlider->setMaximumWidth(200);
  m_ShrinkTool->addWidget(m_YSlider);
  
   m_ZLable = new QLabel(tr("  Z"));
  m_ZLable->setFont(ft);
  m_ShrinkTool->addWidget(m_ZLable);
  m_ZSlider = new QSlider(Qt::Horizontal);
  m_ZSlider->setMaximumWidth(200);
  m_ShrinkTool->addWidget(m_ZSlider);
  connect(m_XSlider , SIGNAL(sliderMoved(int)) , this , SLOT(XSliderChanged(int)));
  connect(m_YSlider , SIGNAL(sliderMoved(int)) , this , SLOT(YSliderChanged(int)));
  connect(m_ZSlider , SIGNAL(sliderMoved(int)) , this , SLOT(ZSliderChanged(int)));
}

void XyzWindow::CreateStatusBar()
{
  m_CurView = new QLabel();
  m_Infomation = new QLabel();
  m_Infomation->setAlignment(Qt::AlignHCenter);
//   statusBar()->addWidget(m_Infomation);
  statusBar()->setFixedHeight(20);
  
  statusBar()->addWidget(m_CurView , 5);
//   statusBar()->showMessage("132");
  QString text = "size = "+QString::number(nx)+"*"+QString::number(ny) + "  z = "+QString::number(nz);
  m_CurView->setText(text);
//   statusBar()->showMessage("123" );
}

// void XyzWindow::CreateDockWidget()
// {
//    QDockWidget *YZDockWidget = new QDockWidget( this);  
//    YZDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
//    
//    QGLFormat glFormat;
//    glFormat.setRgba(true);
//    glFormat.setDoubleBuffer(true);
//    glFormat.setDepth(true);
//    mYZ_GLw = new XyzGL(glFormat , this);
//    mYZ_GLw->resize(nz , ny);
//    YZDockWidget->setWidget(mYZ_GLw);
//    qdebug
//    addDockWidget(Qt::LeftDockWidgetArea , YZDockWidget);
// 
//    QDockWidget *XZDockWidget = new QDockWidget(tr("xz") , this);
//    XZDockWidget->setAllowedAreas(Qt::TopDockWidgetArea|Qt::BottomDockWidgetArea);
//    mXZ_GLw = new XyzGL(glFormat , this);
//    mXZ_GLw->resize(nx , nz);
//    XZDockWidget->setWidget(mXZ_GLw);
//    addDockWidget(Qt::TopDockWidgetArea ,XZDockWidget);
//    
//    this->setCorner(Qt::TopLeftCorner , Qt::LeftDockWidgetArea);
//    resize(mXY_GLw->width()+YZDockWidget->width() , mXY_GLw->height()+XZDockWidget->height());
// }

void XyzWindow::CreateXYZView()
{
  QFrame *XyFrame;
  QFrame *XzFrame;
  QFrame *ZyFrame;
  QWidget *centralWidget;
  
  centralWidget = new QWidget(this);
  QPalette palette;
  palette.setColor(QPalette::Background , QColor(/*200,200,200*/167, 210, 200));
  centralWidget->setAutoFillBackground(true);
  centralWidget->setPalette(palette);
  
  XzFrame = new QFrame(centralWidget);
  XzFrame->setGeometry(QRect(mXorigin1,mYorigin1 , mx+2*FRAM_BORDER , mz+2*FRAM_BORDER));
  XzFrame->setFrameShape(QFrame::StyledPanel);
  XzFrame->setFrameShadow(QFrame::Raised);
  ZyFrame = new QFrame(centralWidget);
  ZyFrame->setGeometry(QRect(mXorigin2 , mYorigin2 , mz+2*FRAM_BORDER ,my+2*FRAM_BORDER));
  ZyFrame->setFrameShape(QFrame::StyledPanel);
  ZyFrame->setFrameShadow(QFrame::Raised);
  XyFrame = new QFrame(centralWidget);
  XyFrame->setGeometry(QRect(mXorigin1 , mYorigin2 , mx+2*FRAM_BORDER  ,my+2*FRAM_BORDER));
  XyFrame->setFrameShape(QFrame::StyledPanel);
  XyFrame->setFrameShadow(QFrame::Raised);
  
  
  palette.setColor(QPalette::Background , QColor(255,0,0));
  XzFrame->setAutoFillBackground(true);
  XzFrame->setPalette(palette);
  ZyFrame->setAutoFillBackground(true);
  ZyFrame->setPalette(palette);
  XyFrame->setAutoFillBackground(true);
  XyFrame->setPalette(palette);
  setCentralWidget(centralWidget);
  
    QGLFormat glFormat;
   glFormat.setRgba(true);
   glFormat.setDoubleBuffer(true);
   glFormat.setDepth(true);
   mXY_GLw = new XyzGL(glFormat , XY_VIEW , XyFrame , this);
   mXY_GLw->setGeometry(FRAM_BORDER , FRAM_BORDER,mx,my);
   mXY_GLw->setAttribute(Qt::WA_DeleteOnClose);
    
   mXZ_GLw = new XyzGL(glFormat , XZ_VIEW , XzFrame , this);
   mXZ_GLw->setGeometry(FRAM_BORDER , FRAM_BORDER , mx ,mz);
   mXZ_GLw->setAttribute(Qt::WA_DeleteOnClose);
   
   mYZ_GLw = new XyzGL(glFormat , ZY_VIEW , ZyFrame , this);
   mYZ_GLw->setGeometry(FRAM_BORDER , FRAM_BORDER ,mz ,my);
   mYZ_GLw->setAttribute(Qt::WA_DeleteOnClose);
}

void XyzWindow::LoadImageData()
{
  qDebug()<<"Load Image Data";
  
    //processbar
  LoadImgWin LoadImgBar;
  LoadImgBar.show();
  LoadImgBar.SetRange(0 , nz);
  QString s = QString("0/%1").arg(nz);
    LoadImgBar.SetLabel(QString("0/%1").arg(nz));
    
      IplImage* iplimage;
    mData = new unsigned char*[nz];
    for(int i=0;i<nz;i++){//加载图片到mData

	LoadImgBar.SetValue(i);
	LoadImgBar.SetLabel(QString("%1/%2").arg(i).arg(nz));
	
      iplimage = mVi->GetIplImage(i);
      
      IplImage* cpy;
      util::CopyToUChar256(iplimage, &cpy, 255, 0, true);
      GetDataFromIplimage(cpy , &mData[i]);
      
      cvReleaseImage(&cpy);
      cvReleaseImage(&iplimage);
    }
  QString info = QString("Image size : %1*%2\n").arg(nx).arg(ny);
    m_imginfo->AddInfo(info);
    info = QString("Sections : %1\n").arg(nz);
    m_imginfo->AddInfo(info);
    m_imginfo->show();
}

void XyzWindow::DrawImage(int view_type)
{
  switch(view_type){
    case XY_VIEW:
      DrawXYImage();
      break;
    case XZ_VIEW:
      DrawXZImage();
      break;
    case ZY_VIEW:
      DrawZYImage();
      break;
  }
}

void XyzWindow::DrawCurrentPoint(int view_type)
{
  qDebug()<<"view"<<view_type;
  int x , y  , z;
  if(mZoom<m_OpenZoom){
    qDebug()<<"wx ="<<wx<<" mx = "<<mx;
    x = (wx - mx)/2 + cx*mZoom;
    y = (wy - my)/2 + cy*mZoom;
    z = (wz - mz)/2 + cz*mZoom;
    switch(view_type){
    case XY_VIEW:
//       if(mCurView == XY_VIEW){
	  glMatrixMode(GL_PROJECTION);
	  glLoadIdentity();
	  glOrtho(0,wx, 0, wy, -1,1);
	  glColor3f(1.0f , 1.0f , 0.0f);
	  b3dDrawPlus(x , y ,5);
// 	  b3dDrawCross(cx , cy ,4);
// 	  b3dDrawCircle(cx,cy,5);
//       }
      break;
    case XZ_VIEW:
//       if(mCurView == XZ_VIEW){
	  glMatrixMode(GL_PROJECTION);
	  glLoadIdentity();
	  glOrtho(0,wx, 0, wz, -1,1);
	  glColor3f(1.0f , 1.0f , 0.0f);
	  b3dDrawPlus(x ,z ,5);
// 	  b3dDrawCircle(cx,cz,5);
//       }
      break;
    case ZY_VIEW:
//       if(mCurView == ZY_VIEW){
        glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,wz, 0, wy, -1,1);
	glColor3f(1.0f , 1.0f , 0.0f);
	 b3dDrawPlus(z ,y ,5);
// 	b3dDrawCircle(cz,cy,5);
//       }
      break;
  }
  }
  else{
    x = cx*(mZoom/m_OpenZoom);
    y = cy*(mZoom/m_OpenZoom);
    z = cz*(mZoom/m_OpenZoom);
    switch(view_type){
    case XY_VIEW:
//       if(mCurView == XY_VIEW){
	  glMatrixMode(GL_PROJECTION);
	  glLoadIdentity();
	  glOrtho(0,nx, 0, ny, -1,1);
	  glColor3f(1.0f , 1.0f , 0.0f);
	  b3dDrawPlus(x , y ,5);
// 	  b3dDrawCross(cx , cy ,4);
// 	  b3dDrawCircle(cx,cy,5);
//       }
      break;
    case XZ_VIEW:
//       if(mCurView == XZ_VIEW){
	  glMatrixMode(GL_PROJECTION);
	  glLoadIdentity();
	  glOrtho(0,nx, 0, nz, -1,1);
	  glColor3f(1.0f , 1.0f , 0.0f);
	  b3dDrawPlus(x ,z ,5);
// 	  b3dDrawCircle(cx,cz,5);
//       }
      break;
    case ZY_VIEW:
//       if(mCurView == ZY_VIEW){
        glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,nz, 0, ny, -1,1);
	glColor3f(1.0f , 1.0f , 0.0f);
	 b3dDrawPlus(z ,y ,5);
// 	b3dDrawCircle(cz,cy,5);
//       }
      break;
  }
  }
  qDebug()<<"CurrentPoint : "<<cx<<" "<<cy<<" "<<cz;
  qDebug()<<"DrawCurrentPoint : "<<x<<" "<<y<<" "<<z<<m_RelativeZoom;
  
}

void XyzWindow::DrawXZImage()
{
  //Load data for XZ view
  LoadXZData();
  
  //draw xz view
  float offsetx , offsety;
  if(mZoom < m_OpenZoom){
    offsetx =(wx - mx)/2;
    offsety = (wz - mz)/2;
  }
  else{
    offsetx = 0;
    offsety = 0;
  }
  glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
    glOrtho(0,wx, 0, wz, -1,1);
   glPixelZoom(mZoom , mZoom);
   glRasterPos2f(offsetx , offsety);
   glDrawPixels(nx, nz, GL_LUMINANCE, GL_UNSIGNED_BYTE, mFdataxz);
}

void XyzWindow::DrawZYImage()
{
  qDebug("DrawZYImage");
  // Load data for YZ view
   LoadYZData();
   
    //draw yz view
   float offsetx , offsety;
  if(mZoom < m_OpenZoom){
    offsetx =(wz - mz)/2;
    offsety = (wy - my)/2;
  }
  else{
    offsetx = 0;
    offsety = 0;
  }
    glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
    glOrtho(0,wz, 0, wy, -1,1);
   glPixelZoom(mZoom , mZoom);
   glRasterPos2f(offsetx , offsety);
   glDrawPixels(nz, ny, GL_LUMINANCE, GL_UNSIGNED_BYTE, mFdatayz);
}

void XyzWindow::DrawXYImage()
{
  unsigned char* sdata;
  if(m_imginfo->m_isChanged){
//     sdata = new unsigned char[nx*ny];
    BrightContractAdjust(mData[cz] , mFdataxy , nx*ny , m_imginfo->m_Brightness , m_imginfo->m_Contract);
    sdata = mFdataxy;
  }
  else{
   sdata = mData[cz];
  }
      //draw XY view
//     glClear(GL_COLOR_BUFFER_BIT);
  float offsetx , offsety;
  if(mZoom < m_OpenZoom){
    offsetx =(wx - mx)/2;
    offsety = (wy - my)/2;
  }
  else{
    offsetx = 0;
    offsety = 0;
  }
  qDebug()<<"offset "<<offsetx<<" "<<offsety;
     glMatrixMode(GL_PROJECTION); //将当前矩阵指定为投影矩阵
      glLoadIdentity();//把矩阵设为单位矩阵
      glOrtho(0,wx, 0, wy, -1,1);//平行投影
    glPixelZoom(mZoom , mZoom);//放大/缩小
//     glPixelZoom((GLfloat)1.0f, (GLfloat)1.0f); 
//    glRasterPos2f(-1, -1);
    glRasterPos2f(offsetx , offsety);//设置文字的显示坐标
   glDrawPixels(nx, ny, GL_LUMINANCE, GL_UNSIGNED_BYTE, sdata);//绘制
}

//private
void XyzWindow::stepZoom(int step)
{

}

// void XyzWindow::sliderChanged(int witch, int value)
// {
// 
// }

//public slot
void XyzWindow::zoomUp()
{
//   stepZoom(1);
  if(mZoom < 0.5)
    mZoom *= 1.5;
  else  mZoom+=0.25;
  m_RelativeZoom = mZoom/m_OpenZoom;
  
   m_SizeLineEdit->setText(QString::number(mZoom));
  mx = mZoom*nx;
  my = mZoom*ny;
  mz = mZoom*nz;
  Update(XY_VIEW);
  Update(XZ_VIEW);
  Update(ZY_VIEW);
}

void XyzWindow::zoomDown()
{
//  stepZoom(-1);
  if(mZoom >= 1)
  mZoom-=0.25;
  else mZoom /= 1.5;
  m_RelativeZoom = mZoom/m_OpenZoom;
  
  m_SizeLineEdit->setText(QString::number(mZoom));
  mx = mZoom*nx;
  my = mZoom*ny;
  mz = mZoom*nz;
  Update(XY_VIEW);
  Update(XZ_VIEW);
  Update(ZY_VIEW);
}

void XyzWindow::XSliderChanged(int i)
{
//   sliderChanged(1 , i);
  qDebug()<<"XSliderChanged cx = "<<i;
  cx = i;
  Update(ZY_VIEW);
}

void XyzWindow::YSliderChanged(int i)
{
//   sliderChanged(2 , i);
  cy = i;
  Update(XZ_VIEW);
}

void XyzWindow::ZSliderChanged(int i)
{
//   sliderChanged(3 ,i);
  cz = i;
  Update(XY_VIEW);
}

void XyzWindow::UpdateToolBar()
{
  m_XSlider->setValue(cx);
  m_YSlider->setValue(cy);
  m_ZSlider->setValue(cz);
}

void XyzWindow::On_Adjust()
{
  Update(XY_VIEW);
  Update(XZ_VIEW);
  Update(ZY_VIEW);
}

void XyzWindow::LoadYZData()
{
 float val;  
   float contract = m_imginfo->m_Contract;
   float bright = m_imginfo->m_Brightness;
    float k = tan((45 + 44*contract)/180*3.14);
  for(int z = 0 ; z< nz ; z++)
    for(int y = 0 ; y<ny ; y++){
      val = mData[z][nx*y + cx];
       val = (val - 128*(1-bright))*k + 128*(1+bright) ;  
                //对灰度值的可能溢出进行处理  
                if(val>255) val=255;  
                if(val<0) val=0;  
      mFdatayz[y*nz+z] = val;
    }
}

void XyzWindow::LoadXZData()
{
   float val;  
   float contract = m_imginfo->m_Contract;
   float bright = m_imginfo->m_Brightness;
    float k = tan((45 + 44*contract)/180*3.14);
  for(int x=0 ; x< nx ; x++)
    for(int z = 0 ; z< nz ; z++){
      val = mData[z][nx*cy + x];
      val = (val - 128*(1-bright))*k + 128*(1+bright) ;  
                //对灰度值的可能溢出进行处理  
                if(val>255) val=255;  
                if(val<0) val=0;  
      mFdataxz[z*nx + x] = val;
    }
}

void XyzWindow::Update(int view_type)
{
  qDebug("update image");
  switch(view_type){
    case XY_VIEW:
      DrawXYImage();
      mXY_GLw->updateGL();
      break;
    case XZ_VIEW:
      DrawXZImage();
      mXZ_GLw->updateGL();
      break;
    case ZY_VIEW:
      DrawZYImage();
      mYZ_GLw->updateGL();
      break;
  }
}

void XyzWindow::SetXyz(int view_type, int x , int y)
{
//   mCurView = view_type;
  qDebug()<<"*****SetXyz   : "<<x<<" "<<y;
  switch(view_type){
    case XY_VIEW:
      if(mZoom>=m_OpenZoom){
	cx = x/mZoom;
	cy = (wy-y)/mZoom;
      }
      else{
	cx = (x -  (wx - mx)/2)/mZoom;
	cy = (wy - y -  (wy - my)/2)/mZoom;
      }
      Update(XZ_VIEW);
      Update(ZY_VIEW);
      break;
    case XZ_VIEW:
      if(mZoom>=m_OpenZoom){
	cx = x/mZoom;
	cz = (wz-y)/mZoom;
      }
      else{
	cx = (x -  (wx - mx)/2)/mZoom;
	cz = (wz - y -  (wz - mz)/2)/mZoom;
      }
      Update(XY_VIEW);
      Update(ZY_VIEW);
//       mXZ_GLw->DrawCurrentPoint(cx , cz);
      break;
    case ZY_VIEW:
      if(mZoom>=m_OpenZoom){
	cz = x/mZoom;
	cy = (wy-y)/mZoom;
      }
      else{
	cz = (x -  (wz - mz)/2)/mZoom;
	cy = (wy - y -  (wy - my)/2)/mZoom;
      }
      Update(XY_VIEW);
      Update(XZ_VIEW);
//       mYZ_GLw->DrawCurrentPoint(cz , cy);
      break;
  } 
  m_XSlider->setValue(cx);
  m_YSlider->setValue(cy);
  m_ZSlider->setValue(cz);
}

//==========================================XyzGL=====================================================

XyzGL::XyzGL(const QGLFormat& format,int view_type , QWidget* parent ,XyzWindow *mainWindow): QGLWidget(format,parent)
{
   qDebug()<<"XyzGL";
  mWin = mainWindow;
  mMousePressed = false;
  mClosing = false;
  mFirstDraw = true;
  mViewtype = view_type;
  mIsDynamicView = false;
  mViewTimer = new QTimer(this);
  mWay = LEFT;
  m_StartMove = false;//added by chice
  m_ContinueMove = false;//added by chice
  connect(mViewTimer , SIGNAL(timeout()) , this , SLOT(DynamicView()));
  connect(this , SIGNAL(ChangeView()) , mWin , SLOT(UpdateToolBar()));
}

void XyzGL::DynamicView()//用来实现动态显示
{
  switch(mViewtype){
    case XY_VIEW:
      MovieZ();
      break;
    case XZ_VIEW:
      MovieY();
      break;
    case ZY_VIEW:
      MovieX();
      break;
  }
}

XyzGL::~XyzGL()
{
  qDebug()<<"~XyzGL";
}

void XyzGL::MovieX()//用来实现动态显示
{
   if(mWay == LEFT){
	if(mWin->cx==0){
	    mWay = RIGHT;
	    mWin->cx = 1;
	}
	else{
	    mWin->cx--;
	}
    }
    else{
	if(mWin->cx == mWin->nx - 1){
	    mWay = LEFT;
	    mWin->cx--;
	}
	else{
	    mWin->cx++;
	}
    }
  mWin->Update(ZY_VIEW);
  emit ChangeView();
}

void XyzGL::MovieY()//用来实现动态显示
{
     if(mWay == LEFT){
	if(mWin->cy==0){
	    mWay = RIGHT;
	    mWin->cy = 1;
	}
	else{
	    mWin->cy--;
	}
    }
    else{
	if(mWin->cy == mWin->ny - 1){
	    mWay = LEFT;
	    mWin->cy--;
	}
	else{
	    mWin->cy++;
	}
    }
  mWin->Update(XZ_VIEW);
  emit ChangeView();
}

void XyzGL::MovieZ()//用来实现动态显示
{
     if(mWay == LEFT){
	if(mWin->cz==0){
	    mWay = RIGHT;
	    mWin->cz = 1;
	}
	else{
	    mWin->cz--;
	}
    }
    else{
	if(mWin->cz == mWin->nz - 1){
	    mWay = LEFT;
	    mWin->cz--;
	}
	else{
	    mWin->cz++;
	}
    }
  mWin->Update(XY_VIEW);
  emit ChangeView();
}

// void XyzGL::DrawCurrentPoint(int x, int y)
// {
//   qDebug()<<"DrawCurrentPoint :"<<x<<" "<<y;
//  
//   glMatrixMode(GL_PROJECTION);
//   glLoadIdentity();
//   glOrtho(0,mWin->nx, 0, mWin->ny, -1,1);
//    glColor3f(1.0f , 1.0f , 0.0f);
//    b3dDrawCircle(x,y,5);
// //    updateGL();
// }

void XyzGL::mouseMoveEvent(QMouseEvent* e)
{
  if(e->type() == QEvent::MouseMove) {
            if(!m_StartMove) {
                m_StartMove = true;
                m_ContinueMove = false;
                m_MousePoint = e->globalPos();
            }
            else {
                QPoint p = e->globalPos();
                int Offset_X = p.x() - m_MousePoint.x();
                int Offset_Y = p.y() - m_MousePoint.y();
                m_ContinueMove = true;
                //m_ImageWidget->move(Offset_X+m_ImageWidget->x(),Offset_Y+m_ImageWidget->y());
		//mWin->move();
                m_MousePoint = p;
            }
        }
        else if(e->type() == QEvent::MouseButtonRelease) {
            if(m_StartMove)
                m_StartMove = false;
       
        }
  QWidget::mouseMoveEvent(e);
}

void XyzGL::mousePressEvent(QMouseEvent* e)
{
  qDebug()<<"mousePressEvent"<<mViewtype;
  mMousePressed = true;
  if (e->button() == Qt::LeftButton){
    qDebug("LeftButtonPressEvent");
    int x = e->x();
    int y = e->y();
    mWin->SetXyz(mViewtype , x , y);
    updateGL();
  }
  else if(e->button() == Qt::MidButton){
     qDebug("midButtonPressEvent");
     if(mIsDynamicView){
       mIsDynamicView = false;
       mViewTimer->stop();
     }
     else{
       mIsDynamicView = true;
       mWay = RIGHT;
       mViewTimer->start(50);
     }
  }

  
  
QWidget::mousePressEvent(e);
}

void XyzGL::mouseReleaseEvent(QMouseEvent* e)
{
  mMousePressed = false;
QWidget::mouseReleaseEvent(e);
}


void XyzGL::paintGL()
{
  
  qDebug()<<"paintGL";
  glClearColor (0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  
  mWin->DrawImage(mViewtype);
  mWin->DrawCurrentPoint(mViewtype);
//   if(mViewtype == XY_VIEW)
//   DrawCurrentPoint(mWin->cx,mWin->cy);
}

void XyzGL::resizeGL(int wdth, int hght)
{
QGLWidget::resizeGL(wdth, hght);
}

void XyzGL::timerEvent(QTimerEvent* e)
{
QObject::timerEvent(e);
}

// #include "XyzWindow.moc"
