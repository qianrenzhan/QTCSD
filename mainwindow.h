#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#if defined (Q_OS_WIN32)
#include <conio.h>
#include <ctime>
#include "GxIAPI.h"
#include "DxImageProc.h"
#endif

#include<iostream>

#include "ImageProcessingTool.h"
#include "colorspacelab.h"
#include "templatelab.h"
#include "account.h"
#include "taskstatistics.h"
#include "accountlab.h"

#define   CALCCCPARAM_LENGTH               (sizeof(VxInt16) * 9)

class QSqlTableModel;
class QSound;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static MainWindow* pMainWindow;
    QSqlTableModel *model;

signals:
    void is_exist(bool is);
    void is_detect(bool is);
    void need_template();

private slots:
    void update();
    void on_btn_start_camera_clicked();

    void on_btn_stop_camera_clicked();
    void is_exist_process(bool is);
    void is_detect_process(bool is);
    void need_template_process();

    void on_btn_calibration_clicked();

    void on_btn_add_template_clicked();

    void on_btn_choose_template_clicked();

    void on_btn_delete_clicked();

private:
    Ui::MainWindow *ui;

    QTimer *timer;

#if defined (Q_OS_WIN32)
    GX_DEV_HANDLE       m_hDevice;              //< 设备句柄
    COLOR_IMG_PROCESS   m_objImageProcess;
    int64_t             m_nPixelColorFilter;    //< Bayer格式
    int64_t nColorCorrectionParam;
    BYTE                *m_pBufferRaw;          //< 原始图像数据
    BYTE                *m_pBufferRGB;	        //< RGB图像数据，用于显示和保存bmp图像
    static void GX_STDC OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame);
#endif

    Mat ImageRaw;
    Mat ImageSmall;
#if defined (Q_OS_WIN32)
    bool processready;
#elif defined (Q_OS_LINUX)
    bool showready;
#endif

    bool mIsExistQueue[20];
    int status;
    Template mCurrentTemplate;
    Template mCurrentDetect;

    int m_total_number;
    int m_qualify_number;


    void updateUI();
    QSound *bellsu;
    QSound *beller;
    VideoCapture cap;
    Mat frame;

    TaskStatistics taskstatistics;   //当前登录用户信息

};

#endif // MAINWINDOW_H
