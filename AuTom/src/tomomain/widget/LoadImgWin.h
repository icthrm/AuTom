#ifndef LoadImgWin_H
#define LoadImgWin_H

//#include <QtGui>
#include <QtWidgets>

class LoadImgWin : public QDialog
{
  Q_OBJECT
private:
  QProgressBar* m_ProgressBar;
  QLabel* m_label;
  
private slots:
  
public:
  LoadImgWin(QWidget* parent = 0);
  virtual ~LoadImgWin();
  void SetRange(int start , int end);
  void SetValue(int i);
  void SetLabel(QString str);
};

#endif
