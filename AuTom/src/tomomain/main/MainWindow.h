 #ifndef MainWindow_H
#define MainWindow_H

#include <QtGui>
#include "widget/Editor.h"
#include "widget/TreeView.h"
#include "widget/PreprocessWin.h"
#include "widget/CNewProject.h"
#include "widget/CoarseAlignWin.h"
#include "widget/FineAlignWin.h"
#include "widget/RestructWin.h"
#include "widget/PostProcessWin.h"
#include "frame/CSectTool.h"
#include "frame/CSliceView.h"
#include "widget/CSetWorkSpace.h"
#include "promanage/ProManage.h"
#include "frame/CSectTool.h"
#include "frame/CSliceView.h"
#include "env/env.h"
#include "widget/datatype.h"
#include "widget/Evaluate_tool.h"
#include "widget/CSelect.h"
#include "widget/AlignWin.h"
#include "widget/CSkip.h"
//#include "utils/InputSaver.h"

#define PREPROCESS 1
#define COARSEALIGN 2
#define GEOMETRY 3
#define FINEALIGN 4
#define RESTRUCT 5
#define POSTPROCESS 6

#define PREPROCESSBASED 7
#define ALIGNBASED 8
#define GEOMETRYBASED 9
#define RESTRUCTBASED 10
#define POSTPROCESSBASED 11
class QAction;
//

class Mainwindow : public QMainWindow
{
    Q_OBJECT
    
public:
    Mainwindow();
    virtual ~Mainwindow();
    
    bool m_isFirstSet;
    int m_currentProcess;
    int m_BasedFree;

    QString FileName;
    void emitSet();
    void showLocation();
private:
    QMenu *m_ProjectMenu;
    QMenu *m_WindowMenu;
    QMenu *m_SettingsMenu;
    QMenu *m_ToolsMenu;
    QMenu *m_HelpMenu;
    QMenu *m_viewMenu;
    
    QAction *m_OpenProcessAction;
    QAction *m_NewProcessAction;
    QAction *m_SetWorkpathAction;
    QAction *m_ChangeMethodAction;
    QAction *m_ProjectionErrorAction;
    QAction *m_NCCAction;
    QAction *m_TrajplotAction;
    QAction *m_about;
    QAction *m_view_processDock_action;
    QAction *m_view_fileDock_action;
    
    QToolBar *m_OpenProcessToolbar;
    QToolBar *m_NewProcessToolbar;
    QToolBar *m_SetWorkpathToolbar;
    
    QDockWidget *m_FileDockwidget;
    QDockWidget *m_OutputDockwidget;
    QDockWidget *m_ProcessDockwidget;
   // QWidget *m_SelectWidget;
    
    QTextEdit *m_OutputTextedit;
    
    QWidget * file_widget;
    
    CSkipWidget *m_SkipWidget;
    
    QPushButton *m_PreprocessPushbutton;
    QPushButton *m_CoarsePushbutton;
    QPushButton *m_FinePushbutton;
    QPushButton *m_GeometryPushbutton;
    QPushButton *m_RestructPushbutton;
    QPushButton *m_PostprocessPushbutton;
    
    QPushButton *m_PreprocessBasedPushbutton;
    QPushButton *m_AlignBasedPushbutton;
    QPushButton *m_GeometryBasedPushbutton;
    QPushButton *m_RestructBasedPushbutton;
    QPushButton *m_PostprocessBasedPushbutton;
    
    QPushButton *m_BackButton;
    QPushButton  *m_SkipButton;
    QPushButton *m_NextButton;
    QPushButton *m_MarkerBasedButton;
    QPushButton *m_MarkerFreeButton;
    
    QBoxLayout *buttons_layout;
    QBoxLayout *process_layout;
    QBoxLayout *tool_layout;
    
    QWidget *buttons_widget;
    QFrame *buttons_frame;
    QFrame *process_frame;
    QFrame *tool_frame;
    
    PreprocessWin *m_PreprocessWin;
    CoarseAlignWin *m_CoarseAlignWin;
    FineAlignWin *m_FineAlignWin;
    RestructWin *m_RestructWin;
    PostProcessWin *m_PostProcessWin;
    AlignWin *m_AlignWin;
    CSelectWidget *m_SelectWidget;
    
    
    
    Evaluate_tool *m_EvaluateTool;
    TreeView* m_FileTreeview;
   
    QMdiArea *m_Mdiarea;
    
    void CreateMenus();
    void CreateToolbars();
    void CreateProcessDockwidget();
    void CreateFileDockwidget();
    void CreateOutputDockwidget();
    void CreateSelectWidget();
    void CreatePushbuttons();
    void CreateToolarea();
    void CreateMdiarea();
    void SetBackNextEnable(int cur);
    
    void ChangeBasedButtons();
    void ChangeFreeButtons();

    void Finish(int step);
    void UnFinish(int step);
    
    void SetProcessButtonEnable(bool b);
    void SetProcessActionEnable(bool b);
    
    void ReInit(QString filename);
    
    void SetStatusHide();
    void SetStatusShow();
    
   // InputSaver * inputSaver;
    
signals:
    void Set();
    void FileChange();
public slots:  
    void ShowSetWorkpathWidget();
private slots:
    //void ShowSetWorkpathWidget();//##!!
    //void OK(QString str);
    //void Quit(QString str);
    void LogOut(QString str);
    void Finished(QString str);
    void Failed();
    void FinishGermetry();
   // void Resize(bool b);
    void ShowProgressWin();
    void ShowCoarseAlignWin();
    void ShowFineAlignWin();
    void ShowGeomeryWin();
    void ShowRestructWin();
    void ShowPostProcessWin();
    void ShowAlignWin();
   // void ShowSetWorkSpaceWin();
    void CloseAllSubWindows();
    void AddProcessFile(QString filename);
    void addProcess();
    void NewProject();
    void ShowSelectButton();
    void MarkerFreeFlowSelected();
    void MarkerBasedFlowSelected();
    void Back();
    void Skip();
    void Next();
    void LockProcessButton();
    void ChangeMethod();
    void Close2NewProject();
    void Close2OpenProject();
    void CloseCurrentProject();
    bool Check(int cur);
    void StartNCCTools();
    void StartProjectionErrorTools();
    void StartTrajplotTools();
    void OpenUrl();
    void showAboutDialog();
    void showDock();
    void enableMenuAction(bool);
    
};

// class SelectWidget: public QWidget
// {
//     Q_OBJECT
// public:
//     SelectWidget(QWidget* parent=0, Qt::WindowFlags f=0);
//     virtual ~SelectWidget();
// signals:
//     void MarkerFreeFlow();
//     private slots:
//     void MarkerFreeFlowClicked();
// };
#endif
