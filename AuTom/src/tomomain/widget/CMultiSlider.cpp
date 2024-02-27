#include "CMultiSlider.h"
#include <cassert>
#include <iostream>

CMultiSlider::CMultiSlider(QWidget* parent, Qt::WindowFlags f): QWidget(parent, f)
{
    setAttribute(Qt::WA_TranslucentBackground, true); 
    setStyleSheet("background-color:transparent");
    setFocusPolicy(Qt::StrongFocus);
    tick_interval = 0.0005;
    active_not_catch_anchor_no = -1;
    active_catch_anchor_no = -1;
    
    timer = new QTimer(this);
    timer->setInterval(48);
    connect(timer, SIGNAL(timeout()), this, SLOT(TimerTick()));
    
    value_limited = false;
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setMinimumWidth(span_w);
    setMinimumHeight(60);
    enabled = true;
//     resize(span_w, 300);
}

CMultiSlider::~CMultiSlider()
{

}

void CMultiSlider::paintEvent(QPaintEvent* e)
{
    static const QColor bar_c(102,102,102);
    static const QColor edge_c(245,245,245);
    static const QColor edge_c2(204,204,204);
    
    QPainter painter(this);
//     painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(bar_c);
    painter.setPen(bar_c);
    painter.drawRect(marge+light_marge+sideline, sideline, thickness, height()-2);
    painter.setPen(edge_c);
    painter.drawLine(marge+light_marge, 0, marge+light_marge+sideline+thickness, 0);
    painter.drawLine(marge+light_marge+sideline+thickness, 0, marge+light_marge+sideline+thickness, height());
    painter.setPen(edge_c2);
    painter.drawLine(marge+light_marge, height(), marge+light_marge+sideline+thickness, height());
    painter.drawLine(marge+light_marge, 0, marge+light_marge, height());
    
    for(int i = 0; i < anchors.size(); i++){
	int yloc = anchors[i].y*height()-__anchor::radius;
	anchors[i].x_p = 0;
	anchors[i].y_p = yloc;
	anchors[i].w_p = span;
	anchors[i].h_p = 2*__anchor::radius;
	DrawRectItem(painter, anchors[i].x_p+light_marge, anchors[i].y_p, anchors[i].w_p, anchors[i].h_p);
    }
    
    for(int i = 0; i < marks.size(); i++){
	int yloc = marks[i].y*height()-__landmark::radius;
	marks[i].x_p = 0;
	marks[i].y_p = yloc;
	marks[i].w_p = span;
	marks[i].h_p = 2*__landmark::radius;
	DrawRectItem(painter, marks[i].x_p+light_marge, marks[i].y_p, marks[i].w_p, marks[i].h_p);
    }
    
//     QWidget::paintEvent();
}

void CMultiSlider::mousePressEvent(QMouseEvent* e)
{
    if(!enabled){
	QWidget::mousePressEvent(e);
	return;
    }
    
    grabMouse();
    
    bool is_active = true;
    bool is_active_not_catch = false;
    int is_active_catch = -1;
    int in_range = -1;
    
    if(!(e->x() >= marge-2 && e->x() <= span-marge+2)){
	is_active = false;
    }
    
    if(IsInLandMark(e->x(), e->y()) != -1){
	is_active = false;
    }
    
    if((is_active_catch = IsInAnchor(e->x(), e->y())) != -1){
	active_catch_anchor_no = is_active_catch;
	is_active = true;
    }
    else{
	is_active_not_catch = true;
	in_range = IsInAnchorRange(e->x(), e->y());
    }
    if(!is_active){
	QWidget::mousePressEvent(e);
    }
    else{
	if(in_range != -1){
	    if(e->y() > anchors[in_range].y*height()){
		if(anchors[in_range].max > anchors[in_range].y+8*tick_interval){
		    anchors[in_range].y += 8*tick_interval;
		    
		    emit valueChanged(in_range, anchors[in_range].y);
		    if(value_limited){ value_limited = false;}
		    update();
		}
		else{
		    anchors[in_range].y = anchors[in_range].max;
		    if(!value_limited){
			emit valueChanged(in_range, anchors[in_range].y);
			update();
		    }
		    value_limited = true;
		}
		isdown = true;
	    }
	    else{
		if(anchors[in_range].min < anchors[in_range].y-8*tick_interval){
		    anchors[in_range].y -= 8*tick_interval;
		    
		    emit valueChanged(in_range, anchors[in_range].y);
		    if(value_limited){ value_limited = false;}
		    update();
		}
		else{
		    anchors[in_range].y = anchors[in_range].min;
		    
		    if(!value_limited){
			emit valueChanged(in_range, anchors[in_range].y);
			update();
		    }
		    value_limited = true;
		}
		isdown = false;
	    }
	    
	    active_not_catch_anchor_no = in_range;
	    QTimer::singleShot(300, this, SLOT(TimerStart()));
	}
    }
}

void CMultiSlider::mouseReleaseEvent(QMouseEvent* e)
{
    if(!enabled){
	QWidget::mouseReleaseEvent(e);
	return;
    }
    
    releaseMouse();
    if(active_not_catch_anchor_no != -1){
	timer->stop();
	active_not_catch_anchor_no = -1;
    }
    else if(active_catch_anchor_no != -1){
	active_catch_anchor_no = -1;
    }
    
    QWidget::mouseReleaseEvent(e);
}

void CMultiSlider::TimerStart()
{
    timer->start();
}

void CMultiSlider::SetEnabled(bool on)
{
    enabled = on;
}

