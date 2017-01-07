#include "stdafx.h"

#include "plug.h"

#include "include/opencv2/opencv.hpp"


char plug::s_name[][260]= { "login", "input", "use", "exit" };
char plug::s_conf[] = "conf\\conf.ini";
std::vector<plug::NAP> plug::naplist_;
std::vector<std::vector<plug::SOP>> plug::soplist_;
fsm plug::fsm_;

plug::plug()
{
}


plug::~plug()
{
}

bool plug::LoadNAP()
{
	int max = GetPrivateProfileIntA("USER_LIST", "MAX", 0, "conf\\conf.ini");

	if (0 == max)
	{
		return false;
	}
	char BUF1[MAX_PATH];
	char BUF2[MAX_PATH];
	for (int i = 0; i < max; ++i)
	{
		sprintf_s(BUF2, "%d", i + 1);
		DWORD l = GetPrivateProfileStringA("USER_LIST", BUF2, 0, BUF1, MAX_PATH, "conf\\conf.ini");
		if (0 == BUF1[0])
		{
			max = i;
			return 0 != max;
		}

		std::string tmp(BUF1);
		int n = tmp.find(',');
		NAP nap;
		nap.n = tmp.substr(0, n);
		nap.p = tmp.substr(n + 1, tmp.size() - 1);
		naplist_.push_back(nap);
	}

	return true;



}

bool plug::LoadSOP()
{
	soplist_.resize(em_max);
	for (int i = 0; i < em_max; ++i)
	{
		int max = GetPrivateProfileIntA("CONF", s_name[i], 0,plug::s_name[i]);
		if (0 == max)
		{
			return false;
		}
		char BUF2[MAX_PATH];
		for (int j = 0; j < max; ++j)
		{
			sprintf_s(BUF2, "%s_%d.bmp", s_name[i], j);
			
			SOP nap;
			nap.name = BUF2;
			nap.type = i;
			nap.n = j;
			sprintf_s(BUF2, "conf\\%s_%d.bmp", s_name[i], j);

			nap.image1 = cvLoadImage(BUF2);
			nap.image2 = plug::StandardFormat(nap.image1);

			soplist_[i].push_back(nap);
		}
	}
	

	return fsm_.LoadConf();
}

IplImage* plug::Screen()
{
	//截屏
	int image_width;
	int image_height;
	int image_depth;
	int image_nchannels;
	IplImage*  screemImage = NULL;

	int right = GetSystemMetrics(SM_CXSCREEN), left = 0, top = 0, bottom = GetSystemMetrics(SM_CYSCREEN);//定义截屏范围 此处设为全屏
	int nWidth, nHeight;
	HDC      hSrcDC = NULL, hMemDC = NULL;
	HBITMAP hBitmap = NULL, hOldBitmap = NULL;

	hSrcDC = CreateDC(L"DISPLAY", NULL, NULL, NULL);
	hMemDC = CreateCompatibleDC(hSrcDC);
	nWidth = right - left;
	nHeight = bottom - top;

	hBitmap = CreateCompatibleBitmap(hSrcDC, nWidth, nHeight);
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

	BitBlt(hMemDC, 0, 0, nWidth, nHeight, hSrcDC, left, top, SRCCOPY);
	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);

	BITMAP bmp;
	int nChannels, depth;
	BYTE *pBuffer;
	GetObject(hBitmap, sizeof(BITMAP), &bmp);
	image_nchannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8;
	image_depth = bmp.bmBitsPixel == 1 ? IPL_DEPTH_1U : IPL_DEPTH_8U;
	image_width = bmp.bmWidth;
	image_height = bmp.bmHeight;

	screemImage = cvCreateImage(cvSize(image_width, image_height), image_depth, image_nchannels);
	if (!screemImage)
	{
		return screemImage;
	}
	pBuffer = new BYTE[image_width*image_height*image_nchannels];
	GetBitmapBits(hBitmap, image_height*image_width*image_nchannels, pBuffer);
	memcpy(screemImage->imageData, pBuffer, image_height*image_width*image_nchannels);
	delete pBuffer;

	SelectObject(hMemDC, hOldBitmap);
	DeleteObject(hOldBitmap);
	DeleteDC(hMemDC);
	SelectObject(hSrcDC, hBitmap);
	DeleteDC(hMemDC);
	DeleteObject(hBitmap);

	IplImage* screem = StandardFormat(screemImage);
	cvReleaseImage(&screemImage);
	return screem;
