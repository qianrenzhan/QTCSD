#include "qmytreewidget.h"

#include "stable.h"

QMyTreeWidget::QMyTreeWidget(QWidget *parent):QTreeWidget(parent)
{
    initTreeWidget();

    addRootNodeAction = new QAction("&addRootNode", this);
    deleteAllNodesAction = new QAction("&deleteAllNodes", this);

    addCategoryAction = new QAction("&addCategory", this);
    addTemplateAction = new QAction("&addTemplate", this);

    deleteNodeAction = new QAction("&deleteNode", this);
    reNameNodeAction = new QAction("&reNameNode", this);

    connect(addRootNodeAction,SIGNAL(triggered()),this,SLOT(addRootNode()));
    connect(deleteAllNodesAction,SIGNAL(triggered()),this,SLOT(deleteAllNodes()));

    connect(addCategoryAction,SIGNAL(triggered()),this,SLOT(addCategory()));
    connect(addTemplateAction,SIGNAL(triggered()),this,SLOT(addTemplate()));

    connect(deleteNodeAction,SIGNAL(triggered()),this,SLOT(deleteNode()));
    connect(reNameNodeAction,SIGNAL(triggered()),this,SLOT(reNameNode()));

    connect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(on_treeWidget_customContextMenuRequested(QPoint)));
}

void QMyTreeWidget::RecursiveRead(QTreeWidgetItem *temp, QString fatherid)
{
    TemplateTreeLab* tpltl = TemplateTreeLab::get();
    QList<TemplateTree> ltplt = tpltl->getTemplateTreeByFatherUuid(fatherid);
    for(int i = 0;i<ltplt.size();i++)
    {
        TemplateTree temptt = ltplt.at(i);
        QTreeWidgetItem *child=new QTreeWidgetItem(temp);
        QVariant var;
        QString str = temptt.getId();
        var.setValue(str);
        child->setData(0,Qt::UserRole,var);
        child->setText(0,temptt.getTitle());
        RecursiveRead(child,temptt.getId());
    }
}

void QMyTreeWidget::initTreeWidget()
{
    this->clear();

    //this->setHeaderHidden(true);
    QStringList strlist;
    strlist.append("cons");
    strlist.append("title");
    this->setHeaderLabels(strlist);

    //从数据库中读取目录结构
    //RecursiveRead(this,-1);     //this指针可以强制转换
    TemplateTreeLab* tpltl = TemplateTreeLab::get();
    QList<TemplateTree> ltplt = tpltl->getTemplateTreeByFatherUuid("-1");
    for(int i = 0;i<ltplt.size();i++)
    {
        TemplateTree temptt = ltplt.at(i);
        QTreeWidgetItem *temp=new QTreeWidgetItem(this);
        QVariant var;
        QString str = temptt.getId();
        var.setValue(str);
        temp->setData(0,Qt::UserRole,var);
        temp->setText(0,temptt.getTitle());
        RecursiveRead(temp,temptt.getId());
    }

    this->expandAll();
}


void QMyTreeWidget::addRootNode()
{
    qDebug() << "addRootNode" << endl;
    //在TEMPLATE表中新加一行
    //新节点ID   根   模板名称  颜色值    是否目录
    //随机UUID  -1   title   colors     1

    QTreeWidgetItem *temp=new QTreeWidgetItem(this);

    QInputDialog dia(this);
    dia.setWindowTitle("Input Dialog");
    dia.setLabelText("Please input text:");
    dia.setInputMode(QInputDialog::TextInput);//可选参数：DoubleInput  TextInput

    if(dia.exec() == QInputDialog::Accepted)
    {
       qDebug() << dia.textValue();
       temp->setText(0,dia.textValue());

       QVariant var;
       QString str = QUuid::createUuid().toString();
       var.setValue(str);
       temp->setData(0,Qt::UserRole,var);

       //修改数据库
       TemplateTree t(str);
       t.setFatherId("-1");
       t.setTitle(dia.textValue());
       t.setIsCategory("1");
       TemplateTreeLab* tpltl = TemplateTreeLab::get();
       tpltl->addTemplateTree(t);
    }
    this->expandAll();
}

void QMyTreeWidget::deleteAllNodes()
{
    qDebug() << "deleteAllNodes" << endl;
    //清空TEMPLATETREE表
    this->clear();

    //修改数据库
    TemplateTreeLab* tpltl = TemplateTreeLab::get();
    tpltl->deleteAllTemplateTree();
    this->expandAll();
}

void QMyTreeWidget::addCategory()
{
    qDebug() << "addCategory" << endl;
    //在TEMPLATE表中新加一行
    //新节点ID     根       模板名称  颜色值     是否目录
    //随机UUID  当前UUID   title   colors       1
    //curItem->data()  当前UUID

    QTreeWidgetItem *temp=new QTreeWidgetItem(curItem);

    QInputDialog dia(this);
    dia.setWindowTitle("Input Dialog");
    dia.setLabelText("Please input text:");
    dia.setInputMode(QInputDialog::TextInput);//可选参数：DoubleInput  TextInput

    if(dia.exec() == QInputDialog::Accepted)
    {
       qDebug() << dia.textValue();
       temp->setText(0,dia.textValue());

       QVariant var;
       QString str = QUuid::createUuid().toString();
       var.setValue(str);
       temp->setData(0,Qt::UserRole,var);

       //修改数据库
       TemplateTree t(str);
       t.setFatherId(curItem->data(0,Qt::UserRole).toString());
       t.setTitle(dia.textValue());
       t.setIsCategory("1");
       TemplateTreeLab* tpltl = TemplateTreeLab::get();
       tpltl->addTemplateTree(t);
    }
    this->expandAll();
}

