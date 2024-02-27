#include "datatype.h"

QPixmap fromIPlImage(const IplImage* img)
{
    uchar *buf = NULL;

    const int& widthStep = img->widthStep;
    const int& height = img->height;
    const int& width = img->width;
    
    switch(img->depth){
    case IPL_DEPTH_8U:
        if(img->nChannels == 1){
            /* OpenCV image is stored with one byte grey pixel. We convert it to an 8 bit depth QImage.*/
            buf = new uchar[width*height];
            uchar* qdata = buf;
            const uchar* src =(const uchar*)img->imageData;

            for(int y = 0; y < height; y++){
                memcpy(qdata, src, width);
                qdata += width;
                src += widthStep;
            }

        }
        else if(img->nChannels == 3){
            /* OpenCV image is stored with 3 byte color pixels(3 channels). We convert it to a 32 bit depth QImage.*/
            buf = new uchar[width*height*4];
            uchar* qdata = buf;
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
            buf = new uchar[width*height];
            uchar *qdata = buf;
            //const uint16_t *src =(const uint16_t *);
            const unsigned int* src =(const unsigned int*)img->imageData;
            for(int y = 0; y < height; y++){
                for(int x = 0; x < width; x++){
                    // We take only the highest part of the 16 bit value. It is
                    //similar to dividing by 256.
                    //bug
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
            buf = new uchar[width*height];
            uchar *qdata = buf;
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
    
    QImage* qimg;
    QVector<QRgb> vcolorTable;
    if(img->nChannels == 1){
        for(int i = 0; i < 256; i++){
            vcolorTable.push_back(qRgb(i, i, i));
        }
        qimg = new QImage(buf, width, height, QImage::Format_Indexed8);
        qimg->setColorTable(vcolorTable);
    }
    else{
        qimg = new QImage(buf, width, height, QImage::Format_RGB32);
    }
    QPixmap tmp = QPixmap::fromImage(*qimg);
    
    delete qimg;
    delete [] buf;
    return tmp;
}

void ItemModel::SetIndex(int idx)
{
    _index = idx;
    if(_img){
	cvReleaseImage(&_img);
    }
    _img = _mrcs.GetIplImage(_index);
    emit indexChanged(_img);
}

bool ItemModel::ReadAnglesByName(const char* name, std::vector< float >* angles)
{
    std::ifstream in(name);
    if(!in.good()) {
	return false;
    }
    
    angles->clear();
    
    while(in.good()){
	float angle;
	in>>angle;
	if(in.fail()){
	    break;
	}
	angles->push_back(angle);
    }
    in.close();
    return true;
}

bool ItemModel::Open(const char* mrcfile, const char* anglefile)
{
    bool is_ok = _mrcs.Open(mrcfile) && ReadAnglesByName(anglefile, &_angles);
    if(!is_ok){
	return is_ok;
    }
    else{
	_index = _angles.size()/2;
	for(int i = 0; i < _angles.size(); i++){
	    if(abs(_angles[i]) < 1){
		_index = i;
		break;
	    }
	}
	
	if(_img){
	    cvReleaseImage(&_img);
	}
	_img = _mrcs.GetIplImage(_index);
	
	_name = std::string(mrcfile);
	_angles_name = std::string(anglefile);
    }
    
    return is_ok;
}

bool ItemModel::Open(const char* mrcfile)
{
    if(!_mrcs.Open(mrcfile)){
	return false;
    }
    _index = 0;
    
    if(_img){
	cvReleaseImage(&_img);
    }
    _img = _mrcs.GetIplImage(_index);
    _name = std::string(mrcfile);
    
    return true;
}

void ViewData::Mirror(const IplImage* img){
    _mirror = img;
    
    if(_degenerated){
	cvReleaseImage(&_degenerated);
    }
    util::CopyToUChar256(_mirror, &_degenerated, _white, _black, true);
    
    ScaleCopy();
}

void ViewData::Mirror(const IplImage* img, float scale, int black, int white)
{
    _scale = scale; _black = black; _white = white;
    Mirror(img);
}

void ViewData::SetRange(int black, int white)
{
    if(_black == black && _white == white){
	return;
    }
    _black = black; _white = white;
    
    Mirror(_mirror);
}

void ViewData::SetScale(float scale)
{
    if(_scale == scale){
	return;
    }
    _scale = scale;
    
    ScaleCopy();
}

void ViewData::SetScaleRange(float scale, int black, int white)
{
    if(_black == black && _white == white && _scale == scale){
	return;
    }
    else if(_black == black && _white == white){
	SetScale(scale);
	return;
    }
    
    _scale = scale; _black = black; _white = white;    
    Mirror(_mirror);
}

void ViewData::ScaleCopy()
{
    _pix = fromIPlImage(_degenerated);
    
    if(_scale != 1){
	_pix = _pix.scaled(_pix.width()*_scale, _pix.height()*_scale);
    }
}

CDoc* CDoc::theGlobalDoc;

#include "datatype.moc"
