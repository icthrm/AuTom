#include "PreprocessWin.h"
#include <cstring>
#include "core/msg_map.h"
#include "core/msg.h"
#include "custom/custom.h"
#include "ImageViewWindow/slicemain/slicemainwindow.h"
#include "promanage/ProManage.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "Ctag.h"
using namespace std;

PreprocessWin::PreprocessWin( QWidget* parent): QWidget(parent)
{
  inputSaver = InputSaver::getInstance();
  
    m_mainWidget = new QWidget(this);
//     setWindowFlags(Qt::FramelessWindowHint);
    CreateImageFram();
    CreateButton();
    QGridLayout* l = new QGridLayout();
    l->addWidget(m_ImageFram , 0 , 0 ,4,0);
    l->addWidget(m_SetFram , 5 ,0);
    m_mainWidget->setLayout(l);
    m_mainWidget->setSizePolicy(QSizePolicy::Fixed , QSizePolicy::Fixed);
    QGridLayout *l0 = new QGridLayout();
    l0->addWidget(m_mainWidget);
    l0->setMargin(0);
    this->setLayout(l0);

    QPalette palette;
    m_mainWidget->setAutoFillBackground(true);
    palette.setColor(QPalette::Background , QColor(200, 200, 200));
    m_mainWidget->setPalette(palette);

    palette.setColor(QPalette::Background , QColor(189, 189, 189));
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    SetInputImage();
    SetOutputImage();
    string rotated_mrc_str = CProManage::GetInstance()->GetDir(PROCESS_DIR) + "_rot.st";
    m_RotatedImage = QString("%1").arg(rotated_mrc_str.data());

    connect(m_Input , SIGNAL(clicked(bool)), this , SLOT(ShowInput()));
    connect(m_OutPut , SIGNAL(clicked(bool)), this , SLOT(ShowOutput()));
    connect(m_start , SIGNAL(clicked(bool)) , this , SLOT(StartNormalize()));
    connect(m_normtype_Combobox, SIGNAL(currentIndexChanged(int)),this,SLOT(ChangeAlgorithm(int)));
    //connect(m_scanHeader , SIGNAL(clicked(bool)) , this , SLOT(ScanHeader(bool)));
   // connect(m_startRotateButton , SIGNAL(clicked(bool)) , this , SLOT(StartRotate(bool)));
    //connect(m_useAsInputButton , SIGNAL(clicked(bool)) , this , SLOT(UseInNext(bool)));
// 	CProgressBar *test = new CProgressBar();
// 	test->show();
    ScanHeader(true);
    
    
    
}

PreprocessWin::~PreprocessWin()
{
  inputSaver->preprocessing_normtype_Combobox_saved = m_normtype_Combobox->currentIndex();
  inputSaver->preprocessing_avg_LineEdit_saved = m_avg_LineEdit->text();
  inputSaver->preprocessing_std_LineEdit_saved = m_std_LineEdit->text();
  inputSaver->serialize();
}

void PreprocessWin::SetInputImage()
{
    qDebug()<<"SetInputImage";
    m_InputImage = CProManage::GetInstance()->GetDir(MRC_DIR).data();
    qDebug()<<"mrc file path"<<m_InputImage;
    if(m_InputImage != "NULL") {
        QFileInfo file(m_InputImage);
        if(file.exists()) {
            ItemModel item;
            ViewData view;
            if(item.Open(m_InputImage.toLatin1().data() , CProManage::GetInstance()->GetDir(ANGLE_DIR).c_str()))
            {
                view.Mirror(item.Img());
                m_Input->SetImage(view.MirrorImage() , 170 , 150);
            }
            else {
                m_Input->setEnabled(false);
                QMessageBox::warning(this , tr("Output Image") , tr("Can't open input file!")  ,QMessageBox::Ok);
            }
        }
        else {
            m_Input->setEnabled(false);
            QMessageBox::warning(this , tr("Set Intput Image") , tr("MRC file not exist!")  ,QMessageBox::Ok);
        }
    }
    else {
        m_Input->setEnabled(false);
        QMessageBox::warning(this , tr("Set Intput Image") , tr("Can't Find input image!")  ,QMessageBox::Ok);
//        m_Input->SetText(tr("in put") , NULL , 170 , 150 ,CSkeleton::FullFill);
    }
}

