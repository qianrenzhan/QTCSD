#include "accountlab.h"


AccountLab* AccountLab::pAccountLab = NULL;

AccountLab::AccountLab()
{
    mbh = DataBaseHelper::get();
}

AccountLab* AccountLab::get()
{
    if (pAccountLab == NULL) {
        pAccountLab = new AccountLab();
    }
    return pAccountLab;
}

bool AccountLab::isAccountExist(QString jobNumber)
{
    QSqlQuery query(mbh->db);
    //jobnumber varchar(50), password varchar(50), department varchar(50), identify varchar(50)
    query.prepare("SELECT * FROM ACCOUNT WHERE jobnumber = ?");
    query.bindValue(0,jobNumber);
    bool success = query.exec();

    if(!success)
    {
        QSqlError lastError = query.lastError();
        QMessageBox msgBox;
        msgBox.setText("database query failed:" + lastError.driverText() + lastError.databaseText());
        msgBox.exec();
        return false;
    }

    if(query.next())
        return true;
    else
        return false;
}

bool AccountLab::verification(QString jobNumber, QString password)
{
    QSqlQuery query(mbh->db);
    //jobnumber varchar(50), password varchar(50), department varchar(50), identify varchar(50)
    query.prepare("SELECT * FROM ACCOUNT WHERE jobnumber = ? and password = ?");
    query.bindValue(0,jobNumber);
    query.bindValue(1,password);
    bool success = query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        QMessageBox msgBox;
        msgBox.setText("database query failed:" + lastError.driverText() + lastError.databaseText());
        msgBox.exec();
        return false;
    }

    if(query.next())
        return true;
    else
        return false;
}

void AccountLab::addAccount(Account account)
{
    QSqlQuery query(mbh->db);
    //jobnumber varchar(50), password varchar(50), department varchar(50), identify varchar(50)
    query.prepare("INSERT INTO ACCOUNT VALUES(:jobnumber,:password,:department,:identify)");
    query.bindValue(":jobnumber",account.getJobNumber());
    query.bindValue(":password",account.getPassword());
    query.bindValue(":department",account.getDepartment());
    query.bindValue(":identify",account.getIdentify());
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

void AccountLab::deleteAccount(QString jobNumber)
{
    QSqlQuery query(mbh->db);
    //jobnumber varchar(50), password varchar(50), department varchar(50), identify varchar(50)
    query.prepare("DELETE FROM ACCOUNT WHERE jobnumber = ?");
    query.bindValue(0,jobNumber);
    bool success = query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        QMessageBox msgBox;
        msgBox.setText("database delete failed:" + lastError.driverText() + lastError.databaseText());
        msgBox.exec();
    }
}

void AccountLab::updatePassword(Account account)
{
    QSqlQuery query(mbh->db);
    //jobnumber varchar(50), password varchar(50), department varchar(50), identify varchar(50)
    query.prepare("UPDATE ACCOUNT SET password = ?  WHERE jobnumber = ?");
    query.bindValue(0,account.getPassword());
    query.bindValue(1,account.getJobNumber());
    bool success = query.exec();
    if(!success)
    {
        QSqlError lastError = query.lastError();
        QMessageBox msgBox;
        msgBox.setText("database update failed:" + lastError.driverText() + lastError.databaseText());
        msgBox.exec();
    }
}

void AccountLab::writeCurrentAccount(Account account)
{
    QSqlQuery query(mbh->db);
    query.prepare("UPDATE CURRENTACCOUNT SET jobnumber = ?  WHERE id = '1'");
    query.bindValue(0,account.getJobNumber());
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

Account AccountLab::readCurrentAccount()
{
    Account account;
    account.setJobNumber("default");
    QSqlQuery query(mbh->db);
    bool success = query.exec("SELECT * FROM CURRENTACCOUNT WHERE id = '1'");
    if(!success)
    {
        QSqlError lastError = query.lastError();
        QMessageBox msgBox;
        msgBox.setText("database insert failed:" + lastError.driverText() + lastError.databaseText());
        msgBox.exec();
    }
    else
    {
        if(query.next())
            account.setJobNumber(query.value(1).toString());
    }
    return account;
}
