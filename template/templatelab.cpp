#include "templatelab.h"
#include <QMessageBox>
//#include <QList>
//#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>



TemplateLab* TemplateLab::pTemplateLab = NULL;

TemplateLab::TemplateLab()
{
    mbh = DataBaseHelper::get();
}

TemplateLab* TemplateLab::get()
{
    if (pTemplateLab == NULL) {
        pTemplateLab = new TemplateLab();
    }
    return pTemplateLab;
}

void TemplateLab::addTemplate(Template t)
{
    QSqlQuery query(mbh->db);
    query.prepare("INSERT INTO TEMPLATE VALUES(:_id,:uuid,:title,:colors)");
    query.bindValue(":uuid",t.getId());
    query.bindValue(":title",t.getTitle());
    query.bindValue(":colors",t.getColors());
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

void TemplateLab::deleteTemplateByUuid(Template t)
{
    QSqlQuery query(mbh->db);
    query.prepare("DELETE FROM TEMPLATE WHERE uuid = ?");
    query.bindValue(0,t.getId());
    bool success = query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        QMessageBox msgBox;
        msgBox.setText("database delete failed:" + lastError.driverText() + lastError.databaseText());
        msgBox.exec();
    }
}
void TemplateLab::deleteAllTemplate()
{
    //先查出所有的记录，再根据uuid删除
    QList<Template> listtp;
    listtp = getTemplates();
    for(int i = 0;i<listtp.size();i++)
        deleteTemplateByUuid(listtp.at(i));
}

QList<Template> TemplateLab::getTemplates()
{
    QList<Template> listtp;
    QSqlQuery query(mbh->db);
    bool success = query.exec("SELECT * FROM TEMPLATE");

    if(!success)
    {
        QSqlError lastError = query.lastError();
        QMessageBox msgBox;
        msgBox.setText("database query failed:" + lastError.driverText() + lastError.databaseText());
        msgBox.exec();
    }
    while(query.next())
    {
        QUuid uuid(query.value(1).toString());
        Template tp(uuid.toString());
        tp.setTitle(query.value(2).toString());
        tp.setColors(query.value(3).toString());
        listtp.append(tp);
    }
    return listtp;
}

Template TemplateLab::getTemplateByUuid(QUuid id)
{
    QSqlQuery query(mbh->db);
    query.prepare("SELECT * FROM TEMPLATE WHERE uuid = ?");
    query.bindValue(0,id.toString());
    bool success = query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        QMessageBox msgBox;
        msgBox.setText("database query failed:" + lastError.driverText() + lastError.databaseText());
        msgBox.exec();
    }
    QUuid uuid(query.value(1).toString());
    Template tp(uuid.toString());
    if(query.next())
    {
        tp.setTitle(query.value(2).toString());
        tp.setColors(query.value(3).toString());
        return tp;
    }
    else
        return tp;
}

void TemplateLab::updateTemplatebyUuid(Template t)
{
    QSqlQuery query(mbh->db);
    query.prepare("UPDATE TEMPLATE SET title = ?,colors = ? WHERE uuid = ?");
    query.bindValue(0,t.getTitle());
    query.bindValue(1,t.getColors());
    query.bindValue(2,t.getId());
    bool success = query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        QMessageBox msgBox;
        msgBox.setText("database update failed:" + lastError.driverText() + lastError.databaseText());
        msgBox.exec();
    }
}
