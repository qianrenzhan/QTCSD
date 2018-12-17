#include "ImageProcessingTool.h"

//int ImageProcessingTool::piccount = 0;

ImageProcessingTool::ImageProcessingTool()
{
}


ImageProcessingTool::~ImageProcessingTool()
{
}

#if defined(Q_OS_WIN32)
Mat ImageProcessingTool::daHeng2Mat(GX_FRAME_CALLBACK_PARAM* pFrame, bool show)
{
	Mat test;
	
	BYTE   *m_pBufferRaw;          //< 原始图像数据
	BYTE    *m_pBufferRGB;	       //< RGB图像数据，用于显示和保存bmp图像


	//为存储原始图像数据申请空间
    m_pBufferRGB = new BYTE[(size_t)(pFrame->nWidth * pFrame->nHeight * 3)];
    m_pBufferRaw = new BYTE[(size_t)(pFrame->nWidth * pFrame->nHeight * 3)];
    test.create(pFrame->nHeight, pFrame->nWidth, CV_8UC3);

	if (pFrame->status == 0)
	{
		//对图像进行某些操作
		/*memcpy(m_pBufferRaw, pFrame->pImgBuf, pFrame->nImgSize);
		// 黑白相机需要翻转数据后显示
		for (int i = 0; i <m_nImageHeight; i++)		{
		memcpy(m_pImageBuffer + i*m_nImageWidth, m_pBufferRaw + (m_nImageHeight - i - 1)*m_nImageWidth, (size_t)m_nImageWidth);
		}
		IplImage* src;
		src = cvCreateImage(cvSize(m_nImageWidth, m_nImageHeight), 8, 1);
		src->imageData = (char*)m_pImageBuffer;
		cvSaveImage("src.jpg", src);*/

		memcpy(m_pBufferRaw, pFrame->pImgBuf, pFrame->nImgSize);

		// RGB转换
		DxRaw8toRGB24(m_pBufferRaw
			, m_pBufferRGB
            , (VxUint32)(pFrame->nWidth)
            , (VxUint32)(pFrame->nHeight)
			, RAW2RGB_NEIGHBOUR
			, DX_PIXEL_COLOR_FILTER(1)
			, false);

        memcpy(test.data, m_pBufferRGB, pFrame->nWidth * pFrame->nHeight * 3);  //1280*1024*3

		if (show)
		{
			namedWindow("大恒原图");
			imshow("大恒原图", test);
			waitKey(1);
		}

	}

	if (m_pBufferRGB != NULL)
	{
		delete[]m_pBufferRGB;
		m_pBufferRGB = NULL;
	}
	if (m_pBufferRaw != NULL)
	{
		delete[]m_pBufferRaw;
		m_pBufferRaw = NULL;
	}

	return test;
}
#endif

QImage ImageProcessingTool::MatToQImage(cv::Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4)
    {
        qDebug() << "CV_8UC4";
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}

cv::Mat ImageProcessingTool::QImageToMat(QImage image)
{
    cv::Mat mat;
    switch (image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), (size_t)image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, CV_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    default:
        break;
    }
    return mat;
}

void ImageProcessingTool::int2str(const int &int_temp, std::string &string_temp) {
    std::stringstream stream;
    stream << int_temp;
    string_temp = stream.str();   //此处也可以用 stream>>string_temp
}

void ImageProcessingTool::InsertSort(int arr[], int n) {
    for (int i = 1; i <= n; ++i) {
        for (int j = i; j > 0; --j) {
            if (arr[j] < arr[j - 1]) {
                int temp = arr[j];
                arr[j] = arr[j - 1];
                arr[j - 1] = temp;
            }
        }
    }
}


