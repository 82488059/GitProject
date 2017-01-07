#pragma once
#include "ImageTool.h"

class Click{
public:
	
	bool LBClick(IplImage* screen)
	{
		CvPoint pt{};
		double maxval = 0;
		ImageTool::FindTemplateXY(screen, click_, pt, maxval);
		if (maxval > 0.98)
		{
			return true;
		}
		return false;
	}
	void SetClickImage(IplImage* image)
	{
		click_ = image;
	}
private:
	IplImage* click_{NULL};
	
};
