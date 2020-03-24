
// ShifratorF.h : главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CShifratorFApp:
// О реализации данного класса см. ShifratorF.cpp
//

class CShifratorFApp : public CWinApp
{
public:
	CShifratorFApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CShifratorFApp theApp;