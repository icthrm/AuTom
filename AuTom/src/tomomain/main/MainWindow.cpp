#include "MainWindow.h"

#include <QtWidgets/QLabel>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QAction>
#include <QFile>
#include <QFileInfo>
#include <QUrl>
#include <iostream>
#include "env/env.h"
#include "core/msg_map.h"
#include "widget/Ctag.h"
#include "widget/AboutDialog.h"
#define BasedMethod 100
#define FreeMethod 101


void Mainwindow::emitSet()
{
    m_isFirstSet = true;
    //弹出设置工作空间的窗口
    emit Set();

}
void Mainwindow::showLocation()
{
    NewProject();

}

Mainwindow::Mainwindow()
{  
    connect(this , SIGNAL(Set()) , this , SLOT(ShowSetWorkpathWidget()));

    m_BasedFree=Null;
    this->setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    this->setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
    CreateMenus();
    CreateMdiarea();
    CreateToolbars();
    CreateProcessDockwidget();
    CreateOutputDockwidget();
    CreateFileDockwidget();
    //    CreateSelectWidget();
    m_ProcessDockwidget->hide();
    m_FileDockwidget->hide();
    m_OutputDockwidget->hide();

    //m_SelectDockwidget->hide();
    /*QMenuBar *menubar = new QMenuBar(this);
     *    menubar->setObjectName(QString::fromUtf8("menubar"));
     *    menubar->setGeometry(QRect(0, 0, 988, 26));
     *    this->setMenuBar(menubar);*/
    QStatusBar *statusbar = new QStatusBar(this);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));
    this->setStatusBar(statusbar);
    this->resize(1000,750);
    //inputSaver = InputSaver::getInstance();
}

void Mainwindow::CreateMenus()
{
    m_ProjectMenu=menuBar()->addMenu("Project");
    m_OpenProcessAction=new QAction(tr("Open Process"),this);
    connect(m_OpenProcessAction, SIGNAL(triggered(bool)), this, SLOT(Close2OpenProject()));
    m_NewProcessAction=new QAction(tr("New Process"),this);

    connect(m_NewProcessAction, SIGNAL(triggered(bool)), this, SLOT(Close2NewProject()));

    m_ChangeMethodAction=new QAction(tr("Change Method"),this);
    connect(m_ChangeMethodAction,SIGNAL(triggered(bool)),this,SLOT(ChangeMethod()));
    m_ProjectMenu->addAction(m_OpenProcessAction);
    m_ProjectMenu->addAction(m_NewProcessAction);
    m_ProjectMenu->addAction(m_ChangeMethodAction);
    m_WindowMenu=menuBar()->addMenu("Window");

    m_SettingsMenu=menuBar()->addMenu("Settings");
    m_SetWorkpathAction=new QAction(tr("Set WorkPath"),this);
    m_SetWorkpathAction->setToolTip(ToolTips::path);
    connect(m_SetWorkpathAction, SIGNAL(triggered(bool)), this, SLOT(ShowSetWorkpathWidget()));
    m_SettingsMenu->addAction(m_SetWorkpathAction);
    m_viewMenu=m_SettingsMenu->addMenu("View");
    m_view_processDock_action = new QAction(tr("Process Dock"),this);
    m_view_fileDock_action = new QAction(tr("File Dock"),this);
    m_viewMenu->addAction(m_view_fileDock_action);
    m_viewMenu->addAction(m_view_processDock_action);
    m_view_fileDock_action->setEnabled(false);
    m_view_processDock_action->setEnabled(false);
    connect(m_view_fileDock_action,SIGNAL(triggered()),this,SLOT(showDock()));
    connect(m_view_processDock_action,SIGNAL(triggered()),this,SLOT(showDock()));   
    

    m_ToolsMenu = menuBar()->addMenu("Tools");
    m_ProjectionErrorAction = new QAction(tr("start projection error"),this);
    m_NCCAction = new QAction(tr("start NCC"),this);
    m_TrajplotAction = new QAction(tr("start Trajplot"),this);
    connect(m_ProjectionErrorAction,SIGNAL(triggered(bool)),this,SLOT(StartProjectionErrorTools()));
    connect(m_NCCAction,SIGNAL(triggered(bool)),this,SLOT(StartNCCTools()));
    connect(m_TrajplotAction,SIGNAL(triggered(bool)),this,SLOT(StartTrajplotTools()));
    m_ToolsMenu -> addAction(m_ProjectionErrorAction);
    m_ToolsMenu -> addAction(m_NCCAction);
    m_ToolsMenu -> addAction(m_TrajplotAction);
    
    m_HelpMenu = menuBar()->addMenu("Help");
    m_about = new QAction(tr("About"),this);
    connect(m_about,SIGNAL(triggered(bool)),this,SLOT(showAboutDialog()));
    m_HelpMenu -> addAction(m_about);
    

}
void Mainwindow::CreateToolbars()
{
    m_OpenProcessToolbar=addToolBar(tr("Open Process"));
    m_OpenProcessToolbar->addAction(m_OpenProcessAction);
    m_OpenProcessToolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_OpenProcessToolbar->setIconSize(QSize(20,20));
    m_OpenProcessToolbar->setMovable(false);

    m_NewProcessToolbar=addToolBar(tr("New Process"));
    m_NewProcessToolbar->addAction(m_NewProcessAction);
    m_NewProcessToolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_NewProcessToolbar->setIconSize(QSize(20,20));
    m_NewProcessToolbar->setMovable(false);

//     m_SetWorkpathToolbar=addToolBar(tr("Set WorkPath"));
//     m_SetWorkpathToolbar->addAction(m_SetWorkpathAction);
//     m_SetWorkpathToolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
//     m_SetWorkpathToolbar->setIconSize(QSize(20,20));
//     m_SetWorkpathToolbar->setMovable(false);

}
void Mainwindow::CreateFileDockwidget()
{
    m_FileDockwidget=new QDockWidget("File System",this);
    m_FileDockwidget->setMinimumWidth(100);
    //m_FileDockwidget->setMaximumWidth(300);
    m_FileDockwidget->setFeatures(QDockWidget::DockWidgetClosable);
    file_widget=new QWidget();
    QGridLayout *file_gridlayout = new QGridLayout();
    m_FileTreeview = new TreeView("project", file_widget);
    m_FileTreeview->SetTreeTitle("project");
     
    //connect(m_FileTreeview,SIGNAL(FileChange()),this, SLOT(Update()));
    //TreeView does NOT have the FileChange signal.Added by liuce
    
    file_gridlayout->addWidget(m_FileTreeview);
    file_gridlayout->setMargin(0);
    file_widget->setLayout(file_gridlayout);
    file_widget->setGeometry(0, 0, 150, file_widget->height());
    m_FileDockwidget->setWidget(file_widget);
    this->addDockWidget(Qt::RightDockWidgetArea,m_FileDockwidget);
    connect(m_FileDockwidget, SIGNAL(visibilityChanged(bool)), this, SLOT(enableMenuAction(bool)));
    connect(this, SIGNAL(FileChange()), m_FileTreeview, SLOT(Update()));
}
void Mainwindow::CreateOutputDockwidget()
{
    m_OutputDockwidget=new QDockWidget("Output",this);
    m_OutputDockwidget->setMinimumHeight(50);
    m_OutputDockwidget->setMaximumHeight(200);
    m_OutputDockwidget->setFeatures(QDockWidget::NoDockWidgetFeatures);;

    QWidget *output_widget=new QWidget();
    QGridLayout *output_layout=new QGridLayout(output_widget);
    output_layout->setContentsMargins(0,0,0,0);
    m_OutputTextedit=new QTextEdit(output_widget);
    m_OutputTextedit->setEnabled(true);
    m_OutputTextedit->setReadOnly(true);
    output_layout->addWidget(m_OutputTextedit,0,0,1,1);
    m_OutputDockwidget->setWidget(output_widget);
    this->addDockWidget(Qt::BottomDockWidgetArea,m_OutputDockwidget);
}
void Mainwindow::CreateProcessDockwidget()
{
    m_ProcessDockwidget=new QDockWidget("Process",this);
    m_ProcessDockwidget->setMinimumSize(QSize(300,300));
    m_ProcessDockwidget->setFeatures(QDockWidget::DockWidgetClosable);

    process_frame=new QFrame(m_ProcessDockwidget);
    process_frame->setFrameShadow(QFrame::Raised);
    process_layout=new QVBoxLayout(process_frame);
    process_layout->setContentsMargins(9,-1,-1,-1);
    buttons_frame=new QFrame(m_ProcessDockwidget);
    buttons_frame->setFrameShape(QFrame::StyledPanel);
    buttons_frame->setFrameShadow(QFrame::Sunken);
    buttons_layout=new QVBoxLayout(buttons_frame);
    buttons_layout->setContentsMargins(9, -1, -1, -1);
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(200, 200, 200));
    buttons_frame->setAutoFillBackground(true);
    buttons_frame->setPalette(palette);

    CreatePushbuttons();
    process_layout->addWidget(buttons_frame);
    buttons_frame->setMinimumHeight(400);

    CreateToolarea();
    //process_layout->addWidget(tool_frame);
    m_ProcessDockwidget->setWidget(process_frame);
    m_ProcessDockwidget->setWindowFlags(Qt::FramelessWindowHint);
    this->addDockWidget(Qt::LeftDockWidgetArea,m_ProcessDockwidget);
    connect(m_ProcessDockwidget,SIGNAL(visibilityChanged(bool)),this,SLOT(enableMenuAction(bool)));
}

