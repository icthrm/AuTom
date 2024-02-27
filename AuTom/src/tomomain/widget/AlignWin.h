#ifndef ALIGNWIN_H
#define ALIGNWIN_H

#include <QtGui>
#include <widget/CSkeleton.h>
#include "CProgressBar.h"
#include "utils/InputSaver.h"

#define ALIGN_MSG		1000+14


class AlignWin : public QWidget{
    Q_OBJECT
private:
    QWidget *m_mainWidget;
    QFrame *m_imageFram;
    QFrame *m_ButtonFrame;
    QFrame *m_ButtonFrame_Alogrithm;
    QFrame *m_buttonFrame_sub;
    QFrame *m_buttonFrame_sub1;
    QFrame *m_buttonFrame_sub2;
    QFrame *m_buttonFrame_sub3;
    CProgressBar *m_progressbar;
    QFrame *m_SetFram;
    
    CSkeleton *m_Input;
    CSkeleton *m_Output;
    
    QLabel *m_alogrithm_Label;
    QComboBox *m_alogrithm_Combobox;
    
    QLabel *m_diameter_Label;
    QLineEdit *m_diameter_LineEdit;
    
    QLabel *m_markers_Label;
    QLineEdit *m_markers_LineEdit;
    
    QPushButton *m_startButton1;
    QPushButton *m_startButton2;
    QPushButton *m_startButton3;
    QPushButton *m_ViewfidmodeButton;
    
    QString m_inputImage;
    QString  m_outputImage ;
    
    QLineEdit *m_AutoLineEdit;
    QLabel *m_AutoLabel;
    QCheckBox *m_checkbox;
    QCheckBox *m_fast_checkbox;
    //     QString m_finMrc_dir;
    
    QStackedLayout *m_ls;
    QStackedLayout *m_button_ls;
    
    InputSaver * inputSaver;
    
protected:
    void CreateImageFrame();
    void CreateButton();

    
public:
    AlignWin(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~AlignWin();
    void SetInputImage();
    void SetOutputImage();
    
    
signals:
    void Log(QString message);   
    void Finish(QString step);
    void Start();
    void RunError();
    
public slots:
    void ShowInput();
    void ShowOutput();
    //     void StartRAPTAR();
    void ViewFidMode(bool);
    void StartMarkerAuto();
 
private:
    virtual void customEvent(QEvent* );
    
};

#endif		
