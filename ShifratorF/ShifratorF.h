
// ShifratorF.h : ������� ���� ��������� ��� ���������� PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"		// �������� �������


// CShifratorFApp:
// � ���������� ������� ������ ��. ShifratorF.cpp
//

class CShifratorFApp : public CWinApp
{
public:
	CShifratorFApp();

// ���������������
public:
	virtual BOOL InitInstance();

// ����������

	DECLARE_MESSAGE_MAP()
};

extern CShifratorFApp theApp;