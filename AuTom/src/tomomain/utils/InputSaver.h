/*
 * <chice> <somec001@outlook.com>
 * The class is for saving the inputs that users input into Autom.
 * Besides, it provides the default or recommanded value for init the program.
 * 
 */

#ifndef INPUTSAVER_H
#define INPUTSAVER_H

#include "SysInfo.h"
#include <QtGui>
#include "promanage/ProManage.h"
#include <QDataStream>
#include <QObject>

class InputSaver : public QObject
{
public:

static InputSaver * getInstance();
InputSaver();
~InputSaver();

void serialize();
InputSaver & deSerialize();

static const int preprocessing_normtype_Combobox_default;
static const QString preprocessing_avg_LineEdit_default;
static const QString preprocessing_std_LineEdit_default;

static const Qt::CheckState alignment_checkbox_default;
static const QString alignment_AutoLineEdit_default;
static const Qt::CheckState alignment_fast_checkbox_default;

static const Qt::CheckState coarseAlignment_checkbox_default;
static const int coarseAlignment_spinBox_default;

static const Qt::CheckState geometry_input_checkbox_default;
static const QString geometry_thickness_default;
static const QString geometry_zshift_default;
static const QString geometry_offset_default;
static const QString geometry_pitch_default;

static const int reconsruction_startModel_Combobox_default;
static const int reconsruction_version_Combobox_default;
static const int reconsruction_alogrithm_Combobox_default;
static const QString reconsruction_threadNum_LineEdit_default;
static const QString reconsruction_ParameterLineEdit_default;
static const QString reconsruction_IterativenumbersLineEdit_default;
static const int reconsruction_Method_ComboBox_default;
static const QString reconsruction_SliceLineEdit1_default;
static const QString reconsruction_SliceLineEdit2_default;
static const QString reconsruction_ICONIterLineEdit1_default;
static const QString reconsruction_ICONIterLineEdit2_default;
static const QString reconsruction_ICONIterLineEdit3_default;
static const QString reconsruction_ThresholdLineEdit_default;
static const QString reconsruction_radiusLineEdit_default;
static const int reconsruction_dataTypeBox_default;
static const QString reconsruction_bin_lineEdit_default;
static const QString reconsruction_Mrcfile_Lineedit_default;
static const Qt::CheckState reconsruction_ctf_enable_checkbox_default;

//=======================inputs save==================================
static int preprocessing_normtype_Combobox_saved;
static QString preprocessing_avg_LineEdit_saved;
static QString preprocessing_std_LineEdit_saved;

static Qt::CheckState alignment_checkbox_saved;
static QString alignment_AutoLineEdit_saved;
static Qt::CheckState alignment_fast_checkbox_saved;

static Qt::CheckState coarseAlignment_checkbox_saved;
static int coarseAlignment_spinBox_saved;

static Qt::CheckState geometry_input_checkbox_saved;
static QString geometry_thickness_saved;
static QString geometry_zshift_saved;
static QString geometry_offset_saved;
static QString geometry_pitch_saved;

static int reconsruction_startModel_Combobox_saved;
static int reconsruction_version_Combobox_saved;
static int reconsruction_alogrithm_Combobox_saved;
static QString reconsruction_threadNum_LineEdit_saved;
static QString reconsruction_ParameterLineEdit_saved;
static QString reconsruction_IterativenumbersLineEdit_saved;
static int reconsruction_Method_ComboBox_saved;
static QString reconsruction_SliceLineEdit1_saved;
static QString reconsruction_SliceLineEdit2_saved;
static QString reconsruction_ICONIterLineEdit1_saved;
static QString reconsruction_ICONIterLineEdit2_saved;
static QString reconsruction_ICONIterLineEdit3_saved;
static QString reconsruction_ThresholdLineEdit_saved;
static QString reconsruction_radiusLineEdit_saved;
static int reconsruction_dataTypeBox_saved;
static QString reconsruction_bin_lineEdit_saved;
static QString reconsruction_Mrcfile_Lineedit_saved;
static Qt::CheckState reconsruction_ctf_enable_checkbox_saved;
/*
*/
private:
static const QString savePath;
static InputSaver * instance;


friend QDataStream & operator>>(QDataStream & in, InputSaver & data);
friend QDataStream & operator<<(QDataStream & out, const InputSaver & data);
static QString calReconsruction_threadNum_LineEdit_default();
static QString calReconsruction_Mrcfile_Lineedit_default();


};

#endif // INPUTSAVER_H
