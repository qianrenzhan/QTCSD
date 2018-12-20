#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stable.h"
#include "mainwindow.h"


MainWindow* MainWindow::pMainWindow = 0;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWindow::pMainWindow = this;

    timer = new QTimer(this);
    timer->setInterval(5);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));

    connect(this, SIGNAL(is_exist(bool)),this, SLOT(is_exist_process(bool)));
    connect(this, SIGNAL(is_detect(bool)),this, SLOT(is_detect_process(bool)));
    connect(this, SIGNAL(need_template()),this,SLOT(need_template_process()));

#if defined (Q_OS_WIN32)
    processready = true;
#elif defined (Q_OS_LINUX)
    showready = true;
#endif

    ui->textEdit_total->setAlignment(Qt::AlignRight);
    ui->textEdit_qualify->setAlignment(Qt::AlignRight);
    ui->textEdit_percent->setAlignment(Qt::AlignRight);

    for(int i = 0;i<20;i++)
        mIsExistQueue[i] = false;

    status = 0;
    m_total_number = 0;
    m_qualify_number = 0;

    ui->btn_stop_camera->setDisabled(true);
    ui->btn_start_camera->setDisabled(false);


    //设置模型
    TemplateLab* ptpl = TemplateLab::get();

    model = new QSqlTableModel(0,ptpl->mbh->db);
    model->setTable("TEMPLATE");//指定使用哪个表

    //把model->放在view
    ui->tableView_template->setModel(model);

    model->select();
    model->removeColumn(1);
    model->removeColumn(2);
    model->removeColumn(4);

    //设置model的编辑模式，手动提交修改
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    QString dir=QCoreApplication::applicationDirPath();
    QString filename1(dir+"/error.wav");
    beller = new QSound(filename1);
    QString filename2(dir+"/su.wav");
    bellsu = new QSound(filename2);

    QPalette palette;
    palette.setColor(QPalette::Background, QColor(0, 0, 0));
    palette.setColor(QPalette::WindowText, QColor(255, 255, 255));
    ui->label_camera->setAutoFillBackground(true);
    ui->label_camera->setPalette(palette);
    ui->label_camera->setText("no signal");
    //ui->label_camera->set
    AccountLab *actl = AccountLab::get();
    Account account = actl->readCurrentAccount();

    ui->label_current_user->setText(account.getJobNumber());


    //statusbar
    QLabel *aixLabel;
    aixLabel = new QLabel("Ready");

    //Optional
    statusBar()->setStyleSheet(QString("QStatusBar::item{border: 0px}")); // 设置不显示label的边框
    statusBar()->setSizeGripEnabled(false); //设置是否显示右边的大小控制点
    //statusBar()->addWidget(locationLabel);
    statusBar()->addWidget(aixLabel, 1);


    QLabel *per1 = new QLabel("Camera is not connected", this);
    per1->setStyleSheet("color: red;border-style: groove;border-width:1;");
    QLabel *per2 = new QLabel("Connection to the database successful ", this);
    per2->setStyleSheet("color: green;border-style: groove;border-width:1;");
    //QLabel *per3 = new QLabel("Ready3", this);
    statusBar()->addPermanentWidget(per1); //现实永久信息
    statusBar()->addPermanentWidget(per2);
    //statusBar()->insertPermanentWidget(2, per3);
    //statusBar()->showMessage("Status is here...", 3000); // 显示临时信息，时间3秒钟.

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update()
{
#if defined (Q_OS_WIN32)
    QTime time;
    time.start();
    GXSendCommand(m_hDevice, GX_COMMAND_TRIGGER_SOFTWARE);
    processready = false;
    while (!processready) Sleep(1);

    //qDebug()<< "0" <<time.elapsed()/1000.0<<"s";
    //64 debug  55ms    release  50ms
    //32        45ms             40ms

    Mat srcImage = ImageSmall;
    QImage disImage = ImageProcessingTool::MatToQImage(srcImage);
    ui->label_camera->setPixmap(QPixmap::fromImage(disImage.scaled(ui->label_camera->size())));
#endif
#if defined (Q_OS_LINUX)
    cap >> frame;
    if (frame.empty()) return;
    //namedWindow("当前视频",4);
    //imshow("当前视频", frame);

//    Mat frame2 = frame(Rect(0,500,1920,200));

//    Mat dst;
//    bool is = ImageProcessingTool::IsExist(frame2,&dst);
//    emit is_exist(is);


//    //更新队列数组
//    for(int i = 19;i>0;i--){
//        mIsExistQueue[i] = mIsExistQueue[i-1];
//    }
//    mIsExistQueue[0] = is;

//    //检测队列数组
//    int count = 0;
//    for(int i = 0;i<5;i++)
//        if(mIsExistQueue[i] == true)
//            count++;
//    if(count == 5 && status == 0)        //连续多少帧后开始采集数据。
//    {
//        status = 1;  //切换到处理状态
//    }

//    if(status == 1)      //
//    {
//        QString s = "";
//        s = ImageProcessingTool::CSD1(frame2,&dst);
//        mCurrentDetect.setColors(s);  //

//        if(mCurrentTemplate.getColorsRead() != "")
//        {
//            //保存图片
//            string file = QUuid::createUuid().toString().toStdString();
//            file += ".bmp";
//            imwrite(file,frame2);

//            QString str = mCurrentDetect.getColorsRead();
//            //对str中的重复元素进行处理
//            QString strpro = ImageProcessingTool::processStr(str);
//            QString strrev = ImageProcessingTool::reversalStr(strpro);
//            if(strpro == mCurrentTemplate.getColorsRead()
//                    || strrev == mCurrentTemplate.getColorsRead() ){
//                emit is_detect(true);
//                bellsu->play();
//            }
//            else{
//                emit is_detect(false);
//                beller->play();
//            }
//        } else {
//            //emit need_template();
//        }
//        status = 2;
//    }

//    //直到检测不到线缆再切换到预处理模式
//    //检测队列数组
//    count = 0;
//    for(int i = 0;i<2;i++)
//        if(mIsExistQueue[i] == false)
//            count++;

//    //退出检测的条件较进入检测宽松
//    if(count == 2 && status == 2)
//        status = 0;

//    if(status == 0 || status == 2)
//    {
//        ImageProcessingTool::CSD1(frame2,&dst);
//    }

//    Mat srcImage = dst;
    QImage disImage = ImageProcessingTool::MatToQImage(frame);   //frame  srcImage
    ui->label_camera->setPixmap(QPixmap::fromImage(disImage.scaled(ui->label_camera->size())));
#endif
}

