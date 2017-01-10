
// LoginDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Login.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "include/opencv2/opencv.hpp"
#include "opencv2/opencv.hpp"
#include <opencv2\highgui\highgui.hpp>  
#include <opencv2\core\core.hpp>  

#include <list>
#include <vector>
#include "tinyxml.h"
#include "test_list.h"
#include "enterkey.h"
#include <thread>
#include <memory>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CLoginDlg �Ի���



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
	ON_BN_CLICKED(IDC_BUTTON4, &CLoginDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CLoginDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CLoginDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CLoginDlg::OnBnClickedButton7)
END_MESSAGE_MAP()


// CLoginDlg ��Ϣ�������

BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CLoginDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CLoginDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CLoginDlg::OnBnClickedButton1()
{
    // ƥ��
    cv::Mat temp = cv::imread("1.bmp");
    cv::Mat src = cv::imread("2.bmp");
    if (temp.empty() || src.empty())
    {
        return;
    }
    TRACE("%d", src.type());
    cv::Point pt{0,0};
    double maxval = 0;
    ImageTool::FindTemplateXY(src, temp, pt, maxval);
    return;
}


void CLoginDlg::OnBnClickedButton2()
{
    // ����
    cv::Mat mat = ImageTool::CaptureScreen();
    if (mat.empty())
    {
        return;
    }
    cv::imwrite("rgba.jpg", mat);
    cv::imwrite("rgba.bmp", mat);
    cv::imwrite("rgba.png", mat);
    return;
}


void CLoginDlg::OnBnClickedButton3()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
    // ��Ļƥ��

    cv::Mat screen = ImageTool::CaptureScreen();
    if (screen.empty())
    {
        return;
    }
    cv::Mat TempPIC;//ģ��ͼ��
    cv::Mat SerchPIC;
    SerchPIC = ImageTool::StandardFormat(screen);
    cv::Point pt{0,0};
    double maxval = 0;
    cv::Mat temp = cv::imread("template\\1.png");
    cv::Mat rgba = cv::imread("rgba.png");

#ifdef _DEBUG
    int rgbtype = rgba.type();
    int tt = temp.type();
#endif // _DEBUG
    
    if (temp.empty())
    {
        return;
    }
    TempPIC = temp;// ImageTool::StandardFormat(temp);
    ImageTool::FindTemplateXY(SerchPIC, TempPIC, pt, maxval);
	if (maxval > 0.95)
	{
		SetCursorPos(pt.x, pt.y);//�ƶ���ĳ������
		Sleep(100);
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, WM_LBUTTONDOWN, 0);//�������
		Sleep(20);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, WM_LBUTTONUP, 0);//�ɿ����
// 		keybd_event(VK_NUMPAD8, MapVirtualKey(VK_NUMPAD8, 2), 0, GetMessageExtraInfo());
// 		keybd_event(VK_NUMPAD8, MapVirtualKey(VK_NUMPAD8, 2), KEYEVENTF_KEYUP, GetMessageExtraInfo());
	
    
    
    }
    return;
}

#define press_down(T) keybd_event((T), MapVirtualKey((T), 2), 0, 0);

#define press_up(T) keybd_event((T), MapVirtualKey((T), 2), KEYEVENTF_KEYUP, 0);

std::shared_ptr<std::thread> thread_;

void CLoginDlg::OnBnClickedButton4()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
    if (thread_)
    {
        thread_->join();
        thread_.reset();
    }
    thread_ = std::make_shared<std::thread>(
        std::bind([](){
        enterkey enter;
        cv::Mat screen = ImageTool::CaptureScreen();
        if (screen.empty())
        {
            return;
        }
        cv::Mat TempPIC;//ģ��ͼ��
        cv::Mat SerchPIC;
        SerchPIC = ImageTool::StandardFormat(screen);
        cv::Point pt{ 0, 0 };
        double maxval = 0;
        cv::Mat tempzh = cv::imread("zh.bmp");
        if (tempzh.empty())
        {
            return;
        }
        TempPIC = tempzh;// ImageTool::StandardFormat(temp);
        ImageTool::FindTemplateXY(SerchPIC, TempPIC, pt, maxval);
        if (maxval < 0.95)
        {
            return;
        }
        SetCursorPos(pt.x, pt.y);//�ƶ���ĳ������

        enterkey::EnterStr("0123456789");
        Sleep(200);

        cv::Mat tempmm = cv::imread("mm.bmp");
        if (tempmm.empty())
        {
            return;
        }
        TempPIC = tempmm;// ImageTool::StandardFormat(temp);
        ImageTool::FindTemplateXY(SerchPIC, TempPIC, pt, maxval);
        if (maxval < 0.95)
        {
            return;
        }

        enterkey::EnterStr("ABCDEFG");
        Sleep(200);
    })
        );
    

}


void CLoginDlg::OnBnClickedButton5()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������


	return;
}


void CLoginDlg::OnBnClickedButton6()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������


}


void CLoginDlg::OnBnClickedButton7()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	test_list test;
	
	if (!test.Init())
	{
		return;
	}
	if (!test.Run())
	{
		return;
	}

}
