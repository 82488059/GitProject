
// TestHtml.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTestHtmlApp: 
// �йش����ʵ�֣������ TestHtml.cpp
//

class CTestHtmlApp : public CWinApp
{
public:
	CTestHtmlApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTestHtmlApp theApp;