void Mainwindow::CreatePushbuttons()
{

    m_PreprocessPushbutton=new QPushButton("Pre-processing",buttons_frame);
    m_PreprocessPushbutton->setMinimumSize(QSize(0,40));
    m_PreprocessPushbutton->setStyleSheet(QString::fromUtf8("font: 16pt \"Ubuntu\";"));
    buttons_layout->addWidget(m_PreprocessPushbutton);
    connect(m_PreprocessPushbutton, SIGNAL(clicked(bool)), this, SLOT(ShowProgressWin()));

    //m_CoarsePushbutton=new QPushButton("Coarse Alignment",buttons_frame);
//    m_CoarsePushbutton->setMinimumSize(QSize(0,40));
//    m_CoarsePushbutton->setStyleSheet(QString::fromUtf8("font: 16pt \"Ubuntu\";"));
//    buttons_layout->addWidget(m_CoarsePushbutton);
//    connect(m_CoarsePushbutton, SIGNAL(clicked(bool)), this, SLOT(ShowCoarseAlignWin()));

    m_FinePushbutton=new QPushButton("Alignment",buttons_frame);
    m_FinePushbutton->setMinimumSize(QSize(0,40));
    m_FinePushbutton->setStyleSheet(QString::fromUtf8("font: 16pt \"Ubuntu\";"));
    buttons_layout->addWidget(m_FinePushbutton);
    connect(m_FinePushbutton, SIGNAL(clicked(bool)), this, SLOT(ShowFineAlignWin()));

    m_GeometryPushbutton=new QPushButton("Geometry",buttons_frame);
    m_GeometryPushbutton->setMinimumSize(QSize(0,40));
    m_GeometryPushbutton->setStyleSheet(QString::fromUtf8("font: 16pt \"Ubuntu\";"));
    buttons_layout->addWidget(m_GeometryPushbutton);
    connect(m_GeometryPushbutton,SIGNAL(clicked(bool)),this,SLOT(ShowGeomeryWin()));

    m_RestructPushbutton=new QPushButton("Reconstruction",buttons_frame);
    m_RestructPushbutton->setMinimumSize(QSize(0,40));
    m_RestructPushbutton->setStyleSheet(QString::fromUtf8("font: 16pt \"Ubuntu\";"));
    buttons_layout->addWidget(m_RestructPushbutton);
    connect(m_RestructPushbutton, SIGNAL(clicked(bool)), this, SLOT(ShowRestructWin()));

     m_PostprocessPushbutton=new QPushButton("Segmentation",buttons_frame);
     m_PostprocessPushbutton->setMinimumSize(QSize(0,40));
     m_PostprocessPushbutton->setStyleSheet(QString::fromUtf8("font: 16pt \"Ubuntu\";"));
     buttons_layout->addWidget(m_PostprocessPushbutton);
     m_PostprocessPushbutton->setDisabled(true);
     connect(m_PostprocessPushbutton, SIGNAL(clicked(bool)), this, SLOT(ShowPostProcessWin()));
    
    
    
    m_PreprocessBasedPushbutton=new QPushButton("Pre-processing",buttons_frame);
    m_PreprocessBasedPushbutton->setMinimumSize(QSize(0,40));
    m_PreprocessBasedPushbutton->setStyleSheet(QString::fromUtf8("font: 16pt \"Ubuntu\";"));
    //buttons_layout->addWidget(m_PreprocessBasedPushbutton);
    connect(m_PreprocessBasedPushbutton, SIGNAL(clicked(bool)), this, SLOT(ShowProgressWin()));

    m_AlignBasedPushbutton=new QPushButton("Alignment",buttons_frame);
    m_AlignBasedPushbutton->setMinimumSize(QSize(0,40));
    m_AlignBasedPushbutton->setStyleSheet(QString::fromUtf8("font: 16pt \"Ubuntu\";"));
    connect(m_AlignBasedPushbutton,SIGNAL(clicked(bool)),this,SLOT(ShowAlignWin()));
    //buttons_layout->addWidget(m_AlignBasedPushbutton);

    m_GeometryBasedPushbutton=new QPushButton("Geometry",buttons_frame);
    m_GeometryBasedPushbutton->setMinimumSize(QSize(0,40));
    m_GeometryBasedPushbutton->setStyleSheet(QString::fromUtf8("font: 16pt \"Ubuntu\";"));
    //buttons_layout->addWidget(m_GeometryBasedPushbutton);
    connect(m_GeometryBasedPushbutton,SIGNAL(clicked(bool)),this,SLOT(ShowGeomeryWin()));

    m_RestructBasedPushbutton=new QPushButton("Reconstruction",buttons_frame);
    m_RestructBasedPushbutton->setMinimumSize(QSize(0,40));
    m_RestructBasedPushbutton->setStyleSheet(QString::fromUtf8("font: 16pt \"Ubuntu\";"));
    //buttons_layout->addWidget(m_RestructBasedPushbutton);
    connect(m_RestructBasedPushbutton, SIGNAL(clicked(bool)), this, SLOT(ShowRestructWin()));
/*
    m_PostprocessBasedPushbutton=new QPushButton("Post-Processing");
    m_PostprocessBasedPushbutton->setMinimumSize(QSize(0,40));
    m_PostprocessBasedPushbutton->setStyleSheet(QString::fromUtf8("font: 16pt \"Ubuntu\";"));*/
    //buttons_layout->addWidget(m_PostprocessBasedPushbutton);
//     connect(m_PostprocessBasedPushbutton, SIGNAL(clicked(bool)), this, SLOT(ShowPostProcessWin()));

    m_PreprocessBasedPushbutton->hide();
    m_AlignBasedPushbutton->hide();
    m_GeometryBasedPushbutton->hide();
    m_RestructBasedPushbutton->hide();
//     m_PostprocessBasedPushbutton->hide();
}



