/**********************************************************************
	created:	2011/07/12
	created:	12:7:2011   17:27
	filename: 	QVAT4MWidget.h
	file path:	./VAT4M_Widget/vat4mWidget
	file base:	QVAT4MWidget
	file ext:	h
	author:		SCCAS LJ&TD+
	
	purpose:	A vis window for Tomography Data.
***********************************************************************/
#pragma once				
//!some useful QT headfiles.
/*!*/
#include <QtCore/QTime>
#include <QtCore/QThread>
#include <QtCore/QMutex>
#include <QtCore/QReadWriteLock>
#include <Qt/qdesktopwidget.h>
#include <Qt/qtimer.h>
#include <Qt/qprogressbar.h>
#include <Qt/qdial.h>
#include <Qt/qgl.h>
#include <Qt/qmenu.h>
#include <Qt/qstring.h>
#include <Qt/qdialog.h>
#include <Qt/qapplication.h>
#include <Qt/qpushbutton.h>
#include <Qt/qradiobutton.h>
#include <Qt/qcheckbox.h>
#include <Qt/qcombobox.h>
#include <Qt/qbuttongroup.h>
#include <Qt/qcursor.h>
#include <Qt/qfiledialog.h>
#include <Qt/qmessagebox.h>
#include <Qt/qpushbutton.h>
#include <Qt/qlabel.h>
#include <Qt/qlineedit.h>
#include <Qt/qtextedit.h>
#include <Qt/qaction.h>
#include <Qt/qactiongroup.h>
#include <Qt/qscrollbar.h>
#include <Qt/qtextcodec.h>
#include <Qt/qpainter.h>
#include <Qt/qevent.h>
#include <Qt/qcolordialog.h>
#include <Qt/qinputdialog.h>
#include <Qt/qcursor.h>
#include <GL/glut.h>	
#include <Qt/qstring.h>
#include <Qt/qvector.h>
#include <QtGui/QSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QPlastiqueStyle>
//!some useful C++ headfiles.
/*!*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <time.h>
#include <vector>
#include <stack>
#include <map>
#include <utility>

//!QVis Window Class.
/*!
  Just new it & everything is OK,
  for some functions are private.
*/
class QVAT4MWidget : public QWidget
{
	Q_OBJECT
private:
	struct _QVAT4MWidgetPrivate* d;		/*
    Some useful structs...			 		*/

public:
	QVAT4MWidget(QWidget* parent);
	~QVAT4MWidget(void);						/*
	Construction & Destruction				*/
	void setFilepath(QString _filepath);/*
	Set the filepath to open				*/
};
