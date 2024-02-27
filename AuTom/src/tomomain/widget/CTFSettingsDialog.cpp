#include<QWidget>
#include<QPushButton>
#include<QLineEdit>
#include<QLabel>
#include<QComboBox>
#include<QCheckBox>
#include<QTextEdit>
#include<QGridLayout>
#include<QIntValidator>
#include<QDoubleValidator>
#include<Qt>
#include<QMessageBox>
#include<QDebug>
#include<QFileInfo>
#include<QFrame>
#include<QStackedLayout>
#include<QHBoxLayout>
#include<QTextCursor>
#include <QTextCodec>

#include<string>

#include "CProgressBar.h"
#include"promanage/ProManage.h"
#include "CTFSettingsDialog.h"
#include "common/mrcimg/mrcheader.h"
#include"core/msg_map.h"
#include"custom/custom.h"

CTFSettingsDialog::CTFSettingsDialog(QWidget* parent):QDialog(parent){
    //frame1
    QGridLayout *l=new QGridLayout();
    QIntValidator* intValidator=new QIntValidator();
    intValidator->setBottom(0);
    QDoubleValidator* doubleValidator = new QDoubleValidator();
    doubleValidator ->setBottom(0.0);
    //QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    m_defocusDifference_label = new QLabel(QString(tr("Defocus Difference (Delta D) for strip extraction [Å]")));
 	m_defocusDifference_lineEdit = new QLineEdit();
 	m_defocusDifference_lineEdit ->setValidator(intValidator);
    l->addWidget(m_defocusDifference_label,0,0,1,2);
    l->addWidget(m_defocusDifference_lineEdit,0,2,1,1);
    
    
    m_XMAG_label = new QLabel("XMAG");
 	m_XMAG_lineEdit = new QLineEdit();
 	m_XMAG_lineEdit->setValidator(intValidator);
    l->addWidget(m_XMAG_label,0,3,1,1);
    l->addWidget(m_XMAG_lineEdit,0,4,1,1);
    
    m_DStep_label = new QLabel(tr("DStep[µm]"));
 	m_DStep_lineEdit = new QLineEdit();
    m_DStep_lineEdit->setValidator(intValidator);
    l->addWidget(m_DStep_label,1,0,1,1);
    l->addWidget(m_DStep_lineEdit,1,1,1,1);
    
    m_tileSize_label = new QLabel("Tile size [pixels] ");
 	m_tileSize_lineEdit = new QLineEdit();
 	m_tileSize_lineEdit->setValidator(intValidator);
    l->addWidget(m_tileSize_label,1,2,1,1);
    l->addWidget(m_tileSize_lineEdit,1,3,1,1);
    
    m_CS_label = new QLabel("CS[mm]");
 	m_CS_lineEdit = new QLineEdit();
    m_CS_lineEdit->setValidator(doubleValidator);
    l->addWidget(m_CS_label,1,4,1,1);
    l->addWidget(m_CS_lineEdit,1,5,1,1);
 	
    m_HT_label = new QLabel("HT[kV]");
 	m_HT_lineEdit = new QLineEdit();
 	m_HT_lineEdit->setValidator(doubleValidator);
    l->addWidget(m_HT_label,2,0,1,1);
    l->addWidget(m_HT_lineEdit,2,1,1,1);
        
    m_AmpCnst_label = new QLabel("AmpCnst");
 	m_AmpCnst_lineEdit = new QLineEdit();
 	m_AmpCnst_lineEdit->setValidator(doubleValidator);
    l->addWidget(m_AmpCnst_label,2,2);
    l->addWidget(m_AmpCnst_lineEdit,2,3);
    
    m_CTFMin_label = new QLabel("CTFMin[0,1]");
 	m_CTFMin_lineEdit = new QLineEdit();
 	QDoubleValidator* CTFMin_doubleValidator= new QDoubleValidator(0,1,5);
    m_CTFMin_lineEdit->setValidator(CTFMin_doubleValidator);
    l->addWidget(m_CTFMin_label,2,4);
    l->addWidget(m_CTFMin_lineEdit,2,5);
    
    m_ResMin_label = new QLabel(tr("ResMin[Å]"));
 	m_ResMin_lineEdit = new QLineEdit();
    m_ResMin_lineEdit->setValidator(doubleValidator);
    l->addWidget(m_ResMin_label,3,0);
    l->addWidget(m_ResMin_lineEdit,3,1);
    
 	m_ResMax_label = new QLabel(tr("ResMax[Å]"));
 	m_ResMax_lineEdit = new QLineEdit();
 	m_ResMax_lineEdit->setValidator(doubleValidator);
    l->addWidget(m_ResMax_label,3,2);
    l->addWidget(m_ResMax_lineEdit,3,3);
    
    m_dFMin_label = new QLabel(tr("dFMin[Å]"));
 	m_dFMin_lineEdit = new QLineEdit();
    m_dFMin_lineEdit->setValidator(doubleValidator);
    l->addWidget(m_dFMin_label,3,4);
    l->addWidget(m_dFMin_lineEdit,3,5);
    
	m_dFMax_label = new QLabel(tr("dFMax[Å]"));
	m_dFMax_lineEdit = new QLineEdit();
	m_dFMax_lineEdit->setValidator(doubleValidator);
    l->addWidget(m_dFMax_label,4,0);
    l->addWidget(m_dFMax_lineEdit,4,1);
        
    m_FStep_label = new QLabel(tr("FStep[Å]"));
	m_FStep_lineEdit = new QLineEdit();
	m_FStep_lineEdit->setValidator(doubleValidator);
    l->addWidget(m_FStep_label,4,2);
    l->addWidget(m_FStep_lineEdit,4,3);
    
    m_defocus_label = new QLabel(tr("Defocus at the untilted plane [Å]"));
	m_defocus_lineEdit = new QLineEdit();
	m_defocus_lineEdit->setValidator(doubleValidator);
    l->addWidget(m_defocus_label,4,4);
    l->addWidget(m_defocus_lineEdit,4,5);    
    
    m_amplitudeCorrection_label = new QLabel("Amplitude Correction?");
	m_amplitudeCorrection_comboBox = new QComboBox();
	m_amplitudeCorrection_comboBox->insertItem(0,"Y");
    m_amplitudeCorrection_comboBox->insertItem(1,"N");
    m_amplitudeCorrection_comboBox->setCurrentIndex(1);
    l->addWidget(m_amplitudeCorrection_label,5,0);
    l->addWidget(m_amplitudeCorrection_comboBox,5,1);
    
    m_processNumber_label = new QLabel("Number of process");
    m_processNumber_lineEdit = new QLineEdit("1");
    QIntValidator* processValidator=new QIntValidator();
    processValidator->setBottom(1);
    m_processNumber_lineEdit->setValidator(processValidator);
    l->addWidget(m_processNumber_label,5,2);
    l->addWidget(m_processNumber_lineEdit,5,3);
    
    m_filter1_label = new QLabel("Wiener filter parameters 1 ");
	m_filter1_lineEdit = new QLineEdit();
	m_filter2_label = new QLabel("Wiener filter parameters 2 ");
	m_filter2_lineEdit = new QLineEdit();    
	m_filter1_lineEdit->setValidator(doubleValidator);
    m_filter2_lineEdit->setValidator(doubleValidator);
    l->addWidget(m_filter1_label,6,0);
    l->addWidget(m_filter1_lineEdit,6,1);
    l->addWidget(m_filter2_label,6,2);
    l->addWidget(m_filter2_lineEdit,6,3);
    m_filter1_lineEdit->setEnabled(false);
    m_filter2_lineEdit->setEnabled(false);
    
    m_enableShellEdit_checkBox = new QCheckBox("I want to edit the shellscript myself");
	m_shell_textEdit = new QTextEdit();
	m_shellScript_label = new QLabel("shellscript:");
    l->addWidget(m_enableShellEdit_checkBox,7,0,1,2);
    l->addWidget(m_shellScript_label,8,0);
    l->addWidget(m_shell_textEdit,9,0,5,4);
    m_shell_textEdit->setEnabled(false);
    
    m_hints_textedit=new QTextEdit();
    m_hints_textedit->setText("Current AuTom does not provide the CTF determination and correction program. This interface is used for users to define the CTF behavior themselves. Users can introduce the third part CTF module by programming the shellscript here.\nNote:1. AuTom also provides a shellscript by default. The default CTF shellscript uses the package TomoCTF, which is developed by J.J. Fernandez. To try the default setting, users should download TomoCTF from its website and make AuTom find it (TomoCTF website: https://sites.google.com/site/3demimageprocessing/tomoctf). The used TomoCTF modules are named as tomops.exe, tomoctffind.exe and CTFcorrect.exe. \n2.Use $(InputMrcFile) to represent the input mrc file.\n3.Use $(InputTiltFile) to represent the iinput tilt file.\n4.Use $(result) to represent the corrected mrc file. \n5.All the parameters mentioned above DO NOT contain the filename extension. \neg: $(InputMrcFile).mrc is the full name of a mrc file.\n");
    m_hints_textedit->setReadOnly(true);
    l->addWidget(m_hints_textedit,9,4,5,4);
    
    m_generateShell_btn = new QPushButton("generate shell script");
    m_start_btn = new QPushButton("execute the shellscript");
	m_cancel_btn = new QPushButton("cancel");
    l->addWidget(m_generateShell_btn,14,0,1,2);
    l->addWidget(m_start_btn,14,2,1,2);
    l->addWidget(m_cancel_btn,14,4,1,2);
    m_XMAG_label -> setAlignment(Qt::AlignRight | Qt::AlignVCenter);
 	m_DStep_label -> setAlignment(Qt::AlignRight | Qt::AlignVCenter); 	
 	m_tileSize_label -> setAlignment(Qt::AlignRight | Qt::AlignVCenter); 	
 	m_CS_label -> setAlignment(Qt::AlignRight | Qt::AlignVCenter); 	
 	m_HT_label -> setAlignment(Qt::AlignRight | Qt::AlignVCenter); 	
 	m_AmpCnst_label -> setAlignment(Qt::AlignRight | Qt::AlignVCenter); 	
 	m_CTFMin_label -> setAlignment(Qt::AlignRight | Qt::AlignVCenter); 	
 	m_ResMin_label -> setAlignment(Qt::AlignRight | Qt::AlignVCenter); 	     
 	m_ResMax_label -> setAlignment(Qt::AlignRight | Qt::AlignVCenter); 	
 	m_dFMin_label -> setAlignment(Qt::AlignRight | Qt::AlignVCenter); 	
	m_dFMax_label -> setAlignment(Qt::AlignRight | Qt::AlignVCenter);	
	m_FStep_label -> setAlignment(Qt::AlignRight | Qt::AlignVCenter);	
	m_defocus_label -> setAlignment(Qt::AlignRight | Qt::AlignVCenter);	
	m_amplitudeCorrection_label -> setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	m_filter1_label -> setAlignment(Qt::AlignRight | Qt::AlignVCenter);	
	m_filter2_label -> setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_processNumber_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    
    m_settigns_frame=new QFrame();
    m_settigns_frame->setLayout(l);
    
    //frame2
    m_progressbar=new CProgressBar();  
    m_progressbar->setWidth(180);
    
    m_stacklayout =new QStackedLayout();
    m_stacklayout->addWidget(m_settigns_frame);
    m_stacklayout->addWidget(m_progressbar);
    m_stacklayout->setCurrentWidget(m_settigns_frame);
    
    QFrame *v1=new QFrame();
    v1->setLayout(m_stacklayout);
    m_output_textEdit=new QTextEdit();
    m_output_textEdit->setReadOnly(true);
    m_output_label=new QLabel("Output");
    QVBoxLayout* rootLayout= new QVBoxLayout();
    rootLayout->addWidget(v1);
    rootLayout->addWidget(m_output_label);
    rootLayout->addWidget(m_output_textEdit);
    setLayout(rootLayout);
    
    settings = new CTFSettings();
    connect(m_enableShellEdit_checkBox,SIGNAL(stateChanged(int)),this,SLOT(enableShellEdit()));
    connect(m_amplitudeCorrection_comboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(enableAmplitudeCorrection()));
    connect(m_generateShell_btn,SIGNAL(clicked(bool)),this,SLOT(generateShell()));
    connect(m_start_btn,SIGNAL(clicked(bool)),this,SLOT(executeShell()));
    connect(this,SIGNAL(Log(QString)),this,SLOT(printMessage(QString)));
    connect(m_cancel_btn,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(this,SIGNAL(RunError(QString)),this,SLOT(printMessage(QString)));
    connect(this,SIGNAL(Finish(QString)),this,SLOT(printMessage(QString)));
}
CTFSettingsDialog::~CTFSettingsDialog()
{
    delete settings;
}
void CTFSettingsDialog::enableShellEdit(){
    bool state = m_enableShellEdit_checkBox->isChecked();
    setLineEditState(!state);
    m_shell_textEdit->setEnabled(state);
}
void CTFSettingsDialog::enableAmplitudeCorrection()
{
    int index=m_amplitudeCorrection_comboBox->currentIndex();
    bool state=(index==0);
    m_filter1_lineEdit->setEnabled(state);
    m_filter2_lineEdit->setEnabled(state);
    
}

void CTFSettingsDialog::setLineEditState(bool state)
{
    m_defocusDifference_lineEdit -> setEnabled(state);    
    m_XMAG_lineEdit -> setEnabled(state);    
    m_DStep_lineEdit -> setEnabled(state);    
    m_tileSize_lineEdit -> setEnabled(state);
    m_CS_lineEdit -> setEnabled(state);
    m_HT_lineEdit -> setEnabled(state);
    m_AmpCnst_lineEdit -> setEnabled(state);
    m_CTFMin_lineEdit -> setEnabled(state);
    m_ResMin_lineEdit -> setEnabled(state);
    m_ResMax_lineEdit -> setEnabled(state);
    m_dFMin_lineEdit -> setEnabled(state);
    m_dFMax_lineEdit -> setEnabled(state);
    m_FStep_lineEdit -> setEnabled(state);
    m_defocus_lineEdit -> setEnabled(state);
    m_amplitudeCorrection_comboBox -> setEnabled(state);
    m_filter1_lineEdit -> setEnabled(state);
    m_filter2_lineEdit -> setEnabled(state);    
    m_generateShell_btn -> setEnabled(state);
    m_processNumber_lineEdit->setEnabled(state);
    if(state){
        if(m_amplitudeCorrection_comboBox->currentIndex()==1){
            m_filter1_lineEdit->setEnabled(false);
            m_filter2_lineEdit->setEnabled(false);
        }
    }
}

void CTFSettingsDialog::generateShell()
{
    bool noEmpty=readSettings();
    if(!noEmpty){
        QMessageBox::critical(this,"errors occur when try to generate","All parameters must be set!");
        return;
    }
    
    
     string inputMrc=CProManage::GetInstance()->GetDir(PROCESS_DIR)+"_fin.mrc";
     string inputTilt=CProManage::GetInstance()->GetDir(PROCESS_DIR)+"_new.tlt";     
    
    MrcHeader* header=new MrcHeader();
    if(!MrcReadHead(header,inputMrc.data())){
        QMessageBox::critical(this,"errors occur when try to generate","MrcFile not found");
    }
    int imageNumber=header->nz;
    
    QString check="\n\
if [ $? -ne 0 ];then\n\
	echo \"%1 fail\"\n\
exit 1\n\
fi\n\
";
       
    QString tomopsShell=QString("tomops.exe << eof\n")
                        .append("0\n")
                        .append("$(InputMrcFile).mrc").append("\n")
                        .append("$(InputTiltFile).tlt").append("\n")
                        .append("power.mrc\n")
                        .append(QString::number(settings->defocusDifference)).append("\n")
                        .append(QString::number(settings->XMAG)).append(",").append(QString::number(settings->DStep)).append("\n")
                        .append(QString::number(settings->tileSize)).append("\n")
                        .append("eof\n")
                        .append(check.arg("tomops"));
    QString tomoctffindShell=QString("tomoctffind.exe << eof\npower.mrc\ndiagnostic.mrc\n")
                                .append(QString::number(settings->CS)+",").append(QString::number(settings->HT)+",").append(QString::number(settings->AmpCnst)+",").append(QString::number(settings->XMAG)+",").append(QString::number(settings->DStep)+"\n")                             
                                .append(QString::number(settings->CTFMin)+",").append(QString::number(settings->ResMin)+",").append(QString::number(settings->ResMax)+"\n")                             
                                .append(QString::number(settings->dFMin)+",").append(QString::number(settings->dFMax)+",").append(QString::number(settings->FStep)+"\n").append("eof\n")
                                .append(check.arg("tomoctffind"));
                                
    QString tomoctfcorrect = QString(
"mkdir __splitMrcFile\nmkdir __ctfCorrectMrcFile\n").append(
"mrcsplit -i $(InputMrcFile).mrc -o __splitMrcFile\n\
for((i=0;i<%1;i++))\n").arg(imageNumber).append("\n\
do\n\
    let j=i+1\n\
    tiltangle=`head -n $j $(InputTiltFile).tlt | tail -n 1`\n\
    echo Correcting image: $i , tilt angle: $tiltangle\n\
    running=`ps -c | grep CTFcorrect.exe | wc -l | awk '{printf(\"%d\",$1+1)}'`\n\
    if [ $running -lt %1 ];then\n").arg(settings->processNumber).append("\
{\n\
CTFcorrect.exe __splitMrcFile/$(InputMrcFile)$i.mrc __ctfCorrectMrcFile/$(InputMrcFile)$i.mrc $tiltangle <<eof\n\
%1\n\
%2\n").arg(settings->defocus).arg(settings->defocusDifference).append("\
%1 %2 %3 %4 %5\n").arg(settings->CS).arg(settings->HT).arg(settings->AmpCnst).arg(settings->XMAG).arg(settings->DStep).append("\
%1\n%2").arg(settings->amplitudeCorrection?"Y":"N").arg(settings->amplitudeCorrection? QString("%1,%2").arg(settings->filter1).arg(settings->filter2)+"\n":"").append("\
eof\n\
}&\n\
else\n\
    CTFcorrect.exe __splitMrcFile/$(InputMrcFile)$i.mrc __ctfCorrectMrcFile/$(InputMrcFile)$i.mrc $tiltangle <<eof\n\
%1\n\
%2\n").arg(settings->defocus).arg(settings->defocusDifference).append("\
%1 %2 %3 %4 %5\n").arg(settings->CS).arg(settings->HT).arg(settings->AmpCnst).arg(settings->XMAG).arg(settings->DStep).append("\
%1\n%2").arg(settings->amplitudeCorrection?"Y":"N").arg(settings->amplitudeCorrection? QString("%1,%2").arg(settings->filter1).arg(settings->filter2)+"\n":"").append("\
eof\n\
fi\n\
done\n").append("\
wait\n\
for((i=0;i<%1;i++))\n").arg(imageNumber).append("\n\
do\n\
    if [ $i == 0 ];then\n\
        cp __splitMrcFile/$(InputMrcFile)$i.mrc $(result).mrc\n\
    else\n\
        mrcCombine -i $(result).mrc -a __ctfCorrectMrcFile/$(InputMrcFile)$i.mrc\n\
        echo combine image $i\n\
    fi\n\
done\n\
rm __splitMrcFile\/*\n\
rmdir __splitMrcFile\n\
rm __ctfCorrectMrcFile\/*\n\
rmdir __ctfCorrectMrcFile\n");
    
    QString shell=tomopsShell+tomoctffindShell+tomoctfcorrect;
    m_shell_textEdit->setText(shell);
    
    
    
                        
}
 void CTFSettingsDialog::executeShell()
{
    m_output_textEdit->clear();
    
    QString shell=m_shell_textEdit->toPlainText();
    if(shell==""){
        QMessageBox::critical(this,"error","shellscript can not be null");
        return;
    }
    QString inputMrc=QString(CProManage::GetInstance()->GetDir(PROCESS_DIR).c_str())+"_fin";
    QString inputTilt=QString(CProManage::GetInstance()->GetDir(PROCESS_DIR).c_str())+"_new";     

    shell.replace("$(InputMrcFile)",inputMrc).replace("$(InputTiltFile)",inputTilt).replace("$(result)",inputMrc+"temp");

    
    scriptFileName = QString(CProManage::GetInstance()->GetDir(PROCESS_DIR).data())+"_CTFCorrect.sh";
    ofstream out(scriptFileName.toStdString().c_str());
    out.write(shell.toStdString().c_str(),shell.toStdString().length());
    out.close();
    
    qDebug()<<"instruction : "<<scriptFileName;
    msg_map::Message* msg = MMTApp::NewMessage(CTF_MSG , this , scriptFileName.toStdString().c_str());
    MMTApp::PostMessage(msg);
    m_stacklayout->setCurrentWidget(m_progressbar);
}
bool CTFSettingsDialog::isEmpty(QLineEdit* lineEdit)
{
    QString str = lineEdit->text();
    static int index=0;
    index++;
    printf("lineedit %d is %s \n",index,str.toStdString().data());
    return str=="";
    
}
bool CTFSettingsDialog::hasEmpty()
{
    bool state=
            isEmpty(m_defocusDifference_lineEdit)||    
            isEmpty(m_XMAG_lineEdit)||    
            isEmpty(m_DStep_lineEdit)||    
            isEmpty(m_tileSize_lineEdit)||
            isEmpty(m_CS_lineEdit)||
            isEmpty(m_HT_lineEdit)||
            isEmpty(m_AmpCnst_lineEdit)||
            isEmpty(m_CTFMin_lineEdit)||
            isEmpty(m_ResMin_lineEdit)||
            isEmpty(m_ResMax_lineEdit)||
            isEmpty(m_dFMin_lineEdit)||
            isEmpty(m_dFMax_lineEdit)||
            isEmpty(m_FStep_lineEdit)||
            isEmpty(m_defocus_lineEdit)||
            isEmpty(m_processNumber_lineEdit);
    
    if(m_amplitudeCorrection_comboBox->currentIndex()==0 && (isEmpty(m_filter1_lineEdit)||isEmpty(m_filter2_lineEdit))){
        state=true;
    }
    
    return state;
    
    
}
void CTFSettingsDialog::printSettigns()
{
    printf("defocusDifference is %d\n",settings->defocusDifference);
    printf("XMAG is %d\n",settings->XMAG);
    printf("DStep is %d\n",settings->DStep);
    printf("tileSize is %d\n",settings->tileSize);
    printf("CS is %lf\n",settings->CS);
    printf("HT is %lf\n",settings->HT);
    printf("AmpCnst is %lf\n",settings->AmpCnst);
    printf("CTFMin is %lf\n",settings->CTFMin);
    printf("ResMin is %lf\n",settings->ResMin);
    printf("ResMax is %lf\n",settings->ResMax);
    printf("dFMin is %lf\n",settings->dFMin);
    printf("dFMax is %lf\n",settings->dFMax);
    printf("FStep is %lf\n",settings->FStep);
    printf("defocus is %d\n",settings->defocus);  //Defocus at the untilted plane
    printf("amplitudeCorrection is %d\n",(int)settings->amplitudeCorrection);
    printf("filter1 is %lf\n",settings->filter1);//Wiener filter parameters: w1, w2
    printf("filter2 is %lf\n",settings->filter2);    
    printf("processNumber is %d\n",settings->processNumber);
        
}

bool CTFSettingsDialog::readSettings()
{   
    //testSetting();
    if(!hasEmpty()){
        settings->defocusDifference = m_defocusDifference_lineEdit ->text().toInt();    
        settings->XMAG = m_XMAG_lineEdit ->text().toInt();    
        settings->DStep = m_DStep_lineEdit ->text().toInt();    
        settings->tileSize = m_tileSize_lineEdit ->text().toInt();
        settings->CS = m_CS_lineEdit ->text().toDouble();
        settings->HT = m_HT_lineEdit ->text().toDouble();
        settings->AmpCnst = m_AmpCnst_lineEdit ->text().toDouble();
        settings->CTFMin = m_CTFMin_lineEdit ->text().toDouble();
        settings->ResMin = m_ResMin_lineEdit ->text().toDouble();
        settings->ResMax = m_ResMax_lineEdit ->text().toDouble();
        settings->dFMin = m_dFMin_lineEdit ->text().toDouble();
        settings->dFMax = m_dFMax_lineEdit ->text().toDouble();
        settings->FStep = m_FStep_lineEdit ->text().toDouble();
        settings->defocus = m_defocus_lineEdit ->text().toInt();
        settings->amplitudeCorrection=(m_amplitudeCorrection_comboBox->currentIndex()==0);
        settings->processNumber = m_processNumber_lineEdit->text().toInt();
        if(settings->amplitudeCorrection){
            settings->filter1 = m_filter1_lineEdit ->text().toDouble();
            settings->filter2 = m_filter2_lineEdit ->text().toDouble();    
        }
        printSettigns();
        return true;
        
    }
    return false;
    
}
void CTFSettingsDialog::printMessage(QString message)
{
    m_output_textEdit->moveCursor(QTextCursor::End);
    m_output_textEdit->textCursor().insertText(message);
}


void CTFSettingsDialog::customEvent(QEvent* e){
    
        msg_map::CSequelEvent* sequel = static_cast<msg_map::CSequelEvent*>(e);
        if(sequel->msg_id == CTF_MSG) {
            switch(sequel->state) {
            case msg_map::MESSAGE: {
                QString message = (char *)sequel->data;
                emit Log(message);
                delete [] (char*)sequel->data;
            }
            break;
            case msg_map::RATE: {
            }
            break;
            case msg_map::RESULT: {
                int* state =static_cast<int*>(sequel->data);
                if(state == PTHREAD_CANCELED) {
                    

                    emit RunError("Canceled.\n");
                }
                else if(*state == 0 ) {
                    //the child process return zero
                    qDebug()<<"1";
                    QString result=QString(CProManage::GetInstance()->GetDir(PROCESS_DIR).c_str())+"_fin";
                    
                    QFileInfo file(result+"temp.mrc");
                    if(file.exists()) {
                        QString cmd="cp "+result+"temp.mrc "+result+".mrc";
                        system(cmd.toStdString().c_str());
                        emit Finish("Success.\n");
                    }else{
                        emit RunError("Error.\n");

                    }
            
                    delete state;
                } else {
                    //fail
                    emit RunError("Error.\n");

                    delete state;
                }
                m_stacklayout->setCurrentWidget(m_settigns_frame);

            }
            break;
            }
        }
}

void CTFSettingsDialog::testSetting()
{
    m_defocusDifference_lineEdit ->setText("2000");
    m_XMAG_lineEdit ->setText("10000");
    m_DStep_lineEdit ->setText("10");
    m_tileSize_lineEdit ->setText("128");
    m_CS_lineEdit ->setText("2.7");
    m_HT_lineEdit ->setText("300");
    m_AmpCnst_lineEdit ->setText("0.07");
    m_CTFMin_lineEdit ->setText("0.5");
    m_ResMin_lineEdit ->setText("10000");
    m_ResMax_lineEdit ->setText("22");
    m_dFMin_lineEdit ->setText("100000");
    m_dFMax_lineEdit ->setText("300000");
    m_FStep_lineEdit ->setText("1000");
    m_defocus_lineEdit ->setText("266500");
    m_amplitudeCorrection_comboBox ->setCurrentIndex(0);
    m_filter1_lineEdit ->setText("0.5");
    m_filter2_lineEdit ->setText("0.25");
    m_processNumber_lineEdit ->setText("24");

}


#include "CTFSettingsDialog.moc"
#include <QFileInfo>
#include <QFileInfo>



