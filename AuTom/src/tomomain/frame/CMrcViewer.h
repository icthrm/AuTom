#ifndef calibGeo_H
#define calibGeo_H

#include <QtGui>
#include <vector>
#include "widget/CCanvas.h"
#include "mrcimg/mrc2img.h"
#include "widget/datatype.h"
#include <cxcore.hpp>
#include <cv.hpp>
#include <QtGui/QPixmap>
#include "CCtrlPanel.h"
#include "widget/CMultiSlider.h"
#include "widget/datatype.h"


class CMrcViewer: public QMainWindow
{
Q_OBJECT
private:
    QSplitter* lfSplitter;
    CCanvas* m_canvas;
    CMultiSlider* m_mslider;
    CCtrlPanel* m_ctrlpanel;
    QLabel* llabel;
    
private:
    ItemModel& _data;
    ViewData _view;
    
private:
    
    static int testc;
private slots:
    void Test(int _no, float val){
// 	std::cout<<testc<<" NO: "<<_no<<" val: "<<val<<std::endl;
// 	std::cout.flush();
	testc++;
	llabel->setText(QString("%1 %2 %3").arg(testc).arg(_no).arg(val));
    }
    
public:
    CMrcViewer(ItemModel& im);
    virtual ~CMrcViewer();
    
public:
    void UpdateCanvas(int index, int white_range, int black_range, float scale);
    void UpdateCanvas();
};

#endif // calibGeo_H
