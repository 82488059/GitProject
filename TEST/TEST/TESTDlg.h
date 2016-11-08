
// TESTDlg.h : 头文件
//

#pragma once


// CTESTDlg 对话框
class CTESTDlg : public CDialogEx
{
// 构造
public:
	CTESTDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TEST_DIALOG };

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
    afx_msg void OnBnClickedBtGo();
    afx_msg void OnBnClickedBtGo2();
    afx_msg void OnBnClickedBtGo3();
    afx_msg void OnBnClickedBtGo4();
    afx_msg void OnBnClickedBtGo5();
    afx_msg void OnBnClickedBtGo6();
};
