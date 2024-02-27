#include "InputSaver.h"

const int InputSaver::preprocessing_normtype_Combobox_default = 1;
const QString InputSaver::preprocessing_avg_LineEdit_default = "1";
const QString InputSaver::preprocessing_std_LineEdit_default = "3";

const Qt::CheckState InputSaver::alignment_checkbox_default = Qt::Unchecked;
const QString InputSaver::alignment_AutoLineEdit_default = "-1";
const Qt::CheckState InputSaver::alignment_fast_checkbox_default = Qt::Unchecked;

const Qt::CheckState InputSaver::coarseAlignment_checkbox_default = Qt::Unchecked;
const int InputSaver::coarseAlignment_spinBox_default = 1;

const Qt::CheckState InputSaver::geometry_input_checkbox_default = Qt::Unchecked;
const QString InputSaver::geometry_thickness_default = "400.0";
const QString InputSaver::geometry_zshift_default = "0.0";
const QString InputSaver::geometry_offset_default = "0.0";
const QString InputSaver::geometry_pitch_default = "0.0";

const int InputSaver::reconsruction_startModel_Combobox_default = 0;
const int InputSaver::reconsruction_version_Combobox_default = 0;
const int InputSaver::reconsruction_alogrithm_Combobox_default = 1;
const QString InputSaver::reconsruction_threadNum_LineEdit_default = calReconsruction_threadNum_LineEdit_default();
const QString InputSaver::reconsruction_ParameterLineEdit_default = "0.2";
const QString InputSaver::reconsruction_IterativenumbersLineEdit_default = "10";
const int InputSaver::reconsruction_Method_ComboBox_default = 0;
const QString InputSaver::reconsruction_SliceLineEdit1_default = "0";
const QString InputSaver::reconsruction_SliceLineEdit2_default = "";
const QString InputSaver::reconsruction_ICONIterLineEdit1_default = "5";
const QString InputSaver::reconsruction_ICONIterLineEdit2_default = "50";
const QString InputSaver::reconsruction_ICONIterLineEdit3_default = "10";
const QString InputSaver::reconsruction_ThresholdLineEdit_default = "0";
const QString InputSaver::reconsruction_radiusLineEdit_default = "-1";
const int InputSaver::reconsruction_dataTypeBox_default = 0;
const QString InputSaver::reconsruction_bin_lineEdit_default = "1";
const QString InputSaver::reconsruction_Mrcfile_Lineedit_default = "";// calReconsruction_Mrcfile_Lineedit_default();
const Qt::CheckState InputSaver::reconsruction_ctf_enable_checkbox_default = Qt::Unchecked;
/*
*/

const QString InputSaver::savePath = "inputs.save";
InputSaver * InputSaver::instance = NULL;

int InputSaver::preprocessing_normtype_Combobox_saved = InputSaver::preprocessing_normtype_Combobox_default;
QString InputSaver::preprocessing_avg_LineEdit_saved = preprocessing_avg_LineEdit_default;
QString InputSaver::preprocessing_std_LineEdit_saved = preprocessing_std_LineEdit_default;

Qt::CheckState InputSaver::alignment_checkbox_saved = alignment_checkbox_default;
QString InputSaver::alignment_AutoLineEdit_saved = alignment_AutoLineEdit_default;
Qt::CheckState InputSaver::alignment_fast_checkbox_saved = alignment_fast_checkbox_default;

Qt::CheckState InputSaver::coarseAlignment_checkbox_saved = coarseAlignment_checkbox_default;
int InputSaver::coarseAlignment_spinBox_saved = coarseAlignment_spinBox_default;

Qt::CheckState InputSaver::geometry_input_checkbox_saved = geometry_input_checkbox_default;
QString InputSaver::geometry_thickness_saved = geometry_thickness_default;
QString InputSaver::geometry_zshift_saved = geometry_zshift_default;
QString InputSaver::geometry_offset_saved = geometry_offset_default;
QString InputSaver::geometry_pitch_saved = geometry_pitch_default;

