#ifndef CSECTTOOL_H__
#define CSECTTOOL_H__

#include <QtGui>
#include "widget/CSkeleton.h"
#include "promanage/ProManage.h"
#include "CSectView.h"
#include "CSliceView.h"
#include <string>
#include "utils/InputSaver.h"
#include "widget/CProgressBar.h"

#define AUTOGDETERM_MSG 1000+17
class CSectTool: public QWidget//geometry步骤中主界面上方的四个图及相应操作
{
Q_OBJECT
private:
    
    CSkeleton* m_scene;//左上角图像
    CSkeleton* m_slice[3];//其他图像
    CSectViewFrame* m_sectviewf;//点击左上角图像之后弹出的图像对话框
    CSliceViewFrame* m_sliceviewf;
    
    int skeleton_w;
    int skeleton_h;
    
private:
    ItemModel& _data;
    ViewData _view;
    QPixmap scene_copy;
    float loc[3];
    
    enum s_state{null, init, written};
    
    struct{
	ItemModel _data;
	ViewData _view;
	QLineF lines[2];
	QPixmap s_copy;
	s_state state;
    }slice_dv[3];
    int cur_slice_idx;
    
private slots:
    void ShowSectView();
    void ShowSlice1();
    void ShowSlice2();
    void ShowSlice3();
    void UpdateScene();
    void UpdateSliceSkeleton();
    
private:
    void ShowSlice(int idx);
    
public:
    void LoadSlice(int idx, const char* slice_name);
    
public:
    explicit CSectTool(ItemModel& im, QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~CSectTool();
    QLineF GetDemarcater(int idx, int lidx);
    void GetYLoc(float* yloc);
    bool IsCalculable();
    //void setSlicesEnable(bool enable);
};

class CSectToolFrame: public QMainWindow{//geometry步骤中的主界面，包含一个CSectTool
Q_OBJECT
public:
    CSectTool* m_secttool;
private:
    CProgressBar *m_progressbar;
    QStackedLayout *m_ls;
    QString m_outputpar;
    QFrame *m_SetFram;
    
   const double thickness_range = 10000.0;
    const double z_o_p_range = 10000.0;
    const int z_o_p_decimals = 6;	
    
    QToolBar* m_topbar;
    QAction* calib;
    QAction* auto_demar;
    QAction* to_set;
    
    //@ wang lian shan
    QDialog* m_setwindow;
    QDialogButtonBox *buttonBox;
    QSpinBox *spinBox;
    
    QWidget *m_mainWidget;
    QFrame *m_imageFram;
    QFrame *m_ButtonFrame;
    
    QPushButton* m_Calibrate;
    QPushButton* m_Set;
    //@ wang lian shan
    
    //@wanglianshan
    
    //added by chice
    QCheckBox * m_input_checkbox;
    QFrame * m_InputFrame;
    QLineEdit * m_thickness;
    QLineEdit * m_zshift;
    QLineEdit * m_offset;
    QLineEdit * m_pitch;
    QPushButton * m_inputFinish;
    QLabel * m_thickness_label;
    QLabel * m_zshift_label;
    QLabel * m_offset_label;
    QLabel * m_pitch_label;
    
    QCheckBox * m_autog_checkbox;
    //int inputmethod;
    
    InputSaver * inputSaver;
    //hy  
    QFrame * m_autogFrame;
    QPushButton* m_autogdeterm;
    QComboBox   *m_dataTypeBox;
    protected:
    void CreateImageFrame();
    void CreateButton();
    void CreateInputFrame();
    void CreateAutogFrame();
    void setImageAndButtonFrameEnable(bool enable);//added by chice
    void setInputFrameEnable(bool enable); // added by chice
    void setAutogFrameEnable(bool enable);
    //@wanglianshan
    
    void setImageAndButtonFrameEnable_auto(bool enable);
    
signals:
    void Finish(QString step);
    void Start();
    void Log(QString message);   
    void RunError();
    
private:
    ItemModel& _data;
    QLineF* lines;				
    
private:
    void SetTopToolBar(QToolBar* toolbar);

    const QLineF& FullLine(const QLineF& itm);
     int isNum(std::string s);
    
    ;
private slots:
    void DoCalibration();
    void writeFile(float z_shift, float offset, float thickness, float pitch_angle, int inputmethod);//added by chice
    void writeFile_geo();
    void finishGeometry();
    void AutoDemarcate();
    void StopAutoDemarcate();
    //@wls
    void SetSetWindow();
    void Setting(QAbstractButton * button);
    //@wls
    
    void ChangeinputMethod();//added by chice
    void inputProcess(); //added by chice
    
    void ChangeinputMethod_auto();
    
    void AutoG();
private:
    virtual void customEvent(QEvent* );
    virtual void closeEvent(QCloseEvent* );
    
public:
    explicit CSectToolFrame(ItemModel& im, QWidget* parent = 0, Qt::WindowFlags flags = 0); 
    virtual ~CSectToolFrame();
    
};

#endif