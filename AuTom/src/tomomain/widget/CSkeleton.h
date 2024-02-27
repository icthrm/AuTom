#ifndef CSKELETON_H__
#define CSKELETON_H__

#include <QtGui>
#include "CCanvas.h"
#include "datatype.h"
#include <cxcore.hpp>
#include <cv.hpp>

class CSkeleton: public QPushButton{
public:
    enum ShowStrategy{FullFill, Zoom};
    
Q_OBJECT
private:
    QPixmap skeleton;
    QLabel* m_label;
    CCanvas* m_canvas;
    
private:
    const static int w_marge = 8;
    const static int h_top_marge = 28;
    const static int h_bott_marge = 10;
    
    constexpr const static float default_w = 1;
    constexpr const static float default_h = 0.618;
    
public:
    explicit CSkeleton(QWidget* parent = 0);
    virtual ~CSkeleton();
    
public:
    void SetText(const QString& str, const IplImage* img, int width, int height, ShowStrategy strategy = CSkeleton::FullFill);
    void SetImage(const IplImage* img ,int width, int height);
    
public:
    QPixmap* Pixmap(){return &skeleton;}
    void Update(){
	int w=m_canvas->width();
	int h=m_canvas->height();
	m_canvas->UpdateImageData(&skeleton);
	m_canvas->resize(w,h);
    }
};

#endif