void Mainwindow::CreateToolarea()
{
    QLabel *tool_label =new QLabel("Tools :");
    tool_label->setMaximumSize(QSize(100, 20));
    tool_label->setFrameShadow(QFrame::Raised);
    //tool_label->setTextFormat(Qt::LogText);
    tool_label->setTextFormat(Qt::PlainText);
    process_layout->addWidget(tool_label);
    //tool_layout->addWidget(tool_label);
    tool_frame=new QFrame(m_ProcessDockwidget);
    tool_frame->setFrameShape(QFrame::StyledPanel);
    tool_frame->setFrameShadow(QFrame::Raised);
    // tool_layout=new QVBoxLayout(tool_frame);
    //tool_layout->setContentsMargins(9,-1,-1,-1);
    m_EvaluateTool = new Evaluate_tool(tool_frame);
    QGridLayout *tool_gridlayout = new QGridLayout();
    QSpacerItem *verticalSpacer;
    verticalSpacer = new QSpacerItem(20, 50, QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
    tool_gridlayout->addWidget(m_EvaluateTool, 0, 0);
    tool_gridlayout->addItem(verticalSpacer, 1, 0);
    tool_gridlayout->setMargin(0);
    tool_frame->setLayout(tool_gridlayout);
    process_layout->addWidget(tool_frame);
    
    //added by liuce 
    connect(m_EvaluateTool,SIGNAL(Log(QString)),this,SLOT(LogOut(QString)));
}
void Mainwindow::CreateMdiarea()
{
    QWidget *CentralWidget=new QWidget(this);
    // QFrame *mainwin_frame=new QFrame();
    m_Mdiarea= new QMdiArea(CentralWidget);
    QHBoxLayout *horizontalLayout_3 = new QHBoxLayout(CentralWidget);
    QGridLayout *GirdLayout = new QGridLayout();
    GirdLayout->addWidget(m_Mdiarea,0,0,3,3);

//     m_SkipWidget = new CSkipWidget();
//     GirdLayout->addWidget(m_SkipWidget,0,3,1,1);
//     m_SkipWidget->hide();

    QFrame *frame_2 = new QFrame(CentralWidget);
    frame_2->setObjectName(QString::fromUtf8("frame_2"));
    frame_2->setFrameShape(QFrame::StyledPanel);
    frame_2->setFrameShadow(QFrame::Raised);
    frame_2->setMinimumWidth(500);//控制中央宽度
    QHBoxLayout *horizontalLayout_2 = new QHBoxLayout(frame_2);
    horizontalLayout_2->setSpacing(0);
    horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    QHBoxLayout *horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    QSpacerItem *horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout->addItem(horizontalSpacer);

    m_BackButton = new QPushButton("Back", frame_2);
    m_BackButton->setObjectName(QString::fromUtf8("m_BackButton"));
    horizontalLayout->addWidget(m_BackButton);

    m_SkipButton = new QPushButton("Skip", frame_2);
    m_SkipButton->setObjectName(QString::fromUtf8("m_SkipButton"));
    horizontalLayout->addWidget(m_SkipButton);


    m_NextButton = new QPushButton("Next", frame_2);
    m_NextButton->setObjectName(QString::fromUtf8("m_NextButton"));
    horizontalLayout->addWidget(m_NextButton);

    m_BackButton->setIcon(QIcon(":/images/back.png"));
    m_NextButton->setIcon(QIcon(":/images/next.png"));
    connect(m_BackButton, SIGNAL(clicked(bool)), this, SLOT(Back()));
    connect(m_SkipButton, SIGNAL(clicked(bool)), this, SLOT(Skip()));
    connect(m_NextButton, SIGNAL(clicked(bool)), this, SLOT(Next()));
    m_BackButton->setVisible(false);
    m_SkipButton->setVisible(false);
    m_NextButton->setVisible(false);
    horizontalLayout_2->addLayout(horizontalLayout);


    GirdLayout->addWidget(frame_2,3,0);
    horizontalLayout_3->addLayout(GirdLayout);
    m_Mdiarea->setBackground(QColor(200,200,200));
    this->setCentralWidget(CentralWidget);
}

void Mainwindow::ShowSetWorkpathWidget()
{
    CSetWorkSpaceWin *w = new CSetWorkSpaceWin(this);
    if(NULL != CEnv::CurrentWorkDirectory()) m_isFirstSet = false;
    if (m_isFirstSet){
     	w->FirstSet();
     	m_isFirstSet = false;
	connect(w, SIGNAL(WindowsClosed()), this, SLOT(NewProject()));
    }
    else{
      w->show();
      connect(w, SIGNAL(WindowsClosed()), this, SLOT(NewProject()));
    }
}
void Mainwindow::addProcess()
{
    //     Cinit::InitSet();
    QString filename = QFileDialog::getOpenFileName(this, tr("open"), QString(CEnv::InitWorkDirectory()), tr("Process files (*.pro)"));
    FileName=filename;
    if (filename.isEmpty());
    else{
	//Load process
	//innit
	QFileInfo fileInfo = QFileInfo(filename);
	QString str = filename.left(filename.length() - fileInfo.fileName().length() - 1);
	
	if (CProManage::LoadProcess(filename.toLatin1().data())){
	    m_FileTreeview->AddProjectFile(str);
	    CProManage::SetHaveProcess(true);
	    // SetProcessButtonEnable(true);
	    m_EvaluateTool->setEnabled(true);
	    if(CProManage::GetInstance()->GetMethod()=="BasedMethod")		m_BasedFree=BasedMethod;
	    else if(CProManage::GetInstance()->GetMethod()=="FreeMethod")    m_BasedFree=FreeMethod;
	    else   m_BasedFree=Null;	    

	    if(m_BasedFree!=Null){
	        if(m_BasedFree==BasedMethod){
		ChangeBasedButtons();
		UnFinish(PREPROCESSBASED);
		m_PreprocessBasedPushbutton->setEnabled(true);
		UnFinish(ALIGNBASED);
		m_AlignBasedPushbutton->setEnabled(true);
		UnFinish(GEOMETRYBASED);
		m_GeometryBasedPushbutton->setEnabled(true);
		UnFinish(RESTRUCTBASED);
		m_RestructBasedPushbutton->setEnabled(true);
		UnFinish(POSTPROCESSBASED);
        //post-process hasn't been implemented,so disable it in UI.
		//m_PostprocessBasedPushbutton->setEnabled(true);
//         m_PostprocessBasedPushbutton*/->hide();
		if (CProManage::GetInstance()->Finished(PREPROCESS_STATE_BASED)) Finish(PREPROCESSBASED);
		if (CProManage::GetInstance()->Finished(ALIGN_STATE_BASED)) Finish(ALIGNBASED);
		if (CProManage::GetInstance()->Finished(GERMETRY_STATE_BASED)) Finish(GEOMETRYBASED);
		if (CProManage::GetInstance()->Finished(RESTRUCT_STATE_BASED)) Finish(RESTRUCTBASED);
		//if (CProManage::GetInstance()->Finished(POSTPROCESS_STATE_BASED)) Finish(POSTPROCESSBASED);
	    }
	        else {
		ChangeFreeButtons();
		UnFinish(PREPROCESS);
		m_PreprocessPushbutton->setEnabled(true);
        //UnFinish(COARSEALIGN);
        //m_CoarsePushbutton->setEnabled(true);
		UnFinish(FINEALIGN);
		m_FinePushbutton->setEnabled(true);
		UnFinish(GEOMETRY);
		m_GeometryPushbutton->setEnabled(true);
		UnFinish(RESTRUCT);
		m_RestructPushbutton->setEnabled(true);
		UnFinish(POSTPROCESS);
        //post-process hasn't been implemented,so disable it in UI.
        m_PostprocessPushbutton->setEnabled(true);
//        m_PostprocessPushbutton->hide();
		if (CProManage::GetInstance()->Finished(PREPROCESS_STATE)) Finish(PREPROCESS);
		if (CProManage::GetInstance()->Finished(COSALI_STATE)) Finish(COARSEALIGN);
		if (CProManage::GetInstance()->Finished(FINEALI_STATE)) Finish(FINEALIGN);
		if (CProManage::GetInstance()->Finished(GERMETRY_STATE)) Finish(GEOMETRY);
		if (CProManage::GetInstance()->Finished(RESTRUCT_STATE)) Finish(RESTRUCT);
		if (CProManage::GetInstance()->Finished(POSTPROCESS_STATE)) Finish(POSTPROCESS);
	    }   
	    m_ProcessDockwidget->show();
	    m_FileDockwidget->show();
	    m_OutputDockwidget->show();
	    m_BackButton->setVisible(true);
	    m_SkipButton->setVisible(true);
	    m_NextButton->setVisible(true);
	    ShowProgressWin();
	    }  
	    else   {ShowSelectButton();    CProManage::ReWriteProFile();} //null

	}
	else{
	    QMessageBox::StandardButton rb = QMessageBox::warning(this, tr("Warning!"), tr("Load Fail!"), QMessageBox::Ok);
	}
    }
}

void Mainwindow::NewProject()
{
    SetStatusHide();
    m_ChangeMethodAction->setEnabled(false);
    qDebug("NewProject");
    CNewProjectWin *w = new CNewProjectWin();
    m_Mdiarea->closeAllSubWindows();
    QMdiSubWindow *subWindow = m_Mdiarea->addSubWindow(w);
    subWindow->setWindowFlags(Qt::FramelessWindowHint);
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(220, 220, 220));
    m_Mdiarea->setPalette(palette);
    subWindow->showMaximized();
    subWindow->setWindowTitle("New Project");
    connect(w, SIGNAL(NewSuccess(QString)), this, SLOT(AddProcessFile(QString)));
    connect(w,SIGNAL(OpenExistedProject()),this,SLOT(addProcess()));
}
void Mainwindow::AddProcessFile(QString filename)
{
    FileName=filename;
    m_EvaluateTool->setEnabled(true);
    m_FileTreeview->AddProjectFile(filename);
    ShowSelectButton();
}
void Mainwindow::Finish(int step)
{
    QString style = "color: green";
    QPalette   palette;
    palette.setColor(QPalette::ButtonText, QColor(0, 0, 0));
    switch (step){
	case PREPROCESS:
	    m_PreprocessPushbutton->setStyleSheet(style);
	    m_PreprocessPushbutton->setDown(true);
	    // 	    m_PreprocessButton->setPalette(palette);
	    break;
    //case COARSEALIGN:
        //m_CoarsePushbutton->setStyleSheet(style);
        //m_CoarsePushbutton->setDown(true);
	    // 	    m_CoarseButton->setPalette(palette);
    //    break;
	case FINEALIGN:
	    m_FinePushbutton->setStyleSheet(style);
	    m_FinePushbutton->setDown(true);
	    // 	    m_FineButton->setPalette(palette);
	    break;
	case GEOMETRY:
	    m_GeometryPushbutton->setStyleSheet(style);
	    m_GeometryPushbutton->setDown(true);
	    // 	    m_GeometryButton->setPalette(palette);
	    break;
	case RESTRUCT:
	    m_RestructPushbutton->setStyleSheet(style);
	    m_RestructPushbutton->setDown(true);
	    break;
	    // 	    m_RestructButton->setPalette(palette);
	case POSTPROCESS:
        m_PostprocessPushbutton->setStyleSheet(style);
        m_PostprocessPushbutton->setDown(true);
	    break;	    
	case PREPROCESSBASED:
	    m_PreprocessBasedPushbutton->setStyleSheet(style);
	    m_PreprocessBasedPushbutton->setDown(true);
	    // 	    m_PreprocessButton->setPalette(palette);
	    break;
	case ALIGNBASED:
	    m_AlignBasedPushbutton->setStyleSheet(style);
	    m_AlignBasedPushbutton->setDown(true);
	    // 	    m_CoarseButton->setPalette(palette);
	    break;
	case GEOMETRYBASED:
	    m_GeometryBasedPushbutton->setStyleSheet(style);
	    m_GeometryBasedPushbutton->setDown(true);
	    // 	    m_GeometryButton->setPalette(palette);
	    break;
	case RESTRUCTBASED:
	    m_RestructBasedPushbutton->setStyleSheet(style);
	    m_RestructBasedPushbutton->setDown(true);
	    break;
	    // 	    m_RestructButton->setPalette(palette);
// 	case POSTPROCESSBASED:
// 	    m_PostprocessBasedPushbutton->setStyleSheet(style);
// 	    m_PostprocessBasedPushbutton->setDown(true);
    }
    
}

