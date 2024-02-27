#ifndef PREPROCESS_H__
#define PREPROCESS_H__

#include <QtGui>
#include <widget/CSkeleton.h>
#include "CProgressBar.h"
#include "utils/InputSaver.h"


#define Hat_PIXEL_MSG		1000+2
#define H_NORMALIZE_MSG		1000+6//??
#define IMOD_MSG 1000+18
#define ROTATE_MRC_MSG 		1000+11

class PreprocessWin : public QWidget
{
    Q_OBJECT
private:
    QWidget *m_mainWidget;
	QFrame *m_ImageFram;
    QFrame *m_ButtonFram;
    
    CProgressBar *m_progressbar;
    QFrame *m_SetFram;
	
    QLabel *m_normtype_Label;
    QComboBox * m_normtype_Combobox;
    
    QLabel *m_avg_Lable;
    QLineEdit *m_avg_LineEdit;
	
	QLabel *m_std_Lable;
    QLineEdit *m_std_LineEdit;
	
	QPushButton *m_start;
	
  QLabel *m_header_Lable;///#!
  
    QPushButton *m_scanHeader;
    QLabel *m_rotLabel;
    QLineEdit *m_rotAngleEdit;
    QPushButton *m_startRotateButton;
    
    QPushButton *m_useAsInputButton;
    
    QTabWidget *m_tabWidget;
	
    QString m_InputImage;
    QString m_outputImage;
    QString m_RotatedImage;
	
    CSkeleton* m_Input;
    CSkeleton* m_OutPut;
    
    QString m_preprocess_dir;
    QStackedLayout *m_ls;
    
    InputSaver * inputSaver;

public:
    PreprocessWin(QWidget* parent=0);
    virtual ~PreprocessWin();
    
    void SetInputImage();
	void SetOutputImage();//+
	
protected:
    void CreateImageFram();
    void CreateButton();  
    
signals:
    void Log(QString message);
	void Finish(QString step);
	void RunError();
	void Start();

public slots:
    void ShowInput();
    void ShowOutput();
    bool StartNormalize();//+
    void ScanHeader(bool);
    void StartRotate(bool);
    void UseInNext(bool);
    void ChangeAlgorithm(int index);
    
private:
    virtual void customEvent(QEvent* );
};

#endif
