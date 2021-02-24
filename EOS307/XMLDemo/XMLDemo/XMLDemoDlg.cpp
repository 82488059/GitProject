
// XMLDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "XMLDemo.h"
#include "XMLDemoDlg.h"
#include "afxdialogex.h"
#include "LoadXML.h"

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


// CXMLDemoDlg 对话框



CXMLDemoDlg::CXMLDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CXMLDemoDlg::IDD, pParent)
    , m_szKeyName(_T(""))
    , m_szKeyValue(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CXMLDemoDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_NAME, m_szKeyName);
    DDX_Text(pDX, IDC_VALUE, m_szKeyValue);
}

BEGIN_MESSAGE_MAP(CXMLDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BT_SELECTFILE, &CXMLDemoDlg::OnBnClickedBtSelectfile)
    ON_BN_CLICKED(IDC_BT_GET, &CXMLDemoDlg::OnBnClickedBtGet)
    ON_BN_CLICKED(IDC_BT_SET, &CXMLDemoDlg::OnBnClickedBtSet)
	ON_BN_CLICKED(IDC_BT_ADDNODE, &CXMLDemoDlg::OnBnClickedBtAddnode)
	ON_BN_CLICKED(IDC_BT_COPYNODE, &CXMLDemoDlg::OnBnClickedBtCopynode)
END_MESSAGE_MAP()


// CXMLDemoDlg 消息处理程序

BOOL CXMLDemoDlg::OnInitDialog()
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
	//((CStatic*)GetDlgItem(IDC_STATIC_FILEPATH))->SetWindowText(_T("E:\\GitCode\\GitProject\\XMLDemo\\XMLDemo\\XMLTemplate\\Response.xml"));
	//m_szKeyName = _T("EBD/EBDID");
	((CStatic*)GetDlgItem(IDC_STATIC_FILEPATH))->SetWindowText(_T("E:\\code\\GitHub\\trunk\\XMLDemo\\XMLDemo\\xxx.xml"));
	m_szKeyName = _T("aes");
    UpdateData(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CXMLDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CXMLDemoDlg::OnPaint()
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
HCURSOR CXMLDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CXMLDemoDlg::OnBnClickedBtSelectfile()
{
    // TODO:  在此添加控件通知处理程序代码
    CFileDialog dlg(TRUE);
    if (IDOK == dlg.DoModal ())
    {
        SetDlgItemText(IDC_STATIC_FILEPATH, dlg.GetPathName());
    }
}


void CXMLDemoDlg::OnBnClickedBtGet()
{
    // TODO:  在此添加控件通知处理程序代码
    CString path;
    GetDlgItemText(IDC_STATIC_FILEPATH, path);
    if (path.IsEmpty ())
    {
        return;
    }
    UpdateData(TRUE);

    CLoadXML xml(path);

    if (xml.init())
    {
        CComBSTR str;
//         if (xml.get_attr_string(CComBSTR(m_szKeyName), CComBSTR(_T("text")), str))
//         {
//             m_szKeyValue = str;
//             UpdateData(FALSE);
//         } 
        if (xml.get_node_text(CComBSTR(m_szKeyName), str))
        {
            m_szKeyValue = str;
            UpdateData(FALSE);
        }
        xml.release();
    }
}


void CXMLDemoDlg::OnBnClickedBtSet()
{
    // TODO:  在此添加控件通知处理程序代码
    CString path;
    GetDlgItemText(IDC_STATIC_FILEPATH, path);
    if (path.IsEmpty())
    {
        return;
    }
    UpdateData(TRUE);

    CLoadXML xml(path);

    if (xml.init())
    {
        CComBSTR str = m_szKeyValue;
        //         if (xml.get_attr_string(CComBSTR(m_szKeyName), CComBSTR(_T("text")), str))
        //         {
        //             m_szKeyValue = str;
        //             UpdateData(FALSE);
        //         } 
        if (xml.set_node_text(CComBSTR(m_szKeyName), str))
        {
            m_szKeyValue = str;
            UpdateData(FALSE);
        }
        xml.release();
    }
}


void CXMLDemoDlg::OnBnClickedBtAddnode()
{
	// TODO:  在此添加控件通知处理程序代码
	CString path;
	GetDlgItemText(IDC_STATIC_FILEPATH, path);
	if (path.IsEmpty())
	{
		return;
	}
	UpdateData(TRUE);

	CLoadXML xml(path);

	if (xml.init())
	{
		CComBSTR str = m_szKeyValue;
		//         if (xml.get_attr_string(CComBSTR(m_szKeyName), CComBSTR(_T("text")), str))
		//         {
		//             m_szKeyValue = str;
		//             UpdateData(FALSE);
		//         } 
		CComBSTR text("<new>ll<new>");
		if (xml.add_node_text(CComBSTR(m_szKeyName), CComBSTR("XXXXXX"), text))
		{
			m_szKeyValue = str;
			UpdateData(FALSE);
		}
		xml.release();
	}
}


void CXMLDemoDlg::OnBnClickedBtCopynode()
{
	// TODO:  在此添加控件通知处理程序代码
	CString path;
	GetDlgItemText(IDC_STATIC_FILEPATH, path);
	if (path.IsEmpty())
	{
		return;
	}
	UpdateData(TRUE);

	CLoadXML xml(path);

	if (xml.init())
	{
		CComBSTR str = m_szKeyValue;
		//         if (xml.get_attr_string(CComBSTR(m_szKeyName), CComBSTR(_T("text")), str))
		//         {
		//             m_szKeyValue = str;
		//             UpdateData(FALSE);
		//         } 
		IXMLDOMNodePtr node;
		if (!xml.get_node(CComBSTR("aes"), node))
		{
			TRACE("get_node ERRROR");
			return;
		}
		//CComBSTR text("<new>ll<new>");
		if (!xml.add_node(CComBSTR(m_szKeyName), node))
		{
			TRACE("add error");
			return;
		}
		m_szKeyValue = str;
		UpdateData(FALSE);
		TRACE("OK");
		xml.release();
	}
}