void Mainwindow::UnFinish(int step)
{
    // qDebug()<<step<<" unfinish";
    //     QString style = "background-color: rgb(255, 192, 255);";
    QString style = "color: red";
    QPalette   palette;
    palette.setColor(QPalette::ButtonText, QColor(255, 0, 0));
    switch (step){ 
	case PREPROCESS:
	    m_PreprocessPushbutton->setStyleSheet(style);
	    m_PreprocessPushbutton->setDown(true);
	    qDebug() << "here";
	    // 	    m_PreprocessButton->setPalette(palette);
	    break;
    //case COARSEALIGN:
        //m_CoarsePushbutton->setStyleSheet(style);
        //m_CoarsePushbutton->setDown(true);
	    // 	    m_CoarseButton->setPalette(palette);
     //   break;
	case FINEALIGN:
	    m_FinePushbutton->setStyleSheet(style);
	    m_FinePushbutton->setDown(true);
	    // 	    m_FineButton->setPalette(palette);
	    break;
	case GEOMETRY:
	    m_GeometryPushbutton->setStyleSheet(style);
	    m_GeometryPushbutton->setDown(true);
	    // 	    m_GeometryButton->setPalette(palette);
	    break;
	case RESTRUCT:
	    m_RestructPushbutton->setStyleSheet(style);
	    m_RestructPushbutton->setDown(true);
	    // 	    m_RestructButton->setPalette(palette);
	    break;
    case POSTPROCESS:
        m_PostprocessPushbutton->setStyleSheet(style);
        m_PostprocessPushbutton->setDown(true);
        break;
	    
	    
	case PREPROCESSBASED:
	    m_PreprocessBasedPushbutton->setStyleSheet(style);
	    m_PreprocessBasedPushbutton->setDown(true);
	    // 	    m_PreprocessButton->setPalette(palette);
	    break;
	case ALIGNBASED:
	    m_AlignBasedPushbutton->setStyleSheet(style);
	    m_AlignBasedPushbutton->setDown(true);
	    // 	    m_CoarseButton->setPalette(palette);
	    break;
	case GEOMETRYBASED:
	    m_GeometryBasedPushbutton->setStyleSheet(style);
	    m_GeometryBasedPushbutton->setDown(true);
	    // 	    m_GeometryButton->setPalette(palette);
	    break;
	case RESTRUCTBASED:
	    m_RestructBasedPushbutton->setStyleSheet(style);
	    m_RestructBasedPushbutton->setDown(true);
	    break;
	    // 	    m_RestructButton->setPalette(palette);
// 	case POSTPROCESSBASED:
// 	    m_PostprocessBasedPushbutton->setStyleSheet(style);
// 	    m_PostprocessBasedPushbutton->setDown(true);
    }
}

void Mainwindow::ShowProgressWin()
{
    m_ChangeMethodAction->setEnabled(true);
    CloseAllSubWindows();	
     if(m_BasedFree==BasedMethod){
	m_currentProcess = PREPROCESSBASED;
	m_PreprocessBasedPushbutton->setDown(true);
	m_PreprocessBasedPushbutton->setEnabled(true);
    }
    else{
	m_currentProcess = PREPROCESS;
	m_PreprocessPushbutton->setDown(true);
	m_PreprocessPushbutton->setEnabled(true);
    }
    qDebug() << "current" << m_currentProcess;

//     m_SkipWidget->show();

    m_PreprocessWin = new PreprocessWin;
    QMdiSubWindow *subWindow = m_Mdiarea->addSubWindow(m_PreprocessWin);
    subWindow->setWindowFlags(Qt::FramelessWindowHint);
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(228, 221, 255));
    subWindow->setPalette(palette);
    subWindow->showMaximized();
    SetBackNextEnable(m_currentProcess);
    connect(m_PreprocessWin, SIGNAL(Log(QString)), this, SLOT(LogOut(QString)));
    connect(m_PreprocessWin, SIGNAL(Start()), this, SLOT(LockProcessButton()));
    connect(m_PreprocessWin, SIGNAL(Finish(QString)), this, SLOT(Finished(QString)));
    connect(m_PreprocessWin, SIGNAL(RunError()), this, SLOT(Failed()));
    
}