int InputSaver::reconsruction_startModel_Combobox_saved = reconsruction_startModel_Combobox_default;
int InputSaver::reconsruction_version_Combobox_saved = reconsruction_version_Combobox_default;
int InputSaver::reconsruction_alogrithm_Combobox_saved = reconsruction_alogrithm_Combobox_default;
QString InputSaver::reconsruction_threadNum_LineEdit_saved = reconsruction_threadNum_LineEdit_default;
QString InputSaver::reconsruction_ParameterLineEdit_saved = reconsruction_ParameterLineEdit_default;
QString InputSaver::reconsruction_IterativenumbersLineEdit_saved = reconsruction_IterativenumbersLineEdit_default;
int InputSaver::reconsruction_Method_ComboBox_saved = reconsruction_Method_ComboBox_default;
QString InputSaver::reconsruction_SliceLineEdit1_saved = reconsruction_SliceLineEdit1_default;
QString InputSaver::reconsruction_SliceLineEdit2_saved = reconsruction_SliceLineEdit2_default;
QString InputSaver::reconsruction_ICONIterLineEdit1_saved = reconsruction_ICONIterLineEdit1_default;
QString InputSaver::reconsruction_ICONIterLineEdit2_saved = reconsruction_ICONIterLineEdit2_default;
QString InputSaver::reconsruction_ICONIterLineEdit3_saved = reconsruction_ICONIterLineEdit3_default;
QString InputSaver::reconsruction_ThresholdLineEdit_saved = reconsruction_ThresholdLineEdit_default;
QString InputSaver::reconsruction_radiusLineEdit_saved = reconsruction_radiusLineEdit_default;
int InputSaver::reconsruction_dataTypeBox_saved = reconsruction_dataTypeBox_default;
QString InputSaver::reconsruction_bin_lineEdit_saved = reconsruction_bin_lineEdit_default;
QString InputSaver::reconsruction_Mrcfile_Lineedit_saved = reconsruction_Mrcfile_Lineedit_default;
Qt::CheckState InputSaver::reconsruction_ctf_enable_checkbox_saved = reconsruction_ctf_enable_checkbox_default;

InputSaver::InputSaver()
{
  deSerialize();
}

InputSaver::~InputSaver()
{
  serialize();
}

InputSaver * InputSaver::getInstance(){
  if(NULL == instance){
    instance = new InputSaver();
  }
  return instance;
}

QDataStream & operator>>(QDataStream & in, Qt::CheckState & checkState){
  int temp;
  in >> temp;
  switch(temp){
    case 0:
      checkState = Qt::Checked;
      break;
    case 1:
      checkState = Qt::Unchecked;
      break;
    case 2:
      checkState = Qt::PartiallyChecked;
      break;
  }
  return in;
}

QDataStream & operator<<(QDataStream & out, const Qt::CheckState & checkState){
  int temp;
  switch(checkState){
    case Qt::Checked:
      temp = 0;
      break;
    case Qt::Unchecked:
      temp = 1;
      break;
    case Qt::PartiallyChecked:
      temp = 2;
      break;
  }
  out << temp;
  return out;
}

