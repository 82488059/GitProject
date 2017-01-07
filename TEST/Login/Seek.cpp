#include "stdafx.h"
#include "Seek.h"
#include "ImageTool.h"

Seek::Seek()
	:action("seek")
{
}


Seek::~Seek()
{
}

bool Seek::Run()
{
	bool find = false;
	int index = 0, step = 0;
	IplImage* screen = ImageTool::Screen();

	for (int i = 0; i < actionlist_.size(); ++i)
	{
		if (actionlist_[i].Check(screen))
		{
			find = true;
		}
	}
	cvReleaseImage(&screen);

	if (!find)
	{
		TRACE("不可处理的页面！");
		return false;
	}

	do 
	{
		int exit = 0;
		
	} while (exit);


	return false;
}
bool Next(std::string s)
{



	return false;
}
