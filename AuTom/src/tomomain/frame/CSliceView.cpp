#include "CSliceView.h"
#include <iostream>
#include "core/msg_map.h"
#include "core/msg.h"
#include "custom/custom.h"
#include "CSectTool.h"

/** CSliceView ***/

CSliceView::CSliceView(ItemModel& im, QWidget* parent, Qt::WindowFlags f):_data(im), QWidget(parent, f)
{    
    QRect rect = QApplication::desktop()->availableGeometry();
    float default_w = rect.width()*0.7;
    float default_h = rect.height()*0.7;
    float scale = default_h/_data.Img()->height < default_w/_data.Img()->width ? default_h/_data.Img()->height: default_w/_data.Img()->width;
    
    _view.Mirror(_data.Img());
    
    _view.SetScale(scale);
    
    QGridLayout* l = new QGridLayout();
    
    m_canvas = new CCanvas(this);
    m_demarcater = new CTransparentDemarcater(m_canvas);//this);
    m_canvas->UpdateImageData(&_view.Pixmap());
    
    m_canvas->setFixedSize(m_canvas->width(), m_canvas->height());
    m_demarcater->MaskCCanvas(m_canvas);
    
    QWidget* q = new QWidget(this);
    q->setFixedSize(m_canvas->width(), m_canvas->height());
    m_canvas->setParent(q);
    m_canvas->setGeometry(0, 0, m_canvas->width(), m_canvas->height());
    
    l->addWidget(q, 0, 0);
    
    setLayout(l);
}

CSliceView::~CSliceView()
{

}

void CSliceView::SetEnabled(bool on)
{
    m_demarcater->SetEnabled(on);
}

void CSliceView::SetFullLine(bool on)
{
    m_demarcater->ShowLineFull(on);
}

bool CSliceView::IsFullLine() const
{
    return m_demarcater->IsLineFull();
}

void CSliceView::AddLine(float x1, float y1, float x2, float y2)
{
    m_demarcater->AddLine(x1, y1, x2, y2);
}

QLineF CSliceView::GetLineValue(int idx)
{
    return m_demarcater->GetLineValue(idx);
}

/** CSliceViewFrame ***/

CSliceViewFrame::CSliceViewFrame(ItemModel& im, QLineF* _lines, QWidget* parent, Qt::WindowFlags flags): QMainWindow(parent, flags), _data(im)
{   
    setWindowTitle("slice view");
    m_sliceview = new CSliceView(_data);
    
    lines = _lines;
    
    for(int i = 0; i < 2; i++){
	m_sliceview->AddLine(lines[i].x1(), lines[i].y1(), lines[i].x2(), lines[i].y2());
    }
    
    QWidget* q = new QWidget(this);
    QGridLayout* l = new QGridLayout();
    q->setLayout(l);
    l->addWidget(m_sliceview);    
    setCentralWidget(q);
    
    locked = false;
    
    m_topbar = new QToolBar("Control");
    addToolBar(Qt::TopToolBarArea, m_topbar);
    m_topbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_topbar->setMovable(false);
    
    SetTopToolBar(m_topbar);
    
//     statusBar()->showMessage("Ready");
}

CSliceViewFrame::~CSliceViewFrame()
{

}

void CSliceViewFrame::SetTopToolBar(QToolBar* toolbar)
{    
    lock = new QAction(QIcon(":/lock.png"), tr("Lock"), this);
    lock->setShortcut(tr("Ctrl+L"));
    
    full_show = new QAction(QIcon(":/fullline.png"), tr("Extend"), this);
    full_show->setShortcut(tr("Ctrl+F"));
    
    toolbar->addAction(lock);
    toolbar->addAction(full_show);
    
    connect(lock, SIGNAL(triggered()), this, SLOT(LockScreen()));
    connect(full_show, SIGNAL(triggered()), this, SLOT(DoFullLine()));
}

void CSliceViewFrame::LockScreen()
{
    locked = !locked;
    m_sliceview->SetEnabled(!locked);
    if(locked){
	lock->setIcon(QIcon(":/key.png"));
	lock->setIconText(tr("Unlock"));
    }
    else{
	lock->setIcon(QIcon(":/lock.png"));
	lock->setIconText(tr("Lock"));
    }
}

void CSliceViewFrame::DoFullLine()
{
    m_sliceview->SetFullLine(!m_sliceview->IsFullLine());
    if(m_sliceview->IsFullLine()){
	full_show->setIcon(QIcon(":/fullline2.png"));
	full_show->setIconText(tr("Shrink"));
    }
    else{
	full_show->setIcon(QIcon(":/fullline.png"));
	full_show->setIconText(tr("Extend"));
    }
}

void CSliceViewFrame::customEvent(QEvent* e)
{
    QObject::customEvent(e);
}

void CSliceViewFrame::closeEvent(QCloseEvent* e)
{
    lines[0] = m_sliceview->GetLineValue(0);
    lines[1] = m_sliceview->GetLineValue(1);
    QWidget::closeEvent(e);
}


#include "CSliceView.moc"