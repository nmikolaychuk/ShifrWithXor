
// ShifratorFDlg.h : ���� ���������
//

#pragma once
#include "afxwin.h"

using namespace std;

// ���������� ���� CShifratorFDlg
class CShifratorFDlg : public CDialogEx
{
// ��������
public:
	CShifratorFDlg(CWnd* pParent = NULL);	// ����������� �����������

// ������ ����������� ����
	enum { IDD = IDD_SHIFRATORF_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// ��������� DDX/DDV


// ����������
protected:
	HICON m_hIcon;

	// ��������� ������� ����� ���������
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
