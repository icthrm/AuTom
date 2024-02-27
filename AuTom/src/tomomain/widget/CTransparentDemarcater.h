#ifndef CTRANSPARENTDEMARCATER_H__
#define CTRANSPARENTDEMARCATER_H__

#include <QtGui>
#include <vector>
#include "CCanvas.h"

class CTransparentDemarcater: public QWidget{

struct __point{
    float x, y;
};
    
struct __line{
enum __state{plain, mouse_on, pressed, focus_on};
    union{
    __point pt1;
    struct{
	float x1;						//x  0~1   is relative to width
	float y1;						//y  0~2   is relative to height
    };
    };
    union{
    __point pt2;
    struct{
	float x2;
	float y2;
    };
    };
    __state pt1_s, pt2_s;
};
    
Q_OBJECT
private:
    CCanvas* background;
    std::vector<__line> lines;
    int mouse_on_id;
    bool mouse_on_0;
    int focus_on_id;
    bool focus_on_0;
    int pressed_id;
    bool pressed_0;
    static const int diffuse_width = 2;
    static const int r = 5;
    bool enabled;
    bool fullline;
    
public:
    explicit CTransparentDemarcater(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~CTransparentDemarcater();
    
private slots:
    void BackGroundChanged(int w, int h);

private:
    void DrawLine(const __line& itm, QPainter& painter);
    void DrawShadowPoint(const QPoint& pt, const QColor& color, int rad, QPainter& painter);
    void DrawEndPoint(const QPoint& pt, __line::__state s, QPainter& painter);
    bool IsInEndPoint1(int x, int y, const QLine& itm);
    bool IsInEndPoint2(int x, int y, const QLine& itm);
//     const QLine& VerticalLine(const QLine& itm, int w);
    const QLine& ConvertToQLine(const __line& itm);
    const __line& FullLine(const __line& itm);
    
public:
    void MaskCCanvas(CCanvas* bak);
    void AddLine(float x1, float y1, float x2, float y2);
    void ShowLineFull(bool on = true);
    bool IsLineFull() const;
    void SetEnabled(bool on = true);
    QLineF GetLineValue(int idx);
    
protected:
    virtual void paintEvent(QPaintEvent* );
    virtual void mousePressEvent(QMouseEvent* );
    virtual void mouseReleaseEvent(QMouseEvent* );
    virtual void mouseMoveEvent(QMouseEvent* );
    virtual void focusOutEvent(QFocusEvent* );
};

#endif