void CMultiSlider::TimerTick()
{
    if(active_not_catch_anchor_no == -1){
// 	std::cout<<"!"<<std::endl;
	return;
    }
    
    if(isdown && anchors[active_not_catch_anchor_no].max > anchors[active_not_catch_anchor_no].y+tick_interval*8){
	anchors[active_not_catch_anchor_no].y += tick_interval*8;
	
	emit valueChanged(active_not_catch_anchor_no, anchors[active_not_catch_anchor_no].y);
	update();
	
	if(value_limited){ value_limited = false;}
    }
    else if(isdown){
	anchors[active_not_catch_anchor_no].y = anchors[active_not_catch_anchor_no].max;
	if(!value_limited){
	    emit valueChanged(active_not_catch_anchor_no, anchors[active_not_catch_anchor_no].y);
	    update();
	}
	value_limited = true;
    }
    else if(!isdown && anchors[active_not_catch_anchor_no].min < anchors[active_not_catch_anchor_no].y-tick_interval*8){
	anchors[active_not_catch_anchor_no].y -= tick_interval*8;
	
	emit valueChanged(active_not_catch_anchor_no, anchors[active_not_catch_anchor_no].y);
	update();
	if(value_limited){ value_limited = false;}	
    }
    else if(!isdown){
	anchors[active_not_catch_anchor_no].y = anchors[active_not_catch_anchor_no].min;
	if(!value_limited){	    
	    emit valueChanged(active_not_catch_anchor_no, anchors[active_not_catch_anchor_no].y);
	    update();
	}
	value_limited = true;
    }
}

void CMultiSlider::mouseMoveEvent(QMouseEvent* e)
{
    if(!enabled){
	QWidget::mouseMoveEvent(e);
	return;
    }
    
    if(active_catch_anchor_no != -1){
	if(anchors[active_catch_anchor_no].max >= e->y()/(float)height()
	    &&anchors[active_catch_anchor_no].min <= e->y()/(float)height()){	
	    anchors[active_catch_anchor_no].y = e->y()/(float)height();
	
	    emit valueChanged(active_catch_anchor_no, anchors[active_catch_anchor_no].y);
	    update();
	
	    if(value_limited){ value_limited = false;}
	}
	else if(anchors[active_catch_anchor_no].max < e->y()/(float)height()){
	    anchors[active_catch_anchor_no].y = anchors[active_catch_anchor_no].max;
	    
	    if(!value_limited){
		emit valueChanged(active_catch_anchor_no, anchors[active_catch_anchor_no].y);
		update();
	    }
	    value_limited = true;
	}
	else if(anchors[active_catch_anchor_no].min > e->y()/(float)height()){
	    anchors[active_catch_anchor_no].y = anchors[active_catch_anchor_no].min;
	    if(!value_limited){
		emit valueChanged(active_catch_anchor_no, anchors[active_catch_anchor_no].y);
		update();
	    }
	    value_limited = true;
	}
    }
    
    QWidget::mouseMoveEvent(e);
}


void CMultiSlider::DrawRectItem(QPainter& painter, int x, int y, int w, int h)
{
    static const QColor body_c(210,210,210);
    static const QColor edge_c(90, 90, 90);
    static const QColor edge_c2(245,245,245);//(204,204,204);
    static const QColor edge_c3(130, 130, 130);
    
    painter.setBrush(body_c);
    painter.setPen(body_c);
    painter.drawRect(x, y, w, h);
    painter.setPen(edge_c2);
    painter.drawLine(x, y+h, x+w, y+h);
    painter.setPen(edge_c);
    painter.drawLine(x, y, x+w, y);
    painter.setPen(edge_c3);
    painter.drawLine(x+w, y, x+w, y+h);
    painter.setPen(edge_c2);
    painter.drawLine(x, y, x, y+h);
}

void CMultiSlider::ReSize(int height)
{
    resize(span_w+2*light_marge, height);
}

void CMultiSlider::SetAnchorValue(int index, float val)
{
    assert(index >=0 && index < anchors.size());
    assert(val >= anchors[index].min && val <= anchors[index].max);
    anchors[index].y = val;
//     emit valueChanged(index, val);
    update();
}

void CMultiSlider::AddAnchor(float loc, float min, float max)
{
    __anchor tmp;
    tmp.y = loc;
    tmp.min = min;
    tmp.max = max;
    anchors.push_back(tmp);
    update();
}

void CMultiSlider::AddLandmark(float loc)
{
    __landmark tmp;
    tmp.y = loc;
    marks.push_back(tmp);
    update();
}

void CMultiSlider::SetTickInterval(float ti)
{
    tick_interval = ti;
}

int CMultiSlider::IsInAnchor(int x, int y)
{
    for(int i = 0; i < anchors.size(); i++){
	if(x >= anchors[i].x_p-2 && x <= anchors[i].x_p+anchors[i].w_p+2 && y >= anchors[i].y_p-2 && y <= anchors[i].y_p+anchors[i].h_p+2){
	    return i;
	}
    }
    
    return -1;
}

int CMultiSlider::IsInLandMark(int x, int y)
{
    for(int i = 0; i < marks.size(); i++){
	if(x >= marks[i].x_p-1 && x <= marks[i].x_p+1 && y >= marks[i].y_p-1 && y <= marks[i].y_p+1){
	    return i;
	}
    }
    
    return -1;
}

int CMultiSlider::IsInAnchorRange(int x, int y)
{
    for(int i = 0; i < anchors.size(); i++){
	if( y >= anchors[i].min*height() && y <= anchors[i].max*height()){
	    return i;
	}
    }
    
    return -1;    
}

int CMultiSlider::Span() const
{
    return span_w;
}

float CMultiSlider::AnchorValue(int index) const
{
    assert(index >=0 && index < anchors.size());
    return anchors[index].y;
}


#include "CMultiSlider.moc"