void MainWindow::updateUI()
{
    ui->textEdit_total->setText(QString::number(m_total_number));
    ui->textEdit_qualify->setText(QString::number(m_qualify_number));
    ui->textEdit_percent->setText(QString::number((float)m_qualify_number/m_total_number,'g',2));

    ui->textEdit_total->setAlignment(Qt::AlignRight);
    ui->textEdit_qualify->setAlignment(Qt::AlignRight);
    ui->textEdit_percent->setAlignment(Qt::AlignRight);
    
}

void MainWindow::is_exist_process(bool is)
{
    if(status == 0)
    {
        if(is)
        {
            //ui->textEdit_current_detect->setTextBackgroundColor(QColor(0,255,0));
            ui->label_current_detect->setStyleSheet("background-color:yellow");
        }
        else
        {
            //ui->textEdit_current_detect->setTextBackgroundColor(QColor(255,0,0));
            ui->label_current_detect->setStyleSheet("background-color:red");
        }
    }
}

void MainWindow::is_detect_process(bool is)
{
    //QApplication::beep();
    ui->label_current_detect->setStyleSheet("background-color:green");
    ui->textEdit_current_detect->setText(mCurrentDetect.getColorsRead());

    if(is){
        ++m_total_number;
        ++m_qualify_number;

    }
    else {
        ++m_total_number;
        //beller->play();
    }
    updateUI();
}

void MainWindow::need_template_process()
{
    QMessageBox msgBox;
    msgBox.setText("please choose a template!");
    msgBox.exec();
}

