// ShifratorFDlg.cpp : файл реализации
//

#include "stdafx.h"
#include "ShifratorF.h"
#include "ShifratorFDlg.h"
#include "afxdialogex.h"

#include <string.h>
#include <string>
#include <fstream>
#include <bitset>
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static int ip[64]{ 57, 49, 41, 33, 25, 17, 9, 1,
										59, 51, 43, 35, 27, 19, 11, 3,
										61, 53, 45, 37, 29, 21, 13, 5,
										63, 55, 47, 39, 31, 23, 15, 7,
										56, 48, 40, 32, 24, 16,  8, 0,
										58, 50, 42, 34, 26, 18, 10, 2,
										60, 52, 44, 36, 28, 20, 12, 4,
										62, 54, 46, 38, 30, 22, 14, 6 };

static int expansion[48]{ 31,  0,  1,  2,  3,  4,  3,  4,
										 5,  6,  7,  8,  7,  8, 9, 10,
										11, 12, 11, 12, 13, 14, 15, 16,
										15, 16, 17, 18, 19, 20, 19, 20,
										21, 22, 23, 24, 23, 24, 25, 26,
										27, 28, 27, 28, 29, 30, 31,  0 };

static byte s_block[8][4][16]
{
					   {{0x0e, 0x04, 0x0d, 0x01, 0x02, 0x0f, 0x0b, 0x08, 0x03, 0x0a, 0x06, 0x0c, 0x05, 0x09, 0x00, 0x07},
						{0x00, 0x0f, 0x07, 0x04, 0x0e, 0x02, 0x0d, 0x01, 0x0a, 0x06, 0x0c, 0x0b, 0x09, 0x05, 0x03, 0x08},
						{0x04, 0x01, 0x04, 0x08, 0x0d, 0x06, 0x02, 0x0b, 0x0f, 0x0c, 0x09, 0x07, 0x03, 0x0a, 0x05, 0x00},
						{0x0f, 0x0c, 0x08, 0x02, 0x04, 0x09, 0x01, 0x07, 0x05, 0x0b, 0x03, 0x0e, 0x0a, 0x00, 0x06, 0x0d}},
					   {{0x0f, 0x01, 0x08, 0x0e, 0x06, 0x0b, 0x03, 0x04, 0x09, 0x07, 0x02, 0x0d, 0x0c, 0x00, 0x05, 0x0a},
						{0x03, 0x0d, 0x04, 0x07, 0x0f, 0x02, 0x08, 0x0e, 0x0c, 0x00, 0x01, 0x0a, 0x06, 0x09, 0x0b, 0x05},
						{0x00, 0x0e, 0x07, 0x0b, 0x0a, 0x04, 0x0d, 0x01, 0x05, 0x08, 0x0c, 0x06, 0x09, 0x03, 0x02, 0x0f},
						{0x0d, 0x08, 0x0a, 0x01, 0x03, 0x0f, 0x04, 0x02, 0x0b, 0x06, 0x07, 0x0c, 0x00, 0x05, 0x0e, 0x09}},
					   {{0x0a, 0x00, 0x09, 0x0e, 0x06, 0x03, 0x0f, 0x05, 0x01, 0x0d, 0x0c, 0x07, 0x0b, 0x04, 0x02, 0x08},
						{0x0d, 0x07, 0x00, 0x09, 0x03, 0x04, 0x06, 0x0a, 0x02, 0x08, 0x05, 0x0e, 0x0c, 0x0b, 0x0f, 0x01},
						{0x0d, 0x06, 0x04, 0x09, 0x08, 0x0f, 0x03, 0x00, 0x0b, 0x01, 0x02, 0x0c, 0x05, 0x0a, 0x0e, 0x07},
						{0x01, 0x0a, 0x0d, 0x00, 0x06, 0x09, 0x08, 0x07, 0x04, 0x0f, 0x0e, 0x03, 0x0b, 0x05, 0x02, 0x0c}},
					   {{0x07, 0x0d, 0x0e, 0x03, 0x00, 0x06, 0x09, 0x0a, 0x01, 0x02, 0x08, 0x05, 0x0b, 0x0c, 0x04, 0x0f},
						{0x0d, 0x08, 0x0b, 0x05, 0x06, 0x0f, 0x00, 0x03, 0x04, 0x07, 0x02, 0x0c, 0x01, 0x0a, 0x0e, 0x09},
						{0x0a, 0x06, 0x09, 0x00, 0x0c, 0x0b, 0x07, 0x0d, 0x0f, 0x01, 0x03, 0x0e, 0x05, 0x02, 0x08, 0x04},
						{0x03, 0x0f, 0x00, 0x06, 0x0a, 0x01, 0x0d, 0x08, 0x09, 0x04, 0x05, 0x0b, 0x0c, 0x07, 0x02, 0x0e}},
					   {{0x02, 0x0c, 0x04, 0x01, 0x07, 0x0a, 0x0b, 0x06, 0x08, 0x05, 0x03, 0x0f, 0x0d, 0x00, 0x0e, 0x09},
						{0x0e, 0x0b, 0x02, 0x0c, 0x04, 0x07, 0x0d, 0x01, 0x05, 0x00, 0x0f, 0x0a, 0x03, 0x09, 0x08, 0x06},
						{0x04, 0x02, 0x01, 0x0b, 0x0a, 0x0d, 0x07, 0x08, 0x0f, 0x09, 0x0c, 0x05, 0x06, 0x03, 0x00, 0x0e},
						{0x0b, 0x08, 0x0c, 0x07, 0x01, 0x0e, 0x02, 0x0d, 0x06, 0x0f, 0x00, 0x09, 0x0a, 0x04, 0x05, 0x03}},
					   {{0x0c, 0x01, 0x0a, 0x0f, 0x09, 0x02, 0x06, 0x08, 0x00, 0x0d, 0x03, 0x04, 0x0e, 0x07, 0x05, 0x0b},
						{0x0a, 0x0f, 0x04, 0x02, 0x07, 0x0c, 0x09, 0x05, 0x06, 0x01, 0x0d, 0x0e, 0x00, 0x0b, 0x03, 0x08},
						{0x09, 0x0e, 0x0f, 0x05, 0x02, 0x08, 0x0c, 0x03, 0x07, 0x00, 0x04, 0x0a, 0x01, 0x0d, 0x01, 0x06},
						{0x04, 0x03, 0x02, 0x0c, 0x09, 0x05, 0x0f, 0x0a, 0x0b, 0x0e, 0x01, 0x07, 0x06, 0x00, 0x08, 0x0d}},
					   {{0x04, 0x0b, 0x02, 0x0e, 0x0f, 0x00, 0x08, 0x0d, 0x03, 0x0c, 0x09, 0x07, 0x05, 0x0a, 0x06, 0x01},
						{0x0d, 0x00, 0x0b, 0x07, 0x04, 0x09, 0x01, 0x0a, 0x0e, 0x03, 0x05, 0x0c, 0x02, 0x0f, 0x08, 0x06},
						{0x01, 0x04, 0x0b, 0x0d, 0x0c, 0x03, 0x07, 0x0e, 0x0a, 0x0f, 0x06, 0x08, 0x00, 0x05, 0x09, 0x02},
						{0x06, 0x0b, 0x0d, 0x08, 0x01, 0x04, 0x0a, 0x07, 0x09, 0x05, 0x00, 0x0f, 0x0e, 0x02, 0x03, 0x0c}},
					   {{0x0d, 0x02, 0x08, 0x04, 0x06, 0x0f, 0x0b, 0x01, 0x0a, 0x09, 0x03, 0x0e, 0x05, 0x00, 0x0c, 0x07},
						{0x01, 0x0f, 0x0d, 0x08, 0x0a, 0x03, 0x07, 0x04, 0x0c, 0x05, 0x06, 0x0b, 0x00, 0x0e, 0x09, 0x02},
						{0x07, 0x0b, 0x04, 0x01, 0x09, 0x0c, 0x0e, 0x02, 0x00, 0x06, 0x0a, 0x0d, 0x0f, 0x03, 0x05, 0x08},
						{0x02, 0x01, 0x0e, 0x07, 0x04, 0x0a, 0x08, 0x0d, 0x0f, 0x0c, 0x09, 0x00, 0x03, 0x05, 0x06, 0x0b}}
};

