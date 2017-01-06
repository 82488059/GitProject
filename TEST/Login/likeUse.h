#pragma once
#include "opencv2/opencv.hpp"

class likeUse
{
public:
    likeUse();
    virtual ~likeUse();


public:
    static bool FindTemplateXY(IplImage* src, IplImage* templ, CvPoint& pt, double& maxval);

};