QString ImageProcessingTool::CSD1(Mat src, Mat *dst)
{
    //对原图进行缩放处理
    Mat src_raw = src.clone();
    int cor = src_raw.cols * 0.1;
    float ratio = src_raw.rows / src_raw.cols;
    Mat src_temp(src_raw, Rect(cor, (int)(cor * ratio), src_raw.cols - 2 * cor,
        src_raw.rows - 2 * (int)(cor * ratio)));
    Mat src_all;
    Size size = Size(src_raw.cols, src_raw.rows);
    resize(src_temp, src_all, size);

    //Mat src_all = src.clone();
    Mat hsv, bgr, rgb;
    cvtColor(src_all, hsv, CV_BGR2Lab);


    float L[WIDTH] = { 0 };
    float A[WIDTH] = { 0 };
    float B[WIDTH] = { 0 };
    float LL[WIDTH] = { 0 };
    float AA[WIDTH] = { 0 };
    float BB[WIDTH] = { 0 };
    int pointcount[WIDTH] = { 0 };
    for (int i = 0; i < hsv.size().width; i++)
    {
        float h, s, v;
        h = 0;
        s = 0;
        v = 0;
        for (int j = hsv.size().height / 2; j < hsv.size().height / 2 + 4; j++)   //hsv.size().height
        {
            Vec3b vec3b = hsv.at<Vec3b>(j, i);
            h += vec3b[0];
            s += vec3b[1];
            v += vec3b[2];
        }
        L[i] = h/2;
        A[i] = s/2;
        B[i] = v/2;
    }

    //需要对数据进行平滑
    int filtersize = 2;
    for (int i = filtersize; i < WIDTH - filtersize; i++)
    {
        int suml = 0, suma = 0, sumb = 0;
        for (int j = -filtersize; j <= filtersize; j++)
        {
            suml += L[i + j];
            suma += L[i + j];
            sumb += L[i + j];
        }
        LL[i] = suml / (filtersize * 2 + 1);
        AA[i] = suma / (filtersize * 2 + 1);
        BB[i] = sumb / (filtersize * 2 + 1);
    }

    //用球包含空间附近的点
    for (int i = 0; i < WIDTH; i++)
    {
        int j = -20;
        int jmax = 20;
        for (; j <= jmax; j++)
        {
            if (i + j >= 0 && i + j < WIDTH)
            {
                int dis = (LL[i] - LL[i + j])*(LL[i] - LL[i + j])
                    + (AA[i] - AA[i + j])*(AA[i] - AA[i + j])
                    + (BB[i] - BB[i + j])*(BB[i] - BB[i + j]);
                if (dis < 1000)
                    pointcount[i]++;
            }
        }
    }

    //pointcount找极小值
    int position[50] = { 0 };
    int position_left[50] = { 0 };
    int position_right[50] = { 0 };
    int current_position_count = 0;
    bool done_flag = 0;

    for (int i = 1; i <= 35; i++)     //最大41
    {
        for (int j = 100; j < WIDTH - 100; j++)
        {
            if (pointcount[j] == i)
            {
                done_flag = 0;
                for (int k = 0; k < current_position_count; k++)
                {
                    if (j > position_left[k] - WIREWIDTH / 2 && j < position_right[k] + WIREWIDTH / 2)
                    {
                        //加入当前position
                        if (j < position_left[k])
                            position_left[k] = j;
                        if (j > position_right[k])
                            position_right[k] = j;
                        done_flag = 1;
                    }

                }

                //如果和所有的都不符合，建立新的的position
                if (!done_flag && current_position_count < 50)
                {
                    position_left[current_position_count] = j;
                    position_right[current_position_count] = j;
                    position[current_position_count] = j;
                    current_position_count++;
                }
            }
        }
    }

    InsertSort(position, current_position_count);

    //分段
    int count = 0;
    int tempcount = 0;
    int left[50] = { 0 };
    int right[50] = { 0 };
    for (int i = 0; i < current_position_count - 1; i++)
    {
        left[i] = position[i + 1];
        right[i] = position[i + 2];
    }
    count = current_position_count;



    std::set<wire_pair> s_wire_pair;

    //从step中取出有用的信息
    int finalleft[50] = { 0 };
    int finalright[50] = { 0 };
    int finalcount = 0;
    for (int i = 0; i < count; i++) {
        if (right[i] - left[i] > WIREWIDTH / 2 && right[i] - left[i] < WIREWIDTH * 2) {             //允许最窄的的线缆  //再增加一个线缆间距的判断。
            finalleft[finalcount] = left[i];
            finalright[finalcount++] = right[i];
            wire_pair wp;
            wp.left = left[i];
            wp.right = right[i];
            s_wire_pair.insert(wp);
        }
    }

    std::vector<int> leftborder;
    std::vector<int> rightborder;

    //还原现场
    leftborder.clear();
    rightborder.clear();
    std::set<wire_pair>::iterator it = s_wire_pair.begin();
    for (it = s_wire_pair.begin(); it != s_wire_pair.end(); it++) {
        leftborder.push_back((*it).left);
        rightborder.push_back((*it).right);
    }


    QString values = "";


    line(src_all, Point(0, hsv.size().height / 2 - hsv.size().height / 4), Point(hsv.size().width, hsv.size().height / 2 - hsv.size().height / 4), Scalar(0, 255, 0), hsv.size().height / 50 );
    line(src_all, Point(0, hsv.size().height / 2 + hsv.size().height / 4), Point(hsv.size().width, hsv.size().height / 2 + hsv.size().height / 4), Scalar(0, 255, 0), hsv.size().height / 50);

    //对框选区域进行颜色识别
    for (int c = 0; c < rightborder.size(); c++) {
        float h, s, v;
        h = 0;
        s = 0;
        v = 0;
        for (int i = leftborder.at(c) + WIREWIDTH / 10; i < rightborder.at(c) - WIREWIDTH / 10; i++) {
            for (int j = hsv.size().height / 2; j < hsv.size().height / 2 + 20; j++)   //hsv.size().height
            {
                Vec3b vec3b = hsv.at<Vec3b>(j, i);
                h += vec3b[0];
                s += vec3b[1];
                v += vec3b[2];
            }
        }
        h = h / (20 * (rightborder.at(c) - leftborder.at(c) - WIREWIDTH / 5));
        s = s / (20 * (rightborder.at(c) - leftborder.at(c) - WIREWIDTH / 5));
        v = v / (20 * (rightborder.at(c) - leftborder.at(c) - WIREWIDTH / 5));

        int h_out = h;
        int s_out = s;
        int v_out = v;
        QString h_out_str, s_out_str, v_out_str;
        h_out_str = QString::number(h_out);
        s_out_str = QString::number(s_out);
        v_out_str = QString::number(v_out);
        values += (h_out_str + "," + s_out_str + "," + v_out_str + ",");
    }

    //框选线缆区域
    for (int i = 0; i < rightborder.size(); i++) {
        line(src_all, Point(leftborder.at(i) + WIREWIDTH / 10, hsv.size().height / 2 - hsv.size().height / 4),
            Point(rightborder.at(i) - WIREWIDTH / 10, hsv.size().height / 2 + hsv.size().height / 4), Scalar(255, 0, 0), hsv.size().height / 50);
    }

    *dst = src_all;
    return values;
}

