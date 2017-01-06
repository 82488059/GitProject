
// TESTDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TEST.h"
#include "TESTDlg.h"
#include "afxdialogex.h"
#include <mfc/process.h>
#include <mfc/screen.h>
#include <thread>
#include <memory>

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


// CTESTDlg 对话框



CTESTDlg::CTESTDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTESTDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTESTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTESTDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BT_GO, &CTESTDlg::OnBnClickedBtGo)
    ON_BN_CLICKED(IDC_BT_GO2, &CTESTDlg::OnBnClickedBtGo2)
    ON_BN_CLICKED(IDC_BT_GO3, &CTESTDlg::OnBnClickedBtGo3)
    ON_BN_CLICKED(IDC_BT_GO4, &CTESTDlg::OnBnClickedBtGo4)
    ON_BN_CLICKED(IDC_BT_GO5, &CTESTDlg::OnBnClickedBtGo5)
    ON_BN_CLICKED(IDC_BT_GO6, &CTESTDlg::OnBnClickedBtGo6)
END_MESSAGE_MAP()


// CTESTDlg 消息处理程序

BOOL CTESTDlg::OnInitDialog()
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

void CTESTDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTESTDlg::OnPaint()
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
HCURSOR CTESTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
BOOL SaveHwndToBmpFile(HWND hWnd, LPCTSTR lpszPath)
{
    HWND hDesktop = ::GetDesktopWindow();
    ASSERT(hDesktop);
    if (NULL == hWnd)
    {
        hWnd = hDesktop;
    }
    RECT rect;
    ::GetWindowRect(hWnd, &rect);

    int nWidht = rect.right - rect.left;
    int nHeight = rect.bottom - rect.top;

    HDC hSrcDC = ::GetWindowDC(hWnd);
    ASSERT(hSrcDC);
    HDC hMemDC = ::CreateCompatibleDC(hSrcDC);
    ASSERT(hMemDC);
    HBITMAP hBitmap = ::CreateCompatibleBitmap(hSrcDC, nWidht, nHeight);
    ASSERT(hBitmap);
    HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hMemDC, hBitmap);
    ::BitBlt(hMemDC, 0, 0, nWidht, nHeight, hSrcDC, 0, 0, SRCCOPY);

    BITMAP bitmap = { 0 };
    ::GetObject(hBitmap, sizeof(BITMAP), &bitmap);
    BITMAPINFOHEADER bi = { 0 };
    BITMAPFILEHEADER bf = { 0 };

    CONST int nBitCount = 24;
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bitmap.bmWidth;
    bi.biHeight = bitmap.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = nBitCount;
    bi.biCompression = BI_RGB;
    DWORD dwSize = ((bitmap.bmWidth * nBitCount + 31) / 32) * 4 * bitmap.bmHeight;

    HANDLE hDib = GlobalAlloc(GHND, dwSize + sizeof(BITMAPINFOHEADER));
    LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
    *lpbi = bi;

    ::GetDIBits(hMemDC, hBitmap, 0, bitmap.bmHeight, (BYTE*)lpbi + sizeof(BITMAPINFOHEADER), (BITMAPINFO*)lpbi, DIB_RGB_COLORS);

    try
    {
        CFile file;
        file.Open(lpszPath, CFile::modeCreate | CFile::modeWrite);
        bf.bfType = 0x4d42;
        dwSize += sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
        bf.bfSize = dwSize;
        bf.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

        file.Write((BYTE*)&bf, sizeof(BITMAPFILEHEADER));
        file.Write((BYTE*)lpbi, dwSize);
        file.Close();
    }
    catch (CFileException* e)
    {
        e->ReportError();
        e->Delete();
    }

    GlobalUnlock(hDib);
    GlobalFree(hDib);

    ::SelectObject(hMemDC, hOldBitmap);
    ::DeleteObject(hBitmap);
    ::DeleteDC(hMemDC);
    ::ReleaseDC(hWnd, hSrcDC);

    return TRUE;
}



