#ifndef CSECTVIEW_H__
#define CSECTVIEW_H__

#include <QtGui>
#include "widget/CCanvas.h"
#include "widget/CMultiSlider.h"
#include "widget/CTransparentFilm.h"
#include "widget/datatype.h"
#include <QLabel>
#include <QComboBox>
class CSectView: public QWidget{
Q_OBJECT
private:
    CCanvas* m_canvas;
    CTransparentFilm* m_film;
    CMultiSlider* m_mslider;
    
private:
    ItemModel& _data;
    ViewData _view;
    
private slots:
    void SynchronizeFilm(int idx, float val);
    void SynchronizeSlider(int idx, float val);
    
public:
    explicit CSectView(ItemModel& im, QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~CSectView();
    
public:
    void SetEnabled(bool on = true);
    float AnchorValue(int idx) const;
    void SetValue(int idx, float val);
};

#define BUILD_MODEL_MSG				1000+1

class CSectViewFrame: public QMainWindow{
Q_OBJECT
private:
    CSectView* m_sectview;
    QToolBar* m_topbar;
    QAction* start_rec;
    QAction* stop_rec;
    QLabel* m_datatype_label;
    QComboBox* m_datatype_combobox;
    
private:
    ItemModel& _data;
    float* loc;				//3
    bool builderror;
    
private:
    void SetTopToolBar(QToolBar* toolbar);
    
private slots:
    void StartBuildRec();
    void StopBuildRec();
    
private:
    virtual void customEvent(QEvent* );
    virtual void closeEvent(QCloseEvent* );
    
public:
    explicit CSectViewFrame(ItemModel& im, float* _loc, QWidget* parent = 0, Qt::WindowFlags flags = 0);
    virtual ~CSectViewFrame();
    
};

#endif