bool ImageProcessingTool::IsExist(Mat src, Mat *dst) {
    //对原图进行缩放处理
    Mat src_raw = src.clone();
    int cor = (int)(src_raw.cols * 0.2);
    float ratio = src_raw.rows / src_raw.cols;
    Mat src_temp(src_raw, Rect(cor, (int) (cor * ratio), src_raw.cols - 2 * cor,
                               src_raw.rows - 2 * (int) (cor * ratio)));
    Mat src_all;
    Size size = Size(src_raw.cols, src_raw.rows);
    resize(src_temp, src_all, size);

    Mat hsv, bgr, rgb;
    src_all.convertTo(rgb, CV_32FC3, 1.0 / 255, 0);
    cvtColor(rgb, bgr, CV_RGB2BGR);

    *dst = src_all;

    //进行颜色分割
    cvtColor(bgr, hsv, CV_BGR2HSV);

    float hh[WIDTH] = {0};
    float ss[WIDTH] = {0};
    float vv[WIDTH] = {0};
    for (int i = 0; i < hsv.size().width; i++) {
        float h, s, v;
        h = 0;
        s = 0;
        v = 0;
        for (int j = hsv.size().height/2; j < hsv.size().height/2+2; j++)   //hsv.size().height
        {
            Vec3f vec3f = hsv.at<Vec3f>(j, i);
            h += vec3f[0];
            s += vec3f[1];
            v += vec3f[2];
        }
        hh[i] = h;
        ss[i] = s * 255;
        vv[i] = v * 255;
    }
    //判断是否有线缆
    //1900
    int countss = 0;
    int countvv = 0;
    for (int i = 0; i < hsv.size().width; i++) {
        if (ss[i] < 100)countss++;
        if (vv[i] > 250)countvv++;
    }
    if (countss > hsv.size().width * 0.99 && countvv > hsv.size().width * 0.99)
        return false;
    else
        return true;
}

QString ImageProcessingTool::reversalStr(QString &str)
{
    QString ret;
    for(QChar sz:str)
    {
        ret.push_front(sz);
    }
    return ret;
}

QString ImageProcessingTool::processStr(QString &str)
{
    QString ret;
    QChar pre_char;
    for(QChar sz:str)
    {
        if(sz != pre_char)
        ret.push_back(sz);
        pre_char = sz;
    }
    return ret;
}