void CTESTDlg::OnBnClickedBtGo()
{
    // TODO:  在此添加控件通知处理程序代码

    mfc::process t;
//     DWORD id = t.GetProcessidFromName(_T("YodaoDict.exe"));
    DWORD id = t.GetProcessidFromName(_T("QQ.exe"));
    if (0 == id)
    {
        return;
    }
    CWnd* wnd = FindWindow(NULL, _T("计算器"));
    
    //HWND hwnd = t.GetHwndByPid(id);
    HWND hwnd = wnd->GetSafeHwnd();
    if (NULL == wnd)
    {
        return;
    }
    CRect rect;
    ::GetWindowRect(wnd->GetSafeHwnd(), &rect);

    wnd->MessageBox(_T("xxx"));
    //HBITMAP hMap = mfc::screen::CopyScreenToBitmap(rect);
    HBITMAP hMap = mfc::screen::CopyExeScreenToBitmap(wnd, rect);
    
    if(NULL == hMap)
    {
        return;
    }
    BOOL ret =mfc::screen::SaveBitmapToFile(hMap, "E:\\局部截图.bmp");
    if (!ret)
    {
        return;
    }

}


void CTESTDlg::OnBnClickedBtGo2()
{
    // TODO:  在此添加控件通知处理程序代码

    CWnd* wnd = FindWindow(NULL, _T("计算器"));
    HWND hwnd = wnd->GetSafeHwnd();
    CRect rc;
    ::GetClientRect(hwnd, &rc);

    //创建 
    HDC hdcScreen = ::GetDC(NULL);
    HDC hdc = CreateCompatibleDC(hdcScreen);
    HBITMAP hbmp = CreateCompatibleBitmap(hdcScreen, rc.right - rc.left, rc.bottom - rc.top);
    SelectObject(hdc, hbmp);
    //wnd->BringWindowToTop();

    //复制 
    ::PrintWindow(hwnd, hdc, PW_CLIENTONLY);
    //PW_CLIENTONLY：Only the client area of the window is copied to hdcBlt. 
    //By default, the entire window is copied.   
    //PW_CLIENTONLY表示仅仅拷贝窗口的客户区域，而默认情况下，执行printwindow会拷贝整个窗口

    //复制到粘贴板 
    ::OpenClipboard(NULL);
    EmptyClipboard();
    SetClipboardData(CF_BITMAP, hbmp);
    CloseClipboard();
}


void CTESTDlg::OnBnClickedBtGo3()
{
    // TODO:  在此添加控件通知处理程序代码
    // 目标窗体不能最小化
    HWND src_hWnd = ::FindWindowW(NULL, _T("计算器"));
    RECT rc;
    ::GetWindowRect(src_hWnd, &rc);

    // 24位图的BITMAPINFO
    BITMAPINFO *pBITMAPINFO = (BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER));
    memset(pBITMAPINFO, 0, sizeof(BITMAPINFOHEADER));
    BITMAPINFOHEADER *pInfo_Header = (BITMAPINFOHEADER *)pBITMAPINFO;
    pInfo_Header->biSize = sizeof(BITMAPINFOHEADER);
    pInfo_Header->biWidth = rc.right - rc.left;
    pInfo_Header->biHeight = rc.bottom - rc.top;
    pInfo_Header->biPlanes = 1;
    pInfo_Header->biBitCount = 24;
    pInfo_Header->biCompression = BI_RGB;

    // 创建兼容DC 打印目标窗体
    HDC src_hdc = ::GetWindowDC(src_hWnd);
    HBITMAP hBitmap = ::CreateCompatibleBitmap(src_hdc,
        pInfo_Header->biWidth, pInfo_Header->biHeight);
    HDC hdcCompatible = CreateCompatibleDC(src_hdc);
    ::SelectObject(hdcCompatible, hBitmap);
    ::PrintWindow(src_hWnd, hdcCompatible, 0);
    CDC *pdcCompatible = CDC::FromHandle(hdcCompatible);
    ::ReleaseDC(src_hWnd, src_hdc);


    // 把兼容DC贴到DC上   
    CDC *pCurDC = this->GetDC();
    pCurDC->BitBlt(0, 0, pInfo_Header->biWidth, pInfo_Header->biHeight, pdcCompatible, 0, 0, SRCCOPY);

    // 获得数据buf
    DWORD bufSize = (pInfo_Header->biWidth * 3 + 3) / 4 * 4 * pInfo_Header->biHeight;
    BYTE * pBuffer = new BYTE[bufSize];
    if (GetDIBits(hdcCompatible, hBitmap, 0, pInfo_Header->biHeight, pBuffer,
        pBITMAPINFO, DIB_RGB_COLORS) == 0)
    {
        AfxMessageBox(_T("GetDIBits"));
    }

    // 建立文件
    CFile file;
    file.Open(_T("E:\\新图.bmp"), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);

    // 写文件头   
    BITMAPFILEHEADER File_Header;
    File_Header.bfType = 0x4d42;
    File_Header.bfOffBits = sizeof(BITMAPFILEHEADER) + pInfo_Header->biSize;
    File_Header.bfSize = File_Header.bfOffBits + bufSize;
    File_Header.bfReserved1 = 0;
    File_Header.bfReserved2 = 0;
    file.Write(&File_Header, sizeof(BITMAPFILEHEADER));

    // 写数据头
    file.Write(pInfo_Header, sizeof(BITMAPINFOHEADER));

    // 写数据
    file.Write(pBuffer, bufSize);

    // 关闭文件 释放内存
    file.Close();
    delete[]pBuffer;
    free(pBITMAPINFO);
    ::DeleteObject(hdcCompatible);
    ::DeleteObject(hBitmap);
    this->ReleaseDC(pCurDC);
}