void PreprocessWin::CreateImageFram()
{
    m_ImageFram = new QFrame(m_mainWidget);
    m_ImageFram->setFrameStyle(QFrame::Panel|QFrame::Raised|QFrame::HLine);
    QGridLayout* l = new QGridLayout();
    m_Input = new CSkeleton(m_ImageFram);
    m_Input->SetText(tr("Input") , NULL , 170 , 150 ,CSkeleton::FullFill);
    m_OutPut = new CSkeleton(m_ImageFram);
    m_OutPut->SetText(tr("Output") , NULL , 170 , 150 ,CSkeleton::FullFill);
    l->addWidget(m_Input , 0 , 0);
    l->addWidget(m_OutPut , 0 ,1);
    m_ImageFram->setLayout(l);
}

void PreprocessWin::CreateButton()
{
    m_ButtonFram = new QFrame(m_mainWidget);
    m_ButtonFram->setFrameStyle(QFrame::Panel|QFrame::Sunken);

//     QFrame *rotateFrame = new QFrame( m_ButtonFram);
//     rotateFrame->setFrameStyle(QFrame::StyledPanel|QFrame::Plain);
//     m_scanHeader = new QPushButton(tr("scan header"));
//     m_rotLabel = new QLabel(tr("Image rotation(degree)"));
//     m_rotAngleEdit = new QLineEdit(rotateFrame);
    //input validity
//     QRegExp rx1("^(-?\\d+)(\\.\\d+)?$");
//      m_rotAngleEdit->setValidator(new QRegExpValidator(rx1, this));
//     m_startRotateButton = new QPushButton(tr("Rotate"));
//      m_useAsInputButton = new QPushButton("Use in normlization" , m_ButtonFram);
//     QGridLayout* l_groupbox = new QGridLayout();
//     l_groupbox->addWidget(m_scanHeader , 0 , 0);
//     l_groupbox->addWidget(m_rotLabel , 0 , 2);
//     l_groupbox->addWidget(m_rotAngleEdit , 0 , 4);
//     l_groupbox->addWidget(m_startRotateButton , 1 , 2);
//     l_groupbox->addWidget(m_useAsInputButton , 1 , 4);
//     l_groupbox->setMargin(0);
//     rotateFrame->setLayout(l_groupbox);
      m_rotAngleEdit = new QLineEdit(m_ButtonFram);
      m_header_Lable=new  QLabel(tr("Header") , m_ButtonFram);
      m_rotAngleEdit->setToolTip(ToolTips::pre_edt_header);
      m_rotAngleEdit->setEnabled(false);
     
    m_normtype_Label = new QLabel(tr("Normtype") , m_ButtonFram);
    m_normtype_Combobox = new QComboBox(m_ButtonFram);
    m_normtype_Combobox->setToolTip(ToolTips::pre_comb_type);
    m_normtype_Combobox->setObjectName("ntype");
    m_normtype_Combobox->insertItem(0 , "SIMPLE");
    m_normtype_Combobox->insertItem(1 , "GAUSSIAN");
    //m_normtype_Combobox->insertItem(2 , "ICONPreProcess");
    //inputSaver->deSerialize();
    m_normtype_Combobox->setCurrentIndex(inputSaver->preprocessing_normtype_Combobox_saved);//(inputSaver->preprocessing_normtype_Combobox_saved);
    

    m_avg_Lable= new QLabel(tr("average") , m_ButtonFram);
    m_avg_LineEdit = new QLineEdit(m_ButtonFram);
    m_avg_LineEdit->setToolTip(ToolTips::pre_edt_avg);
    QRegExp rx2("^(-?\\d+)(\\.\\d+)?$");//浮点数
    m_avg_LineEdit->setValidator(new QRegExpValidator(rx2, this));
    m_avg_LineEdit->setText(inputSaver->preprocessing_avg_LineEdit_saved);

    m_std_Lable= new QLabel(tr("standard deviation") , m_ButtonFram);
    m_std_LineEdit = new QLineEdit(m_ButtonFram);
    m_std_LineEdit->setToolTip(ToolTips::pre_edt_dev);
    QRegExp rx3("^(-?\\d+)(\\.\\d+)?$");//浮点数
    m_std_LineEdit->setValidator(new QRegExpValidator(rx3, this));
    m_std_LineEdit->setText(inputSaver->preprocessing_std_LineEdit_saved);
    m_start = new QPushButton("Normalization" , m_ButtonFram);
    m_start->setToolTip(ToolTips::per_btn_norm);
    
    QGridLayout* l = new QGridLayout();
    //l->addWidget(rotateFrame , 0 , 0 , 1 ,5);
    l->addWidget(m_normtype_Label , 1 , 0 , 1 , 1);
    l->addWidget(m_normtype_Combobox , 1 ,1 , 1 , 1);
    l->addWidget(m_avg_Lable , 1 , 3);
    l->addWidget(m_avg_LineEdit , 1 ,4 , 1 , 1 );
    l->addWidget(m_std_Lable , 2 ,0);
    l->addWidget(m_std_LineEdit , 2, 1 ,1,1);
    l->addWidget(m_start , 2 ,4 , 1,1);
 
    l->addWidget(m_header_Lable, 0 ,0 , 1,1);
     l->addWidget(m_rotAngleEdit ,0,1,1,1);
// 	l->addWidget(m_doneButton , 2 , 4 , 1 ,1);
    m_ButtonFram->setLayout(l);
//
    m_tabWidget = new QTabWidget(m_mainWidget);
//    m_tabWidget->setStyleSheet(tr("QTabWidget::pane{background-color:rgb(132, 171, 208);}"));
    setStyleSheet("QTabWidget::pane{border-width:1px;border-color:rgb(48, 104, 151);\
                                    border-style: outset;background-color: rgb(132, 171, 208);\
                                    background: transparent;} \
                 }\
");
    //m_tabWidget->addTab(rotateFrame , tr("rotate"));//
    m_tabWidget->addTab(m_ButtonFram , tr("normlization"));

    m_progressbar = new CProgressBar(m_mainWidget);
    m_ls = new QStackedLayout;
//     ls->setStackingMode(QStackedLayout::StackAll);
    m_ls->addWidget(m_tabWidget);
    m_ls->addWidget(m_progressbar);

    m_ls->setCurrentWidget(m_tabWidget);
//     m_ls->setCurrentWidget(m_progressbar);
    m_SetFram = new QFrame(m_mainWidget);
    m_SetFram->setLayout(m_ls);
 
}

