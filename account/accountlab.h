#ifndef ACCOUNTLAB_H
#define ACCOUNTLAB_H

#include "account.h"

class DataBaseHelper;


class AccountLab
{
public:
    AccountLab();
    static AccountLab* get();
    DataBaseHelper* mbh;

private:
    static AccountLab* pAccountLab;

public:
    bool isAccountExist(QString jobNumber);
    bool verification(QString jobNumber, QString password);
    void addAccount(Account account);
    void deleteAccount(QString jobNumber);
    void updatePassword(Account account);

    void writeCurrentAccount(Account account);
    Account readCurrentAccount();
};

#endif // ACCOUNTLAB_H
