// HttpServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HttpServer.h"
#include "HttpServerDlg.h"

#include "Serverthread.h"
#include "BlockSock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
extern CMyBlockSocket g_sListen;

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHttpServerDlg dialog

CHttpServerDlg::CHttpServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHttpServerDlg::IDD, pParent)
    , m_port(0)
{
	//{{AFX_DATA_INIT(CHttpServerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHttpServerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CHttpServerDlg)
    DDX_Control(pDX, IDC_STATIC_PATH, m_StaPath);
    DDX_Control(pDX, IDC_STATIC_STATUS, m_StaStatus);
    DDX_Control(pDX, IDC_BTN_STOP, m_BtnStop);
    DDX_Control(pDX, IDOK, m_BtnStart);
    DDX_Control(pDX, IDC_LIST_INFO, m_ListInfo);
    //}}AFX_DATA_MAP
    DDX_Control(pDX, IDC_IPADDRESS, m_ctrlServerIp);
    DDX_Text(pDX, IDC_EDIT_PORT, m_port);
}

BEGIN_MESSAGE_MAP(CHttpServerDlg, CDialog)
	//{{AFX_MSG_MAP(CHttpServerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_STOP, OnBtnStop)
	ON_BN_CLICKED(IDOK, OnStartServer)
	ON_BN_CLICKED(IDC_BTN_SETPATH, OnBtnSetpath)
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BT_TAR, &CHttpServerDlg::OnBnClickedBtTar)
    ON_BN_CLICKED(IDC_BT_XMLTEST, &CHttpServerDlg::OnBnClickedBtXmltest)
    ON_BN_CLICKED(IDC_BT_DB, &CHttpServerDlg::OnBnClickedBtDb)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHttpServerDlg message handlers

BOOL CHttpServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_BtnStart.EnableWindow(TRUE);
	m_BtnStop.EnableWindow(FALSE);
	m_StaStatus.SetWindowText("停止服务");
	m_StaPath.SetWindowText("服务器路径："+g_strDirect);
	
    m_ctrlServerIp.SetWindowText(_T("127.0.0.1"));
    m_ctrlServerIp.SetWindowText(_T("192.168.1.180"));
    m_port = 8089;
    UpdateData(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CHttpServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHttpServerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHttpServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CHttpServerDlg::OnBtnStop() 
{
	try 
	{
		if(g_bListening) 
		{
			g_sListen.Close();
			Sleep(300);
		}
	}
	catch(CMyBlockSocketException* e) {
		LogBlockingSocketException(&m_ListInfo, "主程序：", e);
		e->Delete();
	}

	g_bListening=FALSE;
	g_nConnection=0;
	m_StaStatus.SetWindowText("停止服务");
	m_ListInfo.AddString("HTTP服务器关闭！");
	m_BtnStart.EnableWindow(TRUE);
	m_BtnStop.EnableWindow(FALSE);
}

void CHttpServerDlg::OnStartServer() 
{
    UpdateData(TRUE);
    g_nPortServer = m_port;
    CString szip;
    m_ctrlServerIp.GetWindowText(szip);
    g_strIPServer = szip;

	try 
	{
		CSocketAddress saServer;
//		if(g_strIPServer.IsEmpty()) 
		{
			saServer = CSocketAddress(INADDR_ANY, (USHORT) g_nPortServer);
		}
// 		else 
// 		{
// 			saServer = CSocketAddress(g_strIPServer, (USHORT) g_nPortServer);
// 		}
		g_sListen.Create();
		//绑定地址
		g_sListen.Bind(saServer);
		//开始侦听
		g_sListen.Listen();
		g_bListening = TRUE;
		g_nConnection = 0;
		//创建服务器工作线程
		AfxBeginThread(ServerThreadProc, &m_ListInfo, THREAD_PRIORITY_NORMAL);
		m_ListInfo.AddString("HTTP服务器开始工作！");
		m_StaStatus.SetWindowText("侦听连接");
	}
	catch(CMyBlockSocketException* e) 
	{
		g_sListen.Cleanup();
		LogBlockingSocketException(&m_ListInfo, "主程序：", e);
		e->Delete();
	}
	m_BtnStart.EnableWindow(FALSE);
	m_BtnStop.EnableWindow(TRUE);
}

void CHttpServerDlg::OnBtnSetpath() 
{
	CFileDialog dlg(TRUE,"htm","Default.htm");

	if (dlg.DoModal()==IDOK)
	{
		g_strDirect=dlg.GetPathName();
		g_strDirect.Replace(dlg.GetFileName(),"");
		m_StaPath.SetWindowText("服务器路径："+g_strDirect);
	}
}

//#include <Python.h>
void CHttpServerDlg::OnBnClickedBtTar()
{
    // TODO:  在此添加控件通知处理程序代码
//     Py_Initialize();
//     if (!Py_IsInitialized())
//     {
//         return;
//     }
//     PyObject * pModule = NULL;
//     PyObject * pFunc = NULL;
//     PyObject* pName = NULL;

//     int r = PyRun_SimpleString("import sys");
//     r = PyRun_SimpleString("sys.path.append('./recv/')");
// 
//     PyObject *pName = PyString_FromString("MyTar");
//     PyObject * pModule = PyImport_Import(pName);
//     if (!pModule)
//     {
//         return;
//     }
//     PyObject* pDict = PyModule_GetDict(pModule);
//     if (!pDict)
//     {
//         return;
//     }
//     PyObject* pFunc = PyDict_GetItemString(pDict, "tar_one");
//     if (!pFunc || !PyCallable_Check(pFunc))
//     {
//         return;
//     }
//     PyObject* pArgs = PyTuple_New(1);
//     PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", "XML\\EBDB_100000000001.xml"));
// 
//     PyObject* pReturn = NULL;
// 
//     pReturn = PyEval_CallObject(pFunc, pArgs);
// 
//     int result;
//     PyArg_Parse(pReturn, "i", &result);
// 
//     Py_Finalize();
}
#include "Analyse.h"
#include "LoadXML.h"
void CHttpServerDlg::OnBnClickedBtXmltest()
{
    // TODO:  在此添加控件通知处理程序代码

//     CAnalyse an("");
//     CLoadXML xml1;
//     an.CreateEBMStateResponseXml("EBMStateResponse.Xml", xml1);
}

#include "DBBasicInfo.h"

void CHttpServerDlg::OnBnClickedBtDb()
{
    // TODO:  在此添加控件通知处理程序代码

    CLoadXML xml("config/db.xml");


    if (!xml.init())
    {
        return;
    }
    CString DATABASE;
    if (!xml.get_node_text("CONFIG/DATABASE", DATABASE))
    {
        return;
    }
    CString UID;
    if (!xml.get_node_text("CONFIG/UID", UID))
    {
        return;
    }
    CString PWD;
    if (!xml.get_node_text("CONFIG/PWD", PWD))
    {
        return;
    }
    CString SERVER;
    if (!xml.get_node_text("CONFIG/SERVER", SERVER))
    {
        return;
    }
    CDBBasicInfo db(SERVER, DATABASE, UID, PWD);

    if (!db.OnInitAdoConnection ())
    {
        return;
    }
    if (db.ExecuteSQL("select * from BASIC_INFO"))
    {
        if (!db->adoEOF)
        {
            _variant_t var;
            var = db->GetCollect("EBDID");
            CString ID = var;
        }
    }


}
