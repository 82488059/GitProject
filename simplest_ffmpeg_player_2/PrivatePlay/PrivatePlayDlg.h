
// PrivatePlayDlg.h : ͷ�ļ�
//

#pragma once
#include "video_decoder.h"


// CPrivatePlayDlg �Ի���
class CPrivatePlayDlg : public CDialogEx
{
// ����
public:
	CPrivatePlayDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PRIVATEPLAY_DIALOG };

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
    afx_msg void OnBnClickedBtPlay();
    video_decoder decoder_;

    static int output(const uint8_t* const* data, int w, int h, const int* linesize);
    static void error_out(const std::string& err);

};
