
// ShifratorFDlg.cpp : файл реализации
//

#include "stdafx.h"
#include "ShifratorF.h"
#include "ShifratorFDlg.h"
#include "afxdialogex.h"

#include <string.h>
#include <string>
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// диалоговое окно CShifratorFDlg


CShifratorFDlg::CShifratorFDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CShifratorFDlg::IDD, pParent)
	, edit_text(_T(""))
	, edit_shifr_text(_T("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."))
	, edit_key(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CShifratorFDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SHIFR, edit_text);
	DDX_Text(pDX, IDC_EDIT_UNSHIFR, edit_shifr_text);
	DDX_Text(pDX, IDC_EDIT_KEY, edit_key);
	DDX_Control(pDX, IDC_R_CODE, m_radio_shifr);
	DDX_Control(pDX, IDC_R_UNCODE, m_radio_unshifr);
	DDX_Control(pDX, IDC_RADIO_STANDART_TEXT_LOAD, m_radio_standart_text_load_upload);
	DDX_Control(pDX, IDC_RADIO_SHIFR_TEXT_LOAD, m_radio_shifr_text_load_upload);
}

BEGIN_MESSAGE_MAP(CShifratorFDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START_PROCESS, &CShifratorFDlg::OnBnClickedStartProcess)
	ON_BN_CLICKED(IDC_BUTTON_GENERATE_KEY, &CShifratorFDlg::OnBnClickedButtonGenerateKey)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CShifratorFDlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_UPLOAD, &CShifratorFDlg::OnBnClickedButtonUpload)
END_MESSAGE_MAP()


// обработчики сообщений CShifratorFDlg

BOOL CShifratorFDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CShifratorFDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CShifratorFDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CShifratorFDlg::OnBnClickedStartProcess()
{
	// TODO: добавьте свой код обработчика уведомлений
	if (m_radio_shifr.GetCheck() == BST_CHECKED)
	{
		UpdateData(TRUE);
		if (edit_key.GetLength() == NULL) MessageBox(L"Ошибка! Введите или сгенерируйте ключ!", L"ERROR", MB_OK | MB_ICONERROR);

		CString Text = edit_shifr_text;
		CString Key = edit_key;

		std::string data;
		data.resize(Text.GetLength());
		WideCharToMultiByte(CP_ACP, 0, Text, -1, &data[0], data.size(), NULL, NULL);
		std::string key;
		key.resize(Key.GetLength());
		WideCharToMultiByte(CP_ACP, 0, Key, -1, &key[0], key.size(), NULL, NULL);
		std::string res;
		res.resize(data.length());

		int dif_before = data.length() % key.length();
		
		if (dif_before != 0) {
			int additionLength = key.length() - dif_before;
			int NewLen = data.length() + additionLength;
			data.resize(NewLen, '0');
			res.resize(data.length());

			for (int i = 0; i < NewLen; i++)
			{
				res[i] = data[i] ^ key[i % key.length()];
			}
		}
		else
		{
			for (int i = 0; i < data.length(); i++)
			{
				res[i] = data[i] ^ key[i % key.length()];
			}
		}

		/*std::ofstream out("dataresize.txt");
		out << data << std::endl << "Длина исходного текста: " << data.length() << std::endl << "Остаток от деления: " << dif_before << std::endl << std::endl;
		data.resize(NewLen, '0');
		int dif_after = data.length() % key.length();
		out << data << std::endl << "Длина исходного текста: " << data.length() << std::endl << "Остаток от деления: " << dif_after << std::endl << std::endl;
		out << key << std::endl << key.length() << std::endl << std::endl;*/

		edit_text = res.c_str();
		UpdateData(FALSE);
	}

	if (m_radio_unshifr.GetCheck() == BST_CHECKED)
	{
		UpdateData(TRUE);
		if (edit_key.GetLength() == NULL) MessageBox(L"Ошибка! Введите или сгенерируйте ключ!", L"ERROR", MB_OK | MB_ICONERROR);
		
		CString Text = edit_text;
		CString Key = edit_key;

		std::string data;
		data.resize(Text.GetLength());
		WideCharToMultiByte(CP_ACP, 0, Text, -1, &data[0], data.size(), NULL, NULL);
		std::string key;
		key.resize(Key.GetLength());
		WideCharToMultiByte(CP_ACP, 0, Key, -1, &key[0], key.size(), NULL, NULL);
		std::string res;
		res.resize(data.length());
		
		for (int i = 0; i < data.length(); i++)
		{
			res[i] = data[i] ^ key[i % key.length()];
		}
		
		int dif_before = data.length() % key.length();
		int additionLength = key.length() - dif_before;

		for (int i = res.length(); i > res.length() - key.length(); i--)
		{
			if (res[i] == '0') res.erase(i, 1);
		}
		//res.erase(res.rfind('0'), );
		
		/*std::ofstream out("dataresize.txt");
		out << data << std::endl << "Длина исходного текста: " << data.length() << std::endl << "Остаток от деления: " << dif_before << std::endl << std::endl;
		data.resize(NewLen, '0');
		int dif_after = data.length() % key.length();
		out << data << std::endl << "Длина исходного текста: " << data.length() << std::endl << "Остаток от деления: " << dif_after << std::endl << std::endl;
		out << key << std::endl << key.length() << std::endl << std::endl;*/

		edit_shifr_text = res.c_str();
		UpdateData(FALSE);
	}
}


