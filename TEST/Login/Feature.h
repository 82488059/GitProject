#pragma once
#include "opencv2/opencv.hpp"
// 一个界面上特征点的集合
class Feature
{
public:
	Feature();
	virtual ~Feature();
	
	typedef std::vector<IplImage*> FeatureList;
	FeatureList featList_;
	// 检特征点
	int Check(IplImage* screen);

	void PushFeat(IplImage* feat)
	{
		featList_.push_back(feat);
	}
	int size()
	{
		return featList_.size();
	}
};

