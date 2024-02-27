#include "CTransparentDemarcater.h"

#include <iostream>
#include <cmath>
#include <cassert>
#include <sys/stat.h>

CTransparentDemarcater::CTransparentDemarcater(QWidget* parent, Qt::WindowFlags f): QWidget(parent, f)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::WindowStaysOnTopHint);
    setWindowFlags(Qt::X11BypassWindowManagerHint);
    setFocusPolicy(Qt::StrongFocus);
    
    setMouseTracking(true);
    mouse_on_id = -1;
    focus_on_id = -1;
    pressed_id = -1;
    enabled = true;
    fullline = false;
}

CTransparentDemarcater::~CTransparentDemarcater()
{

}

void CTransparentDemarcater::MaskCCanvas(CCanvas* bak)
{
    background = bak;
    setParent(bak);
    bak->setGeometry(x(), y(), bak->width(), bak->height());
    setFixedSize(background->width(), background->height());    
    connect(background, SIGNAL(sizeChanged(int,int)), this, SLOT(BackGroundChanged(int,int)));
}

void CTransparentDemarcater::BackGroundChanged(int w, int h)
{
    background->setGeometry(x(), y(), background->width(), background->height());
    setGeometry(x(), y(), background->width(), background->height());
    setFixedSize(background->width(), background->height());    
}

void CTransparentDemarcater::AddLine(float x1, float y1, float x2, float y2)
{
    __line tmp;
    tmp.x1 = x1;
    tmp.y1 = y1;
    tmp.x2 = x2;
    tmp.y2 = y2;
    tmp.pt1_s = __line::plain;
    tmp.pt2_s = __line::plain;
    
    lines.push_back(tmp);
}

// const QLine& CTransparentDemarcater::VerticalLine(const QLine& itm, int w)
// {
// #ifndef ROUND(x)
//     #define ROUND(x)		int(x+.5)
//     
//     static QLine tmp;
//     
//     int x1 = itm.x1(), y1 = itm.y1(), x2 = itm.x2(), y2 = itm.y2(), dx = x2-x1, dy = y2-y1;
//     float mx = (x1+x2)*.5, my = (y1+y2)*.5, tanxy = dy/dx, tanyx = dx/dy;
//     float rvaln = (w*.5+1)*sqrt(1+tanyx*tanyx);	//+-
//     float lvaln = tanyx*y1+dx*.5+tanxy*my;
//     float lmuln = -tanxy-tanyx;
//     
//     float r_y1 = (+rvaln-lvaln)/lmuln;
//     float r_y2 = (-rvaln-lvaln)/lmuln;
//     
//     float r_x1 = -tanxy*(r_y1-my)+mx;
//     float r_x2 = -tanxy*(r_y2-my)+mx;
//     
//     tmp.setLine(ROUND(r_x1), ROUND(r_y1), ROUND(r_x2), ROUND(r_y2));
//     return tmp;
//     
//     #undef ROUND(x)
// #endif
// }

const QLine& CTransparentDemarcater::ConvertToQLine(const __line& itm)
{
    static QLine tmp;
    tmp.setLine(itm.x1*width(), itm.y1*height(), itm.x2*width(), itm.y2*height());
    return tmp;
}

const CTransparentDemarcater::__line& CTransparentDemarcater::FullLine(const CTransparentDemarcater::__line& itm)
{
    static __line l;
    l.x1 = 0; l.x2 = 1;
    l.y1 = itm.y1-itm.x1*(itm.y2-itm.y1)/(itm.x2-itm.x1);
    l.y2 = itm.y1+(1-itm.x1)*(itm.y2-itm.y1)/(itm.x2-itm.x1);
    
    return l;
}


void CTransparentDemarcater::DrawLine(const __line& itm, QPainter& painter)
{
    const QColor free_c(35,255,35);

    painter.setPen(QPen(free_c, 2, Qt::SolidLine, Qt::RoundCap));
    QLine l;
    if(fullline){
	l = ConvertToQLine(FullLine(itm));
    }
    else{
	l = ConvertToQLine(itm);
    }
    painter.drawLine(l);
    const QLine& lp = ConvertToQLine(itm);
    
    DrawEndPoint(lp.p1(), itm.pt1_s, painter);
    DrawEndPoint(lp.p2(), itm.pt2_s, painter);
}

