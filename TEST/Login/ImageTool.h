
#pragma once
#include "opencv2/opencv.hpp"
#include <string>
#include <list>


class ImageTool
{
public:
    ImageTool();
    virtual ~ImageTool();

public:
    static bool FindTemplateXY(IplImage*& src, IplImage*& templ, CvPoint& pt, double& maxval);
    static bool FindTemplateXY(cv::Mat& src, cv::Mat& templ, cv::Point& pt, double& maxval);

    static IplImage* Screen();
    static cv::Mat CaptureScreen();


    static IplImage* StandardFormat(IplImage* imagein);
    static cv::Mat StandardFormat(cv::Mat& imagein);
};

