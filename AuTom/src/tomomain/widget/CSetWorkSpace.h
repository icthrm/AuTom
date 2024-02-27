#ifndef CSETWORKSPACE_H
#define CSETWORKSPACE_H
#include "QtWidgets/QDialog"
#include "QtWidgets"
class CSetWorkSpaceWin : public QDialog
{
    Q_OBJECT
private:
    QLabel *m_label;
    QLineEdit *m_pathLine;
    QToolButton *m_ChooseButton;
    QDialogButtonBox *m_ButtonBox;
    QAction *m_ChooseAction;
    
private slots:
    void SetWorkSpace(QAbstractButton * button);
    void ChoosePath();    
public:
    CSetWorkSpaceWin(QWidget* parent = 0, Qt::WindowFlags f = 0);
    void FirstSet();
signals:
    void WindowsClosed();
    
    
};


#endif
