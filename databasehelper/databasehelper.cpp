#include "databasehelper.h"

#include <QMessageBox>

#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
//#include <QVariant>
//#include <QString>

DataBaseHelper* DataBaseHelper::pBaseHelper = NULL;

DataBaseHelper* DataBaseHelper::get()
{
    if (pBaseHelper == NULL) {
        pBaseHelper = new DataBaseHelper();
    }
    return pBaseHelper;
}

DataBaseHelper::DataBaseHelper()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setDatabaseName("csd");       //这里输入你的数据库名
    db.setUserName("gwj");
    db.setPassword("12345678");   //这里输入你的密码    12345678

    if (!db.open())
    {
        QMessageBox::critical(0, QObject::tr("can't open database"),
        "can't connect to database！ ", QMessageBox::Cancel);
        return;
    }

    //颜色空间表
    //查看表是否存在
    QSqlQuery query(db);
    query.exec("SELECT table_name FROM information_schema.TABLES WHERE table_name ='COLORSPACE'");
    if(query.next())
    {
       QVariant var = query.value(0);
       QString str = var.toString();
    }
    else
    {
        bool success = query.exec("CREATE TABLE COLORSPACE (_id int primary key AUTO_INCREMENT, uuid  varchar(50), color_name  varchar(20), l  varchar(10), a  varchar(10), b  varchar(10))");
        QMessageBox msgBox;
        if(success)
            msgBox.setText("The data table was created successfully.");
        else
            msgBox.setText("Cannot create data table");
        msgBox.exec();
    }

    //模板表
    query.exec("SELECT table_name FROM information_schema.TABLES WHERE table_name ='TEMPLATE'");
    if(query.next())
    {
       QVariant var = query.value(0);
       QString str = var.toString();
    }
    else
    {
        bool success = query.exec("CREATE TABLE TEMPLATE (_id int primary key AUTO_INCREMENT, uuid  varchar(50), title  varchar(20), colors  varchar(300))");
        QMessageBox msgBox;
        if(success)
            msgBox.setText("The data table was created successfully.");
        else
            msgBox.setText("Cannot create data table");
        msgBox.exec();
    }


    //模板树表
    query.exec("SELECT table_name FROM information_schema.TABLES WHERE table_name ='TEMPLATETREE'");
    if(query.next())
    {
       QVariant var = query.value(0);
       QString str = var.toString();
    }
    else
    {
        bool success = query.exec("CREATE TABLE TEMPLATETREE (uuid varchar(50), fatheruuid varchar(50), title  varchar(20), colors  varchar(300), iscategory varchar(10))");
        QMessageBox msgBox;
        if(success)
            msgBox.setText("The data table was created successfully.");
        else
            msgBox.setText("Cannot create data table");
        msgBox.exec();
    }

    //current账户表
    query.exec("SELECT table_name FROM information_schema.TABLES WHERE table_name ='CURRENTACCOUNT'");
    if(query.next())
    {
       QVariant var = query.value(0);
       QString str = var.toString();
    }
    else
    {
        bool success = query.exec("CREATE TABLE CURRENTACCOUNT (id varchar(50), jobnumber varchar(50))");
        QMessageBox msgBox;
        if(success)
            msgBox.setText("The data table was created successfully.");
        else
            msgBox.setText("Cannot create data table");
        msgBox.exec();
    }

    //
    query.exec("SELECT * FROM CURRENTACCOUNT WHERE id ='1'");
    if(!query.next())
    {
        query.prepare("INSERT INTO CURRENTACCOUNT VALUES(:id,:jobnumber)");
        query.bindValue(":id","1");
        query.bindValue(":jobnumber","default");
        bool success = query.exec();
        QMessageBox msgBox;
        if(success)
            msgBox.setText("add record successfully.");
        else
            msgBox.setText("Cannot add record");
        msgBox.exec();
    }



    //账户表
    query.exec("SELECT table_name FROM information_schema.TABLES WHERE table_name ='ACCOUNT'");
    if(query.next())
    {
       QVariant var = query.value(0);
       QString str = var.toString();
    }
    else
    {
        bool success = query.exec("CREATE TABLE ACCOUNT (jobnumber varchar(50), password varchar(50), department varchar(50), identify varchar(50))");
        QMessageBox msgBox;
        if(success)
            msgBox.setText("The data table was created successfully.");
        else
            msgBox.setText("Cannot create data table");
        msgBox.exec();
    }

    //部门表
    query.exec("SELECT table_name FROM information_schema.TABLES WHERE table_name ='DEPARTMENT'");
    if(query.next())
    {
       QVariant var = query.value(0);
       QString str = var.toString();
    }
    else
    {
        bool success = query.exec("CREATE TABLE DEPARTMENT (id varchar(50), departmentname varchar(50))");
        query.exec("INSERT INTO DEPARTMENT VALUES('0','department0')");
        query.exec("INSERT INTO DEPARTMENT VALUES('1','department1')");
        query.exec("INSERT INTO DEPARTMENT VALUES('2','department2')");
        QMessageBox msgBox;
        if(success)
            msgBox.setText("The data table was created successfully.");
        else
            msgBox.setText("Cannot create data table");
        msgBox.exec();
    }

    //身份表
    query.exec("SELECT table_name FROM information_schema.TABLES WHERE table_name ='IDENTIFY'");
    if(query.next())
    {
       QVariant var = query.value(0);
       QString str = var.toString();
    }
    else
    {
        bool success = query.exec("CREATE TABLE IDENTIFY (id varchar(50), identifyname varchar(50))");
        if(!success)
        {
            QSqlError lastError = query.lastError();
            QMessageBox msgBox;
            msgBox.setText("database insert failed:" + lastError.driverText() + lastError.databaseText());
            msgBox.exec();
            return;
        }

        success = query.exec("INSERT INTO IDENTIFY VALUES('0','Developer')");
        success = query.exec("INSERT INTO IDENTIFY VALUES('1','Administrator')");
        success = query.exec("INSERT INTO IDENTIFY VALUES('2','Engineer')");
        success = query.exec("INSERT INTO IDENTIFY VALUES('3','Operator')");
        QMessageBox msgBox;
        if(success)
            msgBox.setText("The data table was created successfully.");
        else
            msgBox.setText("Cannot create data table");
        msgBox.exec();
    }

    //任务统计表
    query.exec("SELECT table_name FROM information_schema.TABLES WHERE table_name ='TASKSTATISTICS'");
    if(query.next())
    {
       QVariant var = query.value(0);
       QString str = var.toString();
    }
    else
    {
        bool success = query.exec("CREATE TABLE TASKSTATISTICS (jobnumber varchar(50), starttime varchar(50), stoptime varchar(50), taskuuid varchar(50))");
        if(!success)
        {
            QSqlError lastError = query.lastError();
            QMessageBox msgBox;
            msgBox.setText("database insert failed:" + lastError.driverText() + lastError.databaseText());
            msgBox.exec();
            return;
        }

        QMessageBox msgBox;
        if(success)
            msgBox.setText("The data table was created successfully.");
        else
            msgBox.setText("Cannot create data table");
        msgBox.exec();
    }

    //子任务表
    query.exec("SELECT table_name FROM information_schema.TABLES WHERE table_name ='TASK'");
    if(query.next())
    {
       QVariant var = query.value(0);
       QString str = var.toString();
    }
    else
    {
        bool success = query.exec("CREATE TABLE TASK (taskuuid varchar(50), templateuuid varchar(50), totalnumber varchar(50), number varchar(50))");
        if(!success)
        {
            QSqlError lastError = query.lastError();
            QMessageBox msgBox;
            msgBox.setText("database insert failed:" + lastError.driverText() + lastError.databaseText());
            msgBox.exec();
            return;
        }

        QMessageBox msgBox;
        if(success)
            msgBox.setText("The data table was created successfully.");
        else
            msgBox.setText("Cannot create data table");
        msgBox.exec();
    }
}
