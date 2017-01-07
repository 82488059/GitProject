#pragma once

#include "Feature.h"
#include "click.h"

class OneSpace
{
public:	
	struct Index{
		int type{ 0 };
		int step{ 0 };
	};
	OneSpace(int type, int step);
	virtual ~OneSpace();

	void PushFeat(IplImage* image)
	{
		feat_.PushFeat(image);
	}
	bool Check(IplImage* screen)
	{
		return feat_.Check(screen) == feat_.size();
	}
	void SetClickImage(IplImage* image)
	{
		click_.SetClickImage(image);
	}
	bool Run(IplImage* screen)
	{
		if (Check(screen))
		{

		}
		return false;
	}
	bool LBClick(IplImage* screen)
	{
		return click_.LBClick(screen);
	}
private:
	// 特征集合
	Feature feat_;
	// 点击的点
	Click click_;
	
	// 索引
	Index index_;
};

