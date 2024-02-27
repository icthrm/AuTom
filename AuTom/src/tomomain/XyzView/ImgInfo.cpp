#include "ImgInfo.h"

ImgInfo::ImgInfo(QWidget* parent, Qt::WindowFlags flags): QMainWindow(parent, flags)
{
  m_Brightness = 0.0;
  m_Contract = 0.0;
  m_isChanged = false;
  m_centralWidget = new QWidget(this);
  CreateInfoFrame();
  CreateAdjustFrame();
  
   QGridLayout *l = new QGridLayout();
   l->addWidget(m_InfoFrame , 0 , 0);
   l->addWidget(m_AdjustFrame , 1 , 0);
   m_centralWidget->setLayout(l);
  setCentralWidget(m_centralWidget);
  
  QRect rect = QApplication::desktop()->availableGeometry();
  move(rect.width()*3/4 , rect.height()/4);
  
  connect(m_BrightnessSlider , SIGNAL(valueChanged(int)) , this , SLOT(On_Bright_Changed(int)));
  connect(m_ContractSlider , SIGNAL(valueChanged(int)) , this , SLOT(On_Contract_Changed(int)));
  connect(m_Reset , SIGNAL(clicked(bool)) , this , SLOT(On_Reset()));
}

ImgInfo::~ImgInfo()
{

}

void ImgInfo::CreateInfoFrame()
{
  m_InfoFrame = new QFrame(m_centralWidget);
  m_InfoFrame->setFrameStyle(QFrame::Panel|QFrame::Raised|QFrame::HLine);
  m_InfoTextEdit = new QTextEdit(m_InfoFrame);
  m_InfoTextEdit->setFixedHeight(100);
  m_InfoTextEdit->setReadOnly(true);
  QGridLayout *l = new QGridLayout();
  l->addWidget(m_InfoTextEdit);
  m_InfoFrame->setLayout(l);
}

void ImgInfo::CreateAdjustFrame()
{
  QString str;
  m_AdjustFrame = new QFrame(m_centralWidget);
  m_AdjustFrame->setFrameStyle(QFrame::Panel|QFrame::Raised|QFrame::HLine);
  m_BrightnessLable = new QLabel(tr("Brightness ") , m_AdjustFrame);
  m_BrightnessSlider = new QSlider(m_AdjustFrame);
  m_BrightnessSlider->setOrientation(Qt::Horizontal);
  m_BrightnessSlider->setRange(-100 , 100);
  m_BrightnessSlider->setValue(m_Brightness*100);
  str.setNum(m_Brightness);
  m_BrightNum = new QLabel(str , m_AdjustFrame);
  m_BrightNum->setFixedWidth(35);
  m_ContractLable = new QLabel(tr("Contrast ") , m_AdjustFrame);//Constract->Constrast changed by chice
  m_ContractSlider = new QSlider(m_AdjustFrame);
  m_ContractSlider->setOrientation(Qt::Horizontal);
  m_ContractSlider->setRange(-100 , 100);
  m_ContractSlider->setValue(m_Contract*100);
  str.setNum(m_Contract);
  m_ContractNum = new QLabel(str , m_AdjustFrame);
  m_ContractNum->setFixedWidth(35);
  m_Reset = new QPushButton(tr("Reset") , m_AdjustFrame);
  m_Reset->setFixedWidth(60);
   
   QGridLayout *l = new QGridLayout();
   l->addWidget(m_BrightnessLable , 0 , 0);
   l->addWidget(m_BrightnessSlider , 0 , 1);
   l->addWidget(m_BrightNum , 0 , 2);
   l->addWidget(m_ContractLable , 1, 0);
   l->addWidget(m_ContractSlider , 1 , 1);
   l->addWidget(m_ContractNum , 1 ,2);
   QGridLayout *l1 = new QGridLayout();
   QSpacerItem* spacer = new QSpacerItem(150 , 10 );
   l1->addItem(spacer , 0 , 0);
   l1->addWidget(m_Reset , 0 , 1 );
   QGridLayout *l0 = new QGridLayout();
   l0->addLayout(l , 0 , 0);
   l0->addLayout(l1 , 1 , 0);
   m_AdjustFrame->setLayout(l0);;
}


void ImgInfo::AddInfo(QString str)
{
  m_InfoTextEdit->textCursor().insertText(str);
}

void ImgInfo::On_Bright_Changed(int value)
{
//   qDebug()<<"On_Bright_Changed";
  m_isChanged = true;
//   qDebug()<<value;
  m_Brightness =(float)value/100;
  m_BrightNum->setText(QString("%1").arg(m_Brightness));
  emit Adjust();
}

void ImgInfo::On_Contract_Changed(int value)
{
  m_isChanged = true;
  m_Contract =(float)value/100;
  m_ContractNum->setText(QString("%1").arg(m_Contract));
  emit Adjust();
}

void ImgInfo::On_Reset()
{
  m_isChanged = false;
  m_Brightness = 0.0;
  m_Contract = 0.0;
  m_BrightnessSlider->setValue(0);
  m_ContractSlider->setValue(0);
  m_BrightNum->setText(QString("%1").arg(m_Brightness));
  m_ContractNum->setText(QString("%1").arg(m_Contract));
  emit Adjust();
}
#include "ImgInfo.moc"