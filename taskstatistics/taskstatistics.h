#ifndef TASKSTATISTICS_H
#define TASKSTATISTICS_H

#include <QString>
#include <QList>
#include <QUuid>

class Task
{
public:
    Task();
    QString mTemplateUuid;
    QString mNumber;
    QString mTotalNumber;
    QString mTitle;
};

class TaskStatistics
{
public:
    TaskStatistics();

public:
    QString mJobNumber;
    QString mStartTime;
    QString mStopTime;
    QString mTaskUuid;

    QList<Task> mTasks;
    int tasknumber;   //更新mTasks时更改
};




#endif // TASKSTATISTICS_H
