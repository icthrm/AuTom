#include "CLabelSlider.h"
#include <QValidator>
#include <stdlib.h>
#include <sstream>

CLabelSlider::CLabelSlider(QWidget* parent, Qt::WindowFlags f): QWidget(parent, f)
{
    init(parent, f);
}

CLabelSlider::CLabelSlider(const QString& text, int min, int max, int ti, QWidget* parent, Qt::WindowFlags f):QWidget(parent, f)
{  
    init(parent, f);
    SetLabel(text);
    SetRange(min, max);
    SetTickInterval(ti);
    SetValue(min);
}


CLabelSlider::~CLabelSlider()
{

}

void CLabelSlider::SetLabel(const QString& text)
{
    m_name->setText(QString("%1").arg(text, -6));
}

const QString CLabelSlider::Text()
{
    return m_name->text();
}

void CLabelSlider::SetRange(int min, int max)
{
    m_slider->setRange(min, max);
}

void CLabelSlider::SetTickInterval(int ti)
{
    m_slider->setTickInterval(ti);
}

void CLabelSlider::SetValue(int val)
{
    m_slider->setValue(val);
    QString tmp = QString::number(val);
    if(tmp.size()==1){
	tmp.push_front(tr("00"));
    }
    if(tmp.size()==2){
	tmp.push_front(tr("0"));
    }
    
    m_number->setText(tmp);
}

void CLabelSlider::coinside(int val)
{
    QString tmp = QString::number(val);
    if(tmp.size()==1){
	tmp.push_front(tr("00"));
    }
    if(tmp.size()==2){
	tmp.push_front(tr("0"));
    }
    
    m_number->setText(tmp);
}

void CLabelSlider::init(QWidget* parent, Qt::WindowFlags f)
{
    QHBoxLayout* m_layout = new QHBoxLayout(this);
    m_name = new QLabel(this, f);
    m_slider = new QSlider(Qt::Horizontal, this);
    m_number = new QLabel(this, f);
    
    m_layout->addWidget(m_name);
    m_name->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_name->setAlignment(Qt::AlignLeft);
    m_layout->addWidget(m_slider);
    m_slider->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    m_layout->addWidget(m_number);
    m_number->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_number->setAlignment(Qt::AlignRight);
    
    this->setLayout(m_layout);
    connect(m_slider, SIGNAL(valueChanged(int)), this, SLOT(coinside(int)));
}

QSlider& CSlider(CLabelSlider& slider)
{
    return *(slider.m_slider);
}

#include "CLabelSlider.moc"