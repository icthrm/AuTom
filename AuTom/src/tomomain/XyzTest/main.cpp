#include <QtWidgets/QApplication>
#include <QTextCodec>
//#include <QtGui>
#include <QtWidgets>
#include "XyzView/XyzWindow.h"
#include "XyzView/XyzGL.h"
#include "mrcimg/mrc2img.h"

int main(int argc, char** argv)
{
    Q_INIT_RESOURCE(resource);
    
    QApplication app(argc, argv);
    
    util::MrcStack* images = new util::MrcStack;
    images->Open("/home/wls/projects/BBa/BBa_res.st");
    XyzWindow foo(images);
    foo.show();    
    return app.exec();
}