void CTransparentDemarcater::DrawEndPoint(const QPoint& pt, __line::__state s, QPainter& painter)
{
    static const QColor endp_c(255,255,0);
    static const QColor line_c(35,255,35); 
    
    if(s == __line::plain){
	painter.setBrush(Qt::NoBrush);
	painter.setPen(QPen(endp_c, 1, Qt::SolidLine, Qt::RoundCap));
	painter.drawEllipse(pt.x()-r, pt.y()-r, r*2, r*2);
    }
    else if(s == __line::mouse_on){
	DrawShadowPoint(pt, QColor(255,255,0), r, painter);
	painter.setBrush(Qt::NoBrush);
	painter.setPen(QPen(endp_c, 1, Qt::DashDotLine, Qt::RoundCap));
	painter.drawEllipse(pt.x()-r, pt.y()-r, r*2, r*2);
    }
    else if(s == __line::focus_on){
	DrawShadowPoint(pt, QColor(0,255,50), r, painter);
	painter.setBrush(Qt::NoBrush);
	painter.setPen(QPen(QColor(0,255,0), 1, Qt::DashDotLine, Qt::RoundCap));
	painter.drawEllipse(pt.x()-r, pt.y()-r, r*2, r*2);
    }
    else if(s == __line::pressed){
	DrawShadowPoint(pt, QColor(60,200,60), r, painter);
	painter.setBrush(Qt::NoBrush);
	painter.setPen(QPen(QColor(0,255,0), 1, Qt::DashDotLine, Qt::RoundCap));
	painter.drawEllipse(pt.x()-r, pt.y()-r, r*2, r*2);
    }
}


void CTransparentDemarcater::DrawShadowPoint(const QPoint& pt, const QColor& color, int rad, QPainter& painter)
{
    QRadialGradient radialGradient(pt.x(), pt.y(), rad+diffuse_width, pt.x(), pt.y()); 
    radialGradient.setColorAt(0, Qt::transparent);
    radialGradient.setColorAt(0.4, Qt::transparent); 
    radialGradient.setColorAt(0.7, color); 
    radialGradient.setColorAt(1.0, Qt::transparent);
    painter.setBrush(QBrush(radialGradient));
    painter.setPen(Qt::NoPen); 
    painter.drawEllipse(pt.x()-r-diffuse_width, pt.y()-r-diffuse_width, (r+diffuse_width)*2, (r+diffuse_width)*2); 
}

void CTransparentDemarcater::ShowLineFull(bool on)
{
    fullline = on;
    update();
}

bool CTransparentDemarcater::IsLineFull() const
{
    return fullline;
}

void CTransparentDemarcater::paintEvent(QPaintEvent* e)
{    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    for(int i = 0; i < lines.size(); i++){
	DrawLine(lines[i], painter);
    }
//     QWidget::paintEvent();
}

bool CTransparentDemarcater::IsInEndPoint1(int x, int y, const QLine& itm)
{
    return (x-itm.x1())*(x-itm.x1())+(y-itm.y1())*(y-itm.y1()) <= (r+diffuse_width+3)*(r+diffuse_width+3);
}

bool CTransparentDemarcater::IsInEndPoint2(int x, int y, const QLine& itm)
{
    return (x-itm.x2())*(x-itm.x2())+(y-itm.y2())*(y-itm.y2()) <= (r+diffuse_width+3)*(r+diffuse_width+3);
}