void MainWindow::on_btn_start_camera_clicked()
{
#if defined (Q_OS_WIN32)
    //打开相机
    GX_STATUS emStatus = GX_STATUS_SUCCESS;
    GX_OPEN_PARAM openParam;
    uint32_t      nDeviceNum = 0;
    openParam.accessMode = GX_ACCESS_EXCLUSIVE;
    openParam.openMode = GX_OPEN_INDEX;
    char a[] = "1";
    openParam.pszContent = a;
    // 初始化库
    emStatus = GXInitLib();
    if (emStatus != GX_STATUS_SUCCESS)
    {
        return;
    }
    // 枚举设备列表
    emStatus = GXUpdateDeviceList(&nDeviceNum, 1000);
    if ((emStatus != GX_STATUS_SUCCESS) || (nDeviceNum <= 0))
    {
        return;
    }
    //打开设备
    emStatus = GXOpenDevice(&openParam, &m_hDevice);

    emStatus = GXGetEnum(m_hDevice, GX_ENUM_PIXEL_COLOR_FILTER, &m_nPixelColorFilter);

    //设置为8bit像素
    //emStatus = GXSetEnum(m_hDevice, GX_ENUM_PIXEL_FORMAT, GX_PIXEL_FORMAT_BAYER_RG8);

    //设置触发模式为ON,软触发
    emStatus = GXSetEnum(m_hDevice, GX_ENUM_TRIGGER_MODE, GX_TRIGGER_MODE_ON);
    GXSetEnum(m_hDevice, GX_ENUM_TRIGGER_SOURCE, GX_TRIGGER_SOURCE_SOFTWARE);


    //设置采集模式连续采集
    //emStatus = GXSetEnum(m_hDevice, GX_ENUM_ACQUISITION_MODE, GX_ACQ_MODE_CONTINUOUS); //GX_ACQ_MODE_SINGLE_FRAME
    emStatus = GXSetEnum(m_hDevice, GX_ENUM_ACQUISITION_MODE, GX_ACQ_MODE_SINGLE_FRAME);
    //emStatus = GXSetInt(m_hDevice, GX_INT_ACQUISITION_SPEED_LEVEL, 1);
    emStatus = GXSetEnum(m_hDevice, GX_ENUM_BALANCE_WHITE_AUTO, GX_BALANCE_WHITE_AUTO_CONTINUOUS);

    //设置曝光时间
    GX_FLOAT_RANGE shutterRange;
    emStatus = GXGetFloatRange(m_hDevice, GX_FLOAT_EXPOSURE_TIME, &shutterRange);
    //10ms
    shutterRange.dMax = 7500;
    shutterRange.dMin = 7500;

    emStatus = GXSetFloat(m_hDevice, GX_FLOAT_EXPOSURE_TIME, shutterRange.dMin);
    emStatus = GXSetFloat(m_hDevice, GX_FLOAT_EXPOSURE_TIME, shutterRange.dMax);

    //设置白平衡为D75
    emStatus = GXSetEnum(m_hDevice, GX_ENUM_BALANCE_WHITE_AUTO, GX_BALANCE_WHITE_AUTO_CONTINUOUS);
    emStatus = GXSetEnum(m_hDevice, GX_ENUM_AWB_LAMP_HOUSE, GX_AWB_LAMP_HOUSE_D75);

    emStatus = GXSetEnum(m_hDevice, GX_INT_COLOR_CORRECTION_PARAM, GX_COLOR_CORRECT_ON);

    // 计算图像处理Lut查找表
    int m_nContrast = 70;   //对比度值  38
    double m_dGamma = 1.8;  //Gamma值
    int m_nLightness = 0;  //亮度
    emStatus = DxGetLut((VxInt32)m_nContrast, m_dGamma, m_nLightness, NULL, &m_objImageProcess.nLutLength);
    m_objImageProcess.pProLut = new VxUint8[m_objImageProcess.nLutLength];

    //获取颜色校正系数
    emStatus = GXGetInt(m_hDevice, GX_INT_COLOR_CORRECTION_PARAM, &nColorCorrectionParam);
    nColorCorrectionParam = 0;
    m_objImageProcess.nCCBufLength = CALCCCPARAM_LENGTH;
    m_objImageProcess.parrCC = new VxInt16[CALCCCPARAM_LENGTH];

    // 计算图像彩色调节数组
    int nSaturation = 64;  //饱和度
    int status;
    status = DxCalcCCParam(nColorCorrectionParam, nSaturation, m_objImageProcess.parrCC, CALCCCPARAM_LENGTH);

    // 重新计算图像处理Lut查找表
    status = DxGetLut((VxInt32)m_nContrast, m_dGamma, m_nLightness, m_objImageProcess.pProLut, &m_objImageProcess.nLutLength);
    if (status != DX_OK)
        return;

    //锐化
    m_objImageProcess.bSharpness = true;
    m_objImageProcess.fSharpFactor = 1.8;

    //坏点校正
    m_objImageProcess.bDefectivePixelCorrect = true;

    //图像降噪
    m_objImageProcess.bDenoise = true;

    //cpu加速
    m_objImageProcess.bAccelerate = 0;   //cpu加速只能32位使用

    m_objImageProcess.bFlip = 0;

    m_objImageProcess.cvType = RAW2RGB_NEIGHBOUR;
    m_objImageProcess.emLayOut = (DX_PIXEL_COLOR_FILTER)m_nPixelColorFilter;


    int64_t nWidth= 1280;
    int64_t nHeight= 100;
    int64_t nOffsetX = 0;
    int64_t nOffsetY = 500;

    emStatus = GXSetInt(m_hDevice, GX_INT_WIDTH, nWidth);
    emStatus = GXSetInt(m_hDevice, GX_INT_HEIGHT, nHeight);
    emStatus = GXSetInt(m_hDevice, GX_INT_OFFSET_X, nOffsetX);
    emStatus = GXSetInt(m_hDevice, GX_INT_OFFSET_Y, nOffsetY);


    //注册图像处理回调函数
    //this->On
    emStatus = GXRegisterCaptureCallback(m_hDevice, NULL, this->OnFrameCallbackFun);
    //发送开采命令
    emStatus = GXSendCommand(m_hDevice, GX_COMMAND_ACQUISITION_START);
#elif defined (Q_OS_LINUX)
    //open camera
    cap.open(0);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);    //1920*1080  1024*768  320*240
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 1920);
    cap.set(CAP_PROP_AUTO_EXPOSURE, 0.25);
    cap.set(CV_CAP_PROP_EXPOSURE, 1);

    cap.set(CV_CAP_PROP_BRIGHTNESS, 0.8);//亮度 1
    cap.set(CV_CAP_PROP_CONTRAST,0.5);//对比度 40
    cap.set(CV_CAP_PROP_SATURATION, 0.5);//饱和度 50
    cap.set(CV_CAP_PROP_HUE, 0.5);//色调 50


    qDebug("width = %.2f\n", cap.get(CV_CAP_PROP_FRAME_WIDTH));
    qDebug("height = %.2f\n", cap.get(CV_CAP_PROP_FRAME_HEIGHT));
    qDebug("fbs = %.2f\n", cap.get(CV_CAP_PROP_FPS));
    qDebug("brightness = %.2f\n", cap.get(CV_CAP_PROP_BRIGHTNESS));
    qDebug("contrast = %.2f\n", cap.get(CV_CAP_PROP_CONTRAST));
    qDebug("saturation = %.2f\n", cap.get(CV_CAP_PROP_SATURATION));
    qDebug("hue = %.2f\n", cap.get(CV_CAP_PROP_HUE));
    qDebug("exposure = %.2f\n", cap.get(CV_CAP_PROP_EXPOSURE));
    qDebug("frame_format = %.2f\n", cap.get(CV_CAP_PROP_FORMAT));
    qDebug("cc = %.2f\n", cap.get(CV_CAP_PROP_FOURCC));
    if (!cap.isOpened())
    {
        return;
    }
