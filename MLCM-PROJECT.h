
// MLCM.h : главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CMLCMApp:
// О реализации данного класса см. MLCM.cpp
//

class CMLCMApp : public CWinApp
{
public:
	CMLCMApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CMLCMApp theApp;