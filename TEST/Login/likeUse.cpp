#include "stdafx.h"
#include "likeUse.h"


likeUse::likeUse()
{
}


likeUse::~likeUse()
{
}

bool likeUse::FindTemplateXY(IplImage* src, IplImage* templ, CvPoint& cpt, double& maxval)
{
//     IplImage * templ;//模板图像
//     IplImage * src;//要搜索的图像
    IplImage * RPIC;//算法返回的图像
//     templ = cvLoadImage("template\\1.png");
//     src = cvLoadImage("2.png");
    
    DWORD dwBeginTime = ::GetTickCount();
    CvSize Rsize;
    Rsize.height = src->height - templ->height + 1;
    Rsize.width = src->width - templ->width + 1;
    RPIC = cvCreateImage(Rsize, 32, 1);

    cvMatchTemplate(src, templ, RPIC, CV_TM_CCORR_NORMED);
    //cvNormalize(RPIC,RPIC,1,0,CV_MINMAX);
    CvPoint point = cvPoint(0, 0);
    maxval = 0;
    cvMinMaxLoc(RPIC, NULL, &maxval, NULL, &point, 0);
    CvRect rect = cvRect(point.x, point.y, templ->width - 1, templ->height - 1);
    CvPoint pt1 = cvPoint(rect.x, rect.y);
    CvPoint pt2 = cvPoint(rect.x + rect.width - 1, rect.y + rect.height - 1);
    cvRectangle(src, pt1, pt2, cvScalar(0, 0, 255), 1, 8, 0);
    
    cpt = cvPoint(rect.x + (rect.width - 1) / 2, rect.y + (rect.height - 1) / 2);

    cvLine(src, cpt, cpt, CV_RGB(0, 0, 255), 3, 8, 0);
    TRACE("识别中心:x=%d,y=%d\r\n", cpt.x, cpt.y);
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
