#include "Ctag.h"
namespace ToolTips{
const char *  path="Projects will be built under this directory.";
//new
const char *  new_lbl_mrc="Select the MRC file.";
const char *  new_lbl_angle="Select rawtlt file containing tilt angles.";
const char *  new_lbl_proname="Project name must be distinct from others.";
//select workflow
const char *  sel_btn_marker="The marker-based alignment can automatically\ntrack the fiducial markers in any initial state.";
const char *  sel_btn_free="The marker-free alignment implements an optimization\nfor local structures, thus requires coarse alignment.";
//prepro
const char *  pre_edt_header= "Show the rotation angle(in degrees) from Y axis to present tilt axis.";  
const char *  pre_comb_type= "choose a method in the combobox."; 

const char *  pre_edt_avg="Enter the average.";
const char *  pre_edt_dev="Enter the standard deviation.";
const char *  per_btn_norm="Normalize pixel values.";
//align
const char *  ali_lbl_erase="Erase the fiducial markers in the micrographs if you need.";
const char *  ali_edt_r="Enter the diameter(in pixel) of fiducial markers.\nEnter '-1' to estimate the diameter automatically if you don't know.";
const char *  ali_btn_start="Align the tilt series based on the information of fiducial markers.";
const char *  ali_ckb_fast="Fast mode can make alignment faster in fiducial marker tracking. However, it maybe not so robust.";
//calign falign
const char *  cali_lbl_rot="Correct the azimuth or in-plane rotation of micrographs.";
const char *  cali_lbl_iter="Iteration number.";
const char *  cali_btn_start="Correct the shift in X axis and Y axis based on cross-correlation";
const char *  fali_btn_start="Align the tilt series by scale invariant features.";
//geo
const char *  geoi_btn_thick="Set the sample tomogram thickness larger than expected thickness.";
const char *  geo_btn_buildsec="Select 3 separate sections from tomogram\nand draw boundary lines in each section.";
const char *  geo_btn_calcu="Calculate four geometric parameters of a volume model:\nTHICKNESS, ZSHIFT, OFFSET, PITCH ANGLE. THICKNESS is the\nthickness of the sample. ZSHIFT is the distance of the tilt axis\nto the center of the sample in z-direction. OFFSET is the starting\ntilt angle around Y axis.PITCH ANGLE is the tilt angle(in degrees)\naround the microscope X axis.";

//rec
const char *  rec_lbl_model="Model for single axis data. Dual-axis is under development.";
const char *  rec_lbl_version="choose one of accelerated versions of reconstruction methods";
const char *  rec_lbl_algo="Select reconstruction algorithm.ICON Provides\nthe compressed-sensing based reconstruction method";
const char *  rec_lbl_thnum="# of threads";
const char *  rec_btn_start="Obtain the 3D volumes of the specimen from aligned stack";
//fbp
const char *  rec_lbl_method="choose filter method of FBP";
//sart sirt
const char *  rec_lbl_para="The relaxation factor.";
const char *  rec_lbl_iter="Iteration number.";
const char *  rec_lbl_file=" The reconstructed file of the structure.";
//icon Provides the compressed-sensing based reconstruction method.
const char *  rec_lbl_slice="The reconstructed slices range from slice_a to slice_b.";
const char *  rec_lbl_thresh=" the threshold used in ICON.";
const char *  rec_lbl_iteration="the iteration number including 3 parts. \nFor example, 5,50,10 means that, firstly,\nreconstruct using INFR for 5 iterations to\ngenerate a stable initial value, and then\nreconstruct using ICON for 50 iterations, \nand finally reconstruct using INFR for 10";
const char *  rec_lbl_rad=" The mask radius (in pixel) used in the Fourier domain of the combined 3D reconstruction.";
const char *  rec_lbl_datatype="The type of dataset.\n1 for cryoET or plastic embedded ET (signal in black and background in white);\n2 for negatively stained ET (signal in white and background in black).";

}