#if 0
	// 转 IplImage
	IplImage* screenRGB = 0;
	IplImage* screen_resize = 0;
	//CopyScreenToBitmap(); //得到的图片为RGBA格式,即4通道。
	if (!screen_resize)
		screen_resize = cvCreateImage(cvSize(image_width, image_height), image_depth, image_nchannels);
	cvResize(screemImage, screen_resize, CV_INTER_LINEAR);
	if (!screenRGB)
		screenRGB = cvCreateImage(cvSize(image_width, image_height), IPL_DEPTH_8U, 3);
	cvCvtColor(screen_resize, screenRGB, CV_RGBA2RGB);
	//cvShowImage("s_laplace", screenRGB);
	//cvSaveImage("rgba.jpg", screen_resize);
	//cvWaitKey(10);

	//cvDestroyAllWindows();
	return screen_resize;
#endif

}
IplImage* plug::StandardFormat(IplImage* imagein)
{
	IplImage* TempPIC = cvCreateImage(cvSize(imagein->width, imagein->height), IPL_DEPTH_8U, 3);
	cvCvtColor(imagein, TempPIC, CV_RGBA2RGB);
	return TempPIC;
}

bool plug::FindAndClick(IplImage* temp)
{
	if (!temp)
	{
		return false;
	}

	IplImage * TempPIC = NULL;//模板图像
	IplImage * SerchPIC = NULL;//算法返回的图像
	bool flag = false;
	IplImage* screen = Screen();
	if (!screen)
	{
		return false;
	}
	SerchPIC = StandardFormat(screen);
	CvPoint pt{};
	double maxval = 0;
	//IplImage* temp = cvLoadImage("template\\login\\x.bmp" + name_);
	TempPIC = StandardFormat(temp);
	FindTemplateXY(SerchPIC, TempPIC, pt, maxval);
	if (maxval > 0.95)
	{
		SetCursorPos(pt.x, pt.y);//移动到某点坐标
		Sleep(500);
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, WM_LBUTTONDOWN, 0);//点下左键
		Sleep(20);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, WM_LBUTTONUP, 0);//松开左键
		// 		keybd_event(VK_NUMPAD8, MapVirtualKey(VK_NUMPAD8, 2), 0, GetMessageExtraInfo());
		// 		keybd_event(VK_NUMPAD8, MapVirtualKey(VK_NUMPAD8, 2), KEYEVENTF_KEYUP, GetMessageExtraInfo());
		flag = true;
	}

	//     cvReleaseImage(&temp);
	cvReleaseImage(&screen);
	cvReleaseImage(&TempPIC);
	cvReleaseImage(&SerchPIC);
	return flag;

}

