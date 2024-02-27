// #include "XyzGL.h"
// 
// XyzGL::XyzGL(const QGLFormat& format, QWidget* parent): QGLWidget(format,parent)
// {
//    qDebug()<<"XyzGL";
//   mMousePressed = false;
//   mClosing = false;
//   mFirstDraw = true;
// }
// 
// 
// XyzGL::~XyzGL()
// {
//   
// }
// 
// void XyzGL::mouseMoveEvent(QMouseEvent* e)
// {
// QWidget::mouseMoveEvent(e);
// }
// 
// void XyzGL::mousePressEvent(QMouseEvent* e)
// {
// QWidget::mousePressEvent(e);
// }
// 
// void XyzGL::mouseReleaseEvent(QMouseEvent* e)
// {
// QWidget::mouseReleaseEvent(e);
// }
// 
// 
// void XyzGL::paintGL()
// {
//   qDebug()<<"paintGL";
//   glClearColor (0.0, 0.0, 0.0, 0.0);
//   glShadeModel(GL_FLAT);
//   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//   glClear(GL_COLOR_BUFFER_BIT);
// }
// 
// void XyzGL::resizeGL(int wdth, int hght)
// {
// QGLWidget::resizeGL(wdth, hght);
// }
// 
// void XyzGL::timerEvent(QTimerEvent* e)
// {
// QObject::timerEvent(e);
// }
// 
// 
// 
// // #include "XyzGL.moc"