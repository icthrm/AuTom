#include "CNewProject.h"
#include "widget/Ctag.h"
CNewProjectWin::CNewProjectWin(QWidget* parent, Qt::WindowFlags f): QWidget(parent, f)
{

    QWidget *m_MainWidget=new QWidget();
    m_MainWidget->setWindowTitle("New Project");
   // m_MainWidget->setWindowFlags(Qt::Window);
    m_MRCAction = new QAction(QIcon(":/images/files.png"),tr("files"),this);
    connect(m_MRCAction , SIGNAL(triggered(bool)) , this , SLOT(ChoicePathMRC()));
    
    m_AngleAction = new QAction(QIcon(":/images/files.png"),tr("files"),this);
    connect(m_AngleAction , SIGNAL(triggered(bool)) , this , SLOT(ChoicePath()));
    
    this->resize(500 , 300);
   //this->setWindowFlags(Qt::Window);
    //this->setMaximumSize(750,450);
    QWidget *w = new QWidget(this);
    w->setGeometry(QRect(30 , 20 , 440 ,270));
    QVBoxLayout *l = new QVBoxLayout();
    QHBoxLayout *l1 = new QHBoxLayout();
    m_NameLabel = new QLabel("Project name : ", w);
    l1->addWidget(m_NameLabel);
    m_NameLine = new QLineEdit(w);
    m_NameLine->setToolTip(ToolTips::new_lbl_proname);
    l1->addWidget(m_NameLine);
//     l->addLayout(l1);
//     delete l1;
    
    QHBoxLayout *l2 = new QHBoxLayout();
    m_PathLabel = new QLabel("Location : " , w);
    l2->addWidget(m_PathLabel);
    QSpacerItem *verticalSpacer;
    verticalSpacer = new QSpacerItem(31, 1, QSizePolicy::Fixed,QSizePolicy::Fixed);
    l2->addItem(verticalSpacer);
    m_PathLine = new QLineEdit(w);
    
    //show WorkDirectory
   m_PathLine->setText(CEnv::InitWorkDirectory());//第一次没有
    l2->addWidget(m_PathLine);
//     l->addLayout(l2);
//     delete l2;
    
    QHBoxLayout *l3 = new QHBoxLayout();
    m_MRCLabel = new QLabel("MRC file : " , w);
    l3->addWidget(m_MRCLabel);
    verticalSpacer = new QSpacerItem(33, 1, QSizePolicy::Fixed,QSizePolicy::Fixed);
    l3->addItem(verticalSpacer);
    m_MRCLine = new QLineEdit(w);
    l3->addWidget(m_MRCLine);
    m_MRCButton = new QToolButton(w);
    m_MRCButton->setDefaultAction(m_MRCAction);
    m_MRCButton->setIconSize(QSize(20 , 20 ));
    m_MRCButton->setToolTip(ToolTips::new_lbl_mrc);
    l3->addWidget(m_MRCButton);
    l->addLayout(l3);
//     delete l3;
    
    QHBoxLayout *l4 = new QHBoxLayout();
    m_AngleLabel = new QLabel("angle file : " , w);
    l4->addWidget(m_AngleLabel);
    verticalSpacer = new QSpacerItem(26, 1, QSizePolicy::Fixed,QSizePolicy::Fixed);
    l4->addItem(verticalSpacer);
    m_AngleLine = new QLineEdit(w);
    l4->addWidget(m_AngleLine);
    m_AngleButton = new QToolButton(w);
    m_AngleButton->setDefaultAction(m_AngleAction);
    m_AngleButton->setIconSize(QSize(20 , 20 ));
    m_AngleButton->setToolTip(ToolTips::new_lbl_angle);
    l4->addWidget(m_AngleButton);
    l->addLayout(l4);
     l->addLayout(l1);
      l->addLayout(l2);
//     delete l4;
    
    m_checkbox = new QCheckBox("Copy MRC and angle files to process file" , this);
    l->addWidget(m_checkbox);
    
    m_ButtonBox = new QDialogButtonBox(w);
    m_ButtonBox->setOrientation(Qt::Horizontal);
    m_ButtonBox->setStandardButtons(QDialogButtonBox::Ok);
    m_OpenProjectButton=new QPushButton("Open Existed Project");
    m_ButtonBox->addButton(m_OpenProjectButton,QDialogButtonBox::ActionRole);
    connect(m_OpenProjectButton,SIGNAL(clicked(bool)),this,SLOT(OpenProjectBtnClicked()));
    l->addWidget(m_ButtonBox);
    
    m_PathLine->setDisabled(true);;
    m_MainWidget->setLayout(l);
    m_MainWidget->setMaximumSize(500,300);
    QGridLayout *l0 = new QGridLayout();
    l0->addWidget(m_MainWidget);
    l0->setMargin(0);
    this->setLayout(l0);
    connect(m_ButtonBox , SIGNAL(clicked(QAbstractButton*)) , this , SLOT(CreateProject(QAbstractButton*)));
    connect(this , SIGNAL(FindRawtlt()) , this , SLOT(FindRawtlt_Slot()));
}

