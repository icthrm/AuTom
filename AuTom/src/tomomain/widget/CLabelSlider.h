#ifndef CLABELSLIDER_H__
#define CLABELSLIDER_H__

//#include <QtGui>
#include <QtWidgets>
class CLabelSlider:public QWidget
{
friend QSlider& CSlider(CLabelSlider& slider);
Q_OBJECT
private:
    QLabel* m_name;
    QSlider* m_slider;
    QLabel* m_number;
private slots:
    void coinside(int val);
private:
    void init(QWidget* parent, Qt::WindowFlags f);
public:
    CLabelSlider(QWidget* parent = 0, Qt::WindowFlags f = 0);
    CLabelSlider(const QString& text, int min, int max, int ti, QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~CLabelSlider();
    
public:
    void SetLabel(const QString& text);  
    void SetRange(int min, int max);
    void SetTickInterval(int ti);
    void SetValue(int val);
    const QString Text();
};

QSlider& CSlider(CLabelSlider& slider);

#endif
