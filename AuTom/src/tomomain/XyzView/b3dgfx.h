#ifndef B3DGFX_H
#define B3DGFX_H

#include <qgl.h>
#include <QtGui>
#include <GL/gl.h>
#include <GL/glu.h>
// #include <GL/glut.h>

typedef struct b3d_ci_image
{
     unsigned short *id1;
     unsigned short *id2;
     short width, height;
     short buf, bufSize;

     short dw1, dw2, dh1, dh2;
     short xo1, xo2, yo1, yo2;
     double zx1, zx2, zy1, zy2;
     short  hq1, hq2;
     short cz1, cz2;

}B3dCIImage;

void b3dDrawPoint(int x, int y);
void b3dDrawPlus(int x, int y, int size);
void b3dDrawCross(int x, int y, int size);
void b3dDrawStar(int x, int y, int size);
void b3dDrawTriangle(int x, int y, int size);
void b3dDrawFilledTriangle(int x, int y, int size);
void b3dDrawCircle(int x, int y, int radius);
void b3dDrawFilledCircle(int x, int y, int radius);
void b3dDrawSquare(int x, int y, int size);
void b3dDrawFilledSquare(int x, int y, int size);
void b3dDrawRectangle(int x, int y, int width, int height);
void b3dDrawFilledRectangle(int x, int y, int width, int height);

void b3dDrawLine(int x1, int y1, int x2, int y2);

void b3dColorIndex(int pix);
void b3dFlushImage(B3dCIImage *image);

void b3dSubareaViewport(int xstart, int ystart, int xsize, int ysize);

/*
 *   Some utility functions.
 */
//   double b3dStepPixelZoom(double czoom, int step);
void b3dSetImageOffset(int winsize,     /* window size         */
                       int imsize,      /* image size          */
                       double zoom,     /* zoom factor.        */
                       int &drawsize,   /* size drawn          */
                       int &offset,     /* offset from center. */
                       int &woff,       /* window offset.      */
                       int &doff,       /* data offset in ipixels           */
                       int fillEdge);   /* Fill window to edge, maybe beyond*/

int b3dGetImageType(GLenum *otype, GLenum *oformat);

/*
 *   Pixel drawing.
 */
void b3dDrawGreyScalePixelsHQ
(unsigned char **data, int xsize, int ysize, 
 int xoffset, int yoffset,
 int wx, int wy, 
 int width, int height, 
 B3dCIImage *image, int base, 
 double xzoom, double yzoom, int quality, int slice, int rgba);

#endif