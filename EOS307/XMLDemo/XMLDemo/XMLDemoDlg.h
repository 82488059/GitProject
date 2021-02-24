
// XMLDemoDlg.h : 头文件
//

#pragma once


// CXMLDemoDlg 对话框
class CXMLDemoDlg : public CDialogEx
{
// 构造
public:
	CXMLDemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_XMLDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
