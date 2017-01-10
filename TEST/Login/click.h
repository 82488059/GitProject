#pragma once
#include "ImageTool.h"
#include "mousetool.h"

class Click{
public:
	
	bool LBClick(cv::Mat screen)
	{
		cv::Point pt{};
		double maxval = 0;
		ImageTool::FindTemplateXY(screen, click_, pt, maxval);
		if (maxval > 0.95)
		{
			mousetool::LClieck(pt);
			return true;
		}
		return false;
	}
	void SetClickImage(cv::Mat image)
	{
		click_ = image;
	}
private:
	cv::Mat click_;
	
};
