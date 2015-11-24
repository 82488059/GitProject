
// HttpClientDlg.h : 头文件
//

#pragma once

#include "Http.h"
// CHttpClientDlg 对话框
class CHttpClientDlg : public CDialogEx
{
// 构造
public:
	CHttpClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_HTTPCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
    CHttpClient m_http;
	// 生成的消息映射函数
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
