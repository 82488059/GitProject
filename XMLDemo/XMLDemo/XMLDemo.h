
// XMLDemo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CXMLDemoApp: 
// �йش����ʵ�֣������ XMLDemo.cpp
//

class CXMLDemoApp : public CWinApp
{
public:
	CXMLDemoApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CXMLDemoApp theApp;