void Mainwindow::ShowCoarseAlignWin()
{
    qDebug() << "Show CoarseAlignWin";
    CloseAllSubWindows();
    m_currentProcess = COARSEALIGN;
    qDebug() << "current" << m_currentProcess;
//     if(Check(m_currentProcess)==false) {
//         ShowProgressWin();
//         return;
//     }

    m_CoarseAlignWin = new CoarseAlignWin;
    QMdiSubWindow *subWindow = m_Mdiarea->addSubWindow(m_CoarseAlignWin);
    subWindow->setWindowFlags(Qt::FramelessWindowHint);
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(228, 221, 255));
    subWindow->setPalette(palette);
    subWindow->showMaximized();
    connect(m_CoarseAlignWin, SIGNAL(Log(QString)), this, SLOT(LogOut(QString)));
    connect(m_CoarseAlignWin, SIGNAL(Finish(QString)), this, SLOT(Finished(QString)));
    connect(m_CoarseAlignWin, SIGNAL(Start()), this, SLOT(LockProcessButton()));
    connect(m_CoarseAlignWin, SIGNAL(RunError()), this, SLOT(Failed()));

    SetBackNextEnable(m_currentProcess);

    //m_CoarsePushbutton->setDown(true);
    //m_CoarsePushbutton->setEnabled(true);
}

void Mainwindow::ShowFineAlignWin()
{
    CloseAllSubWindows();
    CloseAllSubWindows();
    m_currentProcess = FINEALIGN;
    qDebug() << "current" << m_currentProcess;

    if(Check(m_currentProcess)==false) {
        ShowProgressWin();
        return;
    }
    m_FineAlignWin = new FineAlignWin;
    QMdiSubWindow *subWindow = m_Mdiarea->addSubWindow(m_FineAlignWin);
    subWindow->setWindowFlags(Qt::FramelessWindowHint);
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(228, 221, 255));
    subWindow->setPalette(palette);
    subWindow->showMaximized();
    connect(m_FineAlignWin, SIGNAL(Log(QString)), this, SLOT(LogOut(QString)));
    connect(m_FineAlignWin, SIGNAL(Finish(QString)), this, SLOT(Finished(QString)));
    connect(m_FineAlignWin, SIGNAL(Start()), this, SLOT(LockProcessButton()));
    connect(m_FineAlignWin, SIGNAL(RunError()), this, SLOT(Failed()));
    SetBackNextEnable(m_currentProcess);

    m_FinePushbutton->setDown(true);
    m_FinePushbutton->setEnabled(true);


}
void Mainwindow::ShowAlignWin()
{
 
    CloseAllSubWindows();
    m_currentProcess= ALIGNBASED;
    qDebug() << "current" << m_currentProcess;
//     if(Check(m_currentProcess)==false) {
//         ShowProgressWin();
//         return;
//     }
    m_AlignWin = new AlignWin;
    QMdiSubWindow *subWindow = m_Mdiarea->addSubWindow(m_AlignWin);
    subWindow->setWindowFlags(Qt::FramelessWindowHint);
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(228, 221, 255));
    subWindow->setPalette(palette);
    subWindow->showMaximized();
    connect(m_AlignWin, SIGNAL(Log(QString)), this, SLOT(LogOut(QString)));
    connect(m_AlignWin, SIGNAL(Finish(QString)), this, SLOT(Finished(QString)));
    connect(m_AlignWin, SIGNAL(Start()), this, SLOT(LockProcessButton()));
    connect(m_AlignWin, SIGNAL(RunError()), this, SLOT(Failed()));

    SetBackNextEnable(m_currentProcess);
    m_AlignBasedPushbutton->setDown(true);
    m_AlignBasedPushbutton->setEnabled(true);
}

void Mainwindow::ShowGeomeryWin()
{
    CloseAllSubWindows();

    qDebug() << "current" << m_currentProcess;

    CDoc::InitInstance();
    QString input, inputangle;

    inputangle = CProManage::GetInstance()->GetDir(ANGLE_DIR).data();

    if (CProManage::GetInstance()->Exite("FIN_MRC_DIR")) {
        input = CProManage::GetInstance()->GetDir(FIN_MRC_DIR).data();
    }
    else if (CProManage::GetInstance()->Exite("ALI_MRC_DIR")) {
        input = CProManage::GetInstance()->GetDir(ALI_MRC_DIR).data();
    }
    else
    {
        input = CProManage::GetInstance()->GetDir(MRC_DIR).data();
    }
    qDebug() << "input = " << input;
    qDebug() << "inputangle = " << inputangle;

    if (!CDoc::GetItemModel().Open(input.toLatin1().data(), inputangle.toLatin1().data())) {
        std::cout << "can't open input file." << endl;
        CDoc::DestoryInstance();
    }
    CDoc::GetSectSetting().SetThickness(400);
    CDoc::GetSectSetting().SetSlice1Name("top_rec.mrc");
    CDoc::GetSectSetting().SetSlice2Name("mid_rec.mrc");
    CDoc::GetSectSetting().SetSlice3Name("bot_rec.mrc");
    CSectToolFrame *foo = new CSectToolFrame(CDoc::GetItemModel());
    QMdiSubWindow *subWindow = m_Mdiarea->addSubWindow(foo);
    subWindow->setWindowFlags(Qt::FramelessWindowHint);
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(228, 221, 255));
    subWindow->setPalette(palette);
    subWindow->showMaximized();

    if(m_BasedFree==BasedMethod) {
        m_currentProcess = GEOMETRYBASED;
        m_GeometryBasedPushbutton->setDown(true);
        m_GeometryBasedPushbutton->setEnabled(true);
    }
    else {
        m_currentProcess = GEOMETRY;
        m_GeometryPushbutton->setDown(true);
        m_GeometryPushbutton->setEnabled(true);
    }
    SetBackNextEnable(m_currentProcess);

    if(Check(m_currentProcess)==false) {
        if(m_BasedFree==BasedMethod) ShowAlignWin();
        else ShowFineAlignWin();
        return;
    }
    if(CProManage::GetInstance()->GetState(GERMETRY_STATE)==0||CProManage::GetInstance()->GetState(GERMETRY_STATE_BASED)==0) {
        CSectTool* sect_tool = (CSectTool*)foo->m_secttool;
        sect_tool->LoadSlice(0,"top_rec.mrc");
        sect_tool->LoadSlice(1,"mid_rec.mrc");
        sect_tool->LoadSlice(2,"bot_rec.mrc");
    }
    
    foo->show();
    connect(foo, SIGNAL(Start()), this, SLOT(LockProcessButton()));
    connect(foo, SIGNAL(Finish(QString)), this, SLOT(Finished(QString)));
    connect(foo, SIGNAL(RunError()), this, SLOT(Failed()));
    connect(foo, SIGNAL(Log(QString)), this, SLOT(LogOut(QString)));
  
     
     
}

