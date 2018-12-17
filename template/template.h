#ifndef TEMPLATE_H
#define TEMPLATE_H
#include <QUuid>
#include <QString>
#include <QStringList>
#include "colorspacelab.h"

class Template
{
public:
    Template();
    Template(QString uuid);

    QString getId(){ return mId;}
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

    QString getColorsRead() {
        return mColorsRead;
    }

    void updateSequenceAndColor();

private:
    QString mId;
    QString mTitle;
    QString mColors;         //只能在程序中设置

    //设置mColors后自动更新，只读
    int mSequence[50][3];
    int mNumber;
    QString mColorsRead;
};

#endif // TEMPLATE_H
