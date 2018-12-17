#include "signupdialog.h"
#include "ui_signupdialog.h"

SignUpDialog::SignUpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SignUpDialog)
{
    ui->setupUi(this);

    //设置模型
    AccountLab* acl = AccountLab::get();

    model = new QSqlTableModel(0,acl->mbh->db);
    model->setTable("ACCOUNT");//指定使用哪个表


    //把model->放在view
    ui->tableView_all_account->setModel(model);
    model->select();
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
}

SignUpDialog::~SignUpDialog()
{
    delete ui;
}

void SignUpDialog::on_btn_signup_clicked()
{
    //读取界面内容
    QString jobnumber = ui->lineEdit_jobnumber->text();
    QString password1 = ui->lineEdit_password->text();
    QString password2 = ui->lineEdit_confirm_password->text();
    if(password1 != password2)
    {
        QMessageBox msgBox;
        msgBox.setText("Inconsistent entry password!");
        msgBox.exec();
        return;
    }

    int department = ui->comboBox_department->currentIndex();
    int identity   = ui->comboBox_identify->currentIndex();
    Account ac;
    ac.setJobNumber(jobnumber);
    ac.setPassword(password1);
    ac.setDepartment(QString::number(department));
    ac.setIdentify(QString::number(identity));

    //数据库交互
    AccountLab *acl = AccountLab::get();
    acl->addAccount(ac);
    model->select();

    QMessageBox msgBox;
    msgBox.setText("registration success!");
    msgBox.exec();
}

void SignUpDialog::on_btn_cancel_clicked()
{
    close();
}

void SignUpDialog::on_btn_query_clicked()
{
    int department = ui->comboBox_department_query->currentIndex();
    int identity   = ui->comboBox_identify_query->currentIndex();
    QString query = "department = '" + QString::number(department) + "' and identify = '"+QString::number(identity)+"'";
    //QString query1 = "department = '" + QString::number(department) + "'";
    //QString query2 = "identify = '" + QString::number(identity) + "'";
    model->setFilter(query);
    //model->setFilter(query2);
    model->select();
}

void SignUpDialog::on_btn_delete_current_clicked()
{
    int curRow = ui->tableView_all_account->currentIndex().row();
    //获取选中的行
    model->removeRow(curRow);
    //删除该行
    int ok = QMessageBox::warning(this,tr("Delete the current line!"),
                                  tr("Are you sure to delete the current line?"),
                                  QMessageBox::Yes,
                                  QMessageBox::No);
    if(ok == QMessageBox::No)
    {
        model->revertAll();//如果不删除，则撤销
    }
    else model->submitAll(); //否则提交，在数据库中删除该行
}

void SignUpDialog::on_btn_confirm_change_clicked()
{
    model->database().transaction(); //开始事务操作
    if (model->submitAll()) // 提交所有被修改的数据到数据库中
    {
        model->database().commit(); //提交成功，事务将真正修改数据库数据
    } else {
        model->database().rollback(); //提交失败，事务回滚
        QMessageBox::warning(this, tr("tableModel"),tr("Database error: %1").arg(model->lastError().text()));
    }
    model->revertAll(); //撤销修改
}

