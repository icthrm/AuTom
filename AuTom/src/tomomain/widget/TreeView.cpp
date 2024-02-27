#include "TreeView.h"
#include "iostream"

#include "core/msg_map.h"
#include <QApplication>
#define FIlENAMEROLE Qt::UserRole +1;
#define FILE 0;
#define FOLDER 1;



TreeView::TreeView(QString title, QWidget* parent)
	  : QTreeView( parent) ,m_treeTilte("project") ,m_numSubTree(1) ,
	  m_mouseStatus(true)
{
    m_curFileName = "";
    m_curTitle = "";
    this->setHeaderHidden(true);;
    m_title = title;	
    model = new QStandardItemModel();
    model->setHeaderData(0,Qt::Horizontal , title);    
    this->setModel(model);    
//  setEditTriggers(!DoubleClicked);
   //connect(this , SIGNAL(doubleClicked(QModelIndex)) , this , SLOT(OpenIndexFile(QModelIndex)));
    
}

void TreeView::AddItem(int index ,int filetype, QString title , QString filename)
{
    QStandardItem *parentItem = model->item(index);
    QStandardItem *childItem = new QStandardItem(title);
    //QVariant *v = new QVariant(&filename);
    childItem->setData(filename ,Qt::UserRole+1);
     switch (filetype)
    {
	case 0 :
	    childItem->setIcon(QIcon(":/images/file.png"));
	    break;
	case 1:
	    childItem->setIcon(QIcon(":/images/folder.png"));
	    break;
    }
    parentItem->appendRow(childItem);
    //FileItem
}

QModelIndex TreeView::AddChildItem(QModelIndex index, int filetype, QString title, QString filename)
{
    QStandardItem *parentItem = model->itemFromIndex(index);
    QStandardItem *childItem = new QStandardItem(title);
       childItem->setData(filename ,Qt::UserRole+1);
     switch (filetype)
    {
	case 0 :
	    childItem->setIcon(QIcon(":/images/file.png"));
	    break;
	case 1:
	    childItem->setIcon(QIcon(":/images/folder.png"));
	    break;
    }
    parentItem->appendRow(childItem);
    return model->indexFromItem(childItem);
}


QString TreeView::GetFileName(int index)
{
    QVariant v = model->item(index)->data();
    return v.toString();
}

void TreeView::SetTreeTitle(QString title)
{
    model->setHeaderData(0 , Qt::Horizontal ,title);
}

void TreeView::SetSubTree(int index, QString title, QString filename ,int filetype)
{
    QStandardItem *item= model->item(index);
  
    switch (filetype)
    {
	case 0 :
	    item->setIcon(QIcon(":/images/file.png"));
	    break;
	case 1:
	    item->setIcon(QIcon(":/images/folder.png"));
	    break;
    }
      item->setText(title);
      item->setData(filename);
}

void TreeView::ClearTree(int index)
{
    QStandardItem *parentItem = model->item(index);
    int i=0;
    for(i=parentItem->rowCount()-1; i>=0 ; i--){
	parentItem->removeRow(i);
    }
}

void TreeView::DeleteItem(int index , QString itemname)
{
    QStandardItem *parentItem = model->item(index);
    for(int i=0 ; i<parentItem->rowCount() ;i++){
	if(itemname == parentItem->child(i)->text()){
	    parentItem->removeRow(i);
	    break;
	}
    }
	
}

void TreeView::AddProjectFile(QString filename)
{
    m_filename = filename;
    model->clear();
    QFileInfo fileInfo=QFileInfo(filename);
    QString file = fileInfo.fileName();
    QStandardItem *parentItem = new QStandardItem(file);
    parentItem->setIcon(QIcon(":/images/folder.png"));
    parentItem->setData(filename);
    model->setItem(model->rowCount() ,parentItem);
   // SetSubTree(0 , file , filename , 1 );
    QModelIndex index = model->indexFromItem(parentItem);
    ListFile(index , fileInfo);    
}


void TreeView::ListFile(QModelIndex index, QFileInfo& parent)
{
    qDebug("listfile");
    QDir dir;
    dir.setPath(parent.filePath());
    dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoSymLinks);
    dir.setSorting(QDir::DirsFirst | QDir::Name);    
 
    const QFileInfoList fileList = dir.entryInfoList();
 
    for (int i = 0; i < fileList.size(); i++) {
	    QFileInfo fileInfo = fileList.at(i);
	    if (fileInfo.fileName() == "." || fileInfo.fileName() == ".." ); // nothing
	    else if(fileInfo.isDir()) {
		//AddChildItem(index , 1 , fileInfo.fileName() , fileInfo.filePath());
		QModelIndex inde = AddChildItem(index , 1 , fileInfo.fileName() , fileInfo.filePath());
            // 查询子目录
                ListFile(inde, fileInfo);
        }
        else {
// 	  printf("add item:%s\n",fileInfo.fileName().toLatin1().data());
            AddChildItem(index ,0 , fileInfo.fileName() ,fileInfo.filePath());
        }
    }
}

void TreeView::Update()
{
//     delete   model;
//     model = new QStandardItemModel();
//     printf("here\n");
//     model->setHeaderData(0,Qt::Horizontal , m_title);  
//     AddProjectFile(m_filename);
    printf("here\n");
    ClearTree(0);
    QFileInfo fileInfo=QFileInfo(m_filename);
    QStandardItem *parentItem = model->item(0);
    QModelIndex index = model->indexFromItem(parentItem);
    ListFile(index , fileInfo);
    
}


void TreeView::mouseDoubleClickEvent(QMouseEvent* event)
{
    if(m_mouseStatus == true && Qt::LeftButton == event->button())
	{
		QModelIndex   index = this->currentIndex();
		m_curIndex = index;
		m_curTitle = index.data().toString();
		m_curFileName = index.data(Qt::UserRole+1).toString();
		qDebug()<< m_curFileName;
		int start = m_curFileName.lastIndexOf('.');
		QString str = m_curFileName.mid(start , m_curFileName.length() - start);
		qDebug()<<str;
		if(str == ".st" || str == ".mrc"){
           //ImageViewWindow* win = ImageViewWindow::ViewMrc(m_curFileName);
           SliceMainWindow* win =SliceMainWindow::ViewMrc(m_curFileName);
		}
// 		else if(str == ".pgm")//##!!!!
// 		{
// 			QString arg=QString("\n%1").arg(m_curFileName);
// 			msg_map::Message* msg = MMTApp::NewMessage(IMOD_MSG, this, arg.toLatin1().data());
// 			MMTApp::PostMessage(msg);
// 		  
// 		}
		else if (str!=""&&str!= m_curFileName){ 
		    Editor* editer = new Editor();
		    editer->setAttribute(Qt::WA_DeleteOnClose);
		    editer->setWindowTitle(m_curFileName);
		    editer->resize(500 ,300 );
		    editer->Open(m_curFileName);
		    editer->show();
		}

	}
    QTreeView::mouseDoubleClickEvent(event);
}


TreeView::~TreeView()
{
    delete   model;
}

#include "TreeView.moc"
