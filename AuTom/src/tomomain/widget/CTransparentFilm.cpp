#include "CTransparentFilm.h"
#include <iostream>
#include <cmath>
#include <cassert>

CTransparentFilm::CTransparentFilm(QWidget* parent, Qt::WindowFlags f): QWidget(parent, f)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::WindowStaysOnTopHint);
    setWindowFlags(Qt::X11BypassWindowManagerHint);
    setFocusPolicy(Qt::StrongFocus);
    
    setMouseTracking(true);
    mouse_on_id = -1;
    focus_on_id = -1;
    pressed_id = -1;
    value_limited = false;
    show_static = true;
    enabled = true;
}

CTransparentFilm::~CTransparentFilm()
{

}

void CTransparentFilm::MaskCCanvas(CCanvas* bak)
{
    background = bak;
    setParent(bak);
    bak->setGeometry(x(), y(), bak->width(), bak->height());
    setFixedSize(background->width(), background->height());    
    connect(background, SIGNAL(sizeChanged(int,int)), this, SLOT(BackGroundChanged(int,int)));
}

void CTransparentFilm::BackGroundChanged(int w, int h)
{
    background->setGeometry(x(), y(), background->width(), background->height());
    setGeometry(x(), y(), background->width(), background->height());
    setFixedSize(background->width(), background->height());    
}

void CTransparentFilm::AddSectionLine(float val, float min, float max, bool _static)
{
    __section tmp;
    tmp.y = val;
    tmp.min = min;
    tmp.max = max;
    tmp.is_static = _static;
    tmp.s = __section::plain;
    
    sections.push_back(tmp);
}

void CTransparentFilm::SetSectionLine(int idx, float val)
{
    assert(idx >=0 && idx < sections.size());
    assert(val >= sections[idx].min && val <= sections[idx].max);
    sections[idx].y = val;
    update();
}

void CTransparentFilm::DrawShadeLine(QPainter& painter, int y, const QColor& color, int w)
{
    QLinearGradient linearGradient(0, y-(w-1)/2, 0, y+(w-1)/2); 
    linearGradient.setColorAt(0, Qt::transparent); 
    linearGradient.setColorAt(0.5, color); 
    linearGradient.setColorAt(1.0, Qt::transparent); 
    painter.setPen(QPen(linearGradient, w, Qt::SolidLine, Qt::RoundCap)); 
    painter.drawLine(0, y, width(), y); 
}

void CTransparentFilm::DrawSectionLine(const CTransparentFilm::__section& itm, QPainter& painter)
{
    static const QColor static_c(255,255,0);
    static const QColor free_c(35,255,35);
    if(itm.is_static && show_static){
	painter.setPen(QPen(static_c, 1, Qt::SolidLine, Qt::RoundCap));
	painter.drawLine(0, itm.y*height(), width(), itm.y*height());
    }
    
    else if(!itm.is_static && itm.s == __section::plain){
	painter.setPen(QPen(free_c, 1, Qt::DashDotLine, Qt::RoundCap));
	painter.drawLine(0, itm.y*height(), width(), itm.y*height());
    }
    else if(!itm.is_static && itm.s == __section::mouse_on){
	DrawShadeLine(painter, itm.y*height(), QColor(255,255,0), 1+diffuse_width*2);
	painter.setPen(QPen(free_c, 1, Qt::SolidLine, Qt::RoundCap));
	painter.drawLine(0, itm.y*height(), width(), itm.y*height());
    }
    else if(!itm.is_static && itm.s == __section::focus_on){
	DrawShadeLine(painter, itm.y*height(), QColor(0,255,50), 1+diffuse_width*2);
	painter.setPen(QPen(QColor(0,255,0), 1, Qt::DashDotLine, Qt::RoundCap));
	painter.drawLine(0, itm.y*height(), width(), itm.y*height());
    }
    else if(!itm.is_static && itm.s == __section::pressed){
	DrawShadeLine(painter, itm.y*height(), QColor(60,200,60), 1+diffuse_width*2);
	painter.setPen(QPen(QColor(0,255,0), 1, Qt::DashDotLine, Qt::RoundCap));
	painter.drawLine(0, itm.y*height(), width(), itm.y*height());
    }
}

void CTransparentFilm::paintEvent(QPaintEvent* e)
{    
    QPainter painter(this);
    
    for(int i = 0; i < sections.size(); i++){
	DrawSectionLine(sections[i], painter);
    }
//     QWidget::paintEvent();
}

