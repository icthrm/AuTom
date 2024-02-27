#ifndef CTFSETTINGSDIALOG_H
#define CTFSETTINGSDIALOG_H
#include <QDialog>
class QLabel;
class QLineEdit;
class QComboBox;
class QPushButton;
class QCheckBox;
class QTextEdit;
class CProgressBar;
class QStackedLayout;
#define CTF_MSG 1000+16
struct CTFSettings{
    int defocusDifference;
    int XMAG;
    int DStep;
    int tileSize;
    double CS;
    double HT;
    double AmpCnst;
    double CTFMin;
    double ResMin;
    double ResMax;
    double dFMin;
    double dFMax;
    double FStep;
    int defocus;  //Defocus at the untilted plane
    bool amplitudeCorrection;
    double filter1;//Wiener filter parameters: w1, w2
    double filter2;    
    int processNumber;
};
class CTFSettingsDialog : public QDialog
{
    
    Q_OBJECT
private:
    QLabel *m_defocusDifference_label;
    QLineEdit *m_defocusDifference_lineEdit;
    
    QLabel *m_XMAG_label;
    QLineEdit *m_XMAG_lineEdit;
    
    QLabel *m_DStep_label;
    QLineEdit *m_DStep_lineEdit;
    
    QLabel *m_tileSize_label;
    QLineEdit *m_tileSize_lineEdit;
    
    QLabel *m_CS_label;
    QLineEdit *m_CS_lineEdit;
    
    QLabel *m_HT_label;
    QLineEdit *m_HT_lineEdit;
    
    QLabel *m_AmpCnst_label;
    QLineEdit *m_AmpCnst_lineEdit;
    
    QLabel *m_CTFMin_label;
    QLineEdit *m_CTFMin_lineEdit;
    
    QLabel *m_ResMin_label;
    QLineEdit *m_ResMin_lineEdit;
    
    QLabel *m_ResMax_label;
    QLineEdit *m_ResMax_lineEdit;
    
    QLabel *m_dFMin_label;
    QLineEdit *m_dFMin_lineEdit;

    QLabel *m_dFMax_label;
    QLineEdit *m_dFMax_lineEdit;
    
    QLabel *m_FStep_label;
    QLineEdit *m_FStep_lineEdit;
    
    QLabel *m_defocus_label;
    QLineEdit *m_defocus_lineEdit;
    
    QLabel *m_amplitudeCorrection_label;
    QComboBox *m_amplitudeCorrection_comboBox;
    
    QLabel *m_filter1_label;
    QLineEdit *m_filter1_lineEdit;
    
    QLabel *m_filter2_label;
    QLineEdit *m_filter2_lineEdit;    
    
    QLabel *m_processNumber_label;
    QLineEdit *m_processNumber_lineEdit;
    
    QCheckBox *m_enableShellEdit_checkBox;
    QLabel *m_shellScript_label;
    QTextEdit *m_shell_textEdit;
    
    QTextEdit *m_hints_textedit;
    
    QPushButton *m_start_btn;
    QPushButton *m_cancel_btn;
    QPushButton *m_generateShell_btn;
    
    CProgressBar *m_progressbar;
    QTextEdit *m_output_textEdit;
    QStackedLayout *m_stacklayout;
    QFrame *m_settigns_frame;
    QLabel *m_output_label;
    CTFSettings* settings;
    
    QString scriptFileName;
    void setLineEditState(bool state);
    bool readSettings();
    void printSettigns();
    bool isEmpty(QLineEdit* lineEdit);
    bool hasEmpty();
    
    //test function
    void testSetting();
    
    virtual void customEvent(QEvent* );
    
private slots:
    void enableShellEdit();
    void enableAmplitudeCorrection();
    void generateShell();
    void executeShell();
    void printMessage(QString message);
signals:
    void Log(QString message); 
    void Finish(QString);
    void Start();
    void RunError(QString);

public:
    CTFSettingsDialog(QWidget* parent = 0);
    ~CTFSettingsDialog();
//     CTFSettings getSettings() const;

};
#endif
