#ifndef SIGNUPDIALOG_H
#define SIGNUPDIALOG_H

#include <QDialog>

#include "account.h"
#include "accountlab.h"

class QSqlTableModel;

namespace Ui {
class SignUpDialog;
}

class SignUpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SignUpDialog(QWidget *parent = 0);
    ~SignUpDialog();

    QSqlTableModel *model;

private slots:

    void on_btn_signup_clicked();

    void on_btn_cancel_clicked();

    void on_btn_query_clicked();

    void on_btn_delete_current_clicked();

    void on_btn_confirm_change_clicked();

private:
    Ui::SignUpDialog *ui;
};

#endif // SIGNUPDIALOG_H
