#ifndef IMGINFO_H
#define IMGINFO_H

//#include <QtGui>
#include <QtWidgets>
class ImgInfo : public QMainWindow
{
  Q_OBJECT
public:
  explicit ImgInfo(QWidget* parent = 0, Qt::WindowFlags flags = 0);
  virtual ~ImgInfo();
  
private:
  QFrame* m_InfoFrame;
  QFrame* m_AdjustFrame;
  
  QWidget *m_centralWidget;
//   QLabel* m_InfoLable;
  QTextEdit* m_InfoTextEdit;
  
  QLabel* m_BrightnessLable;
  QLabel* m_ContractLable;
  QSlider* m_BrightnessSlider;
  QSlider* m_ContractSlider;
  QLabel* m_BrightNum;
  QLabel* m_ContractNum;
  QPushButton* m_Reset;
  
public:
  float m_Brightness;
  float m_Contract;
  bool m_isChanged;
  
protected:
  void CreateInfoFrame();
  void CreateAdjustFrame();
  
public:
  void AddInfo(QString str);
  
public slots:
  void On_Bright_Changed(int value);
  void On_Contract_Changed(int value);
  void On_Reset();
  
signals:
  void Adjust();
};

#endif
