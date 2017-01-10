
// TestRegisterDlg.cpp : ʵ���ļ�
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


// CTestRegisterDlg �Ի���



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


// CTestRegisterDlg ��Ϣ�������

BOOL CTestRegisterDlg::OnInitDialog()
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
    char *p = NULL;
    *p = 1;
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTestRegisterDlg::OnPaint()
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
HCURSOR CTestRegisterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
#ifndef _DEBUG

// filename: �豸�ġ��ļ��� ��
HANDLE OpenDevice(LPCTSTR filename)
{
    HANDLE hDevice;
    // ���� ��
    hDevice = ::CreateFile(filename,   // �ļ���
        GENERIC_READ | GENERIC_WRITE,   // ��д��ʽ
        FILE_SHARE_READ | FILE_SHARE_WRITE, // ����ʽ
        NULL,     // Ĭ �ϵİ�ȫ������
        OPEN_EXISTING,    // ������ʽ   
        0,     // ���������ļ�����
        NULL);     // �������ģ���ļ�
    return hDevice;
}
// ����������IDENTIFY DEVICE���������豸��Ϣ
// hDevice: �豸���
// pIdInfo:  �豸��Ϣ�ṹָ ��
BOOL IdentifyDevice(HANDLE hDevice, PIDINFO pIdInfo)
{
    PSENDCMDINPARAMS pSCIP;  // �������ݽṹָ��
    PSENDCMDOUTPARAMS pSCOP; // ������ݽṹָ��
    DWORD dwOutBytes;   // IOCTL������ݳ���
    BOOL bResult;    // IOCTL����ֵ
    // ��������/�� �����ݽṹ�ռ�
    pSCIP = (PSENDCMDINPARAMS)::GlobalAlloc(LMEM_ZEROINIT,
        sizeof(SENDCMDINPARAMS) - 1);
    pSCOP = (PSENDCMDOUTPARAMS)::GlobalAlloc(LMEM_ZEROINIT,
        sizeof(SENDCMDOUTPARAMS) + sizeof(IDINFO) - 1);
    // ָ��ATA/ATAPI���� �ļĴ���ֵ
    // pSCIP->irDriveRegs.bFeaturesReg = 0;
    // pSCIP->irDriveRegs.bSectorCountReg = 0;
    // pSCIP->irDriveRegs.bSectorNumberReg = 0;
    // pSCIP->irDriveRegs.bCylLowReg = 0;
    // pSCIP->irDriveRegs.bCylHighReg = 0;
    // pSCIP->irDriveRegs.bDriveHeadReg = 0;
    pSCIP->irDriveRegs.bCommandReg = IDE_ATA_IDENTIFY;
    // ָ������/������ݻ�������С
    pSCIP->cBufferSize = 0;
    pSCOP->cBufferSize = sizeof(IDINFO);
    // IDENTIFY DEVICE
    bResult = ::DeviceIoControl(hDevice,  // �豸���
        DFP_RECEIVE_DRIVE_DATA,   // ָ��IOCTL
        pSCIP, sizeof(SENDCMDINPARAMS) - 1, // �������ݻ�����
        pSCOP, sizeof(SENDCMDOUTPARAMS) + sizeof(IDINFO) - 1, // ������ݻ�����
        &dwOutBytes,    // �� �����ݳ���
        (LPOVERLAPPED)NULL);   // ��ͬ��I/O
    // �����豸�����ṹ
    ::memcpy(pIdInfo, pSCOP->bBuffer, sizeof(IDINFO));  // �ͷ�����/������ݿռ�
    ::GlobalFree(pSCOP);
    ::GlobalFree(pSCIP);
    return bResult;
}
// ��SCSI MINI-PORT ��������IDENTIFY DEVICE���������豸��Ϣ
// hDevice: �豸��� 
// pIdInfo:  �豸��Ϣ�ṹָ��
BOOL IdentifyDeviceAsScsi(HANDLE hDevice, int nDrive, PIDINFO pIdInfo)
{
    PSENDCMDINPARAMS pSCIP;  // �������ݽṹָ��
    PSENDCMDOUTPARAMS pSCOP; // ������ݽṹָ��
    PSRB_IO_CONTROL pSRBIO;  // SCSI������ �����ݽṹָ��
    DWORD dwOutBytes;   // IOCTL������ݳ���
    BOOL bResult;    // IOCTL ����ֵ
    // ��������/������ݽṹ�ռ�
    pSRBIO = (PSRB_IO_CONTROL)::GlobalAlloc(LMEM_ZEROINIT,
        sizeof(SRB_IO_CONTROL) + sizeof(SENDCMDOUTPARAMS) + sizeof(IDINFO) - 1);
    pSCIP = (PSENDCMDINPARAMS)((char *)pSRBIO + sizeof(SRB_IO_CONTROL));
    pSCOP = (PSENDCMDOUTPARAMS)((char *)pSRBIO + sizeof(SRB_IO_CONTROL));
    // �������/�������
    pSRBIO->HeaderLength = sizeof(SRB_IO_CONTROL);
    pSRBIO->Timeout = 10000;
    pSRBIO->Length = sizeof(SENDCMDOUTPARAMS) + sizeof(IDINFO) - 1;
    pSRBIO -> ControlCode = IOCTL_SCSI_MINIPORT_IDENTIFY;
    ::strncpy((char *)pSRBIO -> Signature, SCSIDISK, 8);
    // ָ��ATA/ATAPI����ļĴ���ֵ
    // pSCIP->irDriveRegs.bFeaturesReg = 0;
    // pSCIP->irDriveRegs.bSectorCountReg = 0;
    // pSCIP->irDriveRegs.bSectorNumberReg = 0;
    // pSCIP->irDriveRegs.bCylLowReg = 0;
    // pSCIP->irDriveRegs.bCylHighReg = 0;
    // pSCIP->irDriveRegs.bDriveHeadReg = 0;
    pSCIP->irDriveRegs.bCommandReg = IDE_ATA_IDENTIFY;
    pSCIP->bDriveNumber = nDrive;
    // IDENTIFY DEVICE
    bResult = ::DeviceIoControl(hDevice,  // �豸���
        IOCTL_SCSI_MINIPORT,   // ָ�� IOCTL
        pSRBIO, sizeof(SRB_IO_CONTROL) + sizeof(SENDCMDINPARAMS) - 1, // �������ݻ�����   
        pSRBIO, sizeof(SRB_IO_CONTROL) +sizeof(SENDCMDOUTPARAMS) + sizeof(IDINFO) - 1, // ������ݻ�����
        &dwOutBytes,  // ������ݳ���
        (LPOVERLAPPED)NULL); // �� ͬ��I/O
    // �����豸�����ṹ
    ::memcpy(pIdInfo, pSCOP->bBuffer, sizeof(IDINFO));
    // �ͷ�����/������ݿռ�
    ::GlobalFree(pSRBIO);
    return bResult;
}
// �����е��ַ������ߵ�
// ԭ����ATA/ATAPI�е�WORD����Windows���õ��ֽ�˳���� ��
// �����������Ѿ����յ�������ȫ��������������������������
void AdjustString(char* str, int len)
{
    char ch;
    int i;
    // �����ߵ�
    for (i = 0; i < len; i += 2)
    {
        ch = str[i];
        str[i] = str[i + 1];
        str[i + 1] = ch;
    }
    // �����Ҷ���ģ�����Ϊ����� (ȥ����ߵĿո�)
    i = 0;
    while (i < len && str[i] == ' ') i++;
    ::memmove(str, &str[i], len - i);
    // ȥ���ұߵĿո�
    i = len - 1;
    while (i >= 0 && str[i] == ' ')
    {
        str[i] = '\\0';
        i--;
    }
}
// ��ȡIDEӲ�̵��豸��Ϣ�������� �㹻Ȩ��
// nDrive: ��������(0=��һ��Ӳ�̣�1=0=�ڶ���Ӳ�̣�......)
// pIdInfo: �� ����Ϣ�ṹָ��
BOOL GetPhysicalDriveInfoInNT(int nDrive, PIDINFO pIdInfo)
{
    HANDLE hDevice;   // �豸���
    BOOL bResult;   // ���ؽ��
    char szFileName[20]; // �ļ���
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
        // �����ַ���
        ::AdjustString(pIdInfo->sSerialNumber, 20);
        ::AdjustString(pIdInfo->sModelNumber, 40);
        ::AdjustString(pIdInfo -> sFirmwareRev, 8);
    }
    ::CloseHandle(hDevice);
    return bResult;
}
// ��SCSI������ȡIDEӲ�̵��豸��Ϣ������Ȩ����Լ
// nDrive: ��������(0=Primary Master, 1=Promary Slave, 2=Secondary master, 3=Secondary slave)
// pIdInfo: �豸��Ϣ�� ��ָ��
BOOL GetIdeDriveAsScsiInfoInNT(int nDrive, PIDINFO pIdInfo)
{
    HANDLE hDevice;   // �豸���
    BOOL bResult;   // ���ؽ��
    char szFileName[20]; // �ļ��� 
    ::sprintf(szFileName, "\\\\\\\\.\\\\Scsi%d:", nDrive / 2); hDevice = ::OpenDevice(szFileName);
    if (hDevice == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }
    // IDENTIFY DEVICE
    bResult = ::IdentifyDeviceAsScsi(hDevice, nDrive % 2, pIdInfo);
    // ����ǲ� �ǿմ�
    if (pIdInfo->sModelNumber[0] == '\\0')
    {
        bResult = FALSE;
    }
    if (bResult)
    {
        // �����ַ���
        ::AdjustString(pIdInfo -> sSerialNumber, 20);
        ::AdjustString(pIdInfo->sModelNumber, 40);
        ::AdjustString(pIdInfo->sFirmwareRev, 8);
    }
    return bResult;
}
#endif // _DEBUG
void CTestRegisterDlg::OnBnClickedOk()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������

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
