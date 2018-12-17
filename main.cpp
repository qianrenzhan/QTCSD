#include "mainwindow.h"
#include <QApplication>

#include "colorspacelab.h"
#include "templatelab.h"
#include "logindlg.h"
#include "qbasehelper.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);



    LoginDlg login;
    if(login.exec() == QDialog::Accepted)
    {
        MainWindow w;
        QBaseHelper::FormInCenter(&w);
        int width = w.geometry().width();
        int height = w.geometry().height();
        w.setFixedSize(width,height);
        w.show();
        return a.exec();
    }
    else
        return 0;

}
