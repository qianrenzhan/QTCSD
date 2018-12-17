#ifndef TEMPLATETREELAB_H
#define TEMPLATETREELAB_H

#include "templatetree.h"

class DataBaseHelper;

class TemplateTreeLab
{
public:
    TemplateTreeLab();

    static TemplateTreeLab* get();
    DataBaseHelper* mbh;

private:
    static TemplateTreeLab* pTemplateTreeLab;

public:
    void addTemplateTree(TemplateTree t);
    void deleteTemplateTreeByUuid(QString t);
    void deleteAllTemplateTree();
    void DeleteTemplateTreeNode(QString t);
    void RecursiveDelete(QString t);

    QList<TemplateTree> getTemplateTrees();
    TemplateTree getTemplateTreeByUuid(QString id);
    QList<TemplateTree> getTemplateTreeByFatherUuid(QString uuid);

    void updateTemplateTreebyUuid(TemplateTree t);
    void updateTemplateTreeTitlebyUuid(TemplateTree t);

};

#endif // TEMPLATETREELAB_H
