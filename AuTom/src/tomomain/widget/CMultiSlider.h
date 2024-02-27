#ifndef CMULTISLIDER_H__
#define CMULTISLIDER_H__

//#include <QtGui>
#include <QtWidgets>
#include <vector>
#include <sys/stat.h>

#ifndef ROUND
    #define ROUND(X)		int(X+0.5)
#endif

class CMultiSlider:public QWidget{
   
struct __anchor{
    constexpr static const float radius = 12*.618/2;			//abstruct value
    float y;
    float min, max;
    int x_p, y_p, w_p, h_p;
};

struct __landmark{
    constexpr static const float radius = 12*.618/6;		//abstruct value
    float y;
    int x_p, y_p, w_p, h_p;
};

Q_OBJECT
private:
    static const int thickness = 1;			//in pixel
    static const int sideline = 1;			//in pixel
    static const int marge = 6;				//in pixel
    static const int light_marge = 0;
    static const int span = marge*2+sideline*2+thickness+1; //in pixel
    static const int span_w = span+2;
    constexpr static const float y_scale = 1;			//y mesurement of sruct point, anchor, or landmark (0~1)
    
private:
    std::vector<__anchor> anchors;
    std::vector<__landmark> marks;
    float tick_interval;
    int active_not_catch_anchor_no;
    int active_catch_anchor_no;
    bool isdown;
    bool value_limited;
    QTimer* timer;
    bool enabled;
    
signals:
    void valueChanged(int _no, float val);
    
public:
    explicit CMultiSlider(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~CMultiSlider();
    
private:
    void DrawRectItem(QPainter& painter, int x, int y, int w, int h);
//     void DrawRectItemFocused(QPainter& painter, int x, int y, int w, int h);
    int IsInAnchor(int x, int y);
    int IsInLandMark(int x, int y);
    int IsInAnchorRange(int x, int y);
    
private slots:
    void TimerTick();
    void TimerStart();
    
protected:
    virtual void paintEvent(QPaintEvent* );
    virtual void mousePressEvent(QMouseEvent* );
    virtual void mouseReleaseEvent(QMouseEvent* );
    virtual void mouseMoveEvent(QMouseEvent* );
    
public:
    void ReSize(int height);
    void AddAnchor(float loc, float min, float max);	//loc range 0~1
    void AddLandmark(float loc);			//loc range 0~1
    void SetTickInterval(float ti);
    void SetAnchorValue(int index, float val);
    int Span() const;
    float AnchorValue(int index) const;
    void SetEnabled(bool on = true);
};

#endif
