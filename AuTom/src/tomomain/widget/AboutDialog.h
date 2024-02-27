#ifndef ABOUT_DIALOG_H
#define ABOUT_DIALOG_H
#include <QDialog>
class QLabel;
class QPushButton;

class AboutDialog :public QDialog{
        Q_OBJECT
private:
    QLabel* m_programName_label;
    QLabel* m_programDescription_label;
    QLabel* m_programVersion_label;
    QPushButton* m_close_btn;
    QPushButton* m_website_btn;
private slots:
    void openWebsite();
public:
    AboutDialog();
    
        
};


#endif
