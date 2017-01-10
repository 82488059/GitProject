
// TestRegisterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestRegister.h"
#include "TestRegisterDlg.h"
#include "afxdialogex.h"
// #include <winioctl.h>
#include "GetHDSerial.h"

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


// CTestRegisterDlg 对话框



CTestRegisterDlg::CTestRegisterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestRegisterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestRegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestRegisterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDOK, &CTestRegisterDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CTestRegisterDlg 消息处理程序

BOOL CTestRegisterDlg::OnInitDialog()
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
    char *p = NULL;
    *p = 1;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTestRegisterDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestRegisterDlg::OnPaint()
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
HCURSOR CTestRegisterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
#ifndef _DEBUG

// filename: 设备的“文件名 ”
HANDLE OpenDevice(LPCTSTR filename)
{
    HANDLE hDevice;
    // 打开设 备
    hDevice = ::CreateFile(filename,   // 文件名
        GENERIC_READ | GENERIC_WRITE,   // 读写方式
        FILE_SHARE_READ | FILE_SHARE_WRITE, // 共享方式
        NULL,     // 默 认的安全描述符
        OPEN_EXISTING,    // 创建方式   
        0,     // 不需设置文件属性
        NULL);     // 不需参照模板文件
    return hDevice;
}
// 向驱动发“IDENTIFY DEVICE”命令，获得设备信息
// hDevice: 设备句柄
// pIdInfo:  设备信息结构指 针
BOOL IdentifyDevice(HANDLE hDevice, PIDINFO pIdInfo)
{
    PSENDCMDINPARAMS pSCIP;  // 输入数据结构指针
    PSENDCMDOUTPARAMS pSCOP; // 输出数据结构指针
    DWORD dwOutBytes;   // IOCTL输出数据长度
    BOOL bResult;    // IOCTL返回值
    // 申请输入/输 出数据结构空间
    pSCIP = (PSENDCMDINPARAMS)::GlobalAlloc(LMEM_ZEROINIT,
        sizeof(SENDCMDINPARAMS) - 1);
    pSCOP = (PSENDCMDOUTPARAMS)::GlobalAlloc(LMEM_ZEROINIT,
        sizeof(SENDCMDOUTPARAMS) + sizeof(IDINFO) - 1);
    // 指定ATA/ATAPI命令 的寄存器值
    // pSCIP->irDriveRegs.bFeaturesReg = 0;
    // pSCIP->irDriveRegs.bSectorCountReg = 0;
    // pSCIP->irDriveRegs.bSectorNumberReg = 0;
    // pSCIP->irDriveRegs.bCylLowReg = 0;
    // pSCIP->irDriveRegs.bCylHighReg = 0;
    // pSCIP->irDriveRegs.bDriveHeadReg = 0;
    pSCIP->irDriveRegs.bCommandReg = IDE_ATA_IDENTIFY;
    // 指定输入/输出数据缓冲区大小
    pSCIP->cBufferSize = 0;
    pSCOP->cBufferSize = sizeof(IDINFO);
    // IDENTIFY DEVICE
    bResult = ::DeviceIoControl(hDevice,  // 设备句柄
        DFP_RECEIVE_DRIVE_DATA,   // 指定IOCTL
        pSCIP, sizeof(SENDCMDINPARAMS) - 1, // 输入数据缓冲区
        pSCOP, sizeof(SENDCMDOUTPARAMS) + sizeof(IDINFO) - 1, // 输出数据缓冲区
        &dwOutBytes,    // 输 出数据长度
        (LPOVERLAPPED)NULL);   // 用同步I/O
    // 复制设备参数结构
    ::memcpy(pIdInfo, pSCOP->bBuffer, sizeof(IDINFO));  // 释放输入/输出数据空间
    ::GlobalFree(pSCOP);
    ::GlobalFree(pSCIP);
    return bResult;
}
// 向SCSI MINI-PORT 驱动发“IDENTIFY DEVICE”命令，获得设备信息
// hDevice: 设备句柄 
// pIdInfo:  设备信息结构指针
BOOL IdentifyDeviceAsScsi(HANDLE hDevice, int nDrive, PIDINFO pIdInfo)
{
    PSENDCMDINPARAMS pSCIP;  // 输入数据结构指针
    PSENDCMDOUTPARAMS pSCOP; // 输出数据结构指针
    PSRB_IO_CONTROL pSRBIO;  // SCSI输入输 出数据结构指针
    DWORD dwOutBytes;   // IOCTL输出数据长度
    BOOL bResult;    // IOCTL 返回值
    // 申请输入/输出数据结构空间
    pSRBIO = (PSRB_IO_CONTROL)::GlobalAlloc(LMEM_ZEROINIT,
        sizeof(SRB_IO_CONTROL) + sizeof(SENDCMDOUTPARAMS) + sizeof(IDINFO) - 1);
    pSCIP = (PSENDCMDINPARAMS)((char *)pSRBIO + sizeof(SRB_IO_CONTROL));
    pSCOP = (PSENDCMDOUTPARAMS)((char *)pSRBIO + sizeof(SRB_IO_CONTROL));
    // 填充输入/输出数据
    pSRBIO->HeaderLength = sizeof(SRB_IO_CONTROL);
    pSRBIO->Timeout = 10000;
    pSRBIO->Length = sizeof(SENDCMDOUTPARAMS) + sizeof(IDINFO) - 1;
    pSRBIO -> ControlCode = IOCTL_SCSI_MINIPORT_IDENTIFY;
    ::strncpy((char *)pSRBIO -> Signature, SCSIDISK, 8);
    // 指定ATA/ATAPI命令的寄存器值
    // pSCIP->irDriveRegs.bFeaturesReg = 0;
    // pSCIP->irDriveRegs.bSectorCountReg = 0;
    // pSCIP->irDriveRegs.bSectorNumberReg = 0;
    // pSCIP->irDriveRegs.bCylLowReg = 0;
    // pSCIP->irDriveRegs.bCylHighReg = 0;
    // pSCIP->irDriveRegs.bDriveHeadReg = 0;
    pSCIP->irDriveRegs.bCommandReg = IDE_ATA_IDENTIFY;
    pSCIP->bDriveNumber = nDrive;
    // IDENTIFY DEVICE
    bResult = ::DeviceIoControl(hDevice,  // 设备句柄
        IOCTL_SCSI_MINIPORT,   // 指定 IOCTL
        pSRBIO, sizeof(SRB_IO_CONTROL) + sizeof(SENDCMDINPARAMS) - 1, // 输入数据缓冲区   
        pSRBIO, sizeof(SRB_IO_CONTROL) +sizeof(SENDCMDOUTPARAMS) + sizeof(IDINFO) - 1, // 输出数据缓冲区
        &dwOutBytes,  // 输出数据长度
        (LPOVERLAPPED)NULL); // 用 同步I/O
    // 复制设备参数结构
    ::memcpy(pIdInfo, pSCOP->bBuffer, sizeof(IDINFO));
    // 释放输入/输出数据空间
    ::GlobalFree(pSRBIO);
    return bResult;
}
// 将串中的字符两两颠倒
// 原因是ATA/ATAPI中的WORD，与Windows采用的字节顺序相 反
// 驱动程序中已经将收到的数据全部反过来，我们来个负负得正
void AdjustString(char* str, int len)
{
    char ch;
    int i;
    // 两两颠倒
    for (i = 0; i < len; i += 2)
    {
        ch = str[i];
        str[i] = str[i + 1];
        str[i + 1] = ch;
    }
    // 若是右对齐的，调整为左对齐 (去掉左边的空格)
    i = 0;
    while (i < len && str[i] == ' ') i++;
    ::memmove(str, &str[i], len - i);
    // 去掉右边的空格
    i = len - 1;
    while (i >= 0 && str[i] == ' ')
    {
        str[i] = '\\0';
        i--;
    }
}
// 读取IDE硬盘的设备信息，必须有 足够权限
// nDrive: 驱动器号(0=第一个硬盘，1=0=第二个硬盘，......)
// pIdInfo: 设 备信息结构指针
BOOL GetPhysicalDriveInfoInNT(int nDrive, PIDINFO pIdInfo)
{
    HANDLE hDevice;   // 设备句柄
    BOOL bResult;   // 返回结果
    char szFileName[20]; // 文件名
    ::sprintf(szFileName, "\\\\\\\\.\\\\PhysicalDrive%d", nDrive);
    hDevice = ::OpenDevice(szFileName);
    if (hDevice == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }
    // IDENTIFY DEVICE
    bResult = ::IdentifyDevice(hDevice, pIdInfo);
    if (bResult)
    {
        // 调整字符串
        ::AdjustString(pIdInfo->sSerialNumber, 20);
        ::AdjustString(pIdInfo->sModelNumber, 40);
        ::AdjustString(pIdInfo -> sFirmwareRev, 8);
    }
    ::CloseHandle(hDevice);
    return bResult;
}
// 用SCSI驱动读取IDE硬盘的设备信息，不受权限制约
// nDrive: 驱动器号(0=Primary Master, 1=Promary Slave, 2=Secondary master, 3=Secondary slave)
// pIdInfo: 设备信息结 构指针
BOOL GetIdeDriveAsScsiInfoInNT(int nDrive, PIDINFO pIdInfo)
{
    HANDLE hDevice;   // 设备句柄
    BOOL bResult;   // 返回结果
    char szFileName[20]; // 文件名 
    ::sprintf(szFileName, "\\\\\\\\.\\\\Scsi%d:", nDrive / 2); hDevice = ::OpenDevice(szFileName);
    if (hDevice == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }
    // IDENTIFY DEVICE
    bResult = ::IdentifyDeviceAsScsi(hDevice, nDrive % 2, pIdInfo);
    // 检查是不 是空串
    if (pIdInfo->sModelNumber[0] == '\\0')
    {
        bResult = FALSE;
    }
    if (bResult)
    {
        // 调整字符串
        ::AdjustString(pIdInfo -> sSerialNumber, 20);
        ::AdjustString(pIdInfo->sModelNumber, 40);
        ::AdjustString(pIdInfo->sFirmwareRev, 8);
    }
    return bResult;
}
#endif // _DEBUG
void CTestRegisterDlg::OnBnClickedOk()
{
    // TODO:  在此添加控件通知处理程序代码

    HANDLE hPhysical = ::CreateFileA("\\\\.\\PHYSICALDRIVE0",
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
        OPEN_EXISTING, 0, NULL);
//    ::DeviceIoControl();
    CGetHDSerial get;
    CloseHandle(hPhysical);
    TRACE(get.GetHDSerial());
    INT N = sizeof(UINT);
    TRACE("%d", N);

    //CDialogEx::OnOK();
}