#endif

    timer->start();
    ui->btn_start_camera->setDisabled(true);
    ui->btn_stop_camera->setDisabled(false);

}

void MainWindow::on_btn_stop_camera_clicked()
{
    timer->stop();
#if defined (Q_OS_WIN32)
    GX_STATUS emStatus = GX_STATUS_SUCCESS;
    emStatus = GXSendCommand(m_hDevice, GX_COMMAND_ACQUISITION_STOP);
    emStatus = GXUnregisterCaptureCallback(m_hDevice);
    emStatus = GXCloseDevice(m_hDevice);
#elif defined (Q_OS_LINUX)
    cap.release();
#endif

    ui->btn_stop_camera->setDisabled(true);
    ui->btn_start_camera->setDisabled(false);

    m_total_number = 0;
    m_qualify_number = 0;
    updateUI();

    QPalette palette;
    palette.setColor(QPalette::Background, QColor(0, 0, 0));
    palette.setColor(QPalette::WindowText, QColor(255, 255, 255));
    ui->label_camera->setAutoFillBackground(true);
    ui->label_camera->setPalette(palette);
    ui->label_camera->setText("no signal");

}



//图像回调处理函数
#if defined (Q_OS_WIN32)
void GX_STDC MainWindow::OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame)
{
    if (1)
    {
        MainWindow::pMainWindow->m_pBufferRGB = new BYTE[(size_t)(1280 * 1024 * 3)];
        MainWindow::pMainWindow->m_pBufferRaw = new BYTE[(size_t)1310720];
        memcpy(MainWindow::pMainWindow->m_pBufferRaw, pFrame->pImgBuf, pFrame->nImgSize);
        Mat test;
        DxRaw8ImgProcess(MainWindow::pMainWindow->m_pBufferRaw, MainWindow::pMainWindow->m_pBufferRGB, pFrame->nWidth, pFrame->nHeight, &(pMainWindow->m_objImageProcess));
        test.create(pFrame->nHeight, pFrame->nWidth, CV_8UC3);
        memcpy(test.data, MainWindow::pMainWindow->m_pBufferRGB, pFrame->nHeight*pFrame->nWidth*3);  //1280*1024*3
        MainWindow::pMainWindow->ImageRaw = test;
        if (MainWindow::pMainWindow->m_pBufferRGB != NULL)
        {
            delete[]MainWindow::pMainWindow->m_pBufferRGB;
            MainWindow::pMainWindow->m_pBufferRGB = NULL;
        }
        if (MainWindow::pMainWindow->m_pBufferRaw != NULL)
        {
            delete[]MainWindow::pMainWindow->pMainWindow->m_pBufferRaw;
            MainWindow::pMainWindow->m_pBufferRaw = NULL;
        }
    }
    else
        MainWindow::pMainWindow->ImageRaw = ImageProcessingTool::daHeng2Mat(pFrame, false);

    Mat frame = MainWindow::pMainWindow->ImageRaw.clone();
    //Mat frame2 = frame(Rect(0, 400, 1280, 200));
    Mat frame2 = frame;
    //===============================================
    Mat dst;
    bool is = ImageProcessingTool::IsExist(frame2,&dst);
    emit pMainWindow->is_exist(is);


    //更新队列数组
    for(int i = 19;i>0;i--){
        pMainWindow->mIsExistQueue[i] = pMainWindow->mIsExistQueue[i-1];
    }
    pMainWindow->mIsExistQueue[0] = is;

    //检测队列数组
    int count = 0;
    for(int i = 0;i<3;i++)
        if(pMainWindow->mIsExistQueue[i] == true)
            count++;
    if(count == 3 && pMainWindow->status == 0)        //连续多少帧后开始采集数据。
    {
        pMainWindow->status = 1;  //切换到处理状态
    }

    if(pMainWindow->status == 1)      //
    {
        QString s = "";
        s = ImageProcessingTool::CSD1(frame2,&dst);
        pMainWindow->mCurrentDetect.setColors(s);  //

        if(pMainWindow->mCurrentTemplate.getColorsRead() != "")
        {
            QString str = pMainWindow->mCurrentDetect.getColorsRead();
            //对str中的重复元素进行处理
            QString strpro = ImageProcessingTool::processStr(str);
            QString strrev = ImageProcessingTool::reversalStr(strpro);
            if(strpro == pMainWindow->mCurrentTemplate.getColorsRead()
                    || strrev == pMainWindow->mCurrentTemplate.getColorsRead() ){
                emit pMainWindow->is_detect(true);
                pMainWindow->bellsu->play();
            }
            else{
                emit pMainWindow->is_detect(false);
                pMainWindow->beller->play();
            }
        } else {
            emit pMainWindow->need_template();
        }
        pMainWindow->status = 2;
    }

    //直到检测不到线缆再切换到预处理模式
    //检测队列数组
    count = 0;
    for(int i = 0;i<1;i++)
        if(pMainWindow->mIsExistQueue[i] == false)
            count++;

    //退出检测的条件较进入检测宽松
    if(count == 1 && pMainWindow->status == 2)
        pMainWindow->status = 0;

    if(pMainWindow->status == 0 || pMainWindow->status == 2)
    {
        ImageProcessingTool::CSD1(frame2,&dst);
    }

    MainWindow::pMainWindow->ImageSmall = dst;

    MainWindow::pMainWindow->processready = true;

    return;
}
#endif