static int permutation_func[32]{ 15,  6, 19, 20, 28, 11, 27, 16,
												 0, 14, 22, 25,  4, 18, 30, 9,
												 1,  7, 23, 13, 31, 26,  2, 8,
												18, 12, 29,  5, 21, 10,  3, 24 };

static int final_permutation[64]{ 39, 7, 47, 15, 55, 23, 63, 31,
												38, 6, 46, 14, 54, 22, 62, 30,
												37, 5, 45, 13, 53, 21, 61, 29,
												36, 4, 44, 12, 52, 20, 60, 28,
												35, 3, 43, 11, 51, 19, 59, 27,
												34, 2, 42, 10, 50, 18, 58, 26,
												33, 1, 41,  9, 49, 17, 57, 25,
												32, 0, 40,  8, 48, 16, 56, 24 };

static int C_block[28]{ 56, 48, 40, 32, 24, 16, 8,
													0, 57, 49, 41, 33, 25, 17,
													9, 1, 58, 50, 42, 34, 26,
													18, 10, 2, 59, 51, 43, 35 };

static int D_block[28]{ 62, 54, 46, 38, 30, 22, 14,
													6, 61, 53, 45, 37, 29, 21,
													13, 5, 60, 52, 44, 36, 28,
													20, 12, 4, 27, 19, 11, 3 };

static int key_block[48]{ 13, 16, 10, 23, 0, 4,
											2, 27, 14, 5,  20,  9,
											22, 18, 11, 3, 25, 7,
											15, 6, 26, 19, 12, 1,
											40, 51, 30, 36, 46, 54,
											29, 39, 50, 44, 32, 47,
											43, 48, 38, 55, 33, 52,
											45, 41, 49, 35, 28, 31 };

static int key_shift[16]{ 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };
// диалоговое окно CShifratorFDlg
std::string IP_METHOD(std::string s)
{
	std::string res = "";
	for (int i = 0; i < 64; i++)
	{
		res += s[ip[i]];
	}
	return res;
}

std::string E_METHOD(std::string s)
{
	std::string res = "";
	for (int i = 0; i < 48; i++)
	{
		res += s[expansion[i]];
	}
	return res;
}

std::string C_METHOD(std::string s)
{
	std::string res = "";
	for (int i = 0; i < 28; i++)
	{
		res += s[C_block[i]];
	}
	return res;
}

