#include "Editor.h"

Editor::Editor(QWidget* parent)
  :QTextEdit(parent)
{
     action = new QAction(this);  			
    action->setCheckable(true);			
    connect(action, SIGNAL(triggered()), this, SLOT(show()));
    connect(action, SIGNAL(triggered()), this, SLOT(setFocus()));
    isNewFile=true;
}

void Editor::newFile()
{
  //documentNumber=1;
  curFile=tr("document%1.txt");//.arg(documentNumber);//
  isNewFile=true;
 // documentNumber++;
}

Editor* Editor::open(QWidget* parent)
{
  QString filename=QFileDialog::getOpenFileName(parent,tr("open"),".");
  if(filename.isEmpty()) return 0;
  else
  {
    Editor *editor=new Editor(parent);
    if(editor->readFile(filename))
    {
      editor->curFile=filename;
      editor->isNewFile=false;
      return editor;
    }
    else 
    {
      delete editor;
      return 0;
    }
  }
}

void Editor::Open(QString filename)
{
   if(this->readFile(filename))
    {
      this->curFile=filename;
      this->isNewFile=false;
    }
}


bool Editor::readFile(const QString filename)
{
  QFile file(filename);
  if(file.open(QIODevice::ReadWrite|QIODevice::Text))
  {
    QTextStream in(&file);
    setText(in.readAll());
    return true;
  }
  else
  {
    QMessageBox::warning(this,tr("读取文件"),tr("can't read file%1:\n%2").arg(filename).arg(file.errorString()));
    return false;
  }
}

bool Editor::save()
{
  if(isNewFile)
  {
    return saveAs();
  }
  else
  {
    return saveFile(curFile);
  }
}

bool Editor::saveAs()
{
  //返回的名字filename是自己手动输入的名字，或者直接采用的是默认的名字
    QString filename = QFileDialog::getSaveFileName(this, tr("另存为"), curFile);
    if(filename.isEmpty())
        return false;
    return saveFile(filename);
}

bool Editor::saveFile(QString filename)
{
  QFile file(filename);
  if(file.open(QIODevice::WriteOnly|QIODevice::Text))
  {
    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << toPlainText();//以纯文本方式写入，核心函数
    QApplication::restoreOverrideCursor();
    curFile=filename;
    return true;
  }
  else
  {
    QMessageBox::warning(this,tr("保存"),tr("无法保存"));
    return false;
  }
}

bool Editor::okToContinue()
{
      if (document()->isModified()) {
        int r = QMessageBox::warning(this, tr("MDI Editor"),
                        tr("File %1 has been modified.\n"
                           "Do you want to save your changes?").arg(QFileInfo(curFile).fileName()),
                        QMessageBox::Yes | QMessageBox::Default,
                        QMessageBox::No,
                        QMessageBox::Cancel | QMessageBox::Escape);
        if (r == QMessageBox::Yes) {
            return save();
        } else if (r == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

void Editor::closeEvent(QCloseEvent* event)
{
    if(okToContinue()) event->accept();
    else event->ignore();
}

QString Editor::getFolder()
{
  QString str=curFile;
  str.remove(QString("/")+QFileInfo(curFile).fileName());
   return str;
}

void Editor::openfile(QString filename)
{
  if(readFile(filename))
  {
    curFile=filename;
    isNewFile=false;
  }
  /*if(filename.isEmpty()) return ;
  else
  {
    Editor *editor=new Editor(this);
    if(editor->readFile(filename))
    {
      editor->curFile=filename;
      editor->isNewFile=false;
    }
    else 
    {
      delete editor;
      return ;
    }
  }*/
}

#include "Editor.moc"


