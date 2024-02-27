#include "CMrcViewer.h"
#include "mrcimg/img_util.h"


CMrcViewer::CMrcViewer(ItemModel& im):_data(im)
{
//     setMinimumSize(QSize(800, 800));
    _view.Mirror(_data.Img());
    
    lfSplitter = new QSplitter(Qt::Horizontal, this);
    setCentralWidget( lfSplitter );
//     lPixmap = new QPixmap();
//     lfSplitter->addWidget(lPixmap);
    m_canvas = new CCanvas();
    lfSplitter->addWidget(m_canvas);
    UpdateCanvas();
    m_mslider = new CMultiSlider();
    m_mslider->AddAnchor(0.1,0, 0.333);
    m_mslider->AddAnchor(0.45,0.333, 0.666);
    m_mslider->AddAnchor(0.9,0.666, 1);
    m_mslider->AddLandmark(1/3.0);
    lfSplitter->addWidget(m_mslider);
    connect(m_mslider, SIGNAL(valueChanged(int, float)), SLOT(Test(int, float)));
//     m_mslider->ReSize(300);
    m_ctrlpanel = new CCtrlPanel();
    lfSplitter->addWidget(m_ctrlpanel);
    llabel = new QLabel();
    lfSplitter->addWidget(llabel);
    
    QAction* a = new QAction(this);
    a->setText( "Quit" );
    connect(a, SIGNAL(triggered()), SLOT(close()) );
    this->resize(600, 600);
    menuBar()->addMenu( "File" )->addAction( a );
}

CMrcViewer::~CMrcViewer()
{
    /** TODO
     should not delete an instance which is assigned a parent
     */
}

void CMrcViewer::UpdateCanvas()
{
    m_canvas->UpdateImageData(&_view.Pixmap());
}


void CMrcViewer::UpdateCanvas(int index, int white_range, int black_range, float scale)
{
    _data.SetIndex(index);
    _view.SetScaleRange(scale, black_range, white_range);
    
    m_canvas->UpdateImageData(&_view.Pixmap());
}


int CMrcViewer::testc = 0;

#include "CMrcViewer.moc"
