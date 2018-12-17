#ifndef QMYTREEWIDGET_H
#define QMYTREEWIDGET_H

#include <QTreeWidget>
#include <QAction>
#include <QUuid>
#include <QInputDialog>
#include <QDebug>
#include <QMenu>
#include <QList>
#include "templatetree.h"
#include "templatetreelab.h"

class QMyTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit QMyTreeWidget(QWidget *parent = 0);

private slots:
    void on_treeWidget_customContextMenuRequested(const QPoint &pos);
    void addRootNode();
    void deleteAllNodes();
    void addCategory();
    void addTemplate();

    void deleteNode();
    void reNameNode();

private:

    void initTreeWidget();
    void RecursiveRead(QTreeWidgetItem *temp, QString fatheri);

    QTreeWidgetItem* curItem;  //当前被右击的item
    //空白地方点击
    QAction *addRootNodeAction;
    QAction *deleteAllNodesAction;

    //节点上点击
    QAction *addCategoryAction;
    QAction *addTemplateAction;
    QAction *deleteNodeAction;
    QAction *reNameNodeAction;

};

#endif // QMYTREEWIDGET_H