void CTransparentDemarcater::mouseMoveEvent(QMouseEvent* e)
{   
    if(!enabled){
	QWidget::mouseMoveEvent(e);
	return;
    }
    
    int tmp = -1; bool on_0;
    
    for(int i = 0; i < lines.size(); i++){
	if(IsInEndPoint1(e->x(), e->y(), ConvertToQLine(lines[i]))){
	    tmp = i;
	    on_0 = true;
	    break;
	}
	if(IsInEndPoint2(e->x(), e->y(), ConvertToQLine(lines[i]))){
	    tmp = i;
	    on_0 = false;
	    break;
	}
    }
    
    if(tmp == -1 && pressed_id == -1){
	if(mouse_on_id != -1 && (mouse_on_id != focus_on_id || focus_on_0 != mouse_on_0)){
	    mouse_on_0 ? lines[mouse_on_id].pt1_s = __line::plain: lines[mouse_on_id].pt2_s = __line::plain;
	    
	    mouse_on_id = -1;
	    update();
	}
	
	return;
    }
    
    if(tmp != -1 && (focus_on_id == -1 || on_0 != focus_on_0)){
	if(mouse_on_id == tmp && on_0 == mouse_on_0){
	    return;
	}
	else{
	    if(mouse_on_id != -1){
		mouse_on_0 ? lines[mouse_on_id].pt1_s = __line::plain: lines[mouse_on_id].pt2_s = __line::plain;
	    }
	    
	    mouse_on_id = tmp;
	    mouse_on_0 = on_0;
	    mouse_on_0 ? lines[mouse_on_id].pt1_s = __line::mouse_on: lines[mouse_on_id].pt2_s = __line::mouse_on;
	    update();
	    return;
	}	
    }
    
    if(pressed_id != -1){
	__point& now_pt = pressed_0 ? lines[pressed_id].pt1: lines[pressed_id].pt2;
	if(e->x() >= 0 && e->x() <= width() && e->y() >= 0 && e->y() <= height()){	
	    now_pt.x = e->x()/(float)width();
	    now_pt.y = e->y()/(float)height();
	
	    update();
	}
	else if(e->x() < 0 && e->y() >= 0 && e->y() <= height()){
	    bool to_update = false;
	    if(now_pt.x != 0){
		now_pt.x = 0;
		to_update = true;
	    }
	    if(e->y() != (int)now_pt.y*height()){
		now_pt.y = e->y()/(float)height();
		to_update = true;
	    }
	    if(to_update){ update();}
	}
	else if(e->x() < 0 && e->y() < 0){
	    if(now_pt.x != 0 || now_pt.y != 0){
		now_pt.x = 0;
		now_pt.y = 0;
		update();
	    }
	}
	else if(e->x() >= 0 && e->x() <= width() && e->y() < 0){
	    bool to_update = false;
	    if(now_pt.y != 0){
		now_pt.y = 0;
		to_update = true;
	    }
	    if(e->x() != (int)now_pt.x*width()){
		now_pt.x = e->x()/(float)width();
		to_update = true;
	    }
	    if(to_update){ update();}
	}
	else if(e->x() > width() && e->y() < 0){
	    if(now_pt.x != 1 || now_pt.y != 0){
		now_pt.x = 1;
		now_pt.y = 0;
		update();
	    }
	}
	else if(e->x() > width() && e->y() >= 0 && e->y() <= height()){
	    bool to_update = false;
	    if(now_pt.x != 1){
		now_pt.x = 1;
		to_update = true;
	    }
	    if(e->y() != (int)now_pt.y*height()){
		now_pt.y = e->y()/(float)height();
		to_update = true;
	    }
	    if(to_update){ update();}
	}
	else if(e->x() > width() && e->y() > height()){
	    if(now_pt.x != 1 || now_pt.y != 1){
		now_pt.x = 1;
		now_pt.y = 1;
		update();
	    }
	}
	else if(e->x() >= 0 && e->x() <= width() && e->y() > height()){
	    bool to_update = false;
	    if(now_pt.y != 1){
		now_pt.y = 1;
		to_update = true;
	    }
	    if(e->x() != (int)now_pt.x*width()){
		now_pt.x = e->x()/(float)width();
		to_update = true;
	    }
	    if(to_update){ update();}
	}
	else if(e->x() < 0 && e->y() > height()){
	    if(now_pt.x != 0 || now_pt.y != 1){
		now_pt.x = 0;
		now_pt.y = 1;
		update();
	    }
	}
    }
    else{
	if(tmp != focus_on_id || on_0 != focus_on_0){
	    if(mouse_on_id != -1 && mouse_on_id != focus_on_id){
		mouse_on_0 ? lines[mouse_on_id].pt1_s = __line::plain: lines[mouse_on_id].pt2_s = __line::plain;
		mouse_on_id = -1;
	    }
	    mouse_on_id = tmp; mouse_on_0 = on_0;
	    mouse_on_0 ? lines[mouse_on_id].pt1_s = __line::mouse_on: lines[mouse_on_id].pt2_s = __line::mouse_on;
	    update();
	    return;
	}
    }
}