QDataStream & operator>>(QDataStream & in, InputSaver & data){
  in >> data.preprocessing_normtype_Combobox_saved
     >> data.preprocessing_avg_LineEdit_saved
     >> data.preprocessing_std_LineEdit_saved
     >> data.alignment_checkbox_saved
     >> data.alignment_AutoLineEdit_saved
     >> data.alignment_fast_checkbox_saved
     >> data.coarseAlignment_checkbox_saved
     >> data.coarseAlignment_spinBox_saved
     >> data.geometry_input_checkbox_saved
     >> data.geometry_thickness_saved
     >> data.geometry_zshift_saved
     >> data.geometry_offset_saved
     >> data.geometry_pitch_saved
     >> data.reconsruction_startModel_Combobox_saved
     >> data.reconsruction_version_Combobox_saved
     >> data.reconsruction_alogrithm_Combobox_saved
     >> data.reconsruction_threadNum_LineEdit_saved
     >> data.reconsruction_ParameterLineEdit_saved
     >> data.reconsruction_IterativenumbersLineEdit_saved
     >> data.reconsruction_Method_ComboBox_saved
     >> data.reconsruction_SliceLineEdit1_saved
     >> data.reconsruction_SliceLineEdit2_saved
     >> data.reconsruction_ICONIterLineEdit1_saved
     >> data.reconsruction_ICONIterLineEdit2_saved
     >> data.reconsruction_ICONIterLineEdit3_saved
     >> data.reconsruction_ThresholdLineEdit_saved
     >> data.reconsruction_radiusLineEdit_saved
     >> data.reconsruction_dataTypeBox_saved
     >> data.reconsruction_bin_lineEdit_saved
     >> data.reconsruction_Mrcfile_Lineedit_saved
     >> data.reconsruction_ctf_enable_checkbox_saved
     ;
  return in;
}

QDataStream & operator<<(QDataStream & out, const InputSaver & data){
  out << data.preprocessing_normtype_Combobox_saved
      << data.preprocessing_avg_LineEdit_saved
      << data.preprocessing_std_LineEdit_saved
      << data.alignment_checkbox_saved
      << data.alignment_AutoLineEdit_saved
      << data.alignment_fast_checkbox_saved
      << data.coarseAlignment_checkbox_saved
      << data.coarseAlignment_spinBox_saved
      << data.geometry_input_checkbox_saved
      << data.geometry_thickness_saved
      << data.geometry_zshift_saved
      << data.geometry_offset_saved
      << data.geometry_pitch_saved
      << data.reconsruction_startModel_Combobox_saved
      << data.reconsruction_version_Combobox_saved
      << data.reconsruction_alogrithm_Combobox_saved
      << data.reconsruction_threadNum_LineEdit_saved
      << data.reconsruction_ParameterLineEdit_saved
      << data.reconsruction_IterativenumbersLineEdit_saved
      << data.reconsruction_Method_ComboBox_saved
      << data.reconsruction_SliceLineEdit1_saved
      << data.reconsruction_SliceLineEdit2_saved
      << data.reconsruction_ICONIterLineEdit1_saved
      << data.reconsruction_ICONIterLineEdit2_saved
      << data.reconsruction_ICONIterLineEdit3_saved
      << data.reconsruction_ThresholdLineEdit_saved
      << data.reconsruction_radiusLineEdit_saved
      << data.reconsruction_dataTypeBox_saved
      << data.reconsruction_bin_lineEdit_saved
      << data.reconsruction_Mrcfile_Lineedit_saved
      << data.reconsruction_ctf_enable_checkbox_saved
      ;
  return out;
}

void InputSaver::serialize(){
  //inputSaver.preprocessing_normtype_Combobox_saved;
  QFile file(savePath);
  if(!file.open(QIODevice::WriteOnly)) return;
  
  QDataStream out(&file);
  out << *this;
  std::cout<<"save inputs sucessfully!"<<std::endl;
  file.close();
}

InputSaver & InputSaver::deSerialize(){
  
  QFile file(savePath);
  
  if(!file.open(QIODevice::ReadOnly)) return *this;
  
  QDataStream in(&file);
  //InputSaver * inputSaver = getInstance();
  in >> *this;
  file.close();
  return *this;
}

QString InputSaver::calReconsruction_threadNum_LineEdit_default(){
  return QString::number(SysInfo::getLCpuNum());
}

QString InputSaver::calReconsruction_Mrcfile_Lineedit_default(){
  std::string filename=CProManage::GetInstance()->GetDir(PROCESS_DIR)+"_fbp.mrc";
  fstream pfile;
  pfile.open(filename.c_str(), ios_base::in);
  if(pfile.is_open()) {
      return QString::fromStdString(filename);
  }
  return "";
}/**/