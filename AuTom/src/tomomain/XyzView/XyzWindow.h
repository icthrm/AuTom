#ifndef  XYZWINDOW_H
#define XYZWINDOW_H

#include <qmainwindow.h>
#include <QGLWidget>
#include <QCloseEvent>
#include <QTimerEvent>
#include <QLabel>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QtOpenGL/qgl.h>
#include <QtOpenGL/QtOpenGL>
// #include "XyzGL.h"
#include "mrcimg/mrc2img.h"
#include "b3dgfx.h"
#include "ImgInfo.h"

#define XY_VIEW 0
#define XZ_VIEW 1
#define ZY_VIEW 2

#define NUM_AXIS 3

#define LEFT 4
#define RIGHT 5 

class XyzGL;

class XyzWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit XyzWindow(util::MrcStack* vi ,QWidget* parent = 0, Qt::WindowFlags flags = 0);
  virtual ~XyzWindow();

  XyzGL* mXY_GLw;//OpenGL 窗口
  XyzGL* mXZ_GLw;
  XyzGL* mYZ_GLw;
  
  unsigned char *mFdataxz; /* tmp data storage for xz image       */
  unsigned char *mFdatayz; /* tmp data storage for yz image       */
  unsigned char *mFdataxy;
  util::MrcStack* mVi;
  
  unsigned char** mData;
  unsigned char* sdata;
  
  int nx , ny , nz; /*图片大小*/
  int mx , my , mz; /*当前图片显示尺寸*/
  int wx , wy , wz;/*界面中显示的x，y，z大小*/
  int cx , cy ,cz; /*当前鼠标所在位置*/
  float mZoom; //*缩放比例*/
  float m_OpenZoom;
  float m_RelativeZoom;
  int mWidth , mHeight;
  int mXorigin1,mYorigin1, mXorigin2, mYorigin2;
//   int mCurView;

private:
  QToolBar* m_EnlargeTool;
  QToolBar* m_ShrinkTool;
  QLineEdit* m_SizeLineEdit;
  QLabel* m_XLable;
  QLabel* m_YLable;
  QLabel* m_ZLable;
  QSlider* m_XSlider;
  QSlider* m_YSlider;
  QSlider* m_ZSlider;
  QLabel* m_CurView;
  QLabel* m_Infomation;
  QAction* m_EnlargeAction;
  QAction* m_ShrinkAction;
  ImgInfo* m_imginfo;
  
public:
   void CreateActions();
   void CreateToolBars();
   void CreateStatusBar();
//    void CreateDockWidget();
   void CreateXYZView();
   void DrawImage(int view_type);
   void DrawCurrentPoint(int view_type);
   void Update(int view_type);
   void SetXyz(int view_type , int x , int y);
   
private:
  void LoadImageData();
  void LoadYZData();
  void LoadXZData();
  void stepZoom(int step);
//   void sliderChanged(int witch , int value);
  
  void DrawXYImage();
  void DrawXZImage();
  void DrawZYImage();
   
public slots:
  void zoomUp();
  void zoomDown();
  void XSliderChanged(int i);
  void YSliderChanged(int i);
  void ZSliderChanged(int i);
  void UpdateToolBar();
  void On_Adjust();
};

class XyzGL: public QGLWidget
{
  Q_OBJECT
  
public:
    XyzGL(const QGLFormat & format, int view_type ,QWidget * parent = 0 ,XyzWindow *mainWindow=0);
    virtual ~XyzGL();
    bool mClosing;
    int mViewtype;
    int mWay;
    
 
 protected:
  void paintGL();
  void resizeGL( int wdth, int hght );
  void mousePressEvent(QMouseEvent * e );
  void mouseReleaseEvent ( QMouseEvent * e );
  void mouseMoveEvent ( QMouseEvent * e );
  void timerEvent(QTimerEvent *e);
  
private:
  void MovieX();
  void MovieY();
  void MovieZ();
    
private:
   XyzWindow *mWin;
  bool mFirstDraw;
  int mTimerID;
  bool mMousePressed;
  bool mIsDynamicView;
  QTimer* mViewTimer;
  
  bool m_StartMove;
  bool m_ContinueMove;
  QPoint m_MousePoint;
  
private slots:
  void DynamicView();
  
public:
//   void DrawCurrentPoint(int x , int y);
  
signals:
  void ChangeView();
};

#endif
