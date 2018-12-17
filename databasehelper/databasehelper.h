#ifndef BASEHELPER_H
#define BASEHELPER_H

#include <QSqlDatabase>

class DataBaseHelper
{
public:
    DataBaseHelper();
    static DataBaseHelper* get();
    QSqlDatabase db;
private:
    static DataBaseHelper* pBaseHelper;


};

#endif // BASEHELPER_H
