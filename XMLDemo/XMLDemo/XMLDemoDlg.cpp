
// XMLDemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "XMLDemo.h"
#include "XMLDemoDlg.h"
#include "afxdialogex.h"
#include "LoadXML.h"

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


// CXMLDemoDlg �Ի���



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


// CXMLDemoDlg ��Ϣ�������

BOOL CXMLDemoDlg::OnInitDialog()
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
	//((CStatic*)GetDlgItem(IDC_STATIC_FILEPATH))->SetWindowText(_T("E:\\GitCode\\GitProject\\XMLDemo\\XMLDemo\\XMLTemplate\\Response.xml"));
	//m_szKeyName = _T("EBD/EBDID");
	((CStatic*)GetDlgItem(IDC_STATIC_FILEPATH))->SetWindowText(_T("E:\\code\\GitHub\\trunk\\XMLDemo\\XMLDemo\\xxx.xml"));
	m_szKeyName = _T("aes");
    UpdateData(FALSE);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CXMLDemoDlg::OnPaint()
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
HCURSOR CXMLDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CXMLDemoDlg::OnBnClickedBtSelectfile()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CFileDialog dlg(TRUE);
    if (IDOK == dlg.DoModal ())
    {
        SetDlgItemText(IDC_STATIC_FILEPATH, dlg.GetPathName());
    }
}


void CXMLDemoDlg::OnBnClickedBtGet()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
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
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