void Mainwindow::ShowRestructWin()
{
    qDebug() << "current" << m_currentProcess;
    CloseAllSubWindows();
    m_RestructWin = new RestructWin();
    QMdiSubWindow *subWindow = m_Mdiarea->addSubWindow(m_RestructWin);
    subWindow->setWindowFlags(Qt::FramelessWindowHint);
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(228, 221, 255));
    subWindow->setPalette(palette);
    subWindow->showMaximized();
    if(m_BasedFree==BasedMethod) {
        m_currentProcess = RESTRUCTBASED;
        m_RestructBasedPushbutton->setDown(true);
        m_RestructBasedPushbutton->setEnabled(true);
    }
    else {
        m_currentProcess = RESTRUCT;
        m_RestructPushbutton->setDown(true);
        m_RestructPushbutton->setEnabled(true);
    }
    SetBackNextEnable(m_currentProcess);

    if(Check(m_currentProcess)==false) {
        if(m_BasedFree==BasedMethod) ShowAlignWin();
        else ShowFineAlignWin();
        return;
    }
    connect(m_RestructWin, SIGNAL(Log(QString)), this, SLOT(LogOut(QString)));
    connect(m_RestructWin, SIGNAL(Finish(QString)), this, SLOT(Finished(QString)));
    connect(m_RestructWin, SIGNAL(Start()), this, SLOT(LockProcessButton()));
    connect(m_RestructWin, SIGNAL(RunError()), this, SLOT(Failed()));

}

void Mainwindow::ShowPostProcessWin()
{
    CloseAllSubWindows();
    if(m_BasedFree==BasedMethod) {
      m_currentProcess = POSTPROCESSBASED;
    }
    else{
      m_currentProcess = POSTPROCESS;
    }
    qDebug() << "current" << m_currentProcess;
    if(Check(m_currentProcess)==false) {
        ShowRestructWin();
        return;
    }
    m_PostProcessWin = new PostProcessWin;
    QMdiSubWindow *subWindow = m_Mdiarea->addSubWindow(m_PostProcessWin);
    subWindow->setWindowFlags(Qt::FramelessWindowHint);
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(228, 221, 255));
    subWindow->setPalette(palette);
    subWindow->showMaximized();
    SetBackNextEnable(m_currentProcess);

    connect(m_PostProcessWin, SIGNAL(Log(QString)), this, SLOT(LogOut(QString)));
    connect(m_PostProcessWin, SIGNAL(Finish(QString)), this, SLOT(Finished(QString)));
    connect(m_PostProcessWin, SIGNAL(Start()), this, SLOT(LockProcessButton()));
    connect(m_PostProcessWin, SIGNAL(RunError()), this, SLOT(Failed()));
    // m_postProcessingButton->setDown(true);
}

void Mainwindow::FinishGermetry()
{
    cout << "finish" << endl;
    CProManage::AddItem("GERMETRY_STATE", "FINISH");
    Finish(GEOMETRY);
    emit FileChange();
}
void Mainwindow::CloseAllSubWindows()
{
  // if(NULL != m_PreprocessWin) delete m_PreprocessWin;
   // delete m_AlignWin;
    
    for (int i = 0; i<m_Mdiarea->subWindowList().size(); i++) {
        m_Mdiarea->subWindowList().at(i)->close();
    }
    this->setWindowTitle("AuTom");
    if(m_BasedFree==BasedMethod) {
        m_PreprocessBasedPushbutton->setDown(false);
        m_AlignBasedPushbutton->setDown(false);
        m_GeometryBasedPushbutton->setDown(false);
        m_RestructBasedPushbutton->setDown(false);
//         m_PostprocessBasedPushbutton->setDown(false);
    }
    else {
        m_PreprocessPushbutton->setDown(false);
        //m_CoarsePushbutton->setDown(false);
        m_FinePushbutton->setDown(false);
        m_GeometryPushbutton->setDown(false);
        m_RestructPushbutton->setDown(false);
        m_PostprocessPushbutton->setDown(false);
    }

}
void Mainwindow::LogOut(QString str)
{
    m_OutputTextedit->textCursor().insertText(str);
    m_OutputTextedit->moveCursor(QTextCursor::End);
}

void Mainwindow::Failed()
{
    qDebug() << "failed";
    SetProcessButtonEnable(true);
    //     SetProcessActionEnable(true);	why deleted?
    m_EvaluateTool->setEnabled(true);
    // SetBackNextEnable(m_currentProcess)
    UnFinish(m_currentProcess);

    QString str;
    str = " Fail!\n";
    m_OutputTextedit->textCursor().insertText(str);
    m_OutputTextedit->moveCursor(QTextCursor::End);
    emit FileChange();
SetBackNextEnable(m_currentProcess);
SetProcessActionEnable(true);
    //   m_PreprocessButton->setDown(true);
}
void Mainwindow::Finished(QString str)
{
    if (str == "PreProcess") Finish(PREPROCESS);
    else if (str == "Coarse Align") Finish(COARSEALIGN);
    else if (str == "Fine Align") Finish(FINEALIGN);
    else if (str == "Geometry") Finish(GEOMETRY);
    else if (str == "Restruct") Finish(RESTRUCT);
    else if (str == "PostProcessing") Finish(POSTPROCESS);
    else if (str == "PreProcessBased") Finish(PREPROCESSBASED);
    else if (str == "AlignBased") Finish(ALIGNBASED);
    else if (str == "GeometryBased") Finish(GEOMETRYBASED);
    else if (str == "RestructBased") Finish(RESTRUCTBASED);
    else if (str == "PostProcessingBased") Finish(POSTPROCESSBASED);
    SetProcessButtonEnable(true);
    m_EvaluateTool->setEnabled(true);
    SetProcessActionEnable(true);
    SetBackNextEnable(m_currentProcess);
    str += " success!\n";
    m_OutputTextedit->textCursor().insertText(str);
    m_OutputTextedit->moveCursor(QTextCursor::End);

    emit FileChange();
}
void Mainwindow::ShowSelectButton()
{
    m_SelectWidget=new CSelectWidget();
    CloseAllSubWindows();
    QMdiSubWindow *subWindow = m_Mdiarea->addSubWindow(m_SelectWidget);
    subWindow->showMaximized();
    connect(m_SelectWidget,SIGNAL(MarkerFreeFlow()),this,SLOT(MarkerFreeFlowSelected()));
    connect(m_SelectWidget,SIGNAL(MarkerBasedFlow()),this,SLOT(MarkerBasedFlowSelected()));
}
void Mainwindow::MarkerFreeFlowSelected()
{
    // CProManage::GetInstance()->ChangeMethod();
    CProManage::GetInstance()->SetMethod(FreeMethod);
    CProManage::ReWriteProFile();
    m_BasedFree=FreeMethod;
    ChangeFreeButtons();
    UnFinish(PREPROCESS);
    UnFinish(COARSEALIGN);
    UnFinish(FINEALIGN);
    UnFinish(GEOMETRY);
    UnFinish(RESTRUCT);
    UnFinish(POSTPROCESS);
    m_ProcessDockwidget->show();
    m_FileDockwidget->show();
    m_OutputDockwidget->show();
    m_BackButton->setVisible(true);
    m_SkipButton->setVisible(true);
    m_NextButton->setVisible(true);
    ShowProgressWin();


}
void Mainwindow::MarkerBasedFlowSelected()
{
    // CProManage::GetInstance()->ChangeMethod();
    CProManage::GetInstance()->SetMethod(BasedMethod);
    CProManage::ReWriteProFile();
    m_BasedFree=BasedMethod;
    ChangeBasedButtons();
    UnFinish(PREPROCESSBASED);
    UnFinish(ALIGNBASED);
    UnFinish(GEOMETRYBASED);
    UnFinish(RESTRUCTBASED);
    UnFinish(POSTPROCESSBASED);
    m_ProcessDockwidget->show();
    m_FileDockwidget->show();
    m_OutputDockwidget->show();
    m_BackButton->setVisible(true);
    m_SkipButton->setVisible(true);
    m_NextButton->setVisible(true);
    ShowProgressWin();

}
void Mainwindow::ChangeBasedButtons()
{
    if(m_PreprocessPushbutton->isHidden()) return;
    buttons_layout->removeWidget(m_PreprocessPushbutton);
    buttons_layout->removeWidget(m_FinePushbutton);
    //buttons_layout->removeWidget(m_CoarsePushbutton);
    buttons_layout->removeWidget(m_GeometryPushbutton);
    buttons_layout->removeWidget(m_RestructPushbutton);
    buttons_layout->removeWidget(m_PostprocessPushbutton);

    m_PreprocessPushbutton->hide();
    m_FinePushbutton->hide();
    //m_CoarsePushbutton->hide();
    m_GeometryPushbutton->hide();
    m_RestructPushbutton->hide();
    m_PostprocessPushbutton->hide();

    buttons_layout->addWidget(m_PreprocessBasedPushbutton);
    buttons_layout->addWidget(m_AlignBasedPushbutton);
    buttons_layout->addWidget(m_GeometryBasedPushbutton);
    buttons_layout->addWidget(m_RestructBasedPushbutton);
//     buttons_layout->addWidget(m_PostprocessBasedPushbutton);

    m_PreprocessBasedPushbutton->show();
    m_AlignBasedPushbutton->show();
    m_GeometryBasedPushbutton->show();
    m_RestructBasedPushbutton->show();
    //m_PostprocessBasedPushbutton->show();

}
void Mainwindow::ChangeFreeButtons()
{
    if(m_PreprocessBasedPushbutton->isHidden()) return;
    buttons_layout->removeWidget(m_PreprocessBasedPushbutton);
    buttons_layout->removeWidget(m_AlignBasedPushbutton);
    buttons_layout->removeWidget(m_GeometryBasedPushbutton);
    buttons_layout->removeWidget(m_RestructBasedPushbutton);
//     buttons_layout->removeWidget(m_PostprocessBasedPushbutton);

    m_PreprocessBasedPushbutton->hide();
    m_AlignBasedPushbutton->hide();
    m_GeometryBasedPushbutton->hide();
    m_RestructBasedPushbutton->hide();
//     m_PostprocessBasedPushbutton->hide();

    buttons_layout->addWidget(m_PreprocessPushbutton);
    //buttons_layout->addWidget(m_CoarsePushbutton);
    buttons_layout->addWidget(m_FinePushbutton);
    buttons_layout->addWidget(m_GeometryPushbutton);
    buttons_layout->addWidget(m_RestructPushbutton);
    buttons_layout->addWidget(m_PostprocessPushbutton);

    m_PreprocessPushbutton->show();
    m_FinePushbutton->show();
    //m_CoarsePushbutton->show();
    m_GeometryPushbutton->show();
    m_RestructPushbutton->show();
    m_PostprocessPushbutton->show();
}