void QMyTreeWidget::addTemplate()
{
    qDebug() << "addTemplate" << endl;
    //新节点ID     根       模板名称  颜色值    是否目录
    //随机UUID  当前UUID   title   colors      0
    //curItem->data()  当前UUID
    QTreeWidgetItem *temp=new QTreeWidgetItem(curItem);

    QInputDialog dia(this);
    dia.setWindowTitle("Input Dialog");
    dia.setLabelText("Please input text:");
    dia.setInputMode(QInputDialog::TextInput);//可选参数：DoubleInput  TextInput

    if(dia.exec() == QInputDialog::Accepted)
    {
       qDebug() << dia.textValue();
       temp->setText(0,dia.textValue());

       QVariant var;
       QString str = QUuid::createUuid().toString();
       qDebug() << str;
       var.setValue(str);
       temp->setData(0,Qt::UserRole,var);

       //修改数据库
       TemplateTree t(str);
       t.setFatherId(curItem->data(0,Qt::UserRole).toString());
       t.setTitle(dia.textValue());
       t.setIsCategory("0");
       TemplateTreeLab* tpltl = TemplateTreeLab::get();
       tpltl->addTemplateTree(t);
    }
    this->expandAll();
}

void QMyTreeWidget::deleteNode()
{
    qDebug() << "deleteNode" << endl;
    //curItem->data()  当前UUID  删除该索引

    QTreeWidgetItem* parent = curItem->parent();
    if(parent == NULL)
    {
        //得到索引
        int index = 0;
        int count = this->topLevelItemCount();
        for(int i = 0;i<count;i++)
        {
            //QTreeWidget
            //ui->treeWidget->TopLevelItem()
            QTreeWidgetItem* temp = this->topLevelItem(i);
            if(curItem->data(0,Qt::UserRole).toString() == temp->data(0,Qt::UserRole).toString())
            {
                index = i;
            }
        }
        this->takeTopLevelItem(index);
    }
    else
        parent->removeChild(curItem);

    TemplateTreeLab* tpltl = TemplateTreeLab::get();
    //仅删除一行不行，需要进行遍历删除
    tpltl->DeleteTemplateTreeNode(curItem->data(0,Qt::UserRole).toString());

    this->expandAll();
}



void QMyTreeWidget::reNameNode()
{
    qDebug() << "reNameNode" << endl;

    QInputDialog dia(this);
    dia.setWindowTitle("Input Dialog");
    dia.setLabelText("Please input text:");
    dia.setInputMode(QInputDialog::TextInput);//可选参数：DoubleInput  TextInput

    if(dia.exec() == QInputDialog::Accepted)
    {
       qDebug() << dia.textValue();
       curItem->setText(0,dia.textValue());

       //更新数据库
       //curTtem->data   dia.textValue()
       TemplateTree t(curItem->data(0,Qt::UserRole).toString());
       t.setTitle(dia.textValue());
       TemplateTreeLab* tpltl = TemplateTreeLab::get();
       tpltl->updateTemplateTreeTitlebyUuid(t);
    }
}

void QMyTreeWidget::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{
    curItem=this->itemAt(pos);  //获取当前被点击的节点

    //在空白位置点击，弹出菜单：添加根节点，删除所有模板。
    if(curItem == NULL)
    {
        QMenu *popMenu =new QMenu(this);//定义一个右键弹出菜单
        popMenu->addAction(addRootNodeAction);
        popMenu->addAction(deleteAllNodesAction);
        popMenu->exec(QCursor::pos());
    }
    else{
        QVariant var = curItem->data(0,Qt::UserRole);
        qDebug() << var.toString() << endl;
        TemplateTreeLab* tpltl = TemplateTreeLab::get();
        TemplateTree tpt = tpltl->getTemplateTreeByUuid(var.toString());

        //读取表，对isCategory字段进行判断
        if(tpt.getIsCategory().toInt())   //选中目录
        {
            qDebug() << "category" << endl;
            QMenu *popMenu =new QMenu(this);//定义一个右键弹出菜单
            popMenu->addAction(addCategoryAction);
            popMenu->addAction(addTemplateAction);
            popMenu->addAction(deleteNodeAction);
            popMenu->addAction(reNameNodeAction);
            popMenu->exec(QCursor::pos());
        }
        else     //选中模板
        {
            qDebug() << "template" << endl;
            QMenu *popMenu =new QMenu(this);//定义一个右键弹出菜单
            popMenu->addAction(deleteNodeAction);
            popMenu->addAction(reNameNodeAction);
            popMenu->exec(QCursor::pos());
        }
    }

}
