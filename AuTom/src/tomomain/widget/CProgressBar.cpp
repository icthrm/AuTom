#include "CProgressBar.h"
#include <core/msg_map.h>

CProgressBar::CProgressBar(QWidget* parent, Qt::WindowFlags f): QWidget(parent, f)
{
  QWidget *mainWidget = new QWidget(this);
  QGridLayout *gridLayout = new QGridLayout(mainWidget);;
  m_progressbar = new QProgressBar(mainWidget);
  m_progressbar->setFixedWidth(300);
  value = 0;
  m_progressbar->setValue(value);
  m_progressbar->setStyleSheet("QProgressBar::chunk{background-color: rgb(133, 175, 229); width: 10px; margin: 0.5px}");
  m_progressbar->setTextVisible(false);
//   m_lable = new QLabel(tr("runing ..") , this);
  m_kill = new QPushButton( QIcon(":/images/kill.png") ,tr("kill"), mainWidget);
  m_kill->setFixedWidth(50);
  gridLayout->setSpacing(20);
  gridLayout->setContentsMargins(11, 11, 11, 11);
  gridLayout->addWidget(m_progressbar, 0, 0, 1, 1);
  gridLayout->addWidget(m_kill, 0, 1, 1, 1);
//   QGridLayout *l = new QGridLayout();
//   l->addWidget(m_progressbar , 0 , 0 );
//   l->addWidget(m_kill , 0 ,1);
// //   l->addWidget(m_lable , 1 ,0);
//   mainWidget->setLayout(l);
//   mainWidget->resize(100 , 20);
//   mainWidget->setSizePolicy(QSizePolicy::Fixed , QSizePolicy::Fixed);
  
  QGridLayout *l0 = new QGridLayout();
   l0->addWidget(mainWidget);
   this->setLayout(l0);
   mainWidget->setSizePolicy(QSizePolicy::Fixed , QSizePolicy::Fixed);
   m_timer = new QTimer(this);
   connect(m_timer , SIGNAL(timeout()) , this , SLOT(Dynamic()));
   connect(m_kill , SIGNAL(clicked(bool)) , this , SLOT(Kill()));
   Active();
//    QPalette pal = palette();
//     pal.setColor(QPalette::Background, QColor(0x00,0xff,0x00,0x00));
//     setPalette(pal);
}

CProgressBar::~CProgressBar()
{

}

void CProgressBar::Active()
{
  m_timer->start(500);
  
}

void CProgressBar::Dynamic()
{
  value = (value + 10)%100;
  m_progressbar->setValue(value);
}

void CProgressBar::Kill()
{
  emit Kill_Signal();
  MMTApp::AbortCurrentProc();
  
   
}

void CProgressBar::setWidth(int w)
{
  m_progressbar->setFixedWidth(w);
}

#include "CProgressBar.moc"