#pragma once
#include "opencv2/opencv.hpp"
// 一个界面上特征点的集合
class Feature
{
public:
	Feature();
	virtual ~Feature();
	
	typedef std::vector<cv::Mat> FeatureList;
	FeatureList featList_;
	// 检特征点
	int FindFeatureSize(cv::Mat screen);

	void PushFeat(cv::Mat feat)
	{
		featList_.push_back(feat);
	}
	int size()
	{
		return featList_.size();
	}
};

