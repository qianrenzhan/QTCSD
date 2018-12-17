#ifndef LOGINDLG_H
#define LOGINDLG_H

#include <QDialog>
#include <QTextCodec>
#include <QMessageBox>
#include <QEvent>

#include "accountlab.h"


class SignUpDialog;

namespace Ui {
class LoginDlg;
}

class LoginDlg : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDlg(QWidget *parent = 0);
    ~LoginDlg();

protected:
    void changeEvent(QEvent *e);

private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_sure_clicked();

    void on_pushButton_signup_clicked();

private:
    Ui::LoginDlg *ui;
    SignUpDialog *signupdlg;
};

#endif // LOGINDLG_H
