
// HttpClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HttpClient.h"
#include "HttpClientDlg.h"
#include "afxdialogex.h"

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


// CHttpClientDlg 对话框



CHttpClientDlg::CHttpClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHttpClientDlg::IDD, pParent)
    , m_strUrl(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHttpClientDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT1, m_strUrl);
}

BEGIN_MESSAGE_MAP(CHttpClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDOK, &CHttpClientDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDC_BUTTON1, &CHttpClientDlg::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2, &CHttpClientDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CHttpClientDlg 消息处理程序

BOOL CHttpClientDlg::OnInitDialog()
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
    //m_strUrl = _T("http://192.168.1.180:8089");
    m_strUrl = _T("http://127.0.0.1:8089");
    //m_strUrl = _T("http://192.168.12.70:8080/HttpCGISrv.exe");
    //m_strUrl = _T("http://192.168.1.189:8080/Servlet3.0.demo/upload");
    //m_strUrl = _T("http://192.168.1.197:8080/HttpCGISrv.exe";)
    //m_strUrl = _T("http://192.168.12.221:8080/EmergencyBroadcast/upload");
    UpdateData(FALSE);
	// TODO:  在此添加额外的初始化代码
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CHttpClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CHttpClientDlg::OnPaint()
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
HCURSOR CHttpClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CHttpClientDlg::OnBnClickedOk()
{
    // TODO:  在此添加控件通知处理程序代码

    CDialogEx::OnOK();
}


void CHttpClientDlg::OnBnClickedButton1()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);

//	HWND hTemp = ::FindWindow(NULL, "HelloWorld");
    std::string s;
    CFile cfile;
#define  FILE_NAME_STR "EBDB_100000000002.tar"
//#define  FILE_NAME_STR "EBDB100000000132.tar"
    if (!cfile.Open("XML\\"FILE_NAME_STR, CFile::modeRead | CFile::typeBinary))
    {
        return;
    }

    int size = cfile.GetLength();
    char *pBuf = new char[size];

    char head[] = { "--53758868654a4bcd91675cf3ee92a801\r\nContent-Disposition: form-data; name=\"file\"; filename=\""FILE_NAME_STR"\"\r\n\
Content-Type: application/x-tar; charset=utf-8\r\n\r\n" };
    
    char end[] = { "\r\n--53758868654a4bcd91675cf3ee92a801--\r\n" };
    cfile.Read(pBuf, size);
    int len = strlen(head) + strlen(end) + size;
    char *pPostBuffer = new char[len];
    memcpy(pPostBuffer, head, strlen(head));
    memcpy(pPostBuffer + strlen(head), pBuf, size);
    memcpy(pPostBuffer + strlen(head) + size, end, strlen(end));

    //if (SUCCESS == m_http.HttpPost(m_strUrl, pBuf, size, s))
    if (SUCCESS == m_http.HttpPost(m_strUrl, pPostBuffer, len, s))
    {
        //TRACE(s.c_str ());
    }
    delete pBuf;
    pBuf = NULL;
    delete pPostBuffer;
    pPostBuffer = NULL;
}
#include "BigHttpClient.h"

void CHttpClientDlg::OnBnClickedButton2()
{
    // TODO:  在此添加控件通知处理程序代码
    CBigHttpClient http;
    //CString m_strUrl = "http://127.0.0.1:8090";
    CHttpClient m_http;
    std::string s;
    CFile cfile;
    CString szFileName = "EBDB_100000000002.tar";

    if (!cfile.Open("XML\\"+szFileName, CFile::modeRead | CFile::typeBinary))
    {
        return ;
    }

    int size = cfile.GetLength();
    char *pBuf = new char[size];

    CString szHead;
    char head1[] = { "--53758868654a4bcd91675cf3ee92a801\r\nContent-Disposition: form-data; name=\"file\"; filename=\"" };
    char head2[] = { "\"\r\n\r\n" };
    szHead = head1 + szFileName + head2;
    char end[] = { "\r\n--53758868654a4bcd91675cf3ee92a801--\r\n" };

    cfile.Read(pBuf, size);
    cfile.Close();
    int len = szHead.GetLength() + strlen(end) + size;
    char *pPostBuffer = new char[len];
    memcpy(pPostBuffer, szHead, szHead.GetLength());
    memcpy(pPostBuffer + szHead.GetLength(), pBuf, size);
    memcpy(pPostBuffer + szHead.GetLength() + size, end, strlen(end));

    //if (SUCCESS == m_http.HttpPost(m_strUrl, pBuf, size, s))
    if (SUCCESS == http.HttpPostFile(m_strUrl, "XML\\" + szFileName, szHead, end, s))
    {
        //TRACE(s.c_str ());
    }
    delete pBuf;
    pBuf = NULL;
    delete pPostBuffer;
    pPostBuffer = NULL;
    return ;
}
