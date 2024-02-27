#ifndef RESTRUCTWIN_H
#define RESTRUCTWIN_H
#include <QtGui>
#include <widget/CSkeleton.h>
#include "CProgressBar.h"
#include"CTFSettingsDialog.h"
#include "utils/SysInfo.h"
#include "utils/InputSaver.h"

#define RESTRUCT_MSG     1000+5
#define IMOD_MSG 1000+18
class RestructWin : public QWidget
{
    Q_OBJECT
private:
    QWidget *m_mainWidget;
    QFrame *m_imageFram;
    QFrame *m_ButtonFrame;
    QFrame *m_SelectFrame;
    CProgressBar *m_progressbar;
    QFrame *m_SetFram;
    
    CSkeleton *m_Input;
    CSkeleton *m_Output;
    
    QLabel *m_version_Label;
    QComboBox * m_version_Combobox;
    
    QLabel *m_alogrithm_Label;
    QComboBox *m_alogrithm_Combobox;
    
    QLabel *m_startModel_Label;
    QComboBox *m_startModel_Combobox;
    
    QLabel *m_restructNumber_Lable;
    QLineEdit *m_restructNumber_LineEdit;
    
    QLabel *m_threadNum_Lable;
    QLineEdit *m_threadNum_LineEdit;
    
    QLabel *m_factor_Label;
    QLineEdit *m_factor_LineEdit;
    
    QLabel *m_ParameterLabel;
    QLineEdit *m_ParameterLineEdit;
    
    QLabel *m_IterativenumbersLabel;
    QLineEdit *m_IterativenumbersLineEdit;
    
    QLabel *m_Method_Label;
    QComboBox *m_Method_ComboBox;
    
    QLabel      *m_Slice_Label;
    QLineEdit   *m_SliceLineEdit1;
    QLineEdit   *m_SliceLineEdit2;
    QLabel      *m_ICONIteration_Label;
    QLineEdit   *m_ICONIterLineEdit1;
    QLineEdit   *m_ICONIterLineEdit2;
    QLineEdit   *m_ICONIterLineEdit3;
    QLabel      *m_Threshold_Label;
    QLineEdit   *m_ThresholdLineEdit;
    QLabel      *m_radiusLabel;
    QLineEdit   *m_radiusLineEdit;
    QLabel      *m_dataTypeLabel;
    QComboBox   *m_dataTypeBox;
    QLabel      *m_bin_label;
    QLineEdit   *m_bin_lineEdit;
    QLabel *m_Mrcfile_Label;
    QAction *m_Mrcfile_Action;
    QToolButton *m_Mrcfile_Button;
    QLineEdit *m_Mrcfile_Lineedit;
    
    
    QPushButton * m_start;
    
    QString m_inputImage; 
    QString m_outputImage;
    QStackedLayout *m_ls;
    //the following members are for ctf corretion
    QFrame      *m_ctf_frame;
    QLabel      *m_ctf_lable;
    QCheckBox   *m_ctf_enable_checkbox;
    QPushButton *m_ctf_settings_btn;
    CTFSettingsDialog* m_ctf_dialog;
    
    InputSaver * inputSaver;
    
    
protected:
    void CreateImageFrame();
    void CreateButton();
    void CreateSelectFrame();
    void CreateCTFCorrectionFrame();
public:
    explicit RestructWin(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~RestructWin();
    void SetInputImage();
    void SetOutputImage();
    void allhide();
    bool DeleteDirectory(const QString &path);
signals:
    void Log(QString message); 
    void Finish(QString step);
    void Start();
    void RunError();
    
public slots:
    void ShowInput();
    void ShowOutput();
    int StartRestruct();
    int StartforRestruct();
    void SetParVisible();
    void ChangeAlgorithm();
    void ChangeAlgorithm2();//选择cuda或mpi
    void ChooseFile();
    void EnableCTFCorrection();
    void ShowCTFCorrectionSettings();
        
private:
    virtual void customEvent(QEvent* );
    int isNum(std::string s);
    void setDefaultThreadNum();//根据硬件信息设置threadNum的值
    
};

#endif
