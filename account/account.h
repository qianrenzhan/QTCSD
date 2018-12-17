#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QString>

class Account
{
public:
    Account();

    void setJobNumber(QString jobNumber){ mJobNumber = jobNumber;}
    void setPassword(QString password){mPassword = password;}
    void setIdentify(QString identity){mIdentify = identity;}
    void setDepartment(QString department){mDepartment = department;}

    QString getJobNumber(){return mJobNumber;}
    QString getPassword(){return mPassword;}
    QString getIdentify(){return mIdentify;}
    QString getDepartment(){return mDepartment;}

//    bool isAccountExist(QString jobNumber)
//    {

//    }


private:
    QString mJobNumber;
    QString mPassword;
    QString mIdentify;
    QString mDepartment;
};

#endif // ACCOUNT_H
