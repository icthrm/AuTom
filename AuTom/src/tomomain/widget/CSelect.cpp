 #include <QtGui>
 #include "CSelect.h"
#include "Ctag.h"
using namespace std;
 CSelectWidget::CSelectWidget(QWidget* parent, Qt::WindowFlags f): QWidget(parent, f)
 {
     SelectWidget=new QWidget();
     SelectWidget->setWindowTitle("Select");
     QWidget *w=new QWidget();
     QHBoxLayout *select_layout=new QHBoxLayout(w);
     QSpacerItem *verticalSpacer;
     verticalSpacer = new QSpacerItem(100 ,0, QSizePolicy::Minimum, QSizePolicy::Fixed);
     select_layout->addItem(verticalSpacer);
     m_MarkerBasedButton= new QPushButton("Marker-Based-Flow",w);
     m_MarkerFreeButton =new QPushButton("Marker-Free-Flow",w);
     m_MarkerBasedButton->setToolTip(ToolTips::sel_btn_marker);
     m_MarkerFreeButton->setToolTip(ToolTips::sel_btn_free);
     connect(m_MarkerFreeButton, SIGNAL(clicked(bool)), this, SLOT(MarkerFreeFlowClicked()));
     connect(m_MarkerBasedButton, SIGNAL(clicked(bool)), this, SLOT(MarkerBasedFlowClicked()));
     select_layout->addWidget(m_MarkerBasedButton);
     verticalSpacer = new QSpacerItem(100 ,0, QSizePolicy::Minimum, QSizePolicy::Fixed);
     select_layout->addItem(verticalSpacer);
     select_layout->addWidget(m_MarkerFreeButton);
     verticalSpacer = new QSpacerItem(100 ,0, QSizePolicy::Minimum, QSizePolicy::Fixed);
     select_layout->addItem(verticalSpacer);
     SelectWidget->setLayout(select_layout);
     QGridLayout *l0 = new QGridLayout(w);
     l0->addWidget(SelectWidget);
     l0->setMargin(0);
     this->setLayout(l0);
 }
 
 void CSelectWidget::MarkerFreeFlowClicked()
 {
     emit MarkerFreeFlow();
 }
void CSelectWidget::MarkerBasedFlowClicked()
{
    emit MarkerBasedFlow();

}

 #include "CSelect.moc"