bool plug::FindTemplateXY(IplImage*& src, IplImage*& TempPIC, CvPoint& cpt, double& maxval)
{
	//     IplImage * templ;//模板图像
	//     IplImage * src;//要搜索的图像
	IplImage * RPIC;//算法返回的图像
	//     templ = cvLoadImage("template\\1.png");
	//     src = cvLoadImage("2.png");

	DWORD dwBeginTime = ::GetTickCount();
	CvSize Rsize;
	Rsize.height = src->height - TempPIC->height + 1;
	Rsize.width = src->width - TempPIC->width + 1;
	RPIC = cvCreateImage(Rsize, 32, 1);

	cvMatchTemplate(src, TempPIC, RPIC, CV_TM_CCORR_NORMED);
	//cvNormalize(RPIC,RPIC,1,0,CV_MINMAX);
	CvPoint point = cvPoint(0, 0);
	maxval = 0;
	cvMinMaxLoc(RPIC, NULL, &maxval, NULL, &point, 0);
	CvRect rect = cvRect(point.x, point.y, TempPIC->width - 1, TempPIC->height - 1);
	CvPoint pt1 = cvPoint(rect.x, rect.y);
	CvPoint pt2 = cvPoint(rect.x + rect.width - 1, rect.y + rect.height - 1);
	//cvRectangle(src, pt1, pt2, cvScalar(0, 0, 255), 1, 8, 0);
	cpt = cvPoint(rect.x + (rect.width - 1) / 2, rect.y + (rect.height - 1) / 2);
	//cvLine(src, cpt, cpt, CV_RGB(0, 0, 255), 3, 8, 0);
	TRACE("\r\n识别中心:x=%d,y=%d\r\n", cpt.x, cpt.y);
	TRACE("相似度:%.4f\r\n", maxval);
	DWORD dwEndTime = ::GetTickCount();
	DWORD dwSpaceTime = dwEndTime - dwBeginTime;
	TRACE("识别时间:%d\r\n", dwSpaceTime);
	//cvNamedWindow("RPIC");
	//cvShowImage("RPIC",RPIC);
	//cvNamedWindow("SerchPIC");
	//cvShowImage("SerchPIC", SerchPIC);

	//cvWaitKey(0);
	//cvDestroyWindow("SerchPIC");
	//cvDestroyWindow("RPIC");
	//cvReleaseImage(&TempPIC);
	//cvReleaseImage(&SerchPIC);
	cvReleaseImage(&RPIC);
	return true;
}

bool plug::GotoLogin(int type, int index)
{
	double maxval = 0;
	if (!Detection(type, index, maxval))
	{
		return false;
	}
	if (maxval < MAX_VALUE)
	{
		return false;
	}
	if (type == em_login)
	{
		return true;
	}
	int faild_time = 0;
	bool isfind = false;
	for (int i = type; i < soplist_.size(); ++i)
	{
		for (auto j = index; j < soplist_[i].size();)
		{
			IplImage* screen = Screen();
			CvPoint pt{};
			FindTemplateXY(screen, soplist_[i][j].image1, pt, maxval);
			cvReleaseImage(&screen);
			if (maxval > MAX_VALUE)
			{
				if (soplist_[i][j].type == em_exit && soplist_[i][j].n == soplist_[i].size() - 1)
				{
					LClieck(pt);
					TRACE("game is exit %s\r\n", soplist_[i][j].name.c_str());
					return true;
				}
				LClieck(pt);
				Sleep(500);
				++j;
			}
			else
			{
				if (soplist_[i][j].type == em_use)
				{
					LClieck();
					Sleep(100);
					++j;
					continue;
				}
				++faild_time;
				if (faild_time > 30)
				{
					return false;
				}
				LClieck();
				Sleep(1000);
			}
		}
	}

	return false;
}

void plug::LClieck(CvPoint pt)
{
	if (pt.x != 0 || pt.y != 0 )
	{
		SetCursorPos(pt.x, pt.y);
	}
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, WM_LBUTTONDOWN, 0);//点下左键
	Sleep(20);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, WM_LBUTTONUP, 0);//松开左键
}

bool plug::AutoRun()
{
	for (auto it : naplist_)
	{
		int type = 0, index = 0;
		double maxval = 0;

		plug::Detection(type, index, maxval);

		if (!plug::GotoLogin(type, index))
		{
			return false;
		}
		if (!plug::GotoInput(type, index))
		{
			return false;
		}
		LoginUser(it.n, it.p);
	}
	return false;
}

bool plug::LoginUser(std::string name, std::string pwd)
{

	return false;
}

bool plug::FindLogin(int &index)
{
	return FiindXX(index, em_login);
}

bool plug::FindExit(int &index)
{
	return FiindXX(index, em_exit);
}

bool plug::FindInput(int &index)
{
	return FiindXX(index, em_input);
}