void MainWindow::on_btn_calibration_clicked()
{
    timer->stop();
    ColorSpaceLab* csb = ColorSpaceLab::get();
    //弹出一个框，显示两行
    bool ok = false;
    QString text = QInputDialog::getText(
                this,
                tr( "CSD" ),
                tr( "input template colors" ),
                QLineEdit::Normal, mCurrentTemplate.getColorsRead(), &ok);
    if ( ok && !text.isEmpty())
    {
        QStringList ss = mCurrentDetect.getColors().split(",");
        int mNumber = (ss.size()-1)/3;
        if(mNumber == text.size()){    //说明输入的线缆数量和识别到的相同
            for(int i = 0;i<mNumber;i++)
            {
                ColorSpace colorSpace;
                colorSpace.setColorName(text.at(i));
                colorSpace.setLABint(ss.at(i*3).toInt(), ss.at(i*3+1).toInt(),ss.at(i*3+2).toInt());
                //查询数据库是否有对应颜色，如果没有，就加入
                ColorSpace temp_colorSpace = csb->getColorSpaceByColorName(text.at(i));
                if(temp_colorSpace.getA() != "0")   //等于0代表没有找到
                    csb->updateColorSpacebyName(colorSpace);
                else
                    csb->addColorSpace(colorSpace);
            }
            QMessageBox msgBox;
            msgBox.setText("calibrate success:" + QString::number(mNumber));
            msgBox.exec();
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setText("calibrate failed");
            msgBox.exec();
        }
    }
    timer->start();
}

