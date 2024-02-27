#include "CCtrlPanel.h"

CCtrlPanel::CCtrlPanel(QWidget* parent, Qt::WindowFlags f): QWidget(parent, f)
{
    m_b_slider = new CLabelSlider(tr("Black"), 0, 255, 1, this, f);
    m_w_slider = new CLabelSlider(tr("White"), 0, 255, 1, this, f);
    
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(m_b_slider);
    layout->addWidget(m_w_slider);
    this->setLayout(layout);
}

CCtrlPanel::~CCtrlPanel()
{

}

#include "CCtrlPanel.moc"