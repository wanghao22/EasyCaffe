
// EasyCaffe.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CEasyCaffeApp: 
// �йش����ʵ�֣������ EasyCaffe.cpp
//

class CEasyCaffeApp : public CWinApp
{
public:
	CEasyCaffeApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CEasyCaffeApp theApp;