bool plug::FindUse(int& index)
{
	return FiindXX(index, em_use);
}

bool plug::FiindXX(int& index, int type)
{
	for (auto it1 : soplist_[type])
	{
		IplImage* screen = Screen();
		//cvSaveImage("screen.png", screen);
		CvPoint pt{};
		double maxval = 0;
		FindTemplateXY(screen, it1.image1, pt, maxval);
		cvReleaseImage(&screen);
		if (maxval > MAX_VALUE)
		{
			if (it1.type == em_login)
			{
				index = it1.n;
				TRACE("type = %d is find\r\n", type);
				return true;
			}
		}
	}
	return false;
}

bool plug::Detection(int& type, int & index, double & maxval)
{
	IplImage* screen = Screen();
	cvSaveImage("screen.png", screen);

	CvPoint pt{};

	for (int i = type; i < soplist_.size(); ++i)
	{
		for (int j = index; j < soplist_[i].size(); ++j)
		{
			double new_maxval = 0;
			FindTemplateXY(screen, soplist_[i][j].image1, pt, new_maxval);
			if (new_maxval > maxval)
			{
				maxval = new_maxval;
				type = soplist_[i][j].type;
				index = soplist_[i][j].n;
			}
		}
	}
	if (maxval > MAX_VALUE)
	{
		Adjust(screen, type, index, maxval);
		cvReleaseImage(&screen);
		return true;
	}
	cvReleaseImage(&screen);
	return false;
}

bool plug::Adjust(IplImage* screen, int& type, int & index, double & maxval)
{
	if (em_login == type && soplist_[em_login].size()-1 == index)
	{
		CvPoint pt{};
		double new_maxval = 0;
		FindTemplateXY(screen, soplist_[em_input][0].image1, pt, new_maxval);
		if (new_maxval > maxval)
		{
			type = em_input;
			index = 0;
			maxval = new_maxval;
			return true;
		}
		return false;
	}
	if (em_input == type && soplist_[em_input].size()-1 == index)
	{
		CvPoint pt{};
		double new_maxval = 0;
		FindTemplateXY(screen, soplist_[em_input][0].image1, pt, new_maxval);
		if (new_maxval < MAX_VALUE)
		{
			type = em_login;
			index = 3;
			return true;
		}
		return false;
	}
	if (em_exit == type && soplist_[em_exit].size()-2 == index)
	{
		CvPoint pt{};
		double new_maxval = 0;
		FindTemplateXY(screen, soplist_[em_exit][soplist_[em_exit].size()-1].image1, pt, new_maxval);
		if (new_maxval > maxval)
		{
			type = em_exit;
			index = 3;
			maxval = new_maxval;
			return true;
		}
		return false;
	}


	return false;
}

bool plug::GotoInput(int type /*= 0*/, int index /*= 0*/)
{
	double maxval = 0;
	if (!Detection(type, index, maxval))
	{
		return false;
	}
	if (maxval < MAX_VALUE)
	{
		return false;
	}
	if (type == em_input)
	{
		return true;
	}
	int faild_time = 0;
	bool isfind = false;
	for (int i = type; i < soplist_.size(); ++i)
	{
		for (auto j = index; j < soplist_[i].size();)
		{
			IplImage* screen = Screen();
			CvPoint pt{};
			FindTemplateXY(screen, soplist_[i][j].image1, pt, maxval);
			cvReleaseImage(&screen);
			if (maxval > MAX_VALUE)
			{
				if (soplist_[i][j].type == em_login && soplist_[i][j].n == soplist_[i].size() - 1)
				{
					LClieck(pt);
					TRACE("is input %s\r\n", soplist_[i][j].name.c_str());
					return true;
				}
				LClieck(pt);
				Sleep(500);
				++j;
			}
			else
			{
				if (soplist_[i][j].type == em_use)
				{
					LClieck();
					Sleep(10);
					++j;
					continue;
				}
				++faild_time;
				if (faild_time > 100)
				{
					return false;
				}
				Sleep(1000);
			}
		}
	}

	return false;
}