void Mainwindow::Back()
{
    if(m_BasedFree==BasedMethod) {
        switch (m_currentProcess) {
        case ALIGNBASED:
            ShowProgressWin();
            break;
        case GEOMETRYBASED:
            ShowAlignWin();
            break;
        case RESTRUCTBASED:
            ShowGeomeryWin();
            break;
        case POSTPROCESSBASED:
            ShowRestructWin();
            break;
        }
    }
    else {
        switch (m_currentProcess) {
        case COARSEALIGN:
            ShowProgressWin();
            break;
        case FINEALIGN:
            ShowCoarseAlignWin();
            break;
        case GEOMETRY:
            ShowFineAlignWin();
            break;
        case RESTRUCT:
            ShowGeomeryWin();
            break;
        case POSTPROCESS:
            ShowRestructWin();
            break;
        }
    }

}

void Mainwindow::Skip()
{
    if(m_BasedFree==BasedMethod) {
        switch (m_currentProcess) {
        case PREPROCESSBASED:
            ShowAlignWin();
            break;
        case ALIGNBASED:
            ShowGeomeryWin();
            break;
        case GEOMETRYBASED:
            ShowRestructWin();
            break;
//         case RESTRUCTBASED:
//             ShowPostProcessWin();
	    break;
        }
    }
    else {
        switch (m_currentProcess) {
        case PREPROCESS:
            ShowCoarseAlignWin();
            break;
        case COARSEALIGN:
            ShowFineAlignWin();
            break;
        case FINEALIGN:
            ShowGeomeryWin();
            break;
        case GEOMETRY:
            ShowRestructWin();
            break;
        case RESTRUCT:
//             ShowPostProcessWin();
	      break;
        }
    }

}

void Mainwindow::Next()
{
    if(m_BasedFree==BasedMethod) {
        switch (m_currentProcess) {
        case PREPROCESSBASED:
            ShowAlignWin();
            break;
        case ALIGNBASED:
            ShowGeomeryWin();
            break;
        case GEOMETRYBASED:
            ShowRestructWin();
            break;
        case RESTRUCTBASED:
//             ShowPostProcessWin();
             break;
        }
    }
    else {
        switch (m_currentProcess) {
        case PREPROCESS:
            ShowCoarseAlignWin();
            break;
        case COARSEALIGN:
            ShowFineAlignWin();
            break;
        case FINEALIGN:
            ShowGeomeryWin();
            break;
        case GEOMETRY:
            ShowRestructWin();
            break;
        case RESTRUCT:
        //    ShowPostProcessWin();
	  break;
        }
    }
}
void Mainwindow::SetBackNextEnable(int cur)
{
    if (cur == PREPROCESS||cur == PREPROCESSBASED) {
        m_BackButton->setEnabled(false);
        if(CProManage::GetInstance()->GetState(PREPROCESS_STATE)==0||CProManage::GetInstance()->GetState(PREPROCESS_STATE_BASED)==0) {
            m_NextButton->setEnabled(true);//finish
            m_SkipButton->setEnabled(false);
        }
        else { //unfinish
            m_SkipButton->setEnabled(true);
            m_NextButton->setEnabled(false);
        }
    }
    else if (cur == COARSEALIGN) {
        m_BackButton->setEnabled(true);
        if(CProManage::GetInstance()->GetState(COSALI_STATE)==0) {
            m_NextButton->setEnabled(true);//finish
            m_SkipButton->setEnabled(false);
        }
        else { //unfinish
            m_SkipButton->setEnabled(true);
            m_NextButton->setEnabled(false);
        }
    }
    else if (RESTRUCT == cur || RESTRUCTBASED == cur){
	m_BackButton->setEnabled(true);
        m_NextButton->setEnabled(false);
        m_SkipButton->setEnabled(false);
    }
    else if (POSTPROCESS == cur || cur == POSTPROCESSBASED) {
        m_BackButton->setEnabled(true);
        m_NextButton->setEnabled(false);
        m_SkipButton->setEnabled(false);
    }
    else {
        m_BackButton->setEnabled(true);
        m_NextButton->setEnabled(true);
        m_SkipButton->setEnabled(false);
    }
}
void Mainwindow::LockProcessButton()
{
    SetProcessButtonEnable(false);
    SetProcessActionEnable(false);
    m_NextButton->setEnabled(false);
    m_BackButton->setEnabled(false);
    m_SkipButton->setEnabled(false);
    m_EvaluateTool->setEnabled(false);
}


void Mainwindow::SetProcessButtonEnable(bool b)
{
    if(m_BasedFree==BasedMethod) {
        m_PreprocessBasedPushbutton->setEnabled(b);
        m_AlignBasedPushbutton->setEnabled(b);
        m_GeometryBasedPushbutton->setEnabled(b);
        m_RestructBasedPushbutton->setEnabled(b);
        //m_PostprocessBasedPushbutton->setEnabled(b);
    }
    else {
        m_PreprocessPushbutton->setEnabled(/*false*/b);
        //m_CoarsePushbutton->setEnabled(b);
        m_FinePushbutton->setEnabled(b);
        m_GeometryPushbutton->setEnabled(b);
        m_RestructPushbutton->setEnabled(b);
        m_PostprocessPushbutton->setEnabled(b);
    }
}

