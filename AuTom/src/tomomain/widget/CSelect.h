 #ifndef CSelect_H
 #define CSelect_H
// #include <QtGui>
#include <QtWidgets>
 class CSelectWidget: public QWidget
 {
     Q_OBJECT
 public:
     CSelectWidget(QWidget* parent=0, Qt::WindowFlags f=0);
     virtual ~CSelectWidget() {};
 private:
     QWidget *SelectWidget;
     QPushButton* m_MarkerBasedButton;
     QPushButton* m_MarkerFreeButton;
 signals:
     void MarkerFreeFlow();
     void MarkerBasedFlow();
 private slots:
     void MarkerFreeFlowClicked();
     void MarkerBasedFlowClicked();
 };
 #endif
