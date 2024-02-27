// #ifndef XYZGL_H
// #define XYZGL_H
// 
// #include <QGLWidget>
// #include <QCloseEvent>
// #include <QTimerEvent>
// #include <QLabel>
// #include <QMouseEvent>
// #include <QKeyEvent>
// #include <QtOpenGL/qgl.h>
// #include <QtOpenGL/QtOpenGL>
// #include "mrcimg/mrc2img.h"
// 
// 
// 
// class XyzGL: public QGLWidget
// {
//   Q_OBJECT
//   
// public:
//     XyzGL(const QGLFormat & format, QWidget * parent = 0);
//     virtual ~XyzGL();
//     bool mClosing;
//     unsigned char** mData;
//     int mViewtype;
//  
//  protected:
//   void paintGL();
//   void resizeGL( int wdth, int hght );
//   void mousePressEvent(QMouseEvent * e );
//   void mouseReleaseEvent ( QMouseEvent * e );
//   void mouseMoveEvent ( QMouseEvent * e );
//   void timerEvent(QTimerEvent *e);
//     
// private:
//   bool mFirstDraw;
//   int mTimerID;
//   bool mMousePressed;
// };
// 
// #endif