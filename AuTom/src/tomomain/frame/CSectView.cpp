#include "CSectView.h"
#include <iostream>
#include "core/msg_map.h"
#include "core/msg.h"
#include "custom/custom.h"
#include "CSectTool.h"
#include"widget/Ctag.h"

/** CSectView ***/

CSectView::CSectView(ItemModel& im, QWidget* parent, Qt::WindowFlags f):_data(im), QWidget(parent, f)
{    
    QRect rect = QApplication::desktop()->availableGeometry();
    float default_w = rect.width()*0.7;
    float default_h = rect.height()*0.7;
    float scale = default_h/_data.Img()->height < default_w/_data.Img()->width ? default_h/_data.Img()->height: default_w/_data.Img()->width;
    
    _view.Mirror(_data.Img());
    
    _view.SetScale(scale);
    
    QGridLayout* l = new QGridLayout();
    
    m_canvas = new CCanvas(this);
    m_film = new CTransparentFilm(m_canvas);//this);
    m_canvas->UpdateImageData(&_view.Pixmap());
    
    m_canvas->setFixedSize(m_canvas->width(), m_canvas->height());
    m_film->MaskCCanvas(m_canvas);
    m_film->AddSectionLine(1/6.0, 0, 1/3.0, false);
    m_film->AddSectionLine(1/2.0, 1/3.0, 2/3.0, false);
    m_film->AddSectionLine(5/6.0, 2/3.0, 1, false);
    m_film->AddSectionLine(1/3.0, 0, 0, true);
    m_film->AddSectionLine(2/3.0, 0, 0, true);
    
    m_mslider = new CMultiSlider(this);
    m_mslider->AddLandmark(1/3.0);
    m_mslider->AddLandmark(2/3.0);
    m_mslider->AddAnchor(1/6.0, 0, 1/3.0);
    m_mslider->AddAnchor(1/2.0, 1/3.0, 2/3.0);
    m_mslider->AddAnchor(5/6.0, 2/3.0, 1);
    m_mslider->setFixedSize(m_mslider->Span(), m_canvas->height());
    
    connect(m_film, SIGNAL(valueChanged(int,float)), SLOT(SynchronizeSlider(int,float)));
    connect(m_mslider, SIGNAL(valueChanged(int,float)), SLOT(SynchronizeFilm(int,float)));
    
    QWidget* q = new QWidget(this);
    q->setFixedSize(m_canvas->width()+m_mslider->Span(), m_canvas->height());
    m_canvas->setParent(q);
    m_canvas->setGeometry(0, 0, m_canvas->width(), m_canvas->height());
    m_mslider->setParent(q);
    m_mslider->setGeometry(m_canvas->width(), 0, m_mslider->Span(), m_canvas->height());
    
    l->addWidget(q, 0, 0);
    
    setLayout(l);
    
}



CSectView::~CSectView()
{

}

void CSectView::SynchronizeFilm(int idx, float val)
{
    m_film->SetSectionLine(idx, val);
}

void CSectView::SynchronizeSlider(int idx, float val)
{
    m_mslider->SetAnchorValue(idx, val);
}

float CSectView::AnchorValue(int idx) const
{
    return m_mslider->AnchorValue(idx);
}

void CSectView::SetEnabled(bool on)
{
    m_film->SetEnabled(on);
    m_mslider->SetEnabled(on);
}

void CSectView::SetValue(int idx, float val)
{
    m_film->SetSectionLine(idx, val);
    m_mslider->SetAnchorValue(idx, val);
}

/** CSectViewFrame ***/

CSectViewFrame::CSectViewFrame(ItemModel& im, float* _loc, QWidget* parent, Qt::WindowFlags flags): QMainWindow(parent, flags), _data(im), loc(_loc)
{   
    setWindowTitle("section view");
    m_sectview = new CSectView(_data);
    
    QWidget* q = new QWidget(this);
    QGridLayout* l = new QGridLayout();
    q->setLayout(l);
    l->addWidget(m_sectview);    
    setCentralWidget(q);
    
    for(int i = 0; i < 3; i++){
	if(loc[i] >= 0){
	    m_sectview->SetValue(i, loc[i]);
	}
    }
    
    m_topbar = new QToolBar("Control");
    addToolBar(Qt::TopToolBarArea, m_topbar);
    m_topbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_topbar->setMovable(false);
    
    SetTopToolBar(m_topbar);
    
    statusBar()->showMessage("Ready");

}


