#include "CSetWorkSpace.h"
#include "init/Cinit.h"
#include "core/msg_map.h"
#include "env/env.h"

CSetWorkSpaceWin::CSetWorkSpaceWin(QWidget* parent, Qt::WindowFlags f): QDialog(parent, f)
{
    resize(400 ,100);
    this->setWindowTitle("Set Work Path");
    m_ChooseAction = new QAction(QIcon(":/images/files.png"),tr("files"),this);
    connect(m_ChooseAction , SIGNAL(triggered(bool)) , this , SLOT(ChoosePath()));
    
    QHBoxLayout *l1 = new QHBoxLayout(this);
    QVBoxLayout *l2 = new QVBoxLayout();
    QHBoxLayout *l3 = new QHBoxLayout();
    m_label = new QLabel("WorkPath :" , this );
    l2->addWidget(m_label);
    m_pathLine = new QLineEdit(this);
    m_pathLine->setText(CEnv::CurrentWorkDirectory());

    l3->addWidget(m_pathLine);
    m_ChooseButton = new QToolButton(this);
    m_ChooseButton->setDefaultAction(m_ChooseAction);
    m_ChooseButton->setMinimumSize(QSize(30 ,30 ));
    m_ChooseButton->setIconSize(QSize(30 ,30 ));
    l3->addWidget(m_ChooseButton);
    l2->addLayout(l3);
    m_ButtonBox = new QDialogButtonBox( this);   
    m_ButtonBox->setOrientation(Qt::Horizontal);
    m_ButtonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    
    l2->addWidget(m_ButtonBox);;
    l1->addLayout(l2);
        
    connect(m_ButtonBox , SIGNAL(clicked(QAbstractButton*)) , this , SLOT(SetWorkSpace(QAbstractButton*)));
}

void CSetWorkSpaceWin::SetWorkSpace(QAbstractButton * button)
{
    if(button == m_ButtonBox->button(QDialogButtonBox::Cancel)){
	this->close();
    }
    else if(button == m_ButtonBox->button(QDialogButtonBox::Ok)){
	QString s = m_pathLine->text();
	QFileInfo file;
	file.setFile(s);
	if(!file.exists()){
	  if(file.isAbsolute()){
	    QDir dir;
	    if(!dir.mkpath(s)){
	      QMessageBox::warning(this , tr("Make dir error")  , tr("Can't creat work path!") ,QMessageBox::Close);
	      return;
	    }
	  }
	  else{
	    QMessageBox::warning(this , tr("Warning")  , tr("Work path must be absolutely path!!") ,QMessageBox::Close);
	    return;
	  }
	}
		
	
	Cinit::SetWorkSpace(s.toLatin1().data());
	CEnv::SetInitWorkDirectory(s.toLatin1().data());
    
	this->close();
        emit WindowsClosed();

    }
}

void CSetWorkSpaceWin::ChoosePath()
{
     QString filename=QFileDialog::getExistingDirectory(this ,tr("open"), QString(CEnv::CurrentWorkDirectory()));
     m_pathLine->setText(filename);
}

void CSetWorkSpaceWin::FirstSet()
{
  char *path = getenv("HOME"); 
  QString initworkpath = QString(path);
  initworkpath+="/autom_project";
  m_pathLine->setText(initworkpath);
}


#include "CSetWorkSpace.moc"
