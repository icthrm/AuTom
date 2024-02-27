#include "b3dgfx.h"
#include <GL/glu.h>

/***************************************************************/
/* 2D Drawing Functions mapped into window, no transformations */
/* The origin is at the lower-left corner of the window.       */
/* Use OpenGL functions for 3D drawing.                        */

static bool    sStippleNextLine = false;

void b3dDrawPoint(int x, int y)
{
  glBegin(GL_POINTS);
  glVertex2i(x,y);
  glEnd();

  return;
}

void b3dDrawCross(int x, int y, int size)
{
  glBegin(GL_LINES);
  glVertex2i(x - size, y - size);
  glVertex2i(x + size, y + size);
  glEnd();
  glBegin(GL_LINES);
  glVertex2i(x + size, y - size);
  glVertex2i(x - size, y + size);
  glEnd();

  return;
}

void b3dFlushImage(B3dCIImage *image)
{
  B3dCIImage *ri = image;
  if (!image) return;
  ri->dw1 = ri->dw2 = ri->dh1 = ri->dh2 = 0;
  ri->xo1 = ri->xo2 = ri->yo1 = ri->yo2 = -1;
  ri->zx1 = ri->zx2 = ri->zy1 = ri->zy2 = 0.0;
  ri->hq1 = ri->hq2 = -1;
  ri->cz1 = ri->cz2 = -1;
  return;
}

void b3dDrawPlus(int x, int y, int size)
{
  glBegin(GL_LINES);
  glVertex2i(x - size, y);
  glVertex2i(x + size, y);
  glEnd();
  glBegin(GL_LINES);
  glVertex2i(x, y - size);
  glVertex2i(x, y + size);
  glEnd();
  return;
}

void b3dDrawStar(int x, int y, int size)
{ 
  b3dDrawPlus(x,y,size);
  b3dDrawCross(x,y,size);
  return;
}

void b3dDrawTriangle(int x, int y, int size)
{
  glBegin(GL_LINE_LOOP);
  glVertex2i(x, y + size);
  glVertex2i(x + size, y - (size/2));
  glVertex2i(x - size, y - (size/2));
  glEnd();
  return;
}

void b3dDrawFilledTriangle(int x, int y, int size)
{
  glBegin(GL_POLYGON);
  glVertex2i(x, y + size);
  glVertex2i(x + size, y - (size/2));
  glVertex2i(x - size, y - (size/2));
  glVertex2i(x, y + size);
  glEnd();
  return;
}

void b3dDrawCircle(int x, int y, int radius)
{
  int linewidth = 1;
  GLdouble inrad;
  GLdouble dradius = radius;
  if (radius <= 0)
    return;

#ifdef GLU_QUADRIC_HACK
  GLUquadricObj *qobj = gluNewQuadric();
#else
  static GLUquadricObj *qobj = NULL;
  if (!qobj)
    qobj = gluNewQuadric();
#endif

  /* DNM: by request, make circles have current thickness, which is similar
     to what triangles and squares do */
  /*     glLineWidth(1.0f); */
  glPushMatrix();
  glTranslatef((float)x, (float)y, 0.0f);
  glGetIntegerv(GL_LINE_WIDTH, (GLint *)&linewidth);
  inrad = radius - linewidth;
  if (inrad < 0.)
    inrad = 0;
  gluDisk(qobj, inrad, dradius, radius+4, 2);
  glPopMatrix();
#ifdef GLU_QUADRIC_HACK
  gluDeleteQuadric(qobj);
#endif

  return;
}

void b3dDrawFilledCircle(int x, int y, int radius)
{
  if (radius <= 0)
    return;

#ifdef GLU_QUADRIC_HACK
  GLUquadricObj *qobj = gluNewQuadric();
#else
  static GLUquadricObj *qobj = NULL;
  if (!qobj)
    qobj = gluNewQuadric();
#endif

  glPushMatrix();
  glLineWidth(1.0f);
  glTranslatef((float)x, (float)y, 0.0f);
  gluDisk(qobj, 0, radius, radius+4, 1);
  glPopMatrix();
#ifdef GLU_QUADRIC_HACK
  gluDeleteQuadric(qobj);
#endif
  return;
}

