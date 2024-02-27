#ifndef TREEVIEW_H
#define TREEVIEW_H

#include <QtGui>
#include "Editor.h"
#include "ImageViewWindow/slicemain/slicemainwindow.h"

#define FIlENAMEROLE Qt::UserRole +1;
#define IMOD_MSG 1000+18
class TreeView : public QTreeView
{
    Q_OBJECT
public:
    TreeView(QString title = NULL , QWidget *parent = 0);
    virtual ~TreeView();
    void SetTreeTitle(QString title);
    void SetSubTree(int index , QString title ,QString filename , int filetype);
    void AddItem(int index , int filetype , QString title ,QString filename);//根据索引添加新项
    QModelIndex AddChildItem(QModelIndex index ,int filetype , QString title ,QString filename);
    QString GetFileName(int index);
    void AddProjectFile(QString filename);
    void DeleteItem(int index , QString itemname);
    void ClearTree(int index);
public slots:
    void Update();
    
protected:
    void ListFile(QModelIndex index , QFileInfo &parent);
    void mouseDoubleClickEvent(QMouseEvent *event);
private:
    int m_numSubTree;
    bool m_mouseStatus;
    QString m_treeTilte;
    QString m_curTitle;
    QString m_curFileName;
    QModelIndex m_curIndex;
    QStandardItemModel *model;
    QString m_filename;
    QString m_title;
};
#endif
