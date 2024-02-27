#ifndef FINEALIGNWIN_H
#define FINEALIGNWIN_H

//#include <QtGui>
#include <QtWidgets>
#include <widget/CSkeleton.h>
#include "CProgressBar.h"

#define FINE_ALIGN_MSG		1000+4
#define RAPTOR_MSG             1000+7
#define IMOD_MSG 1000+18

enum Alogrithm{ Markless , RAPTOR , MarkerAuto};

class FineAlignWin : public QWidget{
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
//     QString m_finMrc_dir;
    
    Alogrithm m_alogrithm;
    QStackedLayout *m_ls;
    QStackedLayout *m_button_ls;
   
protected:
    void CreateImageFrame();
    void CreateButton();
    
public:
    FineAlignWin(QWidget* parent = 0, Qt::WindowFlags f = 0);
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
    void StartMarklessAlign();
//     void StartRAPTAR();
    void SetButton(QString txt);
    void ViewFidMode(bool);
    
private:
    virtual void customEvent(QEvent* );
    
};

#endif
