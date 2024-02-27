#include "LoadImgWin.h"

LoadImgWin::LoadImgWin(QWidget* parent): QDialog(parent)
{
  resize(400 , 100);
  this->setWindowTitle("Load Image");
  
  
  m_ProgressBar = new QProgressBar();
  m_ProgressBar->setStyleSheet("QProgressBar::chunk{background-color: rgb(133, 175, 229)} QProgressBar{border: 2px solid grey; border-radius: 5px; text-align: center}");
//   m_ProgressBar->setStyleSheet("");
  m_label = new QLabel("0/0" , this);
  m_label->setMinimumWidth(70);
  
//   QLayout* l0 = new QLayout(this);
  QHBoxLayout *l1 = new QHBoxLayout(this);
  QHBoxLayout *l2 = new QHBoxLayout();
  l2->addWidget(m_ProgressBar);
  l2->addWidget(m_label);
  l1->addLayout(l2);
//   l0->addL
}

LoadImgWin::~LoadImgWin()
{
}

void LoadImgWin::SetRange(int start, int end)
{
  m_ProgressBar->setRange(start , end);
}

void LoadImgWin::SetValue(int i)
{
  m_ProgressBar->setValue(i);
}

void LoadImgWin::SetLabel(QString str)
{
  m_label->setText(str);
}


#include "LoadImgWin.moc"