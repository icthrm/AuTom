#include "func.h"
//#include <QtGui>
#include <QtWidgets>
int ivwGetPixelBytes(int mode)
{
  switch (mode) {
  case MRC_MODE_BYTE:
    return 1;
  case MRC_MODE_SHORT:
  case MRC_MODE_USHORT:
    return 2;
  case MRC_MODE_FLOAT:
  case MRC_MODE_COMPLEX_SHORT:
    return 4;
  case MRC_MODE_COMPLEX_FLOAT:
    return 8;
  case MRC_MODE_RGB:
    return 3;
  }
  return 1;
}

void GetDataFromIplimage(const IplImage* img, unsigned char** buf)
{
        const int& widthStep = img->widthStep;
    const int& height = img->height;
    const int& width = img->width;
  
    switch(img->depth){
    case IPL_DEPTH_8U:
        if(img->nChannels == 1){
            /* OpenCV image is stored with one byte grey pixel. We convert it to an 8 bit depth QImage.*/
// 	    qDebug()<<"8";
            *buf = new uchar[width*height];
            uchar* qdata = *buf;
            const uchar* src =(const uchar*)img->imageData;

            for(int y = 0; y < height; y++){
                memcpy(qdata, src, width);
                qdata += width;
                src += widthStep;
            }

        }
        else if(img->nChannels == 3){
            /* OpenCV image is stored with 3 byte color pixels(3 channels). We convert it to a 32 bit depth QImage.*/
            *buf = new uchar[width*height*4];
            uchar* qdata = *buf;
            const uchar* src =(const uchar *) img->imageData;
            for(int y = 0; y < height; y++){
                for(int x = 0; x < width; x++){
                    // We cannot help but copy manually.
                    qdata[0] = src[0];
                    qdata[1] = src[1];
                    qdata[2] = src[2];
                    qdata[3] = 0;

                    qdata += 4;
                    src += 3;
                }
                src += widthStep-3*width;
            }

        }
        else{
            qDebug("IplImageToQImage: image format is not supported : depth=8U and %d channels\n", img->nChannels);
        }
        break;
    case IPL_DEPTH_16U:
        if(img->nChannels == 1){
            /* OpenCV image is stored with 2 bytes grey pixel. We convert it
            to an 8 bit depth QImage.
            */
	    qDebug()<<"16";
            *buf = new uchar[width*height];
            uchar *qdata = *buf;
            //const uint16_t *src =(const uint16_t *);
            const unsigned int* src =(const unsigned int*)img->imageData;
            for(int y = 0; y < height; y++){
                for(int x = 0; x < width; x++){
                    // We take only the highest part of the 16 bit value. It is
                    //similar to dividing by 256.
                    *qdata++ =((*src++) >> 8);
                }
                src += widthStep/sizeof(unsigned int)-width;
            }
        }
        else{
            qDebug("IplImageToQImage: image format is not supported : depth=16U and %d channels\n", img->nChannels);
        }
        break;
    case IPL_DEPTH_32F:
        if(img->nChannels == 1){
            /* OpenCV image is stored with float(4 bytes) grey pixel. We
            convert it to an 8 bit depth QImage.
            */
	    qDebug()<<"32";
            *buf = new uchar[width*height];
            uchar *qdata = *buf;
            const float* src =(const float*) img->imageData;
            for(int y = 0; y < height; y++){
                for(int x = 0; x < width; x++){
                    uchar p;
                    float pf = 255*(*src++);
                    if(pf < 0){ p = 0;}
                    else if(pf > 255){ p = 255;}
                    else{ p =(uchar) pf;}

                    *qdata++ = p;
                }
                src += widthStep/sizeof(float)-width;
            }
        }
        else{
            qDebug("IplImageToQImage: image format is not supported : depth=32F and %d channels\n", img->nChannels);
        }
        break;
    default:
        qDebug("IplImageToQImage: image format is not supported : depth=%d and %d channels\n", img->depth, img->nChannels);
    }
}

int BrightContractAdjust(
  const unsigned char* srcImg,
  unsigned char* dstImg,
  int n,
  float bright,
  float contract)
{
   float val;  
    float k = tan((45 + 44*contract)/180*3.14);
        for (int i = 0; i < n; i++)  
        {  
               val = srcImg[i];  
                val = (val - 128*(1-bright))*k + 128*(1+bright) ;  
                //对灰度值的可能溢出进行处理  
                if(val>255) val=255;  
                if(val<0) val=0;  
                dstImg[i] = val;  
        }  
    return 0;  
}