void CShifratorFDlg::OnBnClickedButtonGenerateKey()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData(TRUE);
	
	int lPass = 8;
	char *key = new char[lPass];
	CString eKey;
	int t, count_ch = 26;

	srand(time(0));
	for (int i = 0; i < lPass; i++)
	{
		key[i] = (t = rand() % (count_ch * 2)) >= count_ch ? 'a' + t % count_ch : 'A' + t;
		eKey += key[i];
	}

	edit_key = eKey;
	UpdateData(FALSE);
}


void CShifratorFDlg::OnBnClickedButtonLoad()
{
	if (m_radio_standart_text_load_upload.GetCheck() == BST_CHECKED)
	{
		setlocale(LC_ALL, "RU");
		CFileDialog fileDialog(TRUE, NULL, L"*.txt");
		int res = fileDialog.DoModal();
		if (res != IDOK)
			return;
		CFile file;
		file.Open(fileDialog.GetPathName(), CFile::modeRead);
		CStringA str;
		LPSTR pBuf = str.GetBuffer(file.GetLength() + 1);
		file.Read(pBuf, file.GetLength() + 1);
		pBuf[file.GetLength()] = NULL;
		CStringA decodedText = str;
		file.Close();
		str.ReleaseBuffer();

		edit_shifr_text = "";
		edit_shifr_text = str;
	}
	if (m_radio_shifr_text_load_upload.GetCheck() == BST_CHECKED)
	{
		setlocale(LC_ALL, "RU");
		CFileDialog fileDialog(TRUE, NULL, L"*.txt");
		int res = fileDialog.DoModal();
		if (res != IDOK)
			return;
		CFile file;
		file.Open(fileDialog.GetPathName(), CFile::modeRead);
		CStringA str;
		LPSTR pBuf = str.GetBuffer(file.GetLength() + 1);
		file.Read(pBuf, file.GetLength() + 1);
		pBuf[file.GetLength()] = NULL;
		CStringA decodedText = str;
		file.Close();
		str.ReleaseBuffer();

		edit_text = "";
		edit_text = str;
	}
	UpdateData(FALSE);
}


void CShifratorFDlg::OnBnClickedButtonUpload()
{
	// TODO: добавьте свой код обработчика уведомлений
	if (m_radio_standart_text_load_upload.GetCheck() == BST_CHECKED)
	{
		UpdateData(TRUE);
		setlocale(LC_ALL, "RU");
		CFileDialog fileDialog(FALSE, NULL, L".txt");
		int res = fileDialog.DoModal();
		if (res != IDOK)
			return;

		CStdioFile oResultFile;
		oResultFile.Open(fileDialog.GetPathName(), CFile::modeWrite | CFile::modeCreate);
		oResultFile.WriteString(edit_shifr_text);
		oResultFile.Close();
	}
	if (m_radio_shifr_text_load_upload.GetCheck() == BST_CHECKED)
	{
		UpdateData(TRUE);
		setlocale(LC_ALL, "RU");
		CFileDialog fileDialog(FALSE, NULL, L".txt");
		int res = fileDialog.DoModal();
		if (res != IDOK)
			return;

		CStdioFile oResultFile;
		oResultFile.Open(fileDialog.GetPathName(), CFile::modeWrite | CFile::modeCreate);
		oResultFile.WriteString(edit_text);
		oResultFile.Close();
	}
	//UpdateData(FALSE);
}

