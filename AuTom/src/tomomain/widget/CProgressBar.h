#ifndef CPROGRESSBAR_H
#define CPROGRESSBAR_H

//#include <QtGui>
#include <QtWidgets>
#include <signal.h>

class CProgressBar : public QWidget
{
  Q_OBJECT
public:
  explicit CProgressBar(QWidget* parent = 0, Qt::WindowFlags f = 0);
  
  void setWidth(int w);
  virtual ~CProgressBar();
  void Active();
private:
  QProgressBar *m_progressbar;
  QLabel *m_lable;
  QPushButton *m_kill;
  QTimer *m_timer;
  
  int value;
private slots:
  void Dynamic();
  void Kill();
 signals:
   void Kill_Signal();
};

#endif
