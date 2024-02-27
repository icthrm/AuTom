#ifndef CTRANSPARENTFILM_H__
#define CTRANSPARENTFILM_H__

#include <QtGui>
#include <vector>
#include "CCanvas.h"

class CTransparentFilm: public QWidget{
    
struct __section{
enum __state{plain, mouse_on, pressed, focus_on};
    float y;
    float min, max;
    __state s;
    bool is_static;
};
    
Q_OBJECT
private:
    CCanvas* background;
    std::vector<__section> sections;
    int mouse_on_id;
    int focus_on_id;
    int pressed_id;
    bool value_limited;
    bool show_static;
    static const int diffuse_width = 2;
    bool enabled;
    
public:
    explicit CTransparentFilm(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~CTransparentFilm();

signals:
void valueChanged(int _no, float val);
    
private slots:
    void BackGroundChanged(int w, int h);

private:
    void DrawSectionLine(const __section& itm, QPainter& painter);
    void DrawShadeLine(QPainter& painter, int y, const QColor& color, int width);
    int isInSectionLine(int x, int y);
    bool isInSectionLineRange(int idx, int x, int y);
    
public:
    void MaskCCanvas(CCanvas* bak);
    void AddSectionLine(float val, float min, float max, bool _static);
    void SetSectionLine(int idx, float val);
    float GetSectionSitus(int idx) const;
    void ShowStaticSection(bool on = true);
    void SetEnabled(bool on = true);
    
protected:
    virtual void paintEvent(QPaintEvent* );
    virtual void mousePressEvent(QMouseEvent* );
    virtual void mouseReleaseEvent(QMouseEvent* );
    virtual void mouseMoveEvent(QMouseEvent* );
    virtual void focusOutEvent(QFocusEvent* );
};

#endif