//未改-
void PreprocessWin::SetOutputImage()
{
    cout<<"Set Output image"<<endl;
    m_outputImage = CProManage::GetInstance()->GetDir(PRE_MRC_DIR).data();
    qDebug()<<m_outputImage;
    if(m_outputImage != "NULL")
    {
        QFileInfo file(m_outputImage);
        if(file.exists()) {
            ItemModel item;
            ViewData view;
            if( item.Open(m_outputImage.toLatin1().data() , CProManage::GetInstance()->GetDir(ANGLE_DIR).c_str()))
            {
                view.Mirror(item.Img());
                m_OutPut->setEnabled(true);
                m_OutPut->SetImage(view.MirrorImage() , 170 , 150);
            }
            else {
                m_OutPut->setEnabled(false);
                QMessageBox::warning(this , tr("Output Image") , tr("Can't open output file!")  ,QMessageBox::Ok);
            }
        }
        else {
            m_OutPut->setEnabled(false);
            QMessageBox::warning(this , tr("Set Output Image") , tr("PreProcess MRC  file not exist!")  ,QMessageBox::Ok);
        }
    }
    else
    {
        m_OutPut->setEnabled(false);
    }
}
//-------------------------------------------

void PreprocessWin::ScanHeader(bool )
{
    util::MrcStack mrcs;
    if(!mrcs.Open(m_InputImage.toLatin1().data())) {
        CProManage::AddItem("PREPROCESS_STATE" , "FAIL");
        m_ls->setCurrentWidget(m_tabWidget);
        QMessageBox::warning(this , tr("ERROR") , tr("Can't open input mrc file!")  ,QMessageBox::Ok);
        return;
    }

    if(mrcs.inplane_rotation == -100) {
        m_ls->setCurrentWidget(m_tabWidget);
        QMessageBox::warning(this , tr("Warning") , tr("There is no tilt axis rotation angle info in mrc file!")  ,QMessageBox::Ok);
        return;
    }

    m_rotAngleEdit->setText(QString("%1").arg(mrcs.inplane_rotation));;
}

