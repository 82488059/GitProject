#pragma once

#include "Feature.h"
#include "click.h"
#include <memory>


class OneSpace  
{
public:	
	//struct Info
	//{
		int feat{ 0 }; // 特征点数量 
		int must{ 0 }; // 必须找到的点
		int fto{ -1 };// 没找到跳转
		int to{ -1 };// 找到后跳转
		int click{ 1 };// 要点击 
		int step{ 0 };
	//};

	OneSpace(int step);
	virtual ~OneSpace();

	void PushFeat(cv::Mat image)
	{
		feat_.PushFeat(image);
	}
	bool Check(cv::Mat screen)
	{
		return feat_.FindFeatureSize(screen) == feat_.size();
	}
	void SetClickImage(cv::Mat image)
	{
		click_.SetClickImage(image);
	}
	bool Run(cv::Mat screen, int & next)
	{
		if (Check(screen))
		{
			if (LBClick(screen))
			{
				next = to;
				return true;
			}
			next = step;
			return false;
		}
		if (-1 == fto)
		{
			next = step+1;
		}
		else
		{
			next = fto;
		}
		return false;
	}
	bool LBClick(cv::Mat screen)
	{
		return click_.LBClick(screen);
	}
private:
	// 特征集合
	Feature feat_;
	// 点击的点
	Click click_;
	
	OneSpace(const OneSpace&) = delete;
	OneSpace& operator=(const OneSpace&) = delete;

};

typedef std::shared_ptr<OneSpace> OneSpacePtr;