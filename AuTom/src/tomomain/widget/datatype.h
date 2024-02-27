#ifndef DATATYPE_H__
#define DATATYPE_H__

#include <iostream>
#include <string>
#include <cstring>
#include <QtGui/QPixmap>
#include <cv.hpp>
#include <cxcore.hpp>
#include <QVector>
#include "mrcimg/img_util.h"
#include "mrcimg/mrc2img.h"
#include <sys/stat.h>
QPixmap fromIPlImage(const IplImage* img);

class ItemModel: public QObject
{
Q_OBJECT
private:
    util::MrcStack _mrcs;
    std::vector<float> _angles;
    int _index;
    IplImage* _img;		//IPL_DEPTH_32F  no connect with white_range/black_range
    std::string _name;
    std::string _angles_name;
    
private:
    static bool ReadAnglesByName(const char* name, std::vector<float>* angles);
    
signals:
    void indexChanged(const IplImage* img);
    
public:
    ItemModel():QObject(), _img(NULL), _index(-1){}
    
    ~ItemModel(){
	if(_img){
	    cvReleaseImage(&_img);
	}
    }
    
    int Index() const{return _index;}
    const std::vector<float>& Angles() const{return _angles;}
    void SetIndex(int idx);
    const char* Name() const{return _name.c_str();}
    const char* AngleFile() const{return _angles_name.c_str();}
    bool Open(const char* mrcfile, const char* anglefile);
    bool Open(const char* mrcfile);
    const IplImage* Img() const{return _img;}
    int Size() const{return _mrcs.Size();}
    int Width() const{return _mrcs.Width();}
    int Height() const{return _mrcs.Height();}
};

class ViewData: public QObject
{
Q_OBJECT
private:
    int _white;
    int _black;
    float _scale;
    const IplImage* _mirror;		//IPL_DEPTH_32F should not be cleared by cvReleaseImage
    IplImage* _degenerated;		//IPL_DEPTH_8U should be cleared by cvReleaseImage(same size with _mirror)
    QPixmap _pix;
    
private:
    void ScaleCopy();
    
public:
    ViewData():QObject(), _mirror(NULL), _black(0), _white(255), _scale(0.25), _degenerated(NULL){}
    
    ~ViewData(){
	if(_degenerated){
	    cvReleaseImage(&_degenerated);
	}
    }
    
public slots: 
    void Mirror(const IplImage* img);
    
public:
    void Mirror(const IplImage* img, float scale, int black, int white);
    void SetRange(int black, int white);
    void SetScale(float scale);
    void SetScaleRange(float scale, int black, int white);
    int White() const{return _white;}
    int Black() const{return _black;}
    float Scale() const{return _scale;}
    const QPixmap& Pixmap() const{return _pix;}
    const IplImage* MirrorImage() const{return _degenerated;}
};

class SectSetting
{
private:
    int thickness0;
    std::string slice1_name;
    std::string slice2_name;
    std::string slice3_name;
    
public:
    int Thickness() const{
	return thickness0;
    }
    
    void SetThickness(int thickness){
	thickness0 = thickness;
    }
    
    const char* Slice1Name() const{
	return slice1_name.c_str();
    }
    
    void SetSlice1Name(const char* name){
	slice1_name = std::string(name);
    }
    
    const char* Slice2Name() const{
	return slice2_name.c_str();
    }
    
    void SetSlice2Name(const char* name){
	slice2_name = std::string(name);
    }
    
    const char* Slice3Name() const{
	return slice3_name.c_str();
    }
    
    const char* SliceName(int idx) const{
	if(idx == 0){
	    return slice1_name.c_str();
	}
	else if(idx == 1){
	    return slice2_name.c_str();
	}
	else if(idx == 2){
	    return slice3_name.c_str();
	}
    }
    
    void SetSlice3Name(const char* name){
	slice3_name = std::string(name);
    }    
};

class CDoc{
    /*According to the code, the CDoc seems to be only active during the geometry process. So do not use it as a global-Settings container. */

private:
    static CDoc* theGlobalDoc;
    
private:
    ItemModel _item;
    SectSetting _setting;
    
private:
    CDoc(){}
    ~CDoc(){}
    
public:
    static void InitInstance(){ theGlobalDoc = new CDoc();}
    static ItemModel& GetItemModel(){ return theGlobalDoc->_item;}
    static SectSetting& GetSectSetting(){ return theGlobalDoc->_setting;}
    static void DestoryInstance(){ delete theGlobalDoc;}
};

#endif