void PreprocessWin::StartRotate(bool )
{
    emit Start();
    m_ls->setCurrentWidget(m_progressbar);

    if(m_outputImage != "NULL") {
        QFile file(m_outputImage);
        if(file.exists()) {
            file.remove();
        }
        CProManage::GetInstance()->SetState(PREPROCESS_STATE , UNFINISH);
        CProManage::GetInstance()->SetDir(PRE_MRC_DIR , "NULL");
        CProManage::ReWriteProFile();
        SetOutputImage();
    }

    string input = CProManage::GetInstance()->GetDir(MRC_DIR).data();
    string output = CProManage::GetInstance()->GetDir(PROCESS_DIR) + "_rot.st";
    QString rot_angle = m_rotAngleEdit->text();
    m_preprocess_dir=output.data();

    QString instruction = QString("\n%1\n%2\n%3").arg(input.data()).arg(output.data()).arg(rot_angle);
    msg_map::Message* msg = MMTApp::NewMessage(ROTATE_MRC_MSG , this, instruction.toLatin1().data());
    MMTApp::PostMessage(msg);
}

void PreprocessWin::UseInNext(bool )
{
    string rotated_mrc_str = CProManage::GetInstance()->GetDir(PROCESS_DIR) + "_rot.st";
    m_RotatedImage = QString("%1").arg(rotated_mrc_str.data());
    QFile file(m_RotatedImage);
    if(!file.exists()) {
        QMessageBox::warning(this , tr("Warning") , tr("Rotated mrc not exit!")  ,QMessageBox::Ok);
        return;
    }

    if(CProManage::GetInstance()->GetMethod()=="BasedMethod") {
        CProManage::AddItem("PREPROCESS_STATE_BASED" , "FINISH");
        CProManage::AddItem("PRE_MRC_DIR" , m_RotatedImage.toLatin1().data());
        emit Finish("PreProcessBased");
    }
    else {
        CProManage::AddItem("PREPROCESS_STATE" , "FINISH");
        CProManage::AddItem("PRE_MRC_DIR" , m_RotatedImage.toLatin1().data());
        emit Finish("PreProcess");
    }
    SetOutputImage();

//     m_InputImage  = rotated_mrc;
//     m_outputImage = "";
//
//     ItemModel item;
//     ViewData view;
// 	 if( item.Open(m_InputImage.toLatin1().data() , CProManage::GetInstance()->GetDir(ANGLE_DIR).c_str()))
// 	 {
// 	    view.Mirror(item.Img());
// 	    m_Input->SetImage(view.MirrorImage() , 170 , 150);
// 	    m_Input->setEnabled(true);
// 	 }
// 	 else{
// 	   m_OutPut->setEnabled(false);
// 	   QMessageBox::warning(this , tr("Output Image") , tr("Can't open output file!")  ,QMessageBox::Ok);
// 	 }

    m_useAsInputButton->setDown(true);

}

bool PreprocessWin::StartNormalize()
{
    if(m_avg_LineEdit->text().isEmpty()||m_std_LineEdit->text().isEmpty()) {
        QMessageBox::warning(this , tr("Warning") , tr("Parameter cannot be null!")  ,QMessageBox::Ok);
        return false;
    }

    emit Start();
    m_ls->setCurrentWidget(m_progressbar);


    if(m_outputImage != "NULL" && m_outputImage != m_RotatedImage) {

        QFile file(m_outputImage);
        if(file.exists()) {
            file.remove();
        }
        CProManage::GetInstance()->SetState(PREPROCESS_STATE , UNFINISH);
        CProManage::GetInstance()->SetDir(PRE_MRC_DIR , "NULL");
        CProManage::ReWriteProFile();
        SetOutputImage();
    }

    QString ntype;
    QString avg;
    QString std;
    int type;

    ntype = m_normtype_Combobox->currentText();
    if(ntype == "SIMPLE")
        type = 0;
    else if(ntype == "GAUSSIAN")
        type = 1;
    else 
        type = 2;

    avg = m_avg_LineEdit->text();
    std = m_std_LineEdit->text();

    string input = m_InputImage.toStdString();
    if(m_outputImage == m_RotatedImage) {
        input = m_RotatedImage.toStdString();
    }

    string output = CProManage::GetInstance()->GetDir(PROCESS_DIR) + "_norm.st";
    m_preprocess_dir=output.data();
//     CProManage::AddItem("PRE_MRC_DIR" , output);
    QString instruction = QString("\n%1\n%2\n%3\n%4\n%5").arg(input.data()).arg(avg).arg(std).arg(type).arg(output.data());
    msg_map::Message* msg = MMTApp::NewMessage(H_NORMALIZE_MSG, this, instruction.toLatin1().data());
    
    MMTApp::PostMessage(msg);

    return true;
}

