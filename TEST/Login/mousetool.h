#pragma once
#include "include/opencv2/opencv.hpp"

class mousetool
{
public:
	mousetool();
	virtual ~mousetool();

	static void LClieck(CvPoint pt)
	{
		if (pt.x != 0 || pt.y != 0)
		{
			SetCursorPos(pt.x, pt.y);
		}
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, WM_LBUTTONDOWN, 0);//µãÏÂ×ó¼ü
		Sleep(20);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, WM_LBUTTONUP, 0);//ËÉ¿ª×ó¼ü
	}
};