void MainWindow::on_btn_add_template_clicked()
{
    //把mCurrentDetect保存为模板
    TemplateLab* ptpl = TemplateLab::get();
    bool ok = false;
    QString text = QInputDialog::getText(
                this,
                tr( "CSD" ),
                tr( "input template name" ),
                QLineEdit::Normal, "default", &ok);
    if ( ok && !text.isEmpty() )
    {
        Template t;
        t.setTitle(text);
        t.setColors(mCurrentDetect.getColors());
        //mCurrentDetect.setTitle(text);
        ptpl->addTemplate(t);
        QMessageBox msgBox;
        msgBox.setText("template saved");
        msgBox.exec();
    }

    model->select();
}

void MainWindow::on_btn_choose_template_clicked()
{
    int curRow = ui->tableView_template->currentIndex().row();
    if(curRow <0 || curRow>10) curRow = 0;
    TemplateLab* ptpl = TemplateLab::get();
    QList<Template> listtp = ptpl->getTemplates();
    mCurrentTemplate = listtp.at(curRow);
    ui->textEdit_current_template->setText(mCurrentTemplate.getColorsRead());
}

void MainWindow::on_btn_delete_clicked()
{
    int curRow = ui->tableView_template->currentIndex().row();
    if(curRow <0 || curRow>10) curRow = 0;
    TemplateLab* ptpl = TemplateLab::get();
    QList<Template> listtp = ptpl->getTemplates();
    mCurrentTemplate = listtp.at(curRow);
    ptpl->deleteTemplateByUuid(mCurrentTemplate);
    model->select();
}
