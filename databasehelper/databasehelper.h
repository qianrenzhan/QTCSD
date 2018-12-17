#ifndef BASEHELPER_H
#define BASEHELPER_H

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QVariant>
#include <QString>

class QSqlDatabase;

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