void CTESTDlg::OnBnClickedBtGo4()
{
    // TODO:  在此添加控件通知处理程序代码

    // 调用方法
    HWND hWnd = ::FindWindow(NULL, _T("计算器"));
    if (hWnd)
    {
        SaveHwndToBmpFile(hWnd, _T("E://12.bmp"));
    }
}


void CTESTDlg::OnBnClickedBtGo5()
{
    // TODO:  在此添加控件通知处理程序代码
    enum{max = 1};
    HWND hwnd[max] = { NULL };
    HWND oldHwnd = NULL;
    for (int i = 0; i < max; ++i)
    {
        oldHwnd = ::FindWindowEx(NULL, oldHwnd, _T("ConsoleWindowClass"), NULL);
        if (NULL == oldHwnd)
        {
            break;
        }
        hwnd[i] = oldHwnd;
    }
    if (NULL == hwnd[0])
    {
        return;
    }
    WORD inputar[8] = { VK_NUMPAD8, VK_NUMPAD8, VK_NUMPAD8, VK_NUMPAD8, VK_NUMPAD8, VK_NUMPAD8, VK_NUMPAD8, VK_NUMPAD8 };
    for (int i = 0; i < max && NULL != hwnd[i]; ++i)
    {
        CRect rc;
        ::GetWindowRect(hwnd[i], &rc);
        ::BringWindowToTop(hwnd[i]);
        Sleep(100);
        ::SetForegroundWindow(hwnd[i]);
        ::SetFocus(hwnd[i]);
//         CPoint pt;
//         GetCursorPos(&pt);//获取鼠标在屏幕的当前位置
//         SetCursorPos(rc.left + 184, rc.top + 271);//移动到某点坐标
        SetCursorPos(rc.left + 534, rc.top + 257);//移动到某点坐标
        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, WM_LBUTTONDOWN, 0);//点下左键
        Sleep(100);
        SetCursorPos(rc.left + 400, rc.top + 257);//移动到某点坐标
        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, WM_LBUTTONUP, 0);//松开左键

        keybd_event(VK_NUMPAD8, MapVirtualKey(VK_NUMPAD8, 2), 0, GetMessageExtraInfo());
        keybd_event(VK_NUMPAD8, MapVirtualKey(VK_NUMPAD8, 2), KEYEVENTF_KEYUP, GetMessageExtraInfo());
        /*
        keybd_event(keyvalue, 0, 0, 0);
        keybd_event(keyvalue, 0, KEYEVENTF_KEYUP, 0);
        */
#if 0
        INPUT input[2];
        memset(input, 0, sizeof(input));
        //按下 向下方向键
        input[0].type = INPUT_KEYBOARD;  
        input[0].ki.wVk = VK_NUMPAD0;
        //松开 向下方向键
        input[1].type = INPUT_KEYBOARD;
        input[1].ki.wVk = VK_NUMPAD0;
        input[1].ki.dwFlags = KEYEVENTF_KEYUP;
        //该函数合成键盘事件和鼠标事件，用来模拟鼠标或者键盘操作。事件将被插入在鼠标或者键盘处理队列里面
        SendInput(2, input, sizeof(INPUT));
#endif
    }
//     mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);//点下左键
//     mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);//松开左键
}
bool srun = true;
void while_run()
{
    while (srun)
    {

    }
}

void CTESTDlg::OnBnClickedBtGo6()
{
    // TODO:  在此添加控件通知处理程序代码
    static std::shared_ptr<std::thread> thread;
    
    //th(std::bind(while_run));

    if (thread)
    {
        srun = false;
        thread->join();
        thread.reset();
    }
    else
    {
        thread = std::make_shared<std::thread>(while_run);
    }
}
