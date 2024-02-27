
#include "AboutDialog.h"
#include <QPushButton>
#include <QDialog>
#include <QLabel>
#include <QUrl>
#include <QDesktopServices>
#include <QDebug>
#include <QLabel>
#include <QGridLayout>
#include <QSpacerItem>
AboutDialog::AboutDialog()
{
    m_programName_label= new QLabel("Autom");
    m_programDescription_label =new QLabel();
    m_programVersion_label = new QLabel("beta 1.0.1");
    
    m_programDescription_label->setText("Automatic Tomography (AuTom) is for automatic reconstruction of electron tomography (ET), \nwhich covered the pre-processing, alignment and reconstruction of electron tomography.");
    
    m_close_btn = new QPushButton("close");
    m_website_btn = new QPushButton("Our Website");
    QSpacerItem* spaceitem1=new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Expanding);	
    QSpacerItem* spaceitem2=new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Expanding);	

    QGridLayout* l=new QGridLayout();
    l->addWidget(m_programName_label,0,0);
    l->addWidget(m_programVersion_label,0,1);
    l->addItem(spaceitem1,1,0,2,3);
    l->addWidget(m_programDescription_label,3,0,3,5);
    l->addItem(spaceitem2,6,0,2,3);
    l->addWidget(m_website_btn,8,2,1,1);
    l->addWidget(m_close_btn,8,4,1,1);
    setLayout(l);
    
    connect(m_website_btn,SIGNAL(clicked()),this,SLOT(openWebsite()));
    connect(m_close_btn,SIGNAL(clicked()),this,SLOT(close()));
    
}
void AboutDialog::openWebsite()
{
    bool result = QDesktopServices::openUrl(QUrl("http://ear.ict.ac.cn", QUrl::TolerantMode));
    qDebug()<<(result ? "open Website http://ear.ict.ac.cn success" : "open Website http://ear.ict.ac.cn fail ");

}

#include "AboutDialog.moc"
