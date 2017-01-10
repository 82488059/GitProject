#pragma once
#include <string>
#include "include/opencv2/opencv.hpp"
class User
{
public:
	User();
	virtual ~User();
	bool Input() const;

	std::string name;
	std::string pwd;

	static int input;
	static cv::Mat nameimage;
	static cv::Mat pwdimage;
	static cv::Mat right;

};

