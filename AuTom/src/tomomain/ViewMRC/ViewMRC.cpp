#include "ImageViewWindow/ImageViewWindow.h"
#include "XyzView/XyzWindow.h"
#include "getopt.h"
#include <QtWidgets/QApplication>
//#include <QtGui>
#include <QtWidgets>

#define ZAP 0
#define XYZ 1
#define ZAP_XYZ 2F

char* filename;

void display_usage()
{
//     printf("description:\n");
//     printf("Display the MRC images\n");
    printf(" usage:\n");
    printf("	./ViewMRC [options] <Image files>  or ./ViewMRC [options]\n");
    printf("Options:\n");
    printf("  -h or -H or --help : Print help message.\n");
//     printf("  -v or -V or -version : Print vertion message\n");
    printf("  -z or -Z :  Open Zap window (follow with <Image files> ) \n");
    printf("  --xyz or -XYZ : Open xyz window(follow with <Image files> )\n");
}

void openMRC(int viewtype){
  printf("openMRC:%s\n" , filename);
  util::MrcStack* images = new util::MrcStack;
    images->Open(filename);  
    
  switch(viewtype){
    case ZAP:
    {
      ImageViewWindow *win = new ImageViewWindow();
      win->LoadImageStack(images);
     QFileInfo file(filename);
      win->SetImageName(file.fileName());
      win->show();
      break;
    }
    case XYZ:
    {
       XyzWindow *foo = new XyzWindow(images);
        foo->show();
	break;
    }
// 	break;
  }
  delete images;
}

int main(int argc, char **argv)
{
    printf("ViewMRC\n");
    Q_INIT_RESOURCE(resource);
    QApplication app(argc, argv);
    static struct option longopts[] = {
        { "help",            no_argument,            NULL,              'h' },
        { "xyz",            required_argument,            NULL,              'x' },
        { NULL,              0,                      NULL,               0  }
    };
    
    if(argc ==1 || argc > 4){
      display_usage();
      exit(1);
    }
    int ch;
//     char *b_input = NULL; 
    filename = NULL;
    while ((ch = getopt_long(argc, argv, ":hHz:Z:x:", longopts, NULL))!= -1) {
      switch(ch){
	case ':':
	  printf("The lack of option parameters\n");
	  break;
	case 'h':
	case 'H':
	  display_usage();
	  exit(1);
	  break;
	case 'z':
	case 'Z':
	  filename = optarg;
	  openMRC(ZAP);
	  break;
	case 'x':
	  filename = optarg;
	  openMRC(XYZ);
	  break;
	default:
	  display_usage();
	  exit(1);
      }
    }
    return app.exec();
}
