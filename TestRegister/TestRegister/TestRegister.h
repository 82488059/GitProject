
// TestRegister.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTestRegisterApp: 
// �йش����ʵ�֣������ TestRegister.cpp
//

class CTestRegisterApp : public CWinApp
{
public:
	CTestRegisterApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTestRegisterApp theApp;