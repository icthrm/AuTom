#ifndef CNEWPROJECT_H
#define CNEWPROJECT_H

#include "QtGui"
#include "env/env.h"
#include "promanage/ProManage.h"
#include "widget/CSelect.h"
class CNewProjectWin : public QWidget
{
  Q_OBJECT
public:
    CNewProjectWin(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~CNewProjectWin();
private:

    QString m_ProcessName;
    QString m_MrcName;
    QString m_AngleName;
    QString m_MrcPath;
    QString m_AnglePath;
    
    QDialogButtonBox *m_ButtonBox;
    QLabel *m_NameLabel;
    QLabel *m_PathLabel;
    QLabel *m_MRCLabel;
    QLabel *m_AngleLabel;
    QLineEdit *m_NameLine;
    QLineEdit *m_PathLine;
    QLineEdit *m_MRCLine;
    QLineEdit *m_AngleLine;
    QCheckBox *m_checkbox;
    QToolButton *m_MRCButton;
    QToolButton *m_AngleButton;
    QAction *m_MRCAction;
    QAction *m_AngleAction;
    QPushButton *m_OpenProjectButton;
    QAction *m_OpenProjectAction;
private slots:
    void CreateProject(QAbstractButton* button);
    void ChoicePath();
    void ChoicePathMRC();
    void FindRawtlt_Slot();
    void OpenProjectBtnClicked();
    
signals:
    void NewSuccess(QString filename);
    void FindRawtlt();
    void OpenExistedProject();

};

#endif