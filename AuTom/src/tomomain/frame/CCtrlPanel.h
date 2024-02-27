#ifndef CCTRLPANEL_H__
#define CCTRLPANEL_H__

#include <QtGui>
#include "widget/CLabelSlider.h"

class CCtrlPanel:public QWidget{
Q_OBJECT
private:
    CLabelSlider* m_w_slider;
    CLabelSlider* m_b_slider;
    
public:
    explicit CCtrlPanel(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~CCtrlPanel();
};

#endif