void CTransparentFilm::mouseMoveEvent(QMouseEvent* e)
{   
    if(!enabled){
	QWidget::mouseMoveEvent(e);
	return;
    }
    
    int tmp = isInSectionLine(e->x(), e->y());
    
    if(tmp == -1 && (focus_on_id == -1 || focus_on_id != mouse_on_id)){
	if(mouse_on_id != -1){
	    sections[mouse_on_id].s = __section::plain;
	    mouse_on_id = -1;
	    update();
	}
	
	return;
    }
    
    if(focus_on_id != -1){
	if(pressed_id != -1){
	    if(sections[pressed_id].max >= e->y()/(float)height()
		&&sections[pressed_id].min <= e->y()/(float)height()){	
		sections[pressed_id].y = e->y()/(float)height();
	    
		emit valueChanged(pressed_id, sections[pressed_id].y);
		update();
	    
		if(value_limited){ value_limited = false;}
	    }
	    else if(sections[pressed_id].max < e->y()/(float)height()){
		sections[pressed_id].y = sections[pressed_id].max;
		
		if(!value_limited){
		    emit valueChanged(pressed_id, sections[pressed_id].y);
		    update();
		}
		value_limited = true;
	    }
	    else if(sections[pressed_id].min > e->y()/(float)height()){
		sections[pressed_id].y = sections[pressed_id].min;
		if(!value_limited){
		    emit valueChanged(pressed_id, sections[pressed_id].y);
		    update();
		}
		value_limited = true;
	    }
	}
	else{
	    if(tmp != focus_on_id){
		if(mouse_on_id != -1 && mouse_on_id != focus_on_id){
		    sections[mouse_on_id].s = __section::plain;
		}
		if(sections[tmp].s == __section::plain){
		    mouse_on_id = tmp;
		    sections[tmp].s = __section::mouse_on;
		    update();
		    return;
		}
	    }
	}
    }
    else{
	if(mouse_on_id == tmp){
	    return;
	}
	else{
	    if(mouse_on_id != -1){
		sections[mouse_on_id].s = __section::plain;
	    }
	    if(sections[tmp].s == __section::plain){
		mouse_on_id = tmp;
		sections[tmp].s = __section::mouse_on;
		update();
		return;
	    }
	}
    }
    
    QWidget::mouseMoveEvent(e);
}

void CTransparentFilm::mousePressEvent(QMouseEvent* e)
{   
    if(!enabled){
	QWidget::mousePressEvent(e);
	return;
    }
    
    grabMouse();
    
    int tmp = isInSectionLine(e->x(), e->y());
    
    if(tmp == -1 && (focus_on_id == -1 || !isInSectionLineRange(focus_on_id, e->x(), e->y()))){	
	if(focus_on_id != -1){
	    sections[focus_on_id].s = __section::plain;
	    focus_on_id = -1;
	    update();
	}
	
	return;
    }
    
    if(pressed_id != -1){
	sections[pressed_id].s = __section::plain;
    }
    
    if(focus_on_id != -1){
	if(isInSectionLineRange(focus_on_id, e->x(), e->y())){
	    
	    pressed_id = focus_on_id;
	    sections[focus_on_id].s = __section::pressed;
	    sections[focus_on_id].y = e->y()/(float)height();
	
	    value_limited = false;
	    emit valueChanged(focus_on_id, sections[focus_on_id].y);
	    update();
	    return;
	}
	else if(tmp != -1 && focus_on_id != tmp){
	    sections[focus_on_id].s = __section::plain;
	    focus_on_id = pressed_id = tmp;
	    
	    sections[pressed_id].s = __section::pressed;
	    update();
	    return;	    
	}
    }
    else{
	if(tmp != -1){
	    focus_on_id = pressed_id = tmp;
	    
	    sections[pressed_id].s = __section::pressed;
	    update();
	    return;
	}
    }
    
    QWidget::mousePressEvent(e);
}

void CTransparentFilm::mouseReleaseEvent(QMouseEvent* e)
{   
    if(!enabled){
	QWidget::mouseReleaseEvent(e);
	return;
    }
    releaseMouse();
    
    if(pressed_id != -1){
	if(pressed_id == focus_on_id){
	    sections[pressed_id].s = __section::focus_on;
	    pressed_id = -1;
	    update();
	}
	else{
	    std::cout<<"error: focus_on_id != pressed_id"<<std::endl;
	}
    }
    
    QWidget::mouseReleaseEvent(e);
}

void CTransparentFilm::focusOutEvent(QFocusEvent* e)
{
    mouse_on_id = -1;
    focus_on_id = -1;
    pressed_id = -1;
    
    for(int i = 0; i < sections.size(); i++){
	sections[i].s = __section::plain;
    }
    
    update();
    
    QWidget::focusOutEvent(e);
}


int CTransparentFilm::isInSectionLine(int x, int y)
{
    for(int i = 0; i < sections.size(); i++){
	if(sections[i].is_static){
	    continue;
	}
	if(abs(sections[i].y*height()-y) <= diffuse_width*2+1-2){
	    return i;
	}
    }
    return -1;
}

void CTransparentFilm::ShowStaticSection(bool on)
{
    show_static = on;
    update();
}

bool CTransparentFilm::isInSectionLineRange(int idx, int x, int y)
{
    return (y >= sections[idx].min*height() && y <= sections[idx].max*height());
}

float CTransparentFilm::GetSectionSitus(int idx) const
{
    return sections[idx].y;
}

void CTransparentFilm::SetEnabled(bool on)
{
    enabled = on;
}

#include "CTransparentFilm.moc"