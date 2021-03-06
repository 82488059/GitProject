#include "stdafx.h"
#include "Feature.h"
#include "ImageTool.h"

Feature::Feature()
{
}


Feature::~Feature()
{
}
// 检特征点
int Feature::FindFeatureSize(cv::Mat screen)
{
	int find_feat = 0;
	for (int i = 0; i < featList_.size(); ++i)
	{
		cv::Point pt{0,0}; 
		double maxval = 0;
		ImageTool::FindTemplateXY(screen, featList_[i], pt, maxval);
		if (maxval> 0.98)
		{
			++find_feat;
		}
	}
	return find_feat;
}

