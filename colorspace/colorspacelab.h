#ifndef COLORSPACELAB_H
#define COLORSPACELAB_H
#include <QList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "colorspace.h"
#include "databasehelper.h"

class ColorSpaceLab
{
public:
    ColorSpaceLab();
    static ColorSpaceLab* get();
    DataBaseHelper* mbh;

private:
    static ColorSpaceLab* pColorSpaceLab;

public:
    void addColorSpace(ColorSpace c);
    void deleteColorSpaceByUuid(ColorSpace c);
    void deleteAllColorSpace();
    QList<ColorSpace> getColorSpaces();
    ColorSpace getColorSpaceByUuid(QUuid id);
    ColorSpace getColorSpaceByColorName(QString color);
    void updateColorSpacebyUuid(ColorSpace colorSpace);
    void updateColorSpacebyName(ColorSpace colorSpace);
    static QString AnalyzeColor(int l, int a, int b);
};

#endif // COLORSPACELAB_H
