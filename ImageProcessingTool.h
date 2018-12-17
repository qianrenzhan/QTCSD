#pragma once
#include<iostream>
#include <time.h>

//opencv
#include<opencv2/opencv.hpp>

#include <QImage>
#include <QDebug>

//大恒
#if defined(Q_OS_WIN32)
#include "GxIAPI.h"
#include "DxImageProc.h"
#elif defined(Q_OS_LINUX)

#endif

#if defined (Q_OS_WIN32)
#define WIDTH 1280
#define WIREWIDTH 20
#elif defined(Q_OS_LINUX)
#define WIDTH 1920
#define WIREWIDTH 36
#endif
using namespace std;
using namespace cv;


struct wire_pair {
    int left;
    int right;

    bool operator<(const wire_pair b) const {
        return this->left < b.left;
    }
};

class ImageProcessingTool
{
public:
	ImageProcessingTool();
	~ImageProcessingTool();


#if defined (Q_OS_WIN32)
	//大恒c语言图像格式转opencv
	static Mat daHeng2Mat(GX_FRAME_CALLBACK_PARAM* pFrame, bool show);
#endif
    //opencv Mat 和 QImage 相互转换
    static QImage MatToQImage(cv::Mat& mat);
    static cv::Mat QImageToMat(QImage image);

    //线缆颜色顺序检测
    static void int2str(const int &int_temp, string &string_temp);
    static void InsertSort(int arr[], int n);
    static QString CSD1(Mat src, Mat *dst);
    static bool IsExist(Mat src, Mat *dst) ;
    static QString reversalStr(QString &str);
    static QString processStr(QString &str);
};

