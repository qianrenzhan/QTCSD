#ifndef COLORSPACELAB_H
#define COLORSPACELAB_H

#include "colorspace.h"

class DataBaseHelper;

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
