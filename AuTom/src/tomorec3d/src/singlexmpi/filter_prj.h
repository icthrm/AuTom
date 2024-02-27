#ifndef FILTER_PRJ_H
#define FILTER_PRJ_H

#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>

#include "atommpi.h"

int filter_prj_sym(float *prj_real, const char *filter, int filtlength, Projection prj, int row_pad, int col_pad, int symmetrize_2D_flag, int angle, int id);

//int filter_prj_X_sym(float *prj_real, const char *filter, int filtlength, Projection prj, int row_pad, int col_pad, int symmetrize_2D_flag, int angle, int id);

#endif