void CTransparentDemarcater::mousePressEvent(QMouseEvent* e)
{   
    if(!enabled){
	QWidget::mousePressEvent(e);
	return;
    }
    
    int tmp = -1; bool on_0;
    
    for(int i = 0; i < lines.size(); i++){
	if(IsInEndPoint1(e->x(), e->y(), ConvertToQLine(lines[i]))){
	    tmp = i;
	    on_0 = true;
	    break;
	}
	if(IsInEndPoint2(e->x(), e->y(), ConvertToQLine(lines[i]))){
	    tmp = i;
	    on_0 = false;
	    break;
	}
    }
    
    if(tmp == -1 && focus_on_id == -1){	
	return;
    }
    
    if(pressed_id != -1){
	pressed_0 ? lines[pressed_id].pt1_s = __line::plain: lines[pressed_id].pt2_s = __line::plain;
	pressed_id = -1;
    }
    
    if(focus_on_id != -1){
	if(tmp == -1 && e->x() >= 0 && e->x() <= width() && e->y() >= 0 && e->y() <= height()){
	    
	    pressed_id = focus_on_id;
	    pressed_0 = focus_on_0;
	    pressed_0 ? lines[pressed_id].pt1_s = __line::pressed: lines[pressed_id].pt2_s = __line::pressed;
	    
	    __point& now_pt = pressed_0 ? lines[pressed_id].pt1: lines[pressed_id].pt2;
	    now_pt.x = e->x()/(float)width();
	    now_pt.y = e->y()/(float)height();
	
	    update();
	    return;
	}
	else if(tmp != -1 && (focus_on_id != tmp || on_0 != focus_on_0)){
	    focus_on_0 ? lines[focus_on_id].pt1_s = __line::plain: lines[focus_on_id].pt2_s = __line::plain;
	    
	    focus_on_id = pressed_id = tmp;
	    focus_on_0 = pressed_0 = on_0;
	    
	    pressed_0 ? lines[pressed_id].pt1_s = __line::pressed: lines[pressed_id].pt2_s = __line::pressed;
	    
	    update();
	    return;	    
	}
	else if(tmp == focus_on_id && on_0 == focus_on_0){
	    pressed_id = focus_on_id;
	    pressed_0 = focus_on_0;
	    pressed_0 ? lines[pressed_id].pt1_s = __line::pressed: lines[pressed_id].pt2_s = __line::pressed;
	    update();
	    return;
	}
    }
    else{
	if(tmp != -1){
	    focus_on_id = pressed_id = tmp;
	    focus_on_0 = pressed_0 = on_0;
	    
	    pressed_0 ? lines[pressed_id].pt1_s = __line::pressed: lines[pressed_id].pt2_s = __line::pressed;
	    update();
	    return;
	}
    }
}

void CTransparentDemarcater::mouseReleaseEvent(QMouseEvent* e)
{   
    if(!enabled){
	QWidget::mouseReleaseEvent(e);
	return;
    }
    
    if(pressed_id != -1){
	if(pressed_id == focus_on_id){
	    pressed_0 ? lines[pressed_id].pt1_s = __line::focus_on: lines[pressed_id].pt2_s = __line::focus_on;
	    focus_on_0 = pressed_0;
	    
	    pressed_id = -1;
	    update();
	}
	else{
	    std::cout<<"error: focus_on_id != pressed_id"<<std::endl;
	}
    }
}

void CTransparentDemarcater::focusOutEvent(QFocusEvent* e)
{
    mouse_on_id = -1;
    focus_on_id = -1;
    pressed_id = -1;
    
    for(int i = 0; i < lines.size(); i++){
	lines[i].pt1_s = __line::plain;
	lines[i].pt2_s = __line::plain;
    }
    
    update();
    
    QWidget::focusOutEvent(e);
}

void CTransparentDemarcater::SetEnabled(bool on)
{
    enabled = on;
}

QLineF CTransparentDemarcater::GetLineValue(int idx)
{
    return QLineF(lines[idx].x1, lines[idx].y1, lines[idx].x2, lines[idx].y2);
}


#include "CTransparentDemarcater.moc"