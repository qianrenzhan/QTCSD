#ifndef TASKSTATISTICSLAB_H
#define TASKSTATISTICSLAB_H

#include <QList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QUuid>
#include <QMessageBox>
#include <QDateTime>
#include "taskstatistics.h"
#include "databasehelper.h"

class TaskStatisticsLab
{
public:
    TaskStatisticsLab();
    static TaskStatisticsLab* get();
    DataBaseHelper* mbh;

private:
    static TaskStatisticsLab* pTaskStatisticsLab;

    //写数据库相关
    void Inspect(QString taskuuid, QString templateuuid, bool is);
    void updatestoptime(QString jobnumber, QString taskuuid);
    QString login(QString jobnumber);

    //读数据库相关
    TaskStatistics getonepersoncurrentdata(QString jobnumber, QString taskuuid);   //获取到此人当前检测内容，传入参数：jobnumber和taskuuid 起止时间
    QList<TaskStatistics> getonepersonalldata(QString jobnumber);    //获取到此人所有检测内容，传入参数：jobnumber 起止时间
    QList<TaskStatistics> getallpersonalldata();    //获取到所有人所以检测内容，传入参数：起止时间

    //处理函数，把原始数据处理成需要绘图的格式


};

#endif // TASKSTATISTICSLAB_H
