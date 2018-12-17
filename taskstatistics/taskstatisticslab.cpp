#include "taskstatisticslab.h"
#include <QList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QUuid>
#include <QMessageBox>
#include <QDateTime>

#include "databasehelper.h"

TaskStatisticsLab* TaskStatisticsLab::pTaskStatisticsLab = NULL;

TaskStatisticsLab::TaskStatisticsLab()
{
    mbh = DataBaseHelper::get();
}

TaskStatisticsLab* TaskStatisticsLab::get()
{
    if (pTaskStatisticsLab == NULL) {
        pTaskStatisticsLab = new TaskStatisticsLab();
    }
    return pTaskStatisticsLab;
}

//jobnumber varchar(50), starttime varchar(50), stoptime varchar(50), taskuuid varchar(50)
//每次登陆，随机生成一个taskuuid，并jobnumber和登录时间保存到数据库。
//程序运行时持有taskuuid，结束时保存退出时间。（每隔1min写一次当前时间到数据库）

//taskuuid varchar(50), templateuuid varchar(50), totalnumber varchar(50), number varchar(50)
//登录后，在该taskuuid下添加任务，
//每检测一个，判断当前taskuuid下的templateuuid是否存在，存在，+1，不存在，新建并赋值1。如果检测成功，


//登录时调用，返回随机生成的taskuuid
QString TaskStatisticsLab::login(QString jobnumber)
{
    //插入一行
    //jobnumber  starttime   starttime   taskuuid
    QString taskuuid = QUuid::createUuid().toString();
    QDateTime time;
    time = QDateTime::currentDateTime();
    QString starttime = time.toString("yyyy-MM-dd hh:mm:ss");

    QSqlQuery query(mbh->db);
    //jobnumber varchar(50), starttime varchar(50), stoptime varchar(50), taskuuid varchar(50)
    query.prepare("INSERT INTO TASKSTATISTICS VALUES(:jobnumber,:starttime,:stoptime,:taskuuid)");
    query.bindValue(":jobnumber",jobnumber);
    query.bindValue(":starttime",starttime);
    query.bindValue(":stoptime",starttime);
    query.bindValue(":taskuuid",taskuuid);
    bool success = query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        QMessageBox msgBox;
        msgBox.setText("database insert failed:" + lastError.driverText() + lastError.databaseText());
        msgBox.exec();
        return "";
    }

    return taskuuid;
}

//更新下线时间调用
void TaskStatisticsLab::updatestoptime(QString jobnumber, QString taskuuid)
{
    //更新一行，根据jobnumber和taskuuid更新结束时间。
    //jobnumber varchar(50), starttime varchar(50), stoptime varchar(50), taskuuid varchar(50)
    QSqlQuery query(mbh->db);
    query.prepare("UPDATE TASKSTATISTICS SET stoptime = ? WHERE jobnumber = ? and taskuuid = ?");

    QDateTime time;
    time = QDateTime::currentDateTime();
    QString stoptime = time.toString("yyyy-MM-dd hh:mm:ss");

    query.bindValue(0,stoptime);
    query.bindValue(1,jobnumber);
    query.bindValue(2,taskuuid);
    bool success = query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        QMessageBox msgBox;
        msgBox.setText("database update failed:" + lastError.driverText() + lastError.databaseText());
        msgBox.exec();
    }
}

//检测后调用
void TaskStatisticsLab::Inspect(QString taskuuid, QString templateuuid, bool is)
{
    //taskuuid varchar(50), templateuuid varchar(50), totalnumber varchar(50), number varchar(50)
    QSqlQuery query(mbh->db);
    query.prepare("SELECT * FROM TASK WHERE taskuuid = ? and templateuuid = ?");   //判断taskuuid和templateuuid所在行是否存在
    query.bindValue(0,taskuuid);
    query.bindValue(1,templateuuid);
    bool success = query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        QMessageBox msgBox;
        msgBox.setText("database query failed:" + lastError.driverText() + lastError.databaseText());
        msgBox.exec();
    }
    if(query.next())   //如果存在，取出，+1
    {
        int totalnumber = query.value(2).toString().toInt();
        int number = query.value(3).toString().toInt();
        if(is){totalnumber++;number++;}
        else{totalnumber++;}

        QSqlQuery query(mbh->db);
        //taskuuid varchar(50), templateuuid varchar(50), totalnumber varchar(50), number varchar(50)
        query.prepare("UPDATE TASK SET totalnumber = ?,number = ? WHERE taskuuid = ? and templateuuid = ?");
        query.bindValue(0,totalnumber);
        query.bindValue(1,number);
        query.bindValue(2,taskuuid);
        query.bindValue(3,templateuuid);
        bool success = query.exec();
        if(!success)
        {
            QSqlError lastError = query.lastError();
            QMessageBox msgBox;
            msgBox.setText("database update failed:" + lastError.driverText() + lastError.databaseText());
            msgBox.exec();
        }
    }
    else   //如果不存在，新建一行，并赋值
    {
        if(is)
        {
            QSqlQuery query(mbh->db);
            //taskuuid varchar(50), templateuuid varchar(50), totalnumber varchar(50), number varchar(50)
            query.prepare("INSERT INTO TASK VALUES(:taskuuid,:templateuuid,:totalnumber,:number)");
            query.bindValue(":taskuuid",taskuuid);
            query.bindValue(":templateuuid",templateuuid);
            query.bindValue(":totalnumber","1");
            query.bindValue(":number","1");
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
        else
        {
            QSqlQuery query(mbh->db);
            //taskuuid varchar(50), templateuuid varchar(50), totalnumber varchar(50), number varchar(50)
            query.prepare("INSERT INTO TASK VALUES(:taskuuid,:templateuuid,:totalnumber,:number)");
            query.bindValue(":taskuuid",taskuuid);
            query.bindValue(":templateuuid",templateuuid);
            query.bindValue(":totalnumber","1");
            query.bindValue(":number","0");
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
    }
}


//从数据库读取统计数据
TaskStatistics TaskStatisticsLab::getonepersoncurrentdata(QString jobnumber, QString taskuuid)
//获取到此人当前检测内容，传入参数：jobnumber和taskuuid 起止时间
{
    TaskStatistics tsts;
    QSqlQuery query(mbh->db);
    //jobnumber varchar(50), starttime varchar(50), stoptime varchar(50), taskuuid varchar(50)
    query.prepare("SELECT * FROM TASKSTATISTICS WHERE jobnumber = ? and taskuuid = ?");
    query.bindValue(0,jobnumber);
    query.bindValue(1,taskuuid);
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
        tsts.mJobNumber = jobnumber;
        tsts.mTaskUuid = taskuuid;
        tsts.mStartTime = query.value(1).toString();
        tsts.mStopTime = query.value(2).toString();

        //查询
        QSqlQuery querynew(mbh->db);
        querynew.prepare("SELECT * FROM TASK WHERE taskuuid = ?");
        querynew.bindValue(0,taskuuid);
        bool success = querynew.exec();
        if(!success)
        {
            QSqlError lastError = querynew.lastError();
            QMessageBox msgBox;
            msgBox.setText("database query failed:" + lastError.driverText() + lastError.databaseText());
            msgBox.exec();
        }
        while(querynew.next())
        {
            //taskuuid varchar(50), templateuuid varchar(50), totalnumber varchar(50), number varchar(50)
            Task task;
            task.mTemplateUuid = querynew.value(1).toString();
            task.mTotalNumber = querynew.value(2).toString();
            task.mNumber = querynew.value(3).toString();
            tsts.mTasks.append(task);
        }
    }

    return tsts;
}

