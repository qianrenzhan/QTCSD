#include "colorspacelab.h"

ColorSpaceLab* ColorSpaceLab::pColorSpaceLab = NULL;

ColorSpaceLab::ColorSpaceLab()
{
    mbh = DataBaseHelper::get();
}

ColorSpaceLab* ColorSpaceLab::get()
{
    if (pColorSpaceLab == NULL) {
        pColorSpaceLab = new ColorSpaceLab();
    }
    return pColorSpaceLab;
}

void ColorSpaceLab::addColorSpace(ColorSpace c)      //测试通过
{
    QSqlQuery query(mbh->db);
    query.prepare("INSERT INTO COLORSPACE VALUES(:_id,:uuid,:color_name,:l,:a,:b)");
    query.bindValue(":uuid",c.getId().toString());
    query.bindValue(":color_name",c.getColorName());
    query.bindValue(":l",c.getL());
    query.bindValue(":a",c.getA());
    query.bindValue(":b",c.getB());
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

void ColorSpaceLab::deleteColorSpaceByUuid(ColorSpace c)   //测试通过
{
    QSqlQuery query(mbh->db);
    query.prepare("DELETE FROM COLORSPACE WHERE uuid = ?");
    query.bindValue(0,c.getId().toString());
    bool success = query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        QMessageBox msgBox;
        msgBox.setText("database delete failed:" + lastError.driverText() + lastError.databaseText());
        msgBox.exec();
    }
}

void ColorSpaceLab::deleteAllColorSpace()             //测试通过
{
    //先查出所有的记录，再根据uuid删除
    QList<ColorSpace> listcs;
    listcs = getColorSpaces();
    for(int i = 0;i<listcs.size();i++)
        deleteColorSpaceByUuid(listcs.at(i));
}

QList<ColorSpace> ColorSpaceLab::getColorSpaces()           //测试通过
{
    QList<ColorSpace> listcs;
    QSqlQuery query(mbh->db);
    query.exec("SELECT * FROM COLORSPACE");

    while(query.next())
    {
        QUuid uuid(query.value(1).toString());
        ColorSpace cs(uuid);
        cs.setColorName(query.value(2).toString());
        cs.setL(query.value(3).toString());
        cs.setA(query.value(4).toString());
        cs.setB(query.value(5).toString());
        listcs.append(cs);
    }
    return listcs;
}

ColorSpace ColorSpaceLab::getColorSpaceByUuid(QUuid id)       //测试通过
{
    QSqlQuery query(mbh->db);
    query.prepare("SELECT * FROM COLORSPACE WHERE uuid = ?");
    query.bindValue(0,id.toString());
    query.exec();
    QUuid uuid(query.value(1).toString());
    ColorSpace cs(uuid);
    if(query.next())
    {
        cs.setColorName(query.value(2).toString());
        cs.setL(query.value(3).toString());
        cs.setA(query.value(4).toString());
        cs.setB(query.value(5).toString());
        return cs;
    }
    else
        return cs;
}

ColorSpace ColorSpaceLab::getColorSpaceByColorName(QString color)      //测试通过
{
    QSqlQuery query(mbh->db);
    query.prepare("SELECT * FROM COLORSPACE WHERE color_name = ?");
    query.bindValue(0,color);
    bool success = query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        QMessageBox msgBox;
        msgBox.setText("database query failed:" + lastError.driverText() + lastError.databaseText());
        msgBox.exec();
    }
    if(query.next())
    {
        QUuid uuid(query.value(1).toString());
        ColorSpace cs(uuid);
        cs.setColorName(query.value(2).toString());
        cs.setL(query.value(3).toString());
        cs.setA(query.value(4).toString());
        cs.setB(query.value(5).toString());
        return cs;
    }
    else
    {
        ColorSpace cs(QUuid::createUuid());
        cs.setA("0");
        return cs;
    }
}

void ColorSpaceLab::updateColorSpacebyUuid(ColorSpace colorSpace)    //测试通过
{
    QSqlQuery query(mbh->db);
    query.prepare("UPDATE COLORSPACE SET color_name = ?,l = ?, a = ?, b = ?  WHERE uuid = ?");
    query.bindValue(0,colorSpace.getColorName());
    query.bindValue(1,colorSpace.getL());
    query.bindValue(2,colorSpace.getA());
    query.bindValue(3,colorSpace.getB());
    query.bindValue(4,colorSpace.getId().toString());
    bool success = query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        QMessageBox msgBox;
        msgBox.setText("database update failed:" + lastError.driverText() + lastError.databaseText());
        msgBox.exec();
    }
}

void ColorSpaceLab::updateColorSpacebyName(ColorSpace colorSpace)   //测试通过
{
    QSqlQuery query(mbh->db);
    query.prepare("UPDATE COLORSPACE SET l = ?, a = ?, b = ?  WHERE color_name = ?");
    query.bindValue(0,colorSpace.getL());
    query.bindValue(1,colorSpace.getA());
    query.bindValue(2,colorSpace.getB());
    query.bindValue(3,colorSpace.getColorName());
    bool success = query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        QMessageBox msgBox;
        msgBox.setText("database update failed:" + lastError.driverText() + lastError.databaseText());
        msgBox.exec();
    }
}

QString ColorSpaceLab::AnalyzeColor(int l, int a, int b)
{
    if(pColorSpaceLab == NULL)
        return "";

    QList<ColorSpace> colorSpaces = pColorSpaceLab->getColorSpaces();
    int min_index = 0;
    double min_distance = 400000;

    if(colorSpaces.size() == 0)
        return "";

    for(int i = 0;i<colorSpaces.size();i++){
        ColorSpace cs = colorSpaces.at(i);
        int temp_l = cs.getL().toInt();
        int temp_a = cs.getA().toInt();
        int temp_b = cs.getB().toInt();

        double distance = (l - temp_l) * (l - temp_l) +
                (a-temp_a) * (a-temp_a) +
                (b-temp_b) * (b-temp_b);

        if (distance < min_distance) {
            min_index = i;
            min_distance = distance;
        }
    }
    ColorSpace csreturn = colorSpaces.at(min_index);
    return csreturn.getColorName();
}
