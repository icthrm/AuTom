#ifndef Editor_H
#define Editor_H

//#include <QtGui>
#include <QtWidgets>
class Editor: public QTextEdit
{
  Q_OBJECT
public:
  Editor(QWidget *parent=0);
  void newFile();
  static Editor *open(QWidget *parent=0);
  void Open(QString filename);
  bool save();
  bool saveAs();
  bool saveFile(QString filename);
  QAction *windowMenuAction() const { return action; }
  QString getFileName() const{return curFile;}
  QString getFolder();
  void openfile(QString filename);
  
private:
  bool readFile(const QString filename);
  QString curFile;//当前处理的文件
  bool isNewFile;//判断是否为一个新建的文件
  bool okToContinue();
  QAction *action;
  //static int documentNumber;
protected:
  void closeEvent(QCloseEvent *event);
  
};
#endif
