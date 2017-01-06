
// LoginDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Login.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "include/opencv2/opencv.hpp"
#include "likeUse.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CLoginDlg 对话框



CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLoginDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON1, &CLoginDlg::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2, &CLoginDlg::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_BUTTON3, &CLoginDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CLoginDlg 消息处理程序

BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CLoginDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CLoginDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CLoginDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CLoginDlg::OnBnClickedButton1()
{
#if 0
    IplImage * TempPIC;//模板图像
    IplImage * SerchPIC;//要搜索的图像
    IplImage * RPIC;//算法返回的图像
    TempPIC = cvLoadImage("template\\1.png");
    SerchPIC = cvLoadImage("2.png");
    DWORD dwBeginTime = ::GetTickCount();
    CvSize Rsize;
    Rsize.height = SerchPIC->height - TempPIC->height + 1;
    Rsize.width = SerchPIC->width - TempPIC->width + 1;
    RPIC = cvCreateImage(Rsize, 32, 1);

    cvMatchTemplate(SerchPIC, TempPIC, RPIC, CV_TM_CCORR_NORMED);
    //cvNormalize(RPIC,RPIC,1,0,CV_MINMAX);
    CvPoint point = cvPoint(0, 0);
    double dMaxval = 0;
    cvMinMaxLoc(RPIC, NULL, &dMaxval, NULL, &point, 0);
    CvRect rect = cvRect(point.x, point.y, TempPIC->width - 1, TempPIC->height - 1);
    CvPoint pt1 = cvPoint(rect.x, rect.y);
    CvPoint pt2 = cvPoint(rect.x + rect.width - 1, rect.y + rect.height - 1);
    cvRectangle(SerchPIC, pt1, pt2, cvScalar(0, 0, 255), 1, 8, 0);
    CvPoint c = cvPoint(rect.x + (rect.width - 1) / 2, rect.y + (rect.height - 1) / 2);

    cvLine(SerchPIC, c, c, CV_RGB(0, 0, 255), 3, 8, 0);
    TRACE("识别中心:x=%d,y=%d\r\n", c.x, c.y);
    TRACE("相似度:%.4f\r\n", dMaxval);
    DWORD dwEndTime = ::GetTickCount();
    DWORD dwSpaceTime = dwEndTime - dwBeginTime;
    TRACE("识别时间:%d\r\n", dwSpaceTime);
    //cvNamedWindow("RPIC");
    //cvShowImage("RPIC",RPIC);
    cvNamedWindow("SerchPIC");
    cvShowImage("SerchPIC", SerchPIC);

    cvWaitKey(0);
    cvDestroyWindow("SerchPIC");
    //cvDestroyWindow("RPIC");
    cvReleaseImage(&TempPIC);
    cvReleaseImage(&SerchPIC);
    cvReleaseImage(&RPIC);
#else
    IplImage * TempPIC;//模板图像
    IplImage * SerchPIC;//要搜索的图像
    IplImage * RPIC;//算法返回的图像
    TempPIC = cvLoadImage("template\\1.png");
    SerchPIC = cvLoadImage("2.png");
    CvPoint pt{};
    double maxval = 0;
    likeUse::FindTemplateXY(SerchPIC, TempPIC, pt, maxval);
    cvReleaseImage(&TempPIC);
    cvReleaseImage(&SerchPIC); 

#endif

}


void CLoginDlg::OnBnClickedButton2()
{
#if 0
    //截屏图片参数
    int image_width;
    int image_height;
    int image_depth;
    int image_nchannels;
    IplImage*  screemImage = NULL;
    int flag = 0;

    // TODO:  在此添加控件通知处理程序代码
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

    if (flag == 0)
    {
        screemImage = cvCreateImage(cvSize(image_width, image_height), image_depth, image_nchannels);
        flag = 1;
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

    IplImage* screenRGB = 0;
    IplImage* screen_resize = 0;
    //CopyScreenToBitmap(); //得到的图片为RGBA格式,即4通道。
    if (!screen_resize)screen_resize = cvCreateImage(cvSize(image_width, image_height), image_depth, image_nchannels);
        cvResize(screemImage, screen_resize, CV_INTER_LINEAR);
    if (!screenRGB)screenRGB = cvCreateImage(cvSize(image_width, image_height), IPL_DEPTH_8U, 3);
        cvCvtColor(screen_resize, screenRGB, CV_RGBA2RGB);
    //cvShowImage("s_laplace", screenRGB);
    cvSaveImage("rgba.jpg", screen_resize);
    //cvWaitKey(10);
    
    //cvDestroyAllWindows();
    return;
#else
    IplImage* screen= likeUse::Screen();
    if (!screen)
    {
        return;
    }
    cvSaveImage("rgba.jpg", screen);
    cvReleaseImage(&screen);
    return;
#endif
}


void CLoginDlg::OnBnClickedButton3()
{
    // TODO:  在此添加控件通知处理程序代码
    IplImage * TempPIC = NULL;//模板图像
    IplImage * SerchPIC = NULL;//算法返回的图像

    IplImage* screen = likeUse::Screen();
    if (!screen)
    {
        return;
    }
    SerchPIC = likeUse::StandardFormat(screen);
    cvReleaseImage(&screen);

    
    CvPoint pt{};
    double maxval = 0;
    IplImage* temp = cvLoadImage("template\\1.bmp");
    TempPIC = likeUse::StandardFormat(temp);

    likeUse::FindTemplateXY(SerchPIC, TempPIC, pt, maxval);

    cvReleaseImage(&TempPIC);
    cvReleaseImage(&SerchPIC);

    return;
}
