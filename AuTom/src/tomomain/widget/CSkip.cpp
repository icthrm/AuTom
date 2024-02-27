#include "CSkip.h"
#include <QtGui>
using namespace std;
CSkipWidget::CSkipWidget(QWidget* parent, Qt::WindowFlags f): QWidget(parent, f)
{
    QFrame *frame = new QFrame(this);
    frame->setFrameShadow(QFrame::Raised);
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setMinimumSize(80,80);
    frame->setMaximumSize(90,90);
    QGridLayout *l =new QGridLayout(frame);
    QGridLayout *l2=new QGridLayout();
    m_SkipButton = new QPushButton(tr("Skip"),frame);
    l->addWidget(m_SkipButton);
    l2->addWidget(frame);
    this->setLayout(l2);
    this->resize(100,100);
}
void CSkipWidget::SetSkipButtonEnable(bool flag)
{
    if(flag==false){
	m_SkipButton->setEnabled(false);
    }
    else {
	m_SkipButton->setEnabled(true);
    }
}

