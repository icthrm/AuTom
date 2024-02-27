#include "CSkeleton.h"

CSkeleton::CSkeleton(QWidget* parent): QPushButton(parent), skeleton(0, 0)
{
    qDebug("CSkeleton");
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//     setLayout(NULL);   
    QGridLayout* l = new QGridLayout();
    m_label = new QLabel(this);
    m_canvas = new CCanvas(this);
    m_label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_canvas->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);  
}

CSkeleton::~CSkeleton()
{

}

void CSkeleton::SetText(const QString& str, const IplImage* img, int width, int height, ShowStrategy strategy)
{
    m_label->setText(str);
    
    if(img){
	if(strategy == FullFill){
// 	    IplImage* tmp = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
// 	    cvResize(img, tmp, CV_INTER_CUBIC);
	    skeleton = fromIPlImage(img);
	    skeleton = skeleton.scaled(width, height);
// 	    cvReleaseImage(&tmp);
	}
	else if(strategy == Zoom){
	    if(img->width <= width && img->height <= height){
		skeleton = fromIPlImage(img);
	    }
	    else{
		float scale_w = img->width/(float)width;
		float scale_h = img->height/(float)height;
		float scale = scale_w > scale_h ? scale_w: scale_h;
		
// 		int a = 680;//(int)((img->width)/scale);
// 		int b = 680;//(int)((img->height)/scale);
// 		std::cout<<a<<" "<<b<<std::endl;
// 		IplImage* tmp = cvCreateImage(cvSize((int)((img->width)/scale)-1, (int)((img->height)/scale)-1), IPL_DEPTH_8U, 1);
// 		cvResize(img, tmp, CV_INTER_CUBIC);
		skeleton = fromIPlImage(img);
		skeleton = skeleton.scaled((img->width)/scale, (img->height)/scale);
// 		cvReleaseImage(&tmp);
	    }
	}
	
    }
    if(!img){
	float scale_w = default_w/(float)width;
	float scale_h = default_h/(float)height;
	float scale = scale_w > scale_h ? scale_w: scale_h;
	
	skeleton = QPixmap(default_w/scale, default_h/scale);
	skeleton.fill(Qt::lightGray);
    }
    
    m_canvas->UpdateImageData(&skeleton);
    
    setFixedSize(w_marge*2+width, h_top_marge+h_bott_marge+height);
    m_canvas->setGeometry(w_marge, h_top_marge, width, height);
    m_canvas->setFixedSize(width, height);
    
    m_label->setGeometry(w_marge, 4, width, h_top_marge-2);
}

void CSkeleton::SetImage(const IplImage* img, int width, int height)
{
	    skeleton = fromIPlImage(img);
	    skeleton =  skeleton.scaled(width, height);
	    m_canvas->UpdateImageData(&skeleton);
    
    setFixedSize(w_marge*2+width, h_top_marge+h_bott_marge+height);
    m_canvas->setGeometry(w_marge, h_top_marge, width, height);
    m_canvas->setFixedSize(width, height);
    
    m_label->setGeometry(w_marge, 4, width, h_top_marge-2);
}


#include "CSkeleton.moc"
