
// HttpClientDlg.h : ͷ�ļ�
//

#pragma once

#include "Http.h"
// CHttpClientDlg �Ի���
class CHttpClientDlg : public CDialogEx
{
// ����
public:
	CHttpClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_HTTPCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
    CHttpClient m_http;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    CString m_strUrl;
    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedButton2();
};
