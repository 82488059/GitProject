
// XMLDemoDlg.h : ͷ�ļ�
//

#pragma once


// CXMLDemoDlg �Ի���
class CXMLDemoDlg : public CDialogEx
{
// ����
public:
	CXMLDemoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_XMLDEMO_DIALOG };

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
    afx_msg void OnBnClickedBtSelectfile();
    afx_msg void OnBnClickedBtGet();
    CString m_szKeyName;
    CString m_szKeyValue;
    afx_msg void OnBnClickedBtSet();
	afx_msg void OnBnClickedBtAddnode();
	afx_msg void OnBnClickedBtCopynode();
};
