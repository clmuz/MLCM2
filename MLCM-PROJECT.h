
// MLCM.h : ������� ���� ��������� ��� ���������� PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"		// �������� �������


// CMLCMApp:
// � ���������� ������� ������ ��. MLCM.cpp
//

class CMLCMApp : public CWinApp
{
public:
	CMLCMApp();

// ���������������
public:
	virtual BOOL InitInstance();

// ����������

	DECLARE_MESSAGE_MAP()
};

extern CMLCMApp theApp;