void PreprocessWin::ShowInput()
{
    if(CProManage::GetInstance()->GetState(MRC_DIR) != UNFINISH) {
//        ImageViewWindow::ViewMrc(m_InputImage);
//           SliceMainWindow::ViewMrc(m_InputImage);
   //     QString instruction = QString("%1%2%3%4").arg("imod ").arg(QDir::currentPath()).arg("/").arg(m_InputImage);
//        printf("%s",instruction.data());
//    //     printf("instruction %s\n",instruction.toStdString().data());
//     //   m_ls->setCurrentWidget(m_progressbar);


        //MMTApp::PostMessage(msg);

//        QProcess cmd;
//        cmd.execute(instruction);
    }

      QString instruction = QString("\n%1").arg(m_InputImage);
      qDebug() << instruction << endl;
      msg_map::Message* msg = MMTApp::NewMessage(IMOD_MSG , this , instruction.toLatin1().data());
      MMTApp::PostMessage(msg);
}

void PreprocessWin::ShowOutput()
{
    QString outpath = CProManage::GetInstance()->GetDir(PRE_MRC_DIR).data();
    QString instruction = QString("\n%1").arg(outpath);
    qDebug() << instruction << endl;
    msg_map::Message* msg = MMTApp::NewMessage(IMOD_MSG , this , instruction.toLatin1().data());
    MMTApp::PostMessage(msg);
    //std::cout<<"m_outputImage:"<<m_outputImage<<"\n";
//    ImageViewWindow::ViewMrc(outpath);
//    SliceMainWindow::ViewMrc(outpath);
//    QProcess cmd;
//    QString instruction = QString("%1 %2").arg("imod").arg(outpath);

//    qDebug()<<"current currentPath: "<<QDir::currentPath();
//    qDebug() << outpath << endl;
//    cmd.execute(instruction);
//    QString instruction = QString("\n%1\n%2").arg("3dmod ").arg(outpath);
//    printf("%s",instruction.data());
// //     printf("instruction %s\n",instruction.toStdString().data());
// //   m_ls->setCurrentWidget(m_progressbar);
//    msg_map::Message* msg = MMTApp::NewMessage(H_NORMALIZE_MSG , this , instruction.toLatin1().data());
//    MMTApp::PostMessage(msg);

}

