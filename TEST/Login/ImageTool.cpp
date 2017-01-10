#include "stdafx.h"
#include "ImageTool.h"


ImageTool::ImageTool()
{
}


ImageTool::~ImageTool()
{
}

bool ImageTool::FindTemplateXY(IplImage*& src, IplImage*& TempPIC, CvPoint& cpt, double& maxval)
{
//     IplImage * templ;//模板图像
//     IplImage * src;//要搜索的图像
    IplImage * RPIC;//算法返回的图像
//     templ = cvLoadImage("template\\1.png");
//     src = cvLoadImage("2.png");
    
    DWORD dwBeginTime = ::GetTickCount();
    CvSize Rsize;
    Rsize.height = src->height - TempPIC->height + 1;
    Rsize.width = src->width - TempPIC->width + 1;
    RPIC = cvCreateImage(Rsize, 32, 1);

    cvMatchTemplate(src, TempPIC, RPIC, CV_TM_CCORR_NORMED);
    //cvNormalize(RPIC,RPIC,1,0,CV_MINMAX);
    CvPoint point = cvPoint(0, 0);
    maxval = 0;
    cvMinMaxLoc(RPIC, NULL, &maxval, NULL, &point, 0);
    CvRect rect = cvRect(point.x, point.y, TempPIC->width - 1, TempPIC->height - 1);
    CvPoint pt1 = cvPoint(rect.x, rect.y);
    CvPoint pt2 = cvPoint(rect.x + rect.width - 1, rect.y + rect.height - 1);
    cvRectangle(src, pt1, pt2, cvScalar(0, 0, 255), 1, 8, 0);
    
    cpt = cvPoint(rect.x + (rect.width - 1) / 2, rect.y + (rect.height - 1) / 2);

    //cvLine(src, cpt, cpt, CV_RGB(0, 0, 255), 3, 8, 0);
    TRACE("\r\n识别中心:x=%d,y=%d\r\n", cpt.x, cpt.y);
    TRACE("相似度:%.4f\r\n", maxval);
    DWORD dwEndTime = ::GetTickCount();
    DWORD dwSpaceTime = dwEndTime - dwBeginTime;
    TRACE("识别时间:%d\r\n", dwSpaceTime);
    //cvNamedWindow("RPIC");
    //cvShowImage("RPIC",RPIC);
    //cvNamedWindow("SerchPIC");
    //cvShowImage("SerchPIC", SerchPIC);

    //cvWaitKey(0);
    //cvDestroyWindow("SerchPIC");
    //cvDestroyWindow("RPIC");
    //cvReleaseImage(&TempPIC);
    //cvReleaseImage(&SerchPIC);
    cvReleaseImage(&RPIC);
    return true;
}

bool ImageTool::FindTemplateXY(cv::Mat& src, cv::Mat& TempPIC, cv::Point& cpt, double& maxval)
{
    //     IplImage * templ;//模板图像
    //     IplImage * src;//要搜索的图像
    cv::Mat RPIC;//算法返回的图像
    //     templ = cvLoadImage("template\\1.png");
    //     src = cvLoadImage("2.png");

    DWORD dwBeginTime = ::GetTickCount();
//     CvSize Rsize;
//     Rsize.height = src.height - TempPIC.height + 1;
//     Rsize.width = src.width - TempPIC.width + 1;
//  RPIC=cvCreateImage(Rsize, 32, 1);
    int result_cols = src.cols - TempPIC.cols + 1;
    int result_rows = src.rows - TempPIC.rows + 1;
    RPIC.create(result_rows, result_cols, src.type());
    cv::matchTemplate(src, TempPIC, RPIC, CV_TM_CCORR_NORMED);
    //cvNormalize(RPIC,RPIC,1,0,CV_MINMAX);
    //cv::normalize(RPIC, RPIC, 0, 1, cv::NORM_MINMAX);
    cv::Point point(0, 0);// = cvPoint(0, 0);
    maxval = 0;
    cv::Point minLoc, maxLoc, matchLoc;
    cv::minMaxLoc(RPIC, 0, &maxval, &minLoc, &point);
    CvRect rect = cvRect(point.x, point.y, TempPIC.cols - 1, TempPIC.rows - 1);
//     CvPoint pt1 = cvPoint(rect.x, rect.y);
//     CvPoint pt2 = cvPoint(rect.x + rect.width - 1, rect.y + rect.height - 1);
//     cvRectangle(src, pt1, pt2, cvScalar(0, 0, 255), 1, 8, 0);

    cpt = cv::Point(rect.x + (rect.width - 1) / 2, rect.y + (rect.height - 1) / 2);

    //cvLine(src, cpt, cpt, CV_RGB(0, 0, 255), 3, 8, 0);
    TRACE("\r\n识别中心:x=%d,y=%d\r\n", cpt.x, cpt.y);
    TRACE("相似度:%.4f\r\n", maxval);
    DWORD dwEndTime = ::GetTickCount();
    DWORD dwSpaceTime = dwEndTime - dwBeginTime;
    TRACE("识别时间:%d\r\n", dwSpaceTime);
    return true;
}

