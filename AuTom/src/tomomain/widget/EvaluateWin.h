#ifndef EVALUATEWIN_H
#define EVALUATEWIN_H

//#include <QtGui>
#include <QtWidgets>

#define EVALUATE_MSG		1000+8

class EvaluateWin : public QWidget
{
  Q_OBJECT
  private:
    QPushButton* ViewProErro_Button;
    QPushButton* ViewRepro_Button;
    
public:
    EvaluateWin(QWidget* parent = 0);
    virtual ~EvaluateWin();
    
    
public slots:
    void ShowOutputProErro();
    void ShowOutputRepro();
};

#endif
