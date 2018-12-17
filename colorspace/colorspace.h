#ifndef COLORSPACE_H
#define COLORSPACE_H

#include <QString>
#include <QUuid>

class ColorSpace
{
private:
    QUuid mId;
    QString mColorName;
    QString mL;
    QString mA;
    QString mB;

public:
    ColorSpace();
    ColorSpace(QUuid uuid);

    QUuid getId(){ return mId;}
    QString getColorName(){return mColorName;}
    QString getL(){return mL;}
    QString getA(){return mA;}
    QString getB(){return mB;}

    void setColorName(QString colorname){mColorName = colorname;}
    void setL(QString l) {mL = l;}
    void setA(QString a) {mA = a;}
    void setB(QString b) {mB = b;}

    void setLABint(int l, int a, int b)
    {
        setL(QString::number(l));
        setA(QString::number(a));
        setB(QString::number(b));
    }
};

#endif // COLORSPACE_H
