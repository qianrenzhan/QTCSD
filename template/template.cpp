#include "template.h"

Template::Template()
{
    mId = QUuid::createUuid().toString();
    mTitle = "Default";
    for(int i = 0;i<50;i++){
        for(int j = 0;j<3;j++){
            mSequence[i][j] = 0;
        }
    }
    mColorsRead = "";
    mNumber = 0;
}

Template::Template(QString uuid)
{
    mId = uuid;
    mTitle = "Default";
    for(int i = 0;i<50;i++){
        for(int j = 0;j<3;j++){
            mSequence[i][j] = 0;
        }
    }
    mColorsRead = "";
    mNumber = 0;
}

void Template::updateSequenceAndColor()
{
    //根据mColors更新sequence的值
    QStringList ss = mColors.split(",");

    mNumber = (ss.size()-1)/3;
    if(mNumber > 50) mNumber = 50;
    for(int i = 0;i<mNumber*3;i++)
        mSequence[i/3][i%3] = ss.at(i).toInt();

    ColorSpaceLab* pcsl = ColorSpaceLab::get();
    QList<ColorSpace> colorSpaces = pcsl->getColorSpaces();    //
    if(colorSpaces.size() == 0)
        return;

    mColorsRead = "";
    QString color = "";
    for(int i = 0;i<mNumber;i++)
    {
        int l = mSequence[i][0];
        int a = mSequence[i][1];
        int b = mSequence[i][2];

        int min_index = 0;
        double min_distance = 400000;

        for(int i = 0;i<colorSpaces.size();i++){
            ColorSpace cs = colorSpaces.at(i);
            int temp_l = cs.getL().toInt();
            int temp_a = cs.getA().toInt();
            int temp_b = cs.getB().toInt();

            double distance = (l - temp_l) * (l - temp_l) * 0.1+
                    (a-temp_a) * (a-temp_a) +
                    (b-temp_b) * (b-temp_b);

            if (distance < min_distance) {
                min_index = i;
                min_distance = distance;
            }
        }
        ColorSpace csreturn = colorSpaces.at(min_index);
        color = csreturn.getColorName();
        mColorsRead = mColorsRead + color;
    }
}