void b3dDrawLine(int x1, int y1, int x2, int y2)
{
//   if (sStippleNextLine)
    glEnable(GL_LINE_STIPPLE);
  glBegin(GL_LINES);
  glVertex2i(x1, y1);
  glVertex2i(x2, y2);
  glEnd();
//   if (sStippleNextLine)
//     glDisable(GL_LINE_STIPPLE);
//   sStippleNextLine = false;
}

void b3dDrawSquare(int x, int y, int size)
{
  b3dDrawRectangle(x-(size/2), y-(size/2), size, size);
  return;
}

void b3dDrawRectangle(int x, int y, int width, int height)
{
  glBegin(GL_LINE_STRIP);
  glVertex2i(x,y);
  glVertex2i(x+width,y);
  glVertex2i(x+width,y+height);
  glVertex2i(x,y+height);
  glVertex2i(x,y);
  glEnd();
  return;
}
void b3dDrawFilledSquare(int x, int y, int size)
{
  b3dDrawFilledRectangle(x-(size/2), y-(size/2), size, size);
  return;
}
void b3dDrawFilledRectangle(int x, int y, int width, int height)
{
  glBegin(GL_POLYGON);
  glVertex2i(x,y);
  glVertex2i(x+width,y);
  glVertex2i(x+width,y+height);
  glVertex2i(x,y+height);
  glEnd();
  return;
}

void b3dSubareaViewport(int xstart, int ystart, int xsize, int ysize)
{
  GLdouble eps = 0.05;
  glViewport((GLint)xstart, (GLint)ystart, (GLsizei)xsize, (GLsizei)ysize);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho((GLdouble)xstart - eps, (GLdouble)(xstart + xsize - eps), 
          (GLdouble)ystart - eps, (GLdouble)(ystart + ysize - eps), 0.5, -0.5);
}
// double b3dStepPixelZoom(double czoom, int step)
// {
//   double *zv = ImodPrefs->getZooms();
//   int i=0;
//   int zoomi;
// 
//   /* DNM: czoom has been stored as a float by caller.  Change it a bit in the
//     desired direction to avoid problems under Windows - instead of former
//     approach of casting to floats */
//   czoom += step * 0.001;
//   if (step > 0) {
//     for(i = 0; i < MAXZOOMS - 1; i++)
//       if (zv[i] > czoom) break;
//     zoomi = (i - 1) + step;
// 
//   } else {
//           
//     for (i = MAXZOOMS - 1; i >= 0; i--)
//       if (zv[i] < czoom) break;
//     zoomi = (i + 1) + step;
//   }
// 
//   if (zoomi < 0) zoomi = 0;
//   if (zoomi >= MAXZOOMS) zoomi = MAXZOOMS - 1;
//   /* imodPrintStderr ("%f %d %d %f\n", czoom, step, zoomi, zv[zoomi]); */
//   return(zv[zoomi]);
// }

