#include "logindlg.h"
#include "ui_logindlg.h"
#include "signupdialog.h"

LoginDlg::LoginDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDlg)
{
    ui->setupUi(this);

    ui->txtpwd->setEchoMode(QLineEdit::Password);

    //运行出界面后默认选中
    ui->txtname->setFocus();
    ui->txtname->setText("qel");
    ui->txtpwd->setText("qel");

    //设置字体颜色为橘红色
    ui->label->setStyleSheet("color:#FF4500");
    ui->label_2->setStyleSheet("color:#FF4500");
    ui->label_4->setStyleSheet("color:#FF4500");
}

LoginDlg::~LoginDlg()
{
    delete ui;
}

void LoginDlg::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void LoginDlg::on_pushButton_cancel_clicked()
{
    this->close();
}

void LoginDlg::on_pushButton_sure_clicked()
{
    AccountLab *actl = AccountLab::get();
    if(actl->verification(this->ui->txtname->text().trimmed(),this->ui->txtpwd->text().trimmed()))
    {
        Account account;
        account.setJobNumber(this->ui->txtname->text().trimmed());
        actl->writeCurrentAccount(account);
        accept();
    }
//    if((this->ui->txtname->text().trimmed()== tr("qel") &&
//        this->ui->txtpwd->text().trimmed()== tr("qel")       ||
//        this->ui->txtname->text().trimmed()== tr("QEL") &&
//        this->ui->txtpwd->text().trimmed()== tr("QEL")))
//    {
//        accept();
//    }
    else
    {
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK") );
        QMessageBox::warning(this,tr("警告"),tr("用户名或密码错误!"),QMessageBox::Yes);
        this->ui->txtname->clear();
        this->ui->txtpwd->clear();
        this->ui->txtname->setFocus();
    }
}

void LoginDlg::on_pushButton_signup_clicked()
{
    signupdlg = new SignUpDialog();
    int width = signupdlg->geometry().width();
    int height = signupdlg->geometry().height();
    signupdlg->setFixedSize(width,height);
    signupdlg->exec();
}
