
// ShifratorFDlg.h : файл заголовка
//

#pragma once
#include "afxwin.h"

using namespace std;

// диалоговое окно CShifratorFDlg
class CShifratorFDlg : public CDialogEx
{
// Создание
public:
	CShifratorFDlg(CWnd* pParent = NULL);	// стандартный конструктор

// Данные диалогового окна
	enum { IDD = IDD_SHIFRATORF_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
//	CString edit_text;
	afx_msg void OnBnClickedStartProcess();
	CString edit_shifr_text;
	CString edit_key;
	afx_msg void OnBnClickedButtonGenerateKey();
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonUpload();
	CButton m_radio_shifr;
	CButton m_radio_unshifr;
	CButton m_radio_standart_text_load_upload;
	CButton m_radio_shifr_text_load_upload;
	CButton m_radio_des;
	CButton m_radio_xor;
	CString edit_text;
	CButton m_shifr;
};
