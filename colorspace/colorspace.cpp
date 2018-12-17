#include "colorspace.h"

ColorSpace::ColorSpace()
{
    mId = QUuid::createUuid().toString();
    mColorName = "A";
    mL = "0";
    mA = "127";
    mB = "127";
}

ColorSpace::ColorSpace(QUuid uuid)
{
    mId = uuid.toString();
    mColorName = "A";
    mL = "0";
    mA = "127";
    mB = "127";
}
