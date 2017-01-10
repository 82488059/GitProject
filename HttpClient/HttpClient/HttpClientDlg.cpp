
// HttpClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HttpClient.h"
#include "HttpClientDlg.h"
#include "afxdialogex.h"

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


// CHttpClientDlg �Ի���



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


// CHttpClientDlg ��Ϣ�������

BOOL CHttpClientDlg::OnInitDialog()
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
    //m_strUrl = _T("http://192.168.1.180:8089");
    m_strUrl = _T("http://127.0.0.1:8089");
    //m_strUrl = _T("http://192.168.12.70:8080/HttpCGISrv.exe");
    //m_strUrl = _T("http://192.168.1.189:8080/Servlet3.0.demo/upload");
    //m_strUrl = _T("http://192.168.1.197:8080/HttpCGISrv.exe";)
    //m_strUrl = _T("http://192.168.12.221:8080/EmergencyBroadcast/upload");
    UpdateData(FALSE);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CHttpClientDlg::OnPaint()
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
HCURSOR CHttpClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CHttpClientDlg::OnBnClickedOk()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������

    CDialogEx::OnOK();
}


void CHttpClientDlg::OnBnClickedButton1()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
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
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
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