IplImage* ImageTool::Screen()
{
    //截屏
    int image_width;
    int image_height;
    int image_depth;
    int image_nchannels;
    IplImage*  screemImage = NULL;

    int right = GetSystemMetrics(SM_CXSCREEN), left = 0, top = 0, bottom = GetSystemMetrics(SM_CYSCREEN);//定义截屏范围 此处设为全屏
    int nWidth, nHeight;
    HDC      hSrcDC = NULL, hMemDC = NULL;
    HBITMAP hBitmap = NULL, hOldBitmap = NULL;

    hSrcDC = CreateDC("DISPLAY", NULL, NULL, NULL);
    hMemDC = CreateCompatibleDC(hSrcDC);
    nWidth = right - left;
    nHeight = bottom - top;

    hBitmap = CreateCompatibleBitmap(hSrcDC, nWidth, nHeight);
    hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

    BitBlt(hMemDC, 0, 0, nWidth, nHeight, hSrcDC, left, top, SRCCOPY);
    hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);

    BITMAP bmp;
    int nChannels, depth;
    BYTE *pBuffer;
    GetObject(hBitmap, sizeof(BITMAP), &bmp);
    image_nchannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8;
    image_depth = bmp.bmBitsPixel == 1 ? IPL_DEPTH_1U : IPL_DEPTH_8U;
    image_width = bmp.bmWidth;
    image_height = bmp.bmHeight;

    screemImage = cvCreateImage(cvSize(image_width, image_height), image_depth, image_nchannels);
    if (!screemImage)
    {
        return screemImage;
    }
    pBuffer = new BYTE[image_width*image_height*image_nchannels];
    GetBitmapBits(hBitmap, image_height*image_width*image_nchannels, pBuffer);
    memcpy(screemImage->imageData, pBuffer, image_height*image_width*image_nchannels);
    delete pBuffer;

    SelectObject(hMemDC, hOldBitmap);
    DeleteObject(hOldBitmap);
    DeleteDC(hMemDC);
    SelectObject(hSrcDC, hBitmap);
    DeleteDC(hMemDC);
    DeleteObject(hBitmap);
	IplImage* screem = StandardFormat(screemImage);
	cvReleaseImage(&screemImage);
	return screem;
#if 0
    // 转 IplImage
    IplImage* screenRGB = 0;
    IplImage* screen_resize = 0;
    //CopyScreenToBitmap(); //得到的图片为RGBA格式,即4通道。
    if (!screen_resize)
        screen_resize = cvCreateImage(cvSize(image_width, image_height), image_depth, image_nchannels);
    cvResize(screemImage, screen_resize, CV_INTER_LINEAR);
    if (!screenRGB)
        screenRGB = cvCreateImage(cvSize(image_width, image_height), IPL_DEPTH_8U, 3);
    cvCvtColor(screen_resize, screenRGB, CV_RGBA2RGB);
    //cvShowImage("s_laplace", screenRGB);
    //cvSaveImage("rgba.jpg", screen_resize);
    //cvWaitKey(10);

    //cvDestroyAllWindows();
    return screen_resize;
#endif
    
}

IplImage* ImageTool::StandardFormat(IplImage* imagein)
{
    IplImage* TempPIC = cvCreateImage(cvSize(imagein->width, imagein->height), IPL_DEPTH_8U, 3);
    cvCvtColor(imagein, TempPIC, CV_RGBA2RGB);
    return TempPIC;
}

cv::Mat ImageTool::StandardFormat(cv::Mat& imagein)
{
    cv::Mat TempPIC;
    // cvCreateImage(cvSize(imagein->width, imagein->height), IPL_DEPTH_8U, 3);
    TempPIC.create(imagein.size(), CV_8UC3/*imagein.type()*/);
    if (TempPIC.empty())
    {
        return TempPIC;
    }
    cv::cvtColor(imagein, TempPIC, CV_RGBA2RGB);
    return TempPIC;
}

#if 1
cv::Mat ImageTool::CaptureScreen()
{
    //截屏
    int image_width;
    int image_height;
    int image_depth;
    int image_nchannels;
    cv::Mat  screemImage;

    int right = GetSystemMetrics(SM_CXSCREEN), left = 0, top = 0, bottom = GetSystemMetrics(SM_CYSCREEN);//定义截屏范围 此处设为全屏
    int nWidth, nHeight;
    HDC      hSrcDC = NULL, hMemDC = NULL;
    HBITMAP hBitmap = NULL, hOldBitmap = NULL;

    hSrcDC = CreateDC("DISPLAY", NULL, NULL, NULL);
    hMemDC = CreateCompatibleDC(hSrcDC);
    nWidth = right - left;
    nHeight = bottom - top;

    hBitmap = CreateCompatibleBitmap(hSrcDC, nWidth, nHeight);
    hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

    BitBlt(hMemDC, 0, 0, nWidth, nHeight, hSrcDC, left, top, SRCCOPY);
    hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);

    BITMAP bmp;
    int nChannels, depth;
    BYTE *pBuffer;
    GetObject(hBitmap, sizeof(BITMAP), &bmp);
    image_nchannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8;
    image_depth = bmp.bmBitsPixel == 1 ? IPL_DEPTH_1U : IPL_DEPTH_8U;
    image_width = bmp.bmWidth;
    image_height = bmp.bmHeight;

//     screemImage = cvCreateImage(cvSize(image_width, image_height), image_depth, image_nchannels);
    screemImage.create(cv::Size(image_width, image_height), CV_MAKE_TYPE(image_depth, image_nchannels)/*image_depth, image_nchannels*/);// = cvCreateImage(cvSize(image_width, image_height), image_depth, image_nchannels);
    if (screemImage.empty())
    {
        return screemImage;
    }
    pBuffer = new BYTE[image_width*image_height*image_nchannels];
    GetBitmapBits(hBitmap, image_height*image_width*image_nchannels, pBuffer);

    memcpy(screemImage.data/*screemImage->imageData*/, pBuffer, image_height*image_width*image_nchannels);
    
    delete pBuffer;

    SelectObject(hMemDC, hOldBitmap);
    DeleteObject(hOldBitmap);
    DeleteDC(hMemDC);
    SelectObject(hSrcDC, hBitmap);
    DeleteDC(hMemDC);
    DeleteObject(hBitmap);
    // cv::Mat screem = StandardFormat(screemImage);

    return screemImage;
}
#endif