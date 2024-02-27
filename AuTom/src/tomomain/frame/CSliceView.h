#ifndef CSLICEVIEW_H__
#define CSLICEVIEW_H__

#include <QtGui>
#include "widget/CCanvas.h"
#include "widget/CTransparentDemarcater.h"
#include "widget/datatype.h"

class CSliceView: public QWidget{
Q_OBJECT
private:
    CCanvas* m_canvas;
    CTransparentDemarcater* m_demarcater;
    
private:
    ItemModel& _data;
    ViewData _view;
    
public:
    explicit CSliceView(ItemModel& im, QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~CSliceView();
    
public:
    void SetEnabled(bool on = true);
    void SetFullLine(bool on);
    bool IsFullLine() const;
    void AddLine(float x1, float y1, float x2, float y2);
    QLineF GetLineValue(int idx);
};

class CSliceViewFrame: public QMainWindow{
Q_OBJECT
private:
    CSliceView* m_sliceview;
    QToolBar* m_topbar;
    QAction* lock;
    QAction* full_show;
    
private:
    ItemModel& _data;
    bool locked;
    QLineF* lines;				
    
private:
    void SetTopToolBar(QToolBar* toolbar);
    
private slots:
    void LockScreen();
    void DoFullLine();
    
private:
    virtual void customEvent(QEvent* );
    virtual void closeEvent(QCloseEvent* );
    
public:
    explicit CSliceViewFrame(ItemModel& im, QLineF* _lines, QWidget* parent = 0, Qt::WindowFlags flags = 0);
    virtual ~CSliceViewFrame();
    
};

#endif