void PreprocessWin::ChangeAlgorithm(int index){
    if(index==2){
        m_avg_Lable->hide();
        m_avg_LineEdit->hide();
        m_std_Lable->hide();
        m_std_LineEdit->hide();
        m_header_Lable->hide();
        m_rotAngleEdit->hide();        
    }
    else{
        m_avg_Lable->show();
        m_avg_LineEdit->show();
        m_std_Lable->show();
        m_std_LineEdit->show();
        m_header_Lable->show();
        m_rotAngleEdit->show();
    }
    
}
void PreprocessWin::customEvent(QEvent* e )
{
    if(e->type() == msg_map::QSequelType) {
        msg_map::CSequelEvent* sequel = static_cast<msg_map::CSequelEvent*>(e);
        if(sequel->msg_id == H_NORMALIZE_MSG)///---------------------------------
        {
            switch(sequel->state) {
            case msg_map::MESSAGE: {
                QString message = (char *)sequel->data;
                emit Log(message);
                delete [] (char*)sequel->data;
            }
            break;
            case msg_map::RATE: {
                int rate = (unsigned long)sequel->data;
            }
            break;
            case msg_map::RESULT:
            {
                m_ls->setCurrentWidget(m_tabWidget);
                int state = (unsigned long)sequel->data;
                if(sequel->data == (void*)-1)
                {
                    qDebug()<<"-1";
                    QFile file(m_preprocess_dir);
                    if(file.exists()) {
                        file.remove();
                    }
                    if(CProManage::GetInstance()->GetMethod()=="BasedMethod") {
                        CProManage::AddItem("PREPROCESS_STATE_BASED" , "FAIL");
                    }
                    else {
                        CProManage::AddItem("PREPROCESS_STATE" , "FAIL");
                    }
                    emit RunError();
                }
                else if(sequel->data == (void*)1)
                {
                    QFileInfo file(m_preprocess_dir);
                    if(file.exists())
                    {
                        if(CProManage::GetInstance()->GetMethod()=="BasedMethod") {
                            CProManage::AddItem("PREPROCESS_STATE_BASED" , "FINISH");
                            CProManage::AddItem("PRE_MRC_DIR" , m_preprocess_dir.toLatin1().data());
                            emit Finish("PreProcessBased");
                        }
                        else {
                            CProManage::AddItem("PREPROCESS_STATE" , "FINISH");
                            CProManage::AddItem("PRE_MRC_DIR" , m_preprocess_dir.toLatin1().data());
                            emit Finish("PreProcess");
                        }
                        SetOutputImage();
                    }
                    else
                    {
                        QFile file(m_preprocess_dir);
                        if(file.exists()) {
                            file.remove();
                        }
                        if(CProManage::GetInstance()->GetMethod()=="BasedMethod") {
                            CProManage::AddItem("PREPROCESS_STATE_BASED" , "FAIL");
                        }
                        else {
                            CProManage::AddItem("PREPROCESS_STATE" , "FAIL");
                        }
                        emit RunError();
                    }
                }
            }
            break;
            }
        }

        if(sequel->msg_id == ROTATE_MRC_MSG )///---------------------------------
        {
            switch(sequel->state) {
            case msg_map::MESSAGE: {
                QString message = (char *)sequel->data;
                emit Log(message);
                delete [] (char*)sequel->data;
            }
            break;
            case msg_map::RATE: {
                int rate = (unsigned long)sequel->data;
            }
            break;
            case msg_map::RESULT:
            {
                m_ls->setCurrentWidget(m_tabWidget);
                int state = (unsigned long)sequel->data;
                if(sequel->data == (void*)-1)
                {
                    QFile file(m_preprocess_dir);
                    if(file.exists()) {
                        file.remove();
                    }
                    if(CProManage::GetInstance()->GetMethod()=="BasedMethod") {
                        CProManage::AddItem("PREPROCESS_STATE_BASED" , "FAIL");
                    }
                    else {
                        CProManage::AddItem("PREPROCESS_STATE" , "FAIL");
                    }
                    emit RunError();
                }
                else if(sequel->data == (void*)1)
                {
                    QFileInfo file(m_preprocess_dir);
                    if(file.exists())
                    {
                        if(CProManage::GetInstance()->GetMethod()=="BasedMethod") {
                            CProManage::AddItem("PREPROCESS_STATE_BASED" , "FINISH");
                            CProManage::AddItem("PRE_MRC_DIR" , m_RotatedImage.toLatin1().data());
                            emit Finish("PreProcessBased");
                        }
                        else {
                            CProManage::AddItem("PREPROCESS_STATE" , "FINISH");
                            CProManage::AddItem("PRE_MRC_DIR" , m_RotatedImage.toLatin1().data());
                            emit Finish("PreProcess");
                        }
                        SetOutputImage();
                    }
                    else
                    {
                        QFile file(m_preprocess_dir);
                        if(file.exists()) {
                            file.remove();
                        }
                        if(CProManage::GetInstance()->GetMethod()=="BasedMethod") {
                            CProManage::AddItem("PREPROCESS_STATE_BASED" , "FAIL");
                        }
                        else {
                            CProManage::AddItem("PREPROCESS_STATE" , "FAIL");
                        }

                        emit RunError();
                    }
                }
            }
            break;
            }
        }
    }

    QObject::customEvent(e);
}


#include "PreprocessWin.moc"
