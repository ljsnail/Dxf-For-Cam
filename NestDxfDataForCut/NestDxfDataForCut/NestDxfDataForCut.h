
// NestDxfDataForCut.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CNestDxfDataForCutApp: 
// �йش����ʵ�֣������ NestDxfDataForCut.cpp
//

class CNestDxfDataForCutApp : public CWinApp
{
public:
	CNestDxfDataForCutApp();

// ��д
public:
	virtual BOOL InitInstance();
	

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CNestDxfDataForCutApp theApp;