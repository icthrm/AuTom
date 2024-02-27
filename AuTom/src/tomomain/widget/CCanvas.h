#ifndef CCANVAS_H__
#define CCANVAS_H__

//#include <QtGui>
#include <QtWidgets>

#define DEFAULT_SIZE		512

/** current only support gray pgm */

class CCanvas:public QWidget
{
Q_OBJECT
protected:
    const QPixmap* img;
    
public:
    CCanvas(QWidget* parent = 0, Qt::WindowFlags f = 0): QWidget(parent, f), img(NULL){}
    ~CCanvas(){}
    
signals:
    void sizeChanged(int w, int h);
    
protected:
    virtual void paintEvent(QPaintEvent* );
    virtual void resizeEvent(QResizeEvent* );
    
public:
    void UpdateImageData(const QPixmap* _img);
    void move(int x, int y);
};

#endif
