#ifndef EVALUATE_TOOL_H
#define EVALUATE_TOOL_H

#include <QtGui>
#include "CProgressBar.h"

#define EVALUATE_MSG		1000+8
#define EVALUATE_NCC_MSG        1000+9
#define EVALUATE_TRAJ_MSG	1000+15

//added by liuce
class CTrajPlotWin:public QWidget{
    Q_OBJECT
    friend class Evaluate_tool;
private:
    QLabel* m_instruction; 
    QPushButton* m_okBtn;
    QPushButton* m_cancelBtn;
   // QPushButton* m_killBtn;
    QComboBox* m_modeBox;
    QLabel* m_rawOrAligned_label;
    QComboBox* m_rawOrAligned_combobox;
    CProgressBar* m_progressbar;
    QStackedLayout* m_stack;
      
    QFrame* frame0;
    QFrame* frame1;
    
public:
    CTrajPlotWin();
    ~CTrajPlotWin();
    
signals:
    void start(QString message,int rawOrAligned);
    //void kill();
public slots:
    void startProcess();
    
    
};

class Evaluate_tool: public QWidget
{
  Q_OBJECT
  friend class Mainwindow;
private:
  QLabel* m_tool_lable;
  QFrame* m_frame;
  QLabel*    m_lable;
  QComboBox* m_methed_combobox;
  QPushButton* m_start_button;
  QFrame *m_ButtonFrame;
  QFrame *m_SetFram;
  CProgressBar *m_progressbar;
  QStackedLayout *m_ls;
  CTrajPlotWin* m_trajplotWin;
  int m_method;

  
public:
  explicit Evaluate_tool(QWidget* parent = 0, Qt::WindowFlags f = 0);
  virtual ~Evaluate_tool();
  void ShowResult();
  void ShowNCCResult();
  
signals:
    void Log(QString message);
    void end();//where is its corresbonding slot?
    
  
public slots:
  void StartEvaluate();
  void StartTrajplotEvaluate(QString message,int rawOrAligned);
  void kill();
  
private:
  void StartProjectionEvaluate(int caller);
  void StartNCCEvaluate();
  void StartTrajplotEvaluate();
  private:
    virtual void customEvent(QEvent* );//overwrite QObject's virtual protected function ```void QObject::customEvent(QEvent * envent)``` ,added by liuce
};


#endif
