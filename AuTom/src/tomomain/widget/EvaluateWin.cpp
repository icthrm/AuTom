#include "EvaluateWin.h"
#include "promanage/ProManage.h"
#include "env/env.h"
#include <cstring>
#include "core/msg_map.h"
#include "core/msg.h"
#include "custom/custom.h"
#include "ImageViewWindow/slicemain/slicemainwindow.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
using namespace std;

EvaluateWin::EvaluateWin(QWidget* parent): QWidget(parent)
{
   ViewProErro_Button = new QPushButton(tr("View \nprojection\nError") , this);
   ViewRepro_Button = new QPushButton(tr("View \nreprojection\nImage") , this);
   ViewProErro_Button->setFixedSize(100 , 100);
   ViewRepro_Button->setFixedSize(100 , 100);
   this->setWindowTitle("Evaluate result");
   QGridLayout* l= new QGridLayout();
   l->addWidget(ViewRepro_Button , 0 , 0);
   l->addWidget(ViewProErro_Button , 0 ,1);
   this->setLayout(l);
   connect(ViewProErro_Button , SIGNAL(clicked(bool)) , this , SLOT(ShowOutputProErro()));
   connect(ViewRepro_Button , SIGNAL(clicked(bool)) , this , SLOT(ShowOutputRepro()));
}

EvaluateWin::~EvaluateWin()
{
    
}

void EvaluateWin::ShowOutputProErro()
{
//  ImageViewWindow::ViewMrc("output_projectionError.mrc");
  SliceMainWindow::ViewMrc("output_projectionError.mrc");
}

void EvaluateWin::ShowOutputRepro()
{
//  ImageViewWindow::ViewMrc("output_reprojection.mrc");
  SliceMainWindow::ViewMrc("output_projectionError.mrc");
}

#include "EvaluateWin.moc"


