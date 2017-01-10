
// PrivatePlayDlg.h : 头文件
//

#pragma once
#include "video_decoder.h"


// CPrivatePlayDlg 对话框
class CPrivatePlayDlg : public CDialogEx
{
// 构造
public:
	CPrivatePlayDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PRIVATEPLAY_DIALOG };

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
    afx_msg void OnBnClickedBtPlay();
    video_decoder decoder_;

    static int output(const uint8_t* const* data, int w, int h, const int* linesize);
    static void error_out(const std::string& err);

};
