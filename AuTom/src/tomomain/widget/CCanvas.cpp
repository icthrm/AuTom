#include <QtGui>
#include "CCanvas.h"

void CCanvas::paintEvent(QPaintEvent* e)
{
    QPainter paint(this);
    if(!img){
	return;
    }
    
    int w_marge = (width()-img->width())/2;
    int h_marge = (height()-img->height())/2;
    paint.drawPixmap(w_marge, h_marge, img->width(), img->height(), *img);
    
//     QWidget::paintEvent();
}

void CCanvas::resizeEvent(QResizeEvent* e)
{
    emit sizeChanged(e->size().width(), e->size().height());
    
    QWidget::resizeEvent(e);
}


void CCanvas::UpdateImageData(const QPixmap* _img)
{
    img = _img;
    
    setMinimumSize(img->width(),img->height());
    
    /*if(width() < img->width()){
	resize(img->width(), height());
    }
    
    if(height() < img->height()){
	resize(width(), img->height());
    }*/
    resize(img->width() , img->height());
    
    update();
}
#include "iostream"
void CCanvas::move(int x, int y){//move的距离是以最开始的坐标为基准，若x,y固定，则无法连续移动。
  if(NULL != parentWidget()){
    //图像宽度高度都小于显示区的情况
    int maxx = parentWidget()->width() - img->width();
    int minx = 0;
    int maxy = parentWidget()->height() - img->height();
    int miny = 0;
    //图像宽度大于显示区的情况
    if(img->width() > parentWidget()->width()){
      maxx = 0;
      minx = parentWidget()->width() - img->width();
    }
    //图像高度大于显示区的情况
    if(img->height() > parentWidget()->height()){
      maxy = 0;
      miny = parentWidget()->height() - img->height();
    }
    if(x > maxx) x = maxx;
    if(x < minx) x = minx;
    if(y > maxy) y = maxy;
    if(y < miny) y = miny;
  }
  QWidget::move(x, y);
}

#include "CCanvas.moc"