CNewProjectWin::~CNewProjectWin()
{

}

void CNewProjectWin::CreateProject(QAbstractButton* button)
{

    if(button == m_ButtonBox->button(QDialogButtonBox::Ok)){
	QFileInfo file;
	m_MrcPath = m_MRCLine->text();
	file.setFile(m_MrcPath);
	if(!file.exists()){
	  QMessageBox::critical(this , "Create Process Error!" , "MRC file not exists." , QMessageBox::Close);
	  return;
	}
	m_MrcName = file.fileName();
	m_AnglePath = m_AngleLine->text();
	file.setFile(m_AnglePath);
	if(!file.exists()){
	  QMessageBox::critical(this , "Create Process Error!" , "Angle file not exists." , QMessageBox::Close);
	  return;
	}
	m_AngleName = file.fileName();
	
	m_ProcessName = m_NameLine->text();
	file.setFile(m_ProcessName);
	if(file.isDir()){
	    QMessageBox::StandardButton rb = QMessageBox::warning(this , tr("Warning!") , tr("Process have existed!") , QMessageBox::Ok);
	    return;
	}
	CEnv::SetCurrentWorkDirectory(CEnv::InitWorkDirectory());
	QDir dir;
	if(!dir.mkpath(m_ProcessName)){
// 	    qDebug("creat process error");
	    QMessageBox::critical(this , "Create directory Error!" , "Can not create project directory. Make sure there is no illegal character in the project name." , QMessageBox::Close);
	    return;
	}
	if(m_checkbox->checkState()){
	    //copy 
	    QString currentDir = QString(CEnv::CurrentWorkDirectory());
           currentDir+="/"+m_ProcessName +"/";
	    QString newMRC = m_MrcName;
	    QString newAngle = m_AngleName;
	    qDebug()<<"MRC = "<<newMRC;
	    qDebug()<<"Angle = "<<newAngle;
	    if(!QFile::copy(m_MrcPath , currentDir+newMRC )){
// 		qDebug("copy error");
		QMessageBox::critical(this , "Create Process Error!" , "Error when copy MRC path." , QMessageBox::Close);
		return;
	    }
	    if(!QFile::copy(m_AnglePath , currentDir+newAngle )){
	      QMessageBox::critical(this , "Create Process Error!" , "Error when copy angle path." , QMessageBox::Close);
	      return;
// 		qDebug("copy error");
	    }
	    m_MrcPath = newMRC;
	    m_AnglePath = newAngle;
	}

	if( CProManage::CreatProcess(m_ProcessName.toLatin1().data() , m_MrcPath.toLatin1().data() , m_AnglePath.toLatin1().data()))
	{
	  string str = CEnv::CurrentWorkDirectory(); 
	  emit NewSuccess(str.data());
	 
	}
	else{
	  QMessageBox::critical(this , "Create Process Error!" , "Error when copy angle path." , QMessageBox::Close);
	}
	this->close();
    }
}

void CNewProjectWin::ChoicePathMRC()
{  
     setWindowTitle("New Project");
       m_MrcPath=QFileDialog::getOpenFileName(this ,tr("open"), ".",tr("MRC files (*.mrc *.st)"));
     int start ,end;
     start = m_MrcPath.lastIndexOf('/');
     end = m_MrcPath.lastIndexOf('.');
     QString name = m_MrcPath.mid(start+1 , end-start-1);
     m_NameLine->setText(name);
     m_MRCLine->setText(m_MrcPath);
     emit FindRawtlt();
}

void CNewProjectWin::ChoicePath()
{
     m_AnglePath=QFileDialog::getOpenFileName(this ,tr("open"), ".",tr("Angele file (*.rawtlt *.tlt)"));
     m_AngleLine->setText(m_AnglePath);
     int start = m_AnglePath.lastIndexOf('/')+1;
     m_AngleName = m_AnglePath.mid(start);   
}
void CNewProjectWin::OpenProjectBtnClicked()
{    
    emit OpenExistedProject();
}

void CNewProjectWin::FindRawtlt_Slot()
{
  QFileInfo file(m_MrcPath);
  if(file.exists()){
    QString rawtlt = file.path() + "/";
    rawtlt += file.baseName();
    rawtlt += ".rawtlt";
    qDebug()<<"rawtlt : "<<rawtlt<<endl;
    file.setFile(rawtlt);
    if(file.exists()){
      m_AnglePath = rawtlt;
      m_AngleLine->setText(rawtlt);
      int start = m_AnglePath.lastIndexOf('/')+1;
     m_AngleName = m_AnglePath.mid(start);
    }
  }

}

#include "CNewProject.moc"