std::string D_METHOD(std::string s)
{
	std::string res = "";
	for (int i = 0; i < 28; i++)
	{
		res += s[D_block[i]];
	}
	return res;
}

std::string CD_METHOD(std::string s)
{
	std::string res = "";
	for (int i = 0; i < 48; i++)
	{
		res += s[key_block[i]];
	}
	return res;
}

std::string P_METHOD(std::string s)
{
	std::string res = "";
	for (int i = 0; i < 32; i++)
	{
		res += s[permutation_func[i]];
	}
	return res;
}

std::string IP_MINUS_METHOD(std::string s)
{
	std::string res = "";
	for (int i = 0; i < 64; i++)
	{
		res += s[final_permutation[i]];
	}
	return res;
}

std::string XOR(std::string s1, std::string s2)
{
	string result = "";

	for (int i = 0; i < s1.length(); i++)
	{
		if (s1[i] == s2[i])
		{
			result += '0';
		}
		else
		{
			result += '1';
		}
	}
	return result;
}

std::string f(std::string R, std::string k)
{
	std::string expanshion_r = E_METHOD(R);
	std::string pred_s_block = XOR(expanshion_r, k);
	std::string pred_s_block_to_block[8];

	for (int i = 0; i < 8; i++)
	{
		pred_s_block_to_block[i] = pred_s_block.substr(i * 6, 6);
	}

	std::string NumberOfString[8];
	std::string NumberOfColumn[8];
	int NumberOfStringHex[8]{ 0 };
	int NumberOfColumnHex[8]{ 0 };
	int S_block_number_int[8]{ 0 };
	std::string S_block_number_str[8];
	std::string S_block_number_bin[8];
	std::string S_block_out = "";
	std::string S_block_out_reverse = "";

	for (int i = 0; i < 8; i++)
	{
		NumberOfString[i] += pred_s_block_to_block[i][0];
		NumberOfString[i] += pred_s_block_to_block[i][5];

		NumberOfColumn[i] += pred_s_block_to_block[i][1];
		NumberOfColumn[i] += pred_s_block_to_block[i][2];
		NumberOfColumn[i] += pred_s_block_to_block[i][3];
		NumberOfColumn[i] += pred_s_block_to_block[i][4];

		for (int j = 0; j < NumberOfString[i].length(); j++)
		{
			NumberOfStringHex[i] *= 2;
			NumberOfStringHex[i] += NumberOfString[i][j] - '0';
		}

		for (int j = 0; j < NumberOfColumn[i].length(); j++)
		{
			NumberOfColumnHex[i] *= 2;
			NumberOfColumnHex[i] += NumberOfColumn[i][j] - '0';
		}

		S_block_number_int[i] = s_block[i][NumberOfStringHex[i]][NumberOfColumnHex[i]];
		S_block_number_str[i] = S_block_number_int[i];


		for (int c : S_block_number_str[i])
		{
			std::bitset<4> bs(c);
			S_block_number_bin[i] += bs.to_string();
			S_block_out += S_block_number_bin[i];
		}
	}

	S_block_out_reverse = P_METHOD(S_block_out);

	return S_block_out_reverse;
}

std::string StringFromBinaryToNormalFormat(std::string input)
{
	string output = "";
	string* Blocks = new string[input.length() / 8];
	for (int i = 0; i < input.length() / 8; i++)
	{
		Blocks[i] = input.substr(i * 8, 8);		//substr - копирование части строки
	}

	for (int i = 0; i < input.length() / 8; i++)
	{
		bitset<8> foo(Blocks[i]);
		unsigned char ch = static_cast<unsigned char> (foo.to_ulong());
		output += ch;
	}

	delete[] Blocks;
	return output;
}

CShifratorFDlg::CShifratorFDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CShifratorFDlg::IDD, pParent)
	, edit_shifr_text(_T("Хотя он ни слова не сказал углекопам о том, что ему запрещено выступать с проповедями, никто и не просил его об этом; видимо, теперь они не нуждались в проповедях. Винсент редко разговаривал с ними. Он теперь вообще редко разговаривал с людьми. Разве что скажет при встрече «добрый день», вот и все. Он не заходил больше в хижины углекопов и не интересовался их жизнью. Рабочие, о чем-то безотчетно догадываясь, по молчаливому уговору даже не упоминали его имени. Они видели, что он чуждается их, но никогда не осуждали его за это. В душе они понимали, что с ним творится."))
	, edit_key(_T(""))
	, edit_text(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CShifratorFDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_EDIT_SHIFR, edit_text);
	DDX_Text(pDX, IDC_EDIT_UNSHIFR, edit_shifr_text);
	DDX_Text(pDX, IDC_EDIT_KEY, edit_key);
	DDX_Control(pDX, IDC_R_CODE, m_radio_shifr);
	DDX_Control(pDX, IDC_R_UNCODE, m_radio_unshifr);
	DDX_Control(pDX, IDC_RADIO_STANDART_TEXT_LOAD, m_radio_standart_text_load_upload);
	DDX_Control(pDX, IDC_RADIO_SHIFR_TEXT_LOAD, m_radio_shifr_text_load_upload);
	DDX_Control(pDX, IDC_RADIO_DES, m_radio_des);
	DDX_Control(pDX, IDC_RADIO_XOR, m_radio_xor);
	DDX_Text(pDX, IDC_EDIT_SHIFR, edit_text);
	DDX_Control(pDX, IDC_BUTTON_GENERATE_KEY, m_shifr);
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

