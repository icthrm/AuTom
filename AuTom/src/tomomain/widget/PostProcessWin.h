#ifndef POSTPROCESSWIN_H
#define POSTPROCESSWIN_H

#include <QtGui>
#include <widget/CSkeleton.h>
#include "CProgressBar.h"
#define POSTPROCESS_MSG		1000+10

class PostProcessWin : public QWidget
{
  Q_OBJECT
  private:
    QWidget *m_mainWidget;
    QFrame *m_imageFram;
    QFrame *m_ButtonFrame;
    QFrame *m_SetFram;
    CProgressBar *m_progressbar;
    
    CSkeleton *m_Input;
    CSkeleton *m_Output;
    
    QLabel *m_timesLabel;
    QSpinBox *m_spinBox;
    QPushButton *m_startButton;
    QCheckBox *m_checkbox;
    
    QString m_inputImage;
     QString  m_outputImage;
    QString m_preali_dir;
    
    QStackedLayout *m_ls;
   
protected:
    void CreateImageFrame();
    void CreateButton();
    
public:
  explicit PostProcessWin(QWidget* parent = 0, Qt::WindowFlags f = 0);
  virtual ~PostProcessWin();
  void SetInputImage();
  void SetOutputImage();
  
signals:
    void Log(QString message);   
    void Finish(QString step);
    void RunError();
    void Start();
    
public slots:
    void ShowInput();
    void ShowOutput();
    void StartPostProcess();
    
private:
    virtual void customEvent(QEvent* );
};

#endif