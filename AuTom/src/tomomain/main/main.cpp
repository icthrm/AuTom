#include <QtWidgets/QApplication>
#include <QTextCodec>
#include "MainWindow.h"
#include <QtGui>
#include "core/msg_map.h"
#include "custom/custom.h"
#include "profunc/profunc.h"
#include "init/Cinit.h"
#include "widget/CSetWorkSpace.h"


BEGIN_MESSAGE_MAP()
    REG_MSG_FUNC(TEST_MSG, profunc::OnTestMsg, DEFAULT_FBF, msg_map::SYNC)
    REG_MSG_FUNC(BUILD_MODEL_MSG, profunc::OnBuildModelMsg, DEFAULT_FBF, msg_map::SYNC)
    REG_MSG_FUNC(Hat_PIXEL_MSG, profunc::OnHatPixel, DEFAULT_FBF, msg_map::SYNC)
    REG_MSG_FUNC(COARSE_ALIGN_MSG , profunc::OnCoarseAlign , DEFAULT_FBF , msg_map::SYNC)
    REG_MSG_FUNC(FINE_ALIGN_MSG , profunc::OnFineAlign , DEFAULT_FBF , msg_map::SYNC)
    REG_MSG_FUNC(RESTRUCT_MSG , profunc::OnRestruct , DEFAULT_FBF , msg_map::SYNC)
    REG_MSG_FUNC(H_NORMALIZE_MSG , profunc::OnNormalize , DEFAULT_FBF , msg_map::SYNC)
    REG_MSG_FUNC(RAPTOR_MSG, profunc::OnRAPTOR , DEFAULT_FBF , msg_map::SYNC)
    REG_MSG_FUNC(EVALUATE_MSG , profunc::OnEvaluate , DEFAULT_FBF , msg_map::SYNC)
    REG_MSG_FUNC(EVALUATE_NCC_MSG , profunc::OnEvaluateNCC , DEFAULT_FBF , msg_map::SYNC)
    REG_MSG_FUNC(POSTPROCESS_MSG , profunc::OnPostProcess , DEFAULT_FBF , msg_map::SYNC)
    REG_MSG_FUNC(ROTATE_MRC_MSG , profunc::OnRotateMrc , DEFAULT_FBF , msg_map::SYNC)
    REG_MSG_FUNC(ALIGN_MSG , profunc::OnAlign , DEFAULT_FBF , msg_map::SYNC)
    REG_MSG_FUNC(EVALUATE_TRAJ_MSG,profunc::onEvaluateTraj,DEFAULT_FBF,msg_map::SYNC)
    REG_MSG_FUNC(CTF_MSG,profunc::onCTFCorrection,DEFAULT_FBF,msg_map::SYNC)
    REG_MSG_FUNC(IMOD_MSG , profunc::onImodOpen , DEFAULT_FBF , msg_map::SYNC)
    REG_MSG_FUNC(AUTOGDETERM_MSG, profunc::OnGeoDeterm, DEFAULT_FBF, msg_map::SYNC)
END_MESSAGE_MAP()

int main(int argc, char** argv)
{
    Q_INIT_RESOURCE(resource);
    
    QApplication app(argc, argv);    
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    
    //设置风格
    QApplication::setStyle(QStyleFactory::create("gtk"));
    QApplication::setPalette(QApplication::style()->standardPalette());
    
    CProManage::InitInstance();
    MMTApp::SetMMTDefaultASyncFunc(msg_map::QtASyncRecovery);
    MMTApp::GetMMT().Run();
     
    Mainwindow foo;
     //foo.setWindowTitle("AuTom");
    if (!Cinit::InitSet()){ 
      foo.emitSet();
      foo.showMaximized();

	//foo.ShowSetWorkSpaceWin();
	
    }
    else{
	foo.showMaximized(); 
	foo.showLocation();
    }
    //qApp->setStyle(new QMotifStyle);
//    MMTApp::GetMMT().Stop();
    return app.exec();
}
