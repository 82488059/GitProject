
// TESTDlg.h : ͷ�ļ�
//

#pragma once


// CTESTDlg �Ի���
class CTESTDlg : public CDialogEx
{
// ����
public:
	CTESTDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtGo();
    afx_msg void OnBnClickedBtGo2();
    afx_msg void OnBnClickedBtGo3();
    afx_msg void OnBnClickedBtGo4();
    afx_msg void OnBnClickedBtGo5();
    afx_msg void OnBnClickedBtGo6();
};
