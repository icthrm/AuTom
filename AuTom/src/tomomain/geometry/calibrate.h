#ifndef CALIBRATE_H__
#define CALIBRATE_H__

/** @c ly : relate to height, 0~1, size-3 @c lz : relate to thickness0, 0~1, size-12 */
void Calibrate(const float ly[], const float lz[], int thickness0, int width, int height, 
	       float* z_shift, float* offset, float* thickness, float* pitch_angle);

static float line_point_y(float x1, float y1, float x2, float y2, float newx);

static float tilt_offset(float x1, float y1, float x2, float y2);		//!!

static float ls_pitch_angle(float x1,float y1,float x2,float y2,float x3,float y3);

#endif
