
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

    static IplImage* Screen();

    static IplImage* StandardFormat(IplImage* imagein);
};

