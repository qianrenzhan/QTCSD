#ifndef TEMPLATELAB_H
#define TEMPLATELAB_H

#include <QList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include "template.h"
#include "databasehelper.h"

class TemplateLab
{
public:
    TemplateLab();

    static TemplateLab* get();
    DataBaseHelper* mbh;

private:
    static TemplateLab* pTemplateLab;

public:
    void addTemplate(Template t);
    void deleteTemplateByUuid(Template t);
    void deleteAllTemplate();
    QList<Template> getTemplates();
    Template getTemplateByUuid(QUuid id);
    //Template getTemplateByColorName(QString color);
    void updateTemplatebyUuid(Template t);
    //void updateTemplatebyName(Template t);
};

#endif // TEMPLATELAB_H
