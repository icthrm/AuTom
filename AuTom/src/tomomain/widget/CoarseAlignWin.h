#ifndef COARSE_ALIGNWIN_H 
#define COARSE_ALIGNWIN_H

#include <QtGui>
#include <widget/CSkeleton.h>
#include "CProgressBar.h"
#include "utils/InputSaver.h"

#define COARSE_ALIGN_MSG		1000+3

class CoarseAlignWin : public QWidget
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
    
    InputSaver * inputSaver;
   
protected:
    void CreateImageFrame();
    void CreateButton();
    
public:
    CoarseAlignWin(QWidget* parent = 0);
    virtual ~CoarseAlignWin();
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
    void StartCoarseAlign();
    
private:
    virtual void customEvent(QEvent* );
    
};

#endif