CSectViewFrame::~CSectViewFrame()
{

}

void CSectViewFrame::SetTopToolBar(QToolBar* toolbar)
{    
    m_datatype_label = new QLabel("  Datatype ",this);
    m_datatype_combobox = new QComboBox();
    m_datatype_combobox->insertItem(0,"cryo-ET or plastic embedded ET");
    m_datatype_combobox->insertItem(1,"negative stained  ET");
    m_datatype_combobox->setCurrentIndex(0);

    start_rec = new QAction(QIcon(":/build.png"), tr("Build Section"), this);
    start_rec->setShortcut(tr("Ctrl+B"));
    start_rec->setToolTip(ToolTips::geo_btn_buildsec);
    stop_rec = new QAction(QIcon(":/stop.png"), tr("Stop"), this);
    stop_rec->setShortcut(tr("Ctrl+S"));
    stop_rec->setDisabled(true);
    
    toolbar->addWidget(m_datatype_label);
    toolbar->addWidget(m_datatype_combobox);
    toolbar->addAction(start_rec);
    toolbar->addAction(stop_rec);
    
    connect(start_rec, SIGNAL(triggered()), this, SLOT(StartBuildRec()));
    connect(stop_rec, SIGNAL(triggered()), this, SLOT(StopBuildRec()));
}

void CSectViewFrame::StartBuildRec()
{
    qDebug("StartBuildRec");
    builderror = false;
    bool to_be_build[3];
    memset(to_be_build, 0, sizeof(bool)*3);
    
    for(int i = 0; i < 3; i++){
	if(loc[i] != m_sectview->AnchorValue(i)){
	    to_be_build[i] = true;
	    loc[i] = m_sectview->AnchorValue(i);
	}
    }
    
            QString inputfile1,anglefile,path2,outputfile,dataType ;
            path2=CEnv::CurrentWorkDirectory();
            QString processDir=CProManage::GetInstance()->GetDir(PROCESS_DIR).data();
            anglefile=QString(CProManage::GetInstance()->GetDir(PROCESS_DIR).c_str())+"_new.tlt";
            
            dataType=QString::number(m_datatype_combobox->currentIndex()+1);

            //generate cmd 
            QString ICONPreprocessCmd = "";
            QString ICONPreprocessOutputFileName=processDir+"_preprocessTemp.mrc";
            ICONPreprocessCmd.append("ICONPreProcess")
                            .append(" -i ").append(_data.Name())
                            .append(" -o ").append(ICONPreprocessOutputFileName)
                            .append(" -bin 1 ");
                            
            // ./ICON -input BBa_fin.mrc -tiltfile BBa_new.tlt -outputPath . -slice 0,63 -ICONIteration 1,7,2 -thr 0
            QString ICONCmdTemplate =
                 QString("ICON")
                .append(" -input ").append(ICONPreprocessOutputFileName)
                .append(" -tiltfile ").append(anglefile)
                .append(" -outputPath ").append(path2)
                .append(" -slice %1,%2 ")
                .append(" -ICONIteration 2,16,2 ")
                .append(" -dataType ").append(dataType)
                .append(" -thr 0 ");
            
            QString ICONMaskCmdTemplate = "";
            

            //user wants to ignore radius,so use parameter crossVfrc and fullRecfrc
            ICONMaskCmdTemplate
            .append("ICONMask")
            .append(" -inputPath ")
            .append(" ./reconstruction ")
            .append(" -tiltfile ").append(anglefile)
            .append(" -output %1 ")
            .append(" -slice %2,%3 ")
            .append(" -thickness ").append(QString::number(CDoc::GetSectSetting().Thickness()))
            .append(" -nm 1 ");
        
        /*        // radius greater than zero,so use parameter radius and ignore crossVfrc and fullRecfrc
                ICONMaskCmd
                .append(ICONMaskPath)
                .append(" -inputPath ").append("./reconstruction")
                .append(" -tiltfile ").append(inputfile2.data())
                .append(" -output ").append(m_outputImage)
                .append(" -slice ")
                .append(QString::number(slice1).toStdString().data()).append(",").append(QString::number(slice2).toStdString().data())
                .append(" -thickness ").append(thickness)
                .append(" -radius ").append(radius)
                .append(" -z ").append(ZShift)
                .append(" -pang ").append(pitch);
        */
        
        QString sliceName[3];
        QString instruction=ICONPreprocessCmd;
        sliceName[0]=CDoc::GetSectSetting().Slice1Name();
        sliceName[1]=CDoc::GetSectSetting().Slice2Name();
        sliceName[2]=CDoc::GetSectSetting().Slice3Name();
        
        for(int i=0;i<3;i++){
            if(to_be_build[i]){
            int slice=_data.Height()*m_sectview->AnchorValue(i);
            QString iconCmd=ICONCmdTemplate.arg(slice).arg(slice);
            QString iconmaskCmd=ICONMaskCmdTemplate.arg(sliceName[i]).arg(slice).arg(slice);
            
            instruction.append(";\n").append(iconCmd).append(";\n").append(iconmaskCmd);    
            }
            
        }
        
    std::cerr<<instruction.toStdString()<<std::endl;
    msg_map::Message* msg = MMTApp::NewMessage(BUILD_MODEL_MSG, this, instruction.toLatin1().data());
    MMTApp::PostMessage(msg);
    
    start_rec->setDisabled(true);
    stop_rec->setEnabled(true);
    m_sectview->SetEnabled(false);
    statusBar()->showMessage("Building...");
}

void CSectViewFrame::StopBuildRec()
{
    MMTApp::AbortCurrentProc();
}



void CSectViewFrame::customEvent(QEvent* e)
{
    if(e->type() == msg_map::QSequelType){
	msg_map::CSequelEvent* sequel = static_cast<msg_map::CSequelEvent*>(e);
	if(sequel->msg_id == BUILD_MODEL_MSG){
	    switch(sequel->state){
            case msg_map::RESULT:{
                if(sequel->data == (void*)-1){
                    statusBar()->showMessage("Building Canceled");
                    
                    start_rec->setEnabled(true);
                    stop_rec->setDisabled(true);
                    m_sectview->SetEnabled(true);
                }
                else{
                    int* result=(int*)sequel->data;
                    if(*result!=0){
                    QMessageBox::critical(this , tr("Build Section Error!") ,QString("Build Section  Error!") ,QMessageBox::Close );
                    statusBar()->showMessage("Error");
                    start_rec->setEnabled(true);
                    stop_rec->setDisabled(true);
                    m_sectview->SetEnabled(true);
                    }
                    else{
                        statusBar()->showMessage("Finished");
                        delete [] (char*)sequel->data;
                    
                        start_rec->setEnabled(true);
                        stop_rec->setDisabled(true);
                        m_sectview->SetEnabled(true);
                        CSectTool* sect_tool = (CSectTool*)parentWidget();
                        for(int i=0;i<3;i++){
                            
                            QFileInfo file(QString(CDoc::GetSectSetting().SliceName(i)));
                            if(file.exists())
                                sect_tool->LoadSlice(i, CDoc::GetSectSetting().SliceName(i));
                            else{
                                QMessageBox::critical(this , tr("Build Section Error!") ,QString("Build Section  Error!") ,QMessageBox::Close );
                                statusBar()->showMessage("Error");
                            }	
                            std::cout<<i<<CDoc::GetSectSetting().SliceName(i)<<std::endl;

                        }
                    }
                }
                }
                break;
            case msg_map::RATE:/*{
                int rate = (unsigned long)sequel->data;
                CSectTool* sect_tool = (CSectTool*)parentWidget();
                
                //看文件是否存在
                QFileInfo file(QString(CDoc::GetSectSetting().SliceName(rate)));
                if(file.exists())
                    sect_tool->LoadSlice(rate, CDoc::GetSectSetting().SliceName(rate));
                else{
                    builderror = true;
                }	
                std::cout<<rate<<CDoc::GetSectSetting().SliceName(rate)<<std::endl;
                }*/
                break;
            case msg_map::MESSAGE: {
                QString message = (char *)sequel->data;
                std::cerr<<message.toStdString()<<std::endl;
                delete [] (char*)sequel->data;
            }
            break;
	    }
	}	
    }

    QObject::customEvent(e);
}

void CSectViewFrame::closeEvent(QCloseEvent* e)
{ 
  if("Ready" != statusBar()->currentMessage())
    StopBuildRec();
    QWidget::closeEvent(e);
}


#include "CSectView.moc"