void Mainwindow::SetProcessActionEnable(bool b)
{
    m_NewProcessAction->setEnabled(b);
    m_OpenProcessAction->setEnabled(b);
    m_SetWorkpathAction->setEnabled(b);
}
void Mainwindow::ChangeMethod()
{
    QMessageBox msgBox;
    msgBox.setText("All your current progress will be lost.");
    msgBox.setInformativeText("Do you want to change method?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel );
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();
    if(ret==QMessageBox::Cancel)
        return;
    
    CloseAllSubWindows();
    CProManage::GetInstance()->ChangeMethod();
    CProManage::ReWriteProFile();
    if(m_BasedFree==BasedMethod) m_BasedFree=FreeMethod;
    else m_BasedFree=BasedMethod;
    ReInit(FileName);
}
void Mainwindow::ReInit(QString filename)
{
    FileName=filename;
    m_EvaluateTool->setEnabled(true);
    m_FileTreeview->ClearTree(0);
    // m_FileTreeview->AddProjectFile(filename);
    m_FileTreeview->Update();
    if(m_BasedFree==BasedMethod) {
        ChangeBasedButtons();
        UnFinish(PREPROCESSBASED);
        UnFinish(ALIGNBASED);
        UnFinish(GEOMETRYBASED);
        UnFinish(RESTRUCTBASED);
        UnFinish(POSTPROCESSBASED);

    }
    else {
        ChangeFreeButtons();
        UnFinish(PREPROCESS);
        UnFinish(COARSEALIGN);
        UnFinish(FINEALIGN);
        UnFinish(GEOMETRY);
        UnFinish(RESTRUCT);
        UnFinish(POSTPROCESS);
    }
    ShowProgressWin();
}

void Mainwindow::SetStatusHide()
{
    m_ProcessDockwidget->hide();
    m_FileDockwidget->hide();
    m_OutputDockwidget->hide();
    m_BackButton->setEnabled(false);
    m_SkipButton->setEnabled(false);
    m_NextButton->setEnabled(false);
}
void Mainwindow::SetStatusShow()
{
    m_ProcessDockwidget->show();
    m_FileDockwidget->show();
    m_OutputDockwidget->show();
}


void Mainwindow::Close2NewProject()
{
    if(!m_FileDockwidget->isVisible())//在新建工程界面
        return;
    QMessageBox msgBox;
    msgBox.setText(tr("Are you sure you want to close this project?"));
    msgBox.setWindowTitle(tr("Close project") );
    msgBox.setStandardButtons(QMessageBox::Cancel|QMessageBox::Close);
    msgBox.setDefaultButton(QMessageBox::Close);
    switch( msgBox.exec() ) {
    case QMessageBox::Cancel:
        break;
    case QMessageBox::Close:
        NewProject();
        break;
    }
}

void Mainwindow::Close2OpenProject()
{
    if(!m_FileDockwidget->isVisible())//在新建工程界面
    {
        addProcess();
        return;
    }
    QMessageBox msgBox;
    msgBox.setText(tr("Are you sure you want to close this project?"));
    msgBox.setWindowTitle(tr("Close project") );
    msgBox.setStandardButtons(QMessageBox::Cancel|QMessageBox::Close);
    msgBox.setDefaultButton(QMessageBox::Close);
    switch( msgBox.exec() ) {
    case QMessageBox::Cancel:
        break;
    case QMessageBox::Close:
        CloseCurrentProject();
        addProcess();
        break;
    }
}

void Mainwindow::CloseCurrentProject()
{
    m_FileTreeview->AddProjectFile("");
    m_Mdiarea->closeAllSubWindows();
    m_OutputTextedit->clear();
    if(m_BasedFree==BasedMethod) {
        ChangeBasedButtons();
        UnFinish(PREPROCESSBASED);
        UnFinish(ALIGNBASED);
        UnFinish(GEOMETRYBASED);
        UnFinish(RESTRUCTBASED);
        UnFinish(POSTPROCESSBASED);
    }
    else {
        ChangeFreeButtons();
        UnFinish(PREPROCESS);
        UnFinish(COARSEALIGN);
        UnFinish(FINEALIGN);
        UnFinish(GEOMETRY);
        UnFinish(RESTRUCT);
        UnFinish(POSTPROCESS);
    }
    NewProject();
}


void Mainwindow::StartProjectionErrorTools(){
    m_EvaluateTool->m_methed_combobox->setCurrentIndex(0);
    m_EvaluateTool->StartEvaluate();
    
}
void  Mainwindow::StartNCCTools(){
    m_EvaluateTool->m_methed_combobox->setCurrentIndex(1);
    m_EvaluateTool->StartEvaluate();
}

void Mainwindow::StartTrajplotTools(){
    m_EvaluateTool->m_methed_combobox->setCurrentIndex(2);
    m_EvaluateTool->StartEvaluate();
}

void Mainwindow::OpenUrl(){
    bool result = QDesktopServices::openUrl(QUrl("http://ear.ict.ac.cn", QUrl::TolerantMode));
    qDebug()<<(result ? "open Website http://ear.ict.ac.cn success" : "open Website http://ear.ict.ac.cn fail ");
}


bool Mainwindow::Check(int cur)
{
    switch (cur) {
    case  FINEALIGN:
      //   if( CProManage::GetInstance()->GetState(COSALI_STATE)==0)  return true;
        //     else { QMessageBox::warning(this , tr("Warning") , tr("You haven't completed CoarseAlignment!")  ,QMessageBox::Close); return false;}
    case  COARSEALIGN:
    case  ALIGNBASED:
        if( CProManage::GetInstance()->GetState(PREPROCESS_STATE)==0|| CProManage::GetInstance()->GetState(PREPROCESS_STATE_BASED)==0)  return true;
        else {
            QMessageBox::warning(this , tr("Warning") , tr("You haven't completed Preprocess!")  ,QMessageBox::Close);
            return false;
        }
    case GEOMETRYBASED:
    case  GEOMETRY:
        if( CProManage::GetInstance()->GetState(ALIGN_STATE_BASED)==0|| CProManage::GetInstance()->GetState(FINEALI_STATE)==0)  return true;
        else {
            QMessageBox::warning(this , tr("Warning") , tr("You haven't completed Alignment!")  ,QMessageBox::Close);
            return false;
        }
    case  RESTRUCT:
    case RESTRUCTBASED :
        if( CProManage::GetInstance()->GetState(GERMETRY_STATE_BASED)==0|| CProManage::GetInstance()->GetState(GERMETRY_STATE)==0)  return true;
        else {
            QMessageBox::warning(this , tr("Warning") , tr("You haven't completed Geometry!")  ,QMessageBox::Close);
            return false;
        }
    case  POSTPROCESS:
      return false;
    case  POSTPROCESSBASED :
        if( CProManage::GetInstance()->GetState(RESTRUCT_STATE_BASED)==0|| CProManage::GetInstance()->GetState(RESTRUCT_STATE)==0)  return true;
        else {
            QMessageBox::warning(this , tr("Warning") , tr("You haven't completed Reconstruction!")  ,QMessageBox::Close);
            return false;
        }
    }

}
void Mainwindow::showAboutDialog(){
    AboutDialog* dialog=new AboutDialog();
    dialog->exec();
    delete dialog;
    
}
Mainwindow::~Mainwindow()
{}
void Mainwindow::enableMenuAction(bool visibel)
{
    if(!visibel){
        QObject* sender=QObject::sender();
        if(sender==m_FileDockwidget){
            m_view_fileDock_action->setEnabled(true);
        }else if(sender==m_ProcessDockwidget){
            m_view_processDock_action->setEnabled(true);            
        }
    }
}
void Mainwindow::showDock()
{
    QObject* sender=QObject::sender();
    if(sender==m_view_fileDock_action){
        m_FileDockwidget->show();
        m_view_fileDock_action->setEnabled(false);
    }
    else if(sender==m_view_processDock_action){
        m_ProcessDockwidget->show();
        m_view_processDock_action->setEnabled(false);
    }
    
}

//#include "MainWindow.moc"