QList<TaskStatistics> TaskStatisticsLab::getonepersonalldata(QString jobnumber)    //获取到此人所有检测内容，传入参数：jobnumber 起止时间
{
    QList<TaskStatistics> ltsts;

    QSqlQuery query(mbh->db);
    //jobnumber varchar(50), starttime varchar(50), stoptime varchar(50), taskuuid varchar(50)
    query.prepare("SELECT * FROM TASKSTATISTICS WHERE jobnumber = ?");
    query.bindValue(0,jobnumber);
    bool success = query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        QMessageBox msgBox;
        msgBox.setText("database query failed:" + lastError.driverText() + lastError.databaseText());
        msgBox.exec();
    }
    while(query.next())
    {
        TaskStatistics tsts;
        tsts.mJobNumber = jobnumber;
        tsts.mTaskUuid = query.value(3).toString();
        tsts.mStartTime = query.value(1).toString();
        tsts.mStopTime = query.value(2).toString();

        //查询
        QSqlQuery querynew(mbh->db);
        querynew.prepare("SELECT * FROM TASK WHERE taskuuid = ?");
        querynew.bindValue(0,tsts.mTaskUuid);
        bool success = querynew.exec();
        if(!success)
        {
            QSqlError lastError = querynew.lastError();
            QMessageBox msgBox;
            msgBox.setText("database query failed:" + lastError.driverText() + lastError.databaseText());
            msgBox.exec();
        }
        while(querynew.next())
        {
            //taskuuid varchar(50), templateuuid varchar(50), totalnumber varchar(50), number varchar(50)
            Task task;
            task.mTemplateUuid = querynew.value(1).toString();
            task.mTotalNumber = querynew.value(2).toString();
            task.mNumber = querynew.value(3).toString();
            tsts.mTasks.append(task);
        }

        ltsts.append(tsts);
    }

    return ltsts;
}

QList<TaskStatistics> TaskStatisticsLab::getallpersonalldata()    //获取到所有人所you检测内容，传入参数：起止时间
{
    QList<TaskStatistics> ltsts;

    QSqlQuery query(mbh->db);
    //jobnumber varchar(50), starttime varchar(50), stoptime varchar(50), taskuuid varchar(50)
    query.prepare("SELECT * FROM TASKSTATISTICS");
    bool success = query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        QMessageBox msgBox;
        msgBox.setText("database query failed:" + lastError.driverText() + lastError.databaseText());
        msgBox.exec();
    }
    while(query.next())
    {
        TaskStatistics tsts;
        tsts.mJobNumber = query.value(0).toString();
        tsts.mTaskUuid = query.value(3).toString();
        tsts.mStartTime = query.value(1).toString();
        tsts.mStopTime = query.value(2).toString();

        //查询
        QSqlQuery querynew(mbh->db);
        querynew.prepare("SELECT * FROM TASK WHERE taskuuid = ?");
        querynew.bindValue(0,tsts.mTaskUuid);
        bool success = querynew.exec();
        if(!success)
        {
            QSqlError lastError = querynew.lastError();
            QMessageBox msgBox;
            msgBox.setText("database query failed:" + lastError.driverText() + lastError.databaseText());
            msgBox.exec();
        }
        while(querynew.next())
        {
            //taskuuid varchar(50), templateuuid varchar(50), totalnumber varchar(50), number varchar(50)
            Task task;
            task.mTemplateUuid = querynew.value(1).toString();
            task.mTotalNumber = querynew.value(2).toString();
            task.mNumber = querynew.value(3).toString();
            tsts.mTasks.append(task);
        }

        ltsts.append(tsts);
    }

    return ltsts;
}

