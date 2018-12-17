#include "templatetreelab.h"

TemplateTreeLab* TemplateTreeLab::pTemplateTreeLab = NULL;

TemplateTreeLab::TemplateTreeLab()
{
    mbh = DataBaseHelper::get();
}

TemplateTreeLab* TemplateTreeLab::get()
{
    if (pTemplateTreeLab == NULL) {
        pTemplateTreeLab = new TemplateTreeLab();
    }
    return pTemplateTreeLab;
}

void TemplateTreeLab::addTemplateTree(TemplateTree t)
{
    QSqlQuery query(mbh->db);
    //uuid varchar(50), fatheruuid varchar(50), title  varchar(20), colors  varchar(300), iscategory varchar(10)
    query.prepare("INSERT INTO TEMPLATETREE VALUES(:uuid, :fatheruuid, :title,:colors, :iscategory)");
    query.bindValue(":uuid",t.getId());
    query.bindValue(":fatheruuid",t.getFatherId());
    query.bindValue(":title",t.getTitle());
    query.bindValue(":colors",t.getColors());
    query.bindValue(":iscategory",t.getIsCategory());
    bool success = query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        QMessageBox msgBox;
        msgBox.setText("database insert failed:" + lastError.driverText() + lastError.databaseText());
        msgBox.exec();
        return;
    }
}

void TemplateTreeLab::deleteTemplateTreeByUuid(QString t)
{
    QSqlQuery query(mbh->db);
    query.prepare("DELETE FROM TEMPLATETREE WHERE uuid = ?");
    query.bindValue(0,t);
    bool success = query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        QMessageBox msgBox;
        msgBox.setText("database delete failed:" + lastError.driverText() + lastError.databaseText());
        msgBox.exec();
    }
}
void TemplateTreeLab::deleteAllTemplateTree()
{
    //先查出所有的记录，再根据uuid删除
    QList<TemplateTree> listtpt;
    listtpt = getTemplateTrees();
    for(int i = 0;i<listtpt.size();i++)
    {
        TemplateTree tpt = listtpt.at(i);
        QString uuid = tpt.getId();
        deleteTemplateTreeByUuid(uuid);
    }

}

QList<TemplateTree> TemplateTreeLab::getTemplateTrees()
{
    QList<TemplateTree> listtp;
    QSqlQuery query(mbh->db);
    bool success = query.exec("SELECT * FROM TEMPLATETREE");

    if(!success)
    {
        QSqlError lastError = query.lastError();
        QMessageBox msgBox;
        msgBox.setText("database query failed:" + lastError.driverText() + lastError.databaseText());
        msgBox.exec();
    }
    while(query.next())
    {
        //uuid varchar(50), fatheruuid varchar(50), title  varchar(20), colors  varchar(300), iscategory varchar(10)
        QUuid uuid(query.value(0).toString());
        TemplateTree tp(uuid.toString());
        tp.setFatherId(query.value(1).toString());
        tp.setTitle(query.value(2).toString());
        tp.setColors(query.value(3).toString());
        tp.setIsCategory(query.value(4).toString());
        listtp.append(tp);
    }
    return listtp;
}

TemplateTree TemplateTreeLab::getTemplateTreeByUuid(QString id)
{
    QSqlQuery query(mbh->db);
    query.prepare("SELECT * FROM TEMPLATETREE WHERE uuid = ?");
    query.bindValue(0,id);
    bool success = query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        QMessageBox msgBox;
        msgBox.setText("database query failed:" + lastError.driverText() + lastError.databaseText());
        msgBox.exec();
    }
    TemplateTree tpn;
    if(query.next())
    {
        QUuid uuid(query.value(0).toString());
        TemplateTree tp(uuid.toString());
        tp.setFatherId(query.value(1).toString());
        tp.setTitle(query.value(2).toString());
        tp.setColors(query.value(3).toString());
        tp.setIsCategory(query.value(4).toString());
        return tp;
    }
    else
        return tpn;
}


QList<TemplateTree> TemplateTreeLab::getTemplateTreeByFatherUuid(QString uuid)
{
    QList<TemplateTree> listtp;
    QSqlQuery query(mbh->db);
    query.prepare("SELECT * FROM TEMPLATETREE WHERE fatheruuid = ?");
    query.bindValue(0,uuid);
    bool success = query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        QMessageBox msgBox;
        msgBox.setText("database query failed:" + lastError.driverText() + lastError.databaseText());
        msgBox.exec();
    }

    while(query.next())
    {
        //uuid varchar(50), fatheruuid varchar(50), title  varchar(20), colors  varchar(300), iscategory varchar(10)
        QUuid uuid(query.value(0).toString());
        TemplateTree tp(uuid.toString());
        tp.setFatherId(query.value(1).toString());
        tp.setTitle(query.value(2).toString());
        //tp.setColors(query.value(3).toString());   //获取颜色,目前会有问题，先注释掉
        tp.setIsCategory(query.value(4).toString());
        listtp.append(tp);
    }
    return listtp;
}

void TemplateTreeLab::updateTemplateTreebyUuid(TemplateTree t)
{
    QSqlQuery query(mbh->db);
    query.prepare("UPDATE TEMPLATETREE SET fatheruuid = ?, title = ?,colors = ? iscategory = ? WHERE uuid = ?");
    query.bindValue(0,t.getFatherId());
    query.bindValue(1,t.getTitle());
    query.bindValue(2,t.getColors());
    query.bindValue(3,t.getIsCategory());
    query.bindValue(4,t.getId());
    bool success = query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        QMessageBox msgBox;
        msgBox.setText("database update failed:" + lastError.driverText() + lastError.databaseText());
        msgBox.exec();
    }
}

void TemplateTreeLab::updateTemplateTreeTitlebyUuid(TemplateTree t)
{
    QSqlQuery query(mbh->db);
    query.prepare("UPDATE TEMPLATETREE SET title = ? WHERE uuid = ?");
    query.bindValue(0,t.getTitle());
    query.bindValue(1,t.getId());
    bool success = query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        QMessageBox msgBox;
        msgBox.setText("database update failed:" + lastError.driverText() + lastError.databaseText());
        msgBox.exec();
    }
}

void TemplateTreeLab::DeleteTemplateTreeNode(QString t)
{
    TemplateTree rootnodedelete = getTemplateTreeByUuid(t);
    deleteTemplateTreeByUuid(rootnodedelete.getId());
    RecursiveDelete(t);
}

void TemplateTreeLab::RecursiveDelete(QString t)
{
    QList<TemplateTree> tptl = getTemplateTreeByFatherUuid(t);
    for(int i = 0;i<tptl.size();i++)
    {
        TemplateTree tpt = tptl.at(i);

        deleteTemplateTreeByUuid(tpt.getId());

        RecursiveDelete(tpt.getId());
    }
}