std::string shifr_text_xor = "";
std::string shifr_text_des = "";

void CShifratorFDlg::OnBnClickedStartProcess()
{
	// TODO: добавьте свой код обработчика уведомлений
	if (m_radio_xor.GetCheck() == BST_CHECKED)
	{
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

			shifr_text_xor = res;

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

	if (m_radio_des.GetCheck() == BST_CHECKED)
	{
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

			// перевод в бинарный код каждого символа ключа (8 символов = 64 бита)

			int sum, ostatok;
			std::ofstream out("bits_shifr.txt");
			out << "Сгенерированный ключ: " << key << '\n' << '\n';

			std::string BinaryKey = "";

			for (char c : key)		//цикл, пробегающий по каждому символу ключа и переводящий в двоичный код
			{
				sum = 0;
				ostatok = 0;

				std::bitset<8> bs(c);		//непосредственно перевод в двоичный код (8 бит)
				std::string bitkey56 = "";
				bitkey56 = bs.to_string();		//записываем в строку

				for (int i = 0; i < bitkey56.length() - 1; i++)
				{
					sum += bitkey56[i];		//объединяем результат каждой буквы и получается 64 бит ключа
				}
				ostatok = sum % 2;		//смотрим четность первых 7 бит, чтобы понять, каким нужно сделать последний 8ой бит
				if (ostatok == 0)
				{
					if (bitkey56.back() != '1')	bitkey56.back() = '1';
				}
				BinaryKey += bitkey56;

				out << c << " : " << bitkey56 << '\n';
			}
			out << '\n' << "Ключ в двоичном коде: " << BinaryKey << '\n' << '\n';

			out << '\n' << "Длина текста до дополнения: " << data.length() << '\n' << '\n';

			int dif_before = data.length() % key.length();		//дополняем исходный текст до кратного ключу(как в предыдущем случае шифрования)
			if (dif_before != 0)
			{
				int additionLength = key.length() - dif_before;
				int NewLen = data.length() + additionLength;
				data.resize(NewLen, '0');
			}

			out << data << '\n' << '\n' << "Длина текста после дополнения: " << data.length() << '\n' << '\n';

			// алгоритм DES

			const int SizeOfBlock = 64;		//длина блока в двоич.
			const int SizeOfChar = 8;		//длина символа в двоич.
			const int QuantityOfRounds = 16;		//число итераций цикла фейстеля

			std::string BinaryData = "";
			for (char c : data)		//аналогия для текста
			{
				std::bitset<8> bs(c);
				BinaryData += bs.to_string();
			}

			int NewSizeOfBlock = BinaryData.length() / SizeOfBlock;

			out << "Длина текста в двоичном формате: " << BinaryData.length() << '\n' << '\n';

			std::string* Blocks = new std::string[NewSizeOfBlock];		//вводим массив блоков (делим текст по блокам)
			int LengthOfBlock = BinaryData.length() / NewSizeOfBlock;

			out << "Длина блока: " << LengthOfBlock << '\n' << '\n' << "Количество блоков: " << NewSizeOfBlock << '\n' << '\n';

			for (int i = 0; i < NewSizeOfBlock; i++)
			{
				Blocks[i] = BinaryData.substr(i * LengthOfBlock, LengthOfBlock);		//substr - копирование части строки
			}

			out << "Текст в двоичном представлении: " << '\n' << '\n';
			for (int i = 0; i < NewSizeOfBlock; i++)
			{
				out << Blocks[i];
			}

			//начальная перестановка
			std::string StartRevers = IP_METHOD(Blocks[0]);		//начальная перестановка (все методы вверху)

			std::string L = Blocks[0].substr(0, LengthOfBlock / 2);		//здесь идет не сам алгоритм, а просто проверка на работоспособность,
			std::string R = Blocks[0].substr(LengthOfBlock / 2, LengthOfBlock / 2);			//поэтому можно пропускать все, что не используется в самом алгоритме
			std::string res = XOR(L, R);

			out << '\n' << '\n' << "Исходный блок: " << Blocks[0] << '\n' << '\n' << "Блок после начальной перестановки: " << StartRevers << '\n' << '\n';

			out << "Левая часть: " << L << '\n' << "Правая часть: " << R << '\n' << "XOR: " << res << '\n' << '\n';

			//расширяем R
			std::string expanshion_r = E_METHOD(R);

			out << "Правая часть: " << R << '\n' << "Расширенная правая часть: " << expanshion_r << '\n' << '\n';

			//значение ключа 56 бит
			std::string key56 = "";
			for (int i = 0; i < BinaryKey.length(); i++)
			{
				if (i != 7 && i != 15 && i != 23 && i != 31 && i != 39 && i != 47 && i != 55 && i != 63)
				{
					key56 += BinaryKey[i];
				}
			}

			out << "Ключ 64 бита: " << BinaryKey << '\n' << "Ключ 56 бит: " << key56 << '\n' << '\n';

			std::string C = key56.substr(0, key56.length() / 2);
			std::string D = key56.substr(key56.length() / 2, key56.length() / 2);

			//перестановка C, D блоков

			//этот участок понадобится, это C,D блоки для нахождения функции f (можно посмотреть в документе инфу и привести аналогию)

			std::string C_reverse[QuantityOfRounds + 1];
			std::string D_reverse[QuantityOfRounds + 1];
			std::string C0_reverse = C_METHOD(BinaryKey);
			std::string D0_reverse = D_METHOD(BinaryKey);
			std::string CD0 = C0_reverse + D0_reverse;

			C_reverse[0] = C0_reverse;
			D_reverse[0] = D0_reverse;
			std::string buf1;
			std::string buf2;

			for (int i = 1; i <= QuantityOfRounds; i++)
			{
				if ((i == 1) || (i == 2) || (i == 9) || (i == 16))
				{
					buf1 = "";
					buf1 = C_reverse[i - 1][0];
					C_reverse[i] = C_reverse[i - 1] + buf1;
					C_reverse[i] = C_reverse[i].erase(0, 1);

					buf2 = "";
					buf2 = D_reverse[i - 1][0];
					D_reverse[i] = D_reverse[i - 1] + buf2;
					D_reverse[i] = D_reverse[i].erase(0, 1);
				}
				else
				{
					buf1 = "";
					buf1 += C_reverse[i - 1][0];
					buf1 += C_reverse[i - 1][1];
					C_reverse[i] = C_reverse[i - 1] + buf1;
					C_reverse[i] = C_reverse[i].erase(0, 2);

					buf2 = "";
					buf2 += D_reverse[i - 1][0];
					buf2 += D_reverse[i - 1][1];
					D_reverse[i] = D_reverse[i - 1] + buf2;
					D_reverse[i] = D_reverse[i].erase(0, 2);
				}
			}

			out << "C0-блок: " << C0_reverse << '\n' << "D0-блок: " << D0_reverse << '\n' << '\n';

			for (int i = 0; i <= QuantityOfRounds; i++)
			{
				out << "C-блок на " << i << "-ой итерации: " << '\t' << C_reverse[i] << '\n';// << "D-блок после перестановки: " << D_reverse[i] << '\n';
			}

			out << '\n';

			for (int i = 0; i <= QuantityOfRounds; i++)
			{
				out << "D-блок на " << i << "-ой итерации: " << '\t' << D_reverse[i] << '\n';// << "D-блок после перестановки: " << D_reverse[i] << '\n';
			}

			out << '\n';
			//ключ из 56 бит в 48 бит

			std::string CD[QuantityOfRounds];
			std::string key48[QuantityOfRounds];
			for (int j = 0; j < QuantityOfRounds; j++)
			{
				CD[j] = C_reverse[j + 1] + D_reverse[j + 1];
				key48[j] = CD_METHOD(CD[j]);
				out << "СD-блок на " << j << "-ой итерации: " << '\t' << CD[j] << '\n';
			}

			out << '\n';

			for (int i = 0; i < QuantityOfRounds; i++)
			{
				out << "Ключ на " << i << "-ой итерации:" << '\t' << key48[i] << '\n';
			}

			//нахождение функции f
			//std::string key48 = CD_METHOD(CD0);

			std::string pred_s_block = XOR(expanshion_r, key48[0]);
			std::string pred_s_block_to_block[8];

			for (int i = 0; i < 8; i++)
			{
				pred_s_block_to_block[i] = pred_s_block.substr(i * 6, 6);
			}

			out << '\n' << "Входная последовательно S-блоков: " << pred_s_block << '\n' << "Вход 1S-блока: " << pred_s_block_to_block[0] << '\n' << '\n';
			out << pred_s_block_to_block[0][0] << pred_s_block_to_block[0][5] << '\n' << '\n';

			std::string NumberOfString[8];
			std::string NumberOfColumn[8];
			int NumberOfStringHex[8]{ 0 };
			int NumberOfColumnHex[8]{ 0 };
			int S_block_number_int[8]{ 0 };
			std::string S_block_number_str[8];
			std::string S_block_number_bin[8];
			std::string S_block_out = "";

			for (int i = 0; i < 8; i++)
			{
				NumberOfString[i] += pred_s_block_to_block[i][0];
				NumberOfString[i] += pred_s_block_to_block[i][5];

				NumberOfColumn[i] += pred_s_block_to_block[i][1];
				NumberOfColumn[i] += pred_s_block_to_block[i][2];
				NumberOfColumn[i] += pred_s_block_to_block[i][3];
				NumberOfColumn[i] += pred_s_block_to_block[i][4];

				for (int j = 0; j < NumberOfString[i].length(); j++)
				{
					NumberOfStringHex[i] *= 2;
					NumberOfStringHex[i] += NumberOfString[i][j] - '0';
				}

				for (int j = 0; j < NumberOfColumn[i].length(); j++)
				{
					NumberOfColumnHex[i] *= 2;
					NumberOfColumnHex[i] += NumberOfColumn[i][j] - '0';
				}

				S_block_number_int[i] = s_block[i][NumberOfStringHex[i]][NumberOfColumnHex[i]];
				S_block_number_str[i] = S_block_number_int[i];


				for (int c : S_block_number_str[i])
				{
					std::bitset<4> bs(c);
					S_block_number_bin[i] += bs.to_string();
					S_block_out += S_block_number_bin[i];
				}
			}

			out << "Номер строки 1S-блока: " << NumberOfString[0] << '\n' << "Номер столбца 1S-блока: " << NumberOfColumn[0] << '\n' << '\n';
			out << "Номер строки 1S-блока (HEX) : " << NumberOfStringHex[0] << '\n' << "Номер столбца 1S-блока (HEX) : " << NumberOfColumnHex[0] << '\n' << '\n';
			out << "Число для 1S-блока: " << S_block_number_int[0] << '\n' << "Число для 1S-блока в двоичной системе: " << S_block_number_bin[0] << '\n' << '\n';
			out << "Выход S-блоков: " << S_block_out << '\n' << '\n';

			std::string S_block_out_reverse = P_METHOD(S_block_out);

			//алгоритм Фейстеля
			//вот непосредственно сам алгоритм, сам по себе небольшой

			std::string StartReversBlocks = "";
			std::string L0 = "";
			std::string R0 = "";
			std::string L_part[QuantityOfRounds];
			std::string R_part[QuantityOfRounds];
			std::string LR = "";
			std::string LR_revers;
			std::string ResultOfFeistel = "";

			//шифрование
			for (int j = 0; j < NewSizeOfBlock; j++)
			{

				StartReversBlocks = IP_METHOD(Blocks[j]);
				L0 = StartReversBlocks.substr(0, LengthOfBlock / 2);
				R0 = StartReversBlocks.substr(LengthOfBlock / 2, LengthOfBlock / 2);

				out << '\n' << j << "-ая итерация." << '\n' << "L0: " << L0 << '\n' << "R0: " << R0 << '\n';

				for (int i = 0; i < QuantityOfRounds; i++)
				{
					if (i == 0)
					{
						L_part[i] = R0;
						R_part[i] = XOR(L0, f(R0, key48[i]));
					}
					else
					{
						L_part[i] = R_part[i - 1];
						R_part[i] = XOR(L_part[i - 1], f(R_part[i - 1], key48[i]));
					}
				}

				LR = "";
				LR_revers = "";
				LR = R_part[15] + L_part[15];
				LR_revers = IP_MINUS_METHOD(LR);

				out << '\n' << "Результат для первого блока текста: " << LR_revers << '\n';

				ResultOfFeistel += LR_revers;
			}

			std::string exit = StringFromBinaryToNormalFormat(ResultOfFeistel);
			shifr_text_des = exit;

			string test = "01000100";

			edit_text = exit.c_str();
			UpdateData(FALSE);

			out << '\n' << "Результат шифрования методом DES (BIN): " << '\n' << ResultOfFeistel << '\n' << '\n';
			out << "Длина зашифрованного текста в двоичном виде: " << ResultOfFeistel.length() << '\n' << '\n';
			out << "Результат шифрования методом DES (NORMAL): " << '\n' << exit << '\n' << '\n';
			out << "Результат шифрования методом DES (NORMAL) C_STR: " << '\n' << exit.c_str() << '\n' << '\n';
			out << "Длина зашифрованного текста в нормальном виде: " << exit.length() << '\n' << '\n';
			out << "TEST: " << test << '\n' << '\n' << "TEST BUKVA: " << StringFromBinaryToNormalFormat(test) << '\n' << '\n';


			delete[] Blocks;

			out.close();
		}

		if (m_radio_unshifr.GetCheck() == BST_CHECKED)			//расшифрование не работает
		{
			UpdateData(TRUE);
			if (edit_key.GetLength() == NULL) MessageBox(L"Ошибка! Введите или сгенерируйте ключ!", L"ERROR", MB_OK | MB_ICONERROR);

			CString Text = edit_text;
			CString Key = edit_key;

			std::string data = shifr_text_des;
			/*data.resize(Text.GetLength());
			WideCharToMultiByte(CP_ACP, 0, Text, -1, &data[0], data.size(), NULL, NULL);*/
			std::string key;
			key.resize(Key.GetLength());
			WideCharToMultiByte(CP_ACP, 0, Key, -1, &key[0], key.size(), NULL, NULL);

			int sum, ostatok;
			std::ofstream out("bits_unshifr.txt");
			out << "Сгенерированный ключ: " << key << '\n' << '\n';

			std::string BinaryKey = "";

			for (char c : key)
			{
				sum = 0;
				ostatok = 0;

				std::bitset<8> bs(c);
				std::string bitkey56 = "";
				bitkey56 = bs.to_string();

				for (int i = 0; i < bitkey56.length() - 1; i++)
				{
					sum += bitkey56[i];
				}
				ostatok = sum % 2;
				if (ostatok == 0)
				{
					if (bitkey56.back() != '1')	bitkey56.back() = '1';
				}
				BinaryKey += bitkey56;

				out << c << " : " << bitkey56 << '\n';
			}

			out << '\n' << "Ключ в двоичном коде: " << BinaryKey << '\n' << '\n';

			out << '\n' << "Зашифрованный текст лок: " << data << '\n' << '\n';

			out << '\n' << "Зашифрованный текст глоб: " << shifr_text_des << '\n' << '\n';

			out << '\n' << "Длина зашифрованного текста: " << shifr_text_des.length() << '\n' << '\n';

			// алгоритм DES

			const int SizeOfBlock = 64;
			const int SizeOfChar = 8;
			const int QuantityOfRounds = 16;

			std::string BinaryData = "";
			for (char c : data)
			{
				std::bitset<8> bs(c);
				BinaryData += bs.to_string();
			}

			int NewSizeOfBlock = BinaryData.length() / SizeOfBlock;

			out << "Длина текста в двоичном формате: " << BinaryData.length() << '\n' << '\n';

			std::string* Blocks = new std::string[NewSizeOfBlock];
			int LengthOfBlock = BinaryData.length() / NewSizeOfBlock;

			out << "Длина блока: " << LengthOfBlock << '\n' << '\n' << "Количество блоков: " << NewSizeOfBlock << '\n' << '\n';

			for (int i = 0; i < NewSizeOfBlock; i++)
			{
				Blocks[i] = BinaryData.substr(i * LengthOfBlock, LengthOfBlock);
			}

			out << "Текст в двоичном представлении: " << '\n' << '\n';
			for (int i = 0; i < NewSizeOfBlock; i++)
			{
				out << Blocks[i];
			}

			//начальная перестановка
			std::string StartRevers = IP_METHOD(Blocks[0]);

			std::string L = Blocks[0].substr(0, LengthOfBlock / 2);
			std::string R = Blocks[0].substr(LengthOfBlock / 2, LengthOfBlock / 2);
			std::string res = XOR(L, R);

			out << '\n' << '\n' << "Исходный блок: " << Blocks[0] << '\n' << '\n' << "Блок после начальной перестановки: " << StartRevers << '\n' << '\n';

			out << "Левая часть: " << L << '\n' << "Правая часть: " << R << '\n' << "XOR: " << res << '\n' << '\n';

			//расширяем R
			std::string expanshion_r = E_METHOD(R);

			out << "Правая часть: " << R << '\n' << "Расширенная правая часть: " << expanshion_r << '\n' << '\n';

			//значение ключа 56 бит
			std::string key56 = "";
			for (int i = 0; i < BinaryKey.length(); i++)
			{
				if (i != 7 && i != 15 && i != 23 && i != 31 && i != 39 && i != 47 && i != 55 && i != 63)
				{
					key56 += BinaryKey[i];
				}
			}

			out << "Ключ 64 бита: " << BinaryKey << '\n' << "Ключ 56 бит: " << key56 << '\n' << '\n';

			std::string C = key56.substr(0, key56.length() / 2);
			std::string D = key56.substr(key56.length() / 2, key56.length() / 2);

			//перестановка C, D блоков

			std::string C_reverse[QuantityOfRounds + 1];
			std::string D_reverse[QuantityOfRounds + 1];
			std::string C0_reverse = C_METHOD(BinaryKey);
			std::string D0_reverse = D_METHOD(BinaryKey);
			std::string CD0 = C0_reverse + D0_reverse;

			C_reverse[0] = C0_reverse;
			D_reverse[0] = D0_reverse;
			std::string buf1;
			std::string buf2;

			for (int i = 1; i <= QuantityOfRounds; i++)
			{
				if ((i == 1) || (i == 2) || (i == 9) || (i == 16))
				{
					buf1 = "";
					buf1 = C_reverse[i - 1][0];
					C_reverse[i] = C_reverse[i - 1] + buf1;
					C_reverse[i] = C_reverse[i].erase(0, 1);

					buf2 = "";
					buf2 = D_reverse[i - 1][0];
					D_reverse[i] = D_reverse[i - 1] + buf2;
					D_reverse[i] = D_reverse[i].erase(0, 1);
				}
				else
				{
					buf1 = "";
					buf1 += C_reverse[i - 1][0];
					buf1 += C_reverse[i - 1][1];
					C_reverse[i] = C_reverse[i - 1] + buf1;
					C_reverse[i] = C_reverse[i].erase(0, 2);

					buf2 = "";
					buf2 += D_reverse[i - 1][0];
					buf2 += D_reverse[i - 1][1];
					D_reverse[i] = D_reverse[i - 1] + buf2;
					D_reverse[i] = D_reverse[i].erase(0, 2);
				}
			}

			out << "C0-блок: " << C0_reverse << '\n' << "D0-блок: " << D0_reverse << '\n' << '\n';

			for (int i = 0; i <= QuantityOfRounds; i++)
			{
				out << "C-блок на " << i << "-ой итерации: " << '\t' << C_reverse[i] << '\n';// << "D-блок после перестановки: " << D_reverse[i] << '\n';
			}

			out << '\n';

			for (int i = 0; i <= QuantityOfRounds; i++)
			{
				out << "D-блок на " << i << "-ой итерации: " << '\t' << D_reverse[i] << '\n';// << "D-блок после перестановки: " << D_reverse[i] << '\n';
			}

			out << '\n';
			//ключ из 56 бит в 48 бит

			std::string CD[QuantityOfRounds];
			std::string key48[QuantityOfRounds];
			for (int j = 0; j < QuantityOfRounds; j++)
			{
				CD[j] = C_reverse[j + 1] + D_reverse[j + 1];
				key48[j] = CD_METHOD(CD[j]);
				out << "СD-блок на " << j << "-ой итерации: " << '\t' << CD[j] << '\n';
			}

			out << '\n';

			for (int i = 0; i < QuantityOfRounds; i++)
			{
				out << "Ключ на " << i << "-ой итерации:" << '\t' << key48[i] << '\n';
			}

			//нахождение функции f
			//std::string key48 = CD_METHOD(CD0);

			std::string pred_s_block = XOR(expanshion_r, key48[0]);
			std::string pred_s_block_to_block[8];

			for (int i = 0; i < 8; i++)
			{
				pred_s_block_to_block[i] = pred_s_block.substr(i * 6, 6);
			}

			out << '\n' << "Входная последовательно S-блоков: " << pred_s_block << '\n' << "Вход 1S-блока: " << pred_s_block_to_block[0] << '\n' << '\n';
			out << pred_s_block_to_block[0][0] << pred_s_block_to_block[0][5] << '\n' << '\n';

			std::string NumberOfString[8];
			std::string NumberOfColumn[8];
			int NumberOfStringHex[8]{ 0 };
			int NumberOfColumnHex[8]{ 0 };
			int S_block_number_int[8]{ 0 };
			std::string S_block_number_str[8];
			std::string S_block_number_bin[8];
			std::string S_block_out = "";

			for (int i = 0; i < 8; i++)
			{
				NumberOfString[i] += pred_s_block_to_block[i][0];
				NumberOfString[i] += pred_s_block_to_block[i][5];

				NumberOfColumn[i] += pred_s_block_to_block[i][1];
				NumberOfColumn[i] += pred_s_block_to_block[i][2];
				NumberOfColumn[i] += pred_s_block_to_block[i][3];
				NumberOfColumn[i] += pred_s_block_to_block[i][4];

				for (int j = 0; j < NumberOfString[i].length(); j++)
				{
					NumberOfStringHex[i] *= 2;
					NumberOfStringHex[i] += NumberOfString[i][j] - '0';
				}

				for (int j = 0; j < NumberOfColumn[i].length(); j++)
				{
					NumberOfColumnHex[i] *= 2;
					NumberOfColumnHex[i] += NumberOfColumn[i][j] - '0';
				}

				S_block_number_int[i] = s_block[i][NumberOfStringHex[i]][NumberOfColumnHex[i]];
				S_block_number_str[i] = S_block_number_int[i];


				for (int c : S_block_number_str[i])
				{
					std::bitset<4> bs(c);
					S_block_number_bin[i] += bs.to_string();
					S_block_out += S_block_number_bin[i];
				}
			}

			out << "Номер строки 1S-блока: " << NumberOfString[0] << '\n' << "Номер столбца 1S-блока: " << NumberOfColumn[0] << '\n' << '\n';
			out << "Номер строки 1S-блока (HEX) : " << NumberOfStringHex[0] << '\n' << "Номер столбца 1S-блока (HEX) : " << NumberOfColumnHex[0] << '\n' << '\n';
			out << "Число для 1S-блока: " << S_block_number_int[0] << '\n' << "Число для 1S-блока в двоичной системе: " << S_block_number_bin[0] << '\n' << '\n';
			out << "Выход S-блоков: " << S_block_out << '\n' << '\n';

			std::string S_block_out_reverse = P_METHOD(S_block_out);

			//алгоритм Фейстеля

			std::string StartReversBlocks = "";
			std::string L0 = "";
			std::string R0 = "";
			std::string L_part[QuantityOfRounds];
			std::string R_part[QuantityOfRounds];
			std::string LR = "";
			std::string LR_revers;
			std::string ResultOfFeistel = "";

			//шифрование
			for (int j = 0; j < NewSizeOfBlock; j++)
			{

				StartReversBlocks = IP_METHOD(Blocks[j]);
				L0 = StartReversBlocks.substr(0, LengthOfBlock / 2);
				R0 = StartReversBlocks.substr(LengthOfBlock / 2, LengthOfBlock / 2);

				out << '\n' << j << "-ая итерация." << '\n' << "L0: " << L0 << '\n' << "R0: " << R0 << '\n';

				for (int i = 0; i < QuantityOfRounds; i++)
				{
					if (i == 0)
					{
						L_part[i] = R0;
						R_part[i] = XOR(L0, f(R0, key48[QuantityOfRounds - 1 - i]));
					}
					else
					{
						L_part[i] = R_part[i - 1];
						R_part[i] = XOR(L_part[i - 1], f(R_part[i - 1], key48[QuantityOfRounds - 1 - i]));
					}
				}

				LR = "";
				LR_revers = "";
				LR = R_part[15] + L_part[15];
				LR_revers = IP_MINUS_METHOD(LR);

				out << '\n' << "Результат для первого блока текста: " << LR_revers << '\n';

				ResultOfFeistel += LR_revers;
			}

			std::string exit = StringFromBinaryToNormalFormat(ResultOfFeistel);

			for (int i = exit.length(); i > exit.length() - 8; i--)
			{
				if (exit[i] == '0') exit.erase(i, 1);
			}

			edit_shifr_text = exit.c_str();
			UpdateData(FALSE);

			out << '\n' << "Результат шифрования методом DES (BIN): " << '\n' << ResultOfFeistel << '\n' << '\n';
			out << "Длина зашифрованного текста в двоичном виде: " << ResultOfFeistel.length() << '\n' << '\n';
			out << "Результат шифрования методом DES (NORMAL): " << '\n' << exit << '\n' << '\n';
			out << "Длина зашифрованного текста в нормальном виде: " << exit.length() << '\n' << '\n';

			delete[] Blocks;

			out.close();
		}
	}
}

void CShifratorFDlg::OnBnClickedButtonGenerateKey()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData(TRUE);

	int lPass = 8;
	char* key = new char[lPass];
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

