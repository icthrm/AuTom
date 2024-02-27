 #ifndef CSKIP_H
 #define CSKIP_H
 //#include <QtGui>
#include <QtWidgets>
 class CSkipWidget: public QWidget
 {
 public:
     CSkipWidget(QWidget* parent=0, Qt::WindowFlags f=0);
     virtual ~CSkipWidget() {};
     void SetSkipButtonEnable(bool flag);
 private:
     QPushButton *m_SkipButton;
 };
 #endif
