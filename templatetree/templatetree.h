#ifndef TEMPLATETREE_H
#define TEMPLATETREE_H

#include <QUuid>
#include <QString>
#include <QStringList>
#include "colorspacelab.h"

class TemplateTree
{
public:
    TemplateTree();
    TemplateTree(QString uuid);

    QString getId(){ return mId;}

    void setId(QUuid uuid){
        mId = uuid.toString();
    }

    QString getFatherId(){ return mFatherId;}

    void setFatherId(QString uuid){
        mFatherId = uuid;
    }

    QString getTitle() {
        return mTitle;
    }

    void setTitle(QString title) {
        mTitle = title;
    }

    void setColors(QString colors) {
        mColors = colors;
        updateSequenceAndColor();
    }

    QString getColors() {
        return mColors;
    }

    void setIsCategory(QString isCategory){
        mIsCategory = isCategory;
    }

    QString getIsCategory(){
        return mIsCategory;
    }

    QString getColorsRead() {
        return mColorsRead;
    }

    void updateSequenceAndColor();

private:
    QString mId;
    QString mFatherId;
    QString mTitle;
    QString mColors;         //只能在程序中设置
    QString mIsCategory;     //0  1

    //设置mColors后自动更新，只读
    int mSequence[50][3];
    int mNumber;
    QString mColorsRead;

};

#endif // TEMPLATETREE_H
