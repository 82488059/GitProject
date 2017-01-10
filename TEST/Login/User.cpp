#include "stdafx.h"
#include "User.h"

#include "enterkey.h"
#include "ImageTool.h"
#include "mousetool.h"

User::User()
{
}


User::~User()
{
}

bool User::Input() const
{
	cv::Mat screen = ImageTool::CaptureScreen();

	screen = ImageTool::StandardFormat(screen);

	cv::Point point{ 0, 0 };
	double maxval = 0;
	// ���ұ�
	ImageTool::FindTemplateXY(screen, User::right, point, maxval);
	if (maxval < 0.95)
	{
		return false;
	}
	int right = point.x;
	// �û���y��
	ImageTool::FindTemplateXY(screen, User::nameimage, point, maxval);
	if (maxval < 0.95)
	{
		return false;
	}
	point.x = right;
	mousetool::LClieck(point);
	enterkey::Enter30Backspace();

	if (!enterkey::EnterStr(name))
	{
		return false;
	}
	// ����y��
	ImageTool::FindTemplateXY(screen, User::pwdimage, point, maxval);
	if (maxval < 0.95)
	{
		return false;
	}
	point.x = right;
	mousetool::LClieck(point);
	enterkey::Enter30Backspace();

	if (!enterkey::EnterStr(pwd))
	{
		return false;
	}

	return true;
}

cv::Mat User::right;

cv::Mat User::pwdimage;

cv::Mat User::nameimage;


int User::input = 0;