void b3dSetImageOffset(int winsize,     /* window size in wpixels.          */
                       int imsize,      /* image size in ipixels            */
                       double zoom,     /* zoom factor.                     */
                       int &drawsize,   /* size drawn          */
                       int &offset,     /* offset from center. */
                       int &woff,       /* window offset.      */
                       int &doff,       /* data offset in ipixels           */
                       int fillEdge)    /* Fill window to edge, maybe beyond*/
{
  int newoffs;
  //imodPrintStderr("winsize %d  imsize %d  zoom %f  offset %d\n", winsize, imsize,
  //zoom, offset);
  /* Fits completely inside of window. (Why test against imsize - 1 ?*/
  if ( (int)((imsize) * zoom) <= winsize ) {
    drawsize = imsize;
    woff     = (int)(( winsize - (int)((imsize) * zoom)) / 2);
    doff     = 0;
    //imodPrintStderr("1 ds do offset wo %d %d %d %d\n", drawsize, doff, offset, woff);

  } else {

    /* Draw sub image. */
    woff = 0;
    drawsize = (int)(winsize / zoom);
    doff = (int)((imsize / 2 ) - (winsize / zoom / 2));
    doff -= offset;

    if (doff < 0){

      /* Offset in lower corner. */
      /* maxborder. */
      int maxwoff = winsize/6;
      woff = (int)(-(doff) * zoom);

      /* DNM 3/8/04: only change offset if woff needs to be limited, to 
         prevent image creep */
      if (woff > maxwoff) {
        woff = maxwoff;
        offset   = (int)(imsize*0.5f - ((winsize*0.5f - woff)/zoom));
      }
      doff = 0;
      drawsize = (int)((winsize - woff) / zoom);

    } else if ( (doff + drawsize) > (imsize - 1)){

      /* Offset in upper corner */
      /* The minimum drawsize. */
      int minds = (int)((winsize * 0.8333333)/zoom);

      drawsize = imsize - doff;

      if (drawsize < minds){
        drawsize = minds;
        doff     = imsize - drawsize;

        // 12/12/07: it was -2, don't even know why it should be -1, but
        // keep the offset from becoming more negative
        newoffs = (int)(imsize * 0.5 - doff - (winsize*0.5f)/zoom - 1.0f);
	if(newoffs>offset)
        offset   =  newoffs;
      }
      //imodPrintStderr("2 ds do offset wo %d %d %d %d\n", drawsize, doff, offset, woff);
      return;
    }
    
    /* try and fill to edge if flag is +, or at least allow draw of one extra
       pixel if flag is 0 */
    if ((fillEdge > 0 && (int)(zoom * drawsize) < winsize - woff) ||
        (!fillEdge && (int)(zoom * (drawsize + 1) < winsize + 1 - woff)))
      drawsize++;
    //imodPrintStderr("3 ds do offset wo %d %d %d %d\n", drawsize, doff, offset, woff);

  }
  return;
}

void b3dColorIndex(int pix)
{
  QColor qcol;
  glIndexi(pix);
//   if (App->rgba){
//     qcol = ImodPrefs->namedColor(pix);
    glColor3ub(qcol.red(), qcol.green(), qcol.blue());
//   }
}

int b3dGetImageType(GLenum *otype, GLenum *oformat)
{
  GLint unpack = 1;
  GLenum format = GL_COLOR_INDEX;
  GLenum type   = GL_UNSIGNED_BYTE;

//   if (App->rgba){
    unpack = 4;
    type = GL_UNSIGNED_BYTE; 
    /* DNM 1/20/02: eliminated unused conditional on SGI */
    format   = GL_RGBA;
//   }else{
//     if (App->depth > 8){ 
//       unpack = 2; 
//       type = GL_UNSIGNED_SHORT; 
//     }else{
//       /* use default values. */
//     }
//   }
  if (otype)
    *otype   = type;
  if (oformat)
    *oformat = format;

  glPixelStorei(GL_UNPACK_ALIGNMENT, unpack);
     
  return unpack;
}

/* This is the display routine called from the zap window, for regular or
   high quality data */
/* DNM 1/20/02: Added slice argument to govern image re-use, rather than
   assuming CurZ */
void b3dDrawGreyScalePixelsHQ(unsigned char **dataPtrs,  /* input data lines */
                              int xsize, int ysize,     /* size of input   */
                              int xoffset, int yoffset, /* data offsets into input */
                              int wx, int wy,           /* window start for drawing */
                              int width, int height,    /* sub-area size of input data */
                              B3dCIImage *image,        /* tmp image data. */
                              int base,                 /* colorindex ramp base */
                              double xzoom,
                              double yzoom,
                              int quality, int slice, int rgba)  
{
  
    
   int drawwidth = (int)(width );
   int dheight = (int)height;
   
  unsigned short *sdata = new unsigned short [drawwidth*dheight];
  unsigned char  *bdata = (unsigned char *)sdata;
  unsigned int   *idata = (unsigned int *)sdata;

   
   GLenum type, format;
   GLint unpack = b3dGetImageType(&type, &format);
  
   glPixelZoom((GLfloat)1.0f, (GLfloat)1.0f); 
   glRasterPos2f((float)wx, (float)wy);
  glDrawPixels(drawwidth, dheight, format, type, sdata); 
  delete [] sdata; 
}