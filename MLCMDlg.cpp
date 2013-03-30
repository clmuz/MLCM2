
// MLCMDlg.cpp : ���� ����������
//

#include "stdafx.h"
#include "MLCM-PROJECT.h"
#include "MLCMDlg.h"
#include "afxdialogex.h"
#include <fstream>
#include <vector>

using namespace std;

// ���������� ���� CAboutDlg ������������ ��� �������� �������� � ����������

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ������ ����������� ����
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

// ����������
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// ���������� ���� CMLCMDlg



CMLCMDlg::CMLCMDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMLCMDlg::IDD, pParent)
	, mSaveSett(_T(""))
	, mLoadSett(_T(""))
	, mComboPcp(_T(""))
	, mComboDeck(_T(""))
	, mComboDat(_T(""))
	, mEditPcp(_T(""))
	, mEditDeck(_T(""))
	, mEditDat(_T(""))
	, mCalFrom(COleDateTime::GetCurrentTime())
	, mCalTo(COleDateTime::GetCurrentTime())
	, mValFrom(COleDateTime::GetCurrentTime())
	, mValTo(COleDateTime::GetCurrentTime())
	, mModFrom(COleDateTime::GetCurrentTime())
	, mModTo(COleDateTime::GetCurrentTime())
	, mInFormat(0)
	, mOutFormat(0)
	, mOutFile(_T(""))
	, mSaveMlcm(_T(""))
	, mLoadMlcm(_T(""))
	, mComboModAndVal(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	mIsDeck = 0;
	mIsOutfile = 0;
	mIsPcp = 0;
	mInfo = new InfoDlg();
	mH = new Hydrograph();
	try {
		loadConfig("default.config");
	}
	catch (const int &a) {
		printError(a);
	}
}

CMLCMDlg::~CMLCMDlg()
{
	delete mInfo;
}

bool CMLCMDlg::doFileName(bool loadFile, CString &edit_str, char **charName)
{
	UpdateData(1);
	if (edit_str == "") {
		CFileDialog fileDlg(loadFile);
		if (fileDlg.DoModal() == IDOK) {
			edit_str = fileDlg.m_ofn.lpstrFile;
			UpdateData(0);
		}
		else
			return 0;
	}
	int size = edit_str.GetLength();
	char *name = new char[size];
	for (int i = 0; i < size; i++) {
		name[i] = edit_str[i];
	}
	name[size] = 0;
	*charName = name;
	return 1;
}

void CMLCMDlg::loadConfig(char *configName)
{
	ifstream confin(configName, ios::out);
	confin >> mCalType >> mFitnType >> mValType;
	mH->setCalibrationType(mCalType);
	mH->setFitnessType(mFitnType, mValType);
	int inFormat, outFormat;
	confin >> inFormat >> outFormat;
	mH->setInOutFormat(inFormat, outFormat);
	double c1, c2;
	confin >> c1 >> c2;
	mH->setCLim(c1, c2);
	int maxA[11], maxZ[10];
	confin >> maxA[0];
	int i;
	for (i = 0; i < 10; i++) {
		confin >> maxA[i + 1] >> maxZ[i];
	}
	mH->setMaxAandZ(maxA, maxZ);
	int bruteforceSteps, bruteforceIter;
	confin >> bruteforceSteps >> bruteforceIter;
	mH->setBruteforceParams(bruteforceSteps, bruteforceIter);
	double NMStop;
	int NMLim;
	confin >> NMStop >> NMLim;
	mH->setNMStopAndLim(NMStop, NMLim);
	double NMKoeff1, NMKoeff2;
	confin >> NMKoeff1 >> NMKoeff2;
	mH->setNMKoeffs(NMKoeff1, NMKoeff2);
	int complexNMKoeffsNum;
	confin >> complexNMKoeffsNum;
	vector<double> NMKoeffs;
	double tmp;
	for (i = 0; i < complexNMKoeffsNum * 2; i++) {
		confin >> tmp;
		NMKoeffs.push_back(tmp);
	}
	mH->setNMKoeffs(NMKoeffs);
	double slsStep;
	int slsLim, slsCalType;
	confin >> slsStep >> slsLim >> slsCalType;
	mH->setSlsParam(slsStep, slsLim, slsCalType);
	double minGrowth;
	if (confin.ios::eof()) {
		confin.close();
		throw(0);
	}
	confin >> minGrowth;
	mH->setMinGrowth(minGrowth);
	confin.close();
}

void CMLCMDlg::saveConfig(char *configName)
{
	ofstream confout(configName, ios::out);
	int calType, defFitnType, valType, inFormat, outFormat;
	mH->getCalAndFitnessTypes(calType, defFitnType, valType);
	confout << calType << " " << defFitnType << " " << valType << endl;
	mH->getInAndOutFormat(inFormat, outFormat);
	confout << inFormat << " " << outFormat << endl;
	int maxA[11], maxZ[10];
	mH->getMaxAandZ(maxA, maxZ);
	double c1, c2;
	mH->getCLim(c1, c2);
	confout << c1 << " " << c2 << endl;
	confout << maxA[0] << endl;
	int i;
	for (i = 0; i < 10; i++)
	{
		confout << maxA[i + 1] << " " << maxZ[i] << endl;
	}
	int bruteforceSteps, bruteforceIter;
	mH->getBruteforceParams(bruteforceSteps, bruteforceIter);
	confout << bruteforceSteps << " " << bruteforceIter << endl;
	double NMStop;
	int NMLim;
	mH->getNMParams(NMStop, NMLim);
	confout << NMStop << " " << NMLim << endl;
	vector<double> koeffs;
	int koeffsNum = mH->getNMKoeffs(koeffs);
	confout << koeffs[0] << " " << koeffs[1] << endl << koeffsNum << endl;
	for (i = 2; i < (koeffsNum + 1) * 2; i += 2) {
		confout << koeffs[i] << " " << koeffs[i + 1] << endl;
	}
	double slsStep;
	int slsLim, slsCalType;
	mH->getSlsParams(slsStep, slsLim, slsCalType);
	confout << slsStep << " " << slsLim << " " << slsCalType << endl;
	confout << mH->getMinGrowth();
	confout.close();
}

void CMLCMDlg::printError(const int &a)
{
	CString text;
	switch (a) {
	case 0:
		text = L"���-�� ����� �� ��� :(";
		break;
	case 1:
		text = L"������ ����� ������� ���������� ������ �������";
		break;
	default:
		text = L"���-�� ����� �� ��� :(";
		break;
	}
	mInfo->print(text);
}

void CMLCMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, Edit_SaveSett, mSaveSett);
	DDX_Text(pDX, Edit_LoadSett, mLoadSett);
	DDX_CBString(pDX, Combo_pcp, mComboPcp);
	DDX_CBString(pDX, Combo_deck, mComboDeck);
	DDX_CBString(pDX, Combo_dat, mComboDat);
	DDX_Text(pDX, Edit_pcp, mEditPcp);
	DDX_Text(pDX, Edit_deck, mEditDeck);
	DDX_Text(pDX, Edit_dat, mEditDat);
	DDX_DateTimeCtrl(pDX, Date_CalFrom, mCalFrom);
	DDX_DateTimeCtrl(pDX, Date_CalTo, mCalTo);
	DDX_DateTimeCtrl(pDX, Date_ValFrom, mValFrom);
	DDX_DateTimeCtrl(pDX, Date_ValTo, mValTo);
	DDX_DateTimeCtrl(pDX, Date_ModFrom, mModFrom);
	DDX_DateTimeCtrl(pDX, Date_ModTo, mModTo);
	DDX_CBIndex(pDX, Combo_InFormat, mInFormat);
	DDV_MinMaxInt(pDX, mInFormat, 0, 2);
	DDX_CBIndex(pDX, Combo_OutFormat, mOutFormat);
	DDV_MinMaxInt(pDX, mOutFormat, 0, 3);
	DDX_Text(pDX, Edit_OutFile, mOutFile);
	DDX_Text(pDX, Edit_SaveMlcm, mSaveMlcm);
	DDX_Text(pDX, Edit_LoadMlcm, mLoadMlcm);
	DDX_CBIndex(pDX, Combo_ModAndVal, mComboModAndVal);
}

BEGIN_MESSAGE_MAP(CMLCMDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(Button_Tune, &CMLCMDlg::OnBnClickedTune)
	ON_BN_CLICKED(Button_MainInfo, &CMLCMDlg::OnBnClickedMaininfo)
	ON_BN_CLICKED(Button_ModelInfo, &CMLCMDlg::OnBnClickedModelinfo)
	ON_BN_CLICKED(Button_ConfigInfo, &CMLCMDlg::OnBnClickedConfiginfo)
	ON_BN_CLICKED(Button_MlcmInfo, &CMLCMDlg::OnBnClickedMlcminfo)
	ON_BN_CLICKED(Button_DeckInfo, &CMLCMDlg::OnBnClickedDeckinfo)
	ON_BN_CLICKED(Button_InOutFormatInfo, &CMLCMDlg::OnBnClickedInoutformatinfo)
	ON_BN_CLICKED(Button_DimInfo, &CMLCMDlg::OnBnClickedDiminfo)
	ON_BN_CLICKED(Button_LoadSett, &CMLCMDlg::OnBnClickedLoadsett)
	ON_BN_CLICKED(Button_SaveSett, &CMLCMDlg::OnBnClickedSavesett)
	ON_BN_CLICKED(Button_SaveMlcm, &CMLCMDlg::OnBnClickedSavemlcm)
	ON_BN_CLICKED(Button_LoadMlcm, &CMLCMDlg::OnBnClickedLoadmlcm)
	ON_BN_CLICKED(Button_setInOutFormat, &CMLCMDlg::OnBnClickedsetinoutformat)
	ON_BN_CLICKED(Button_outputFile, &CMLCMDlg::OnBnClickedoutputfile)
	ON_BN_CLICKED(Button_deck, &CMLCMDlg::OnBnClickeddeck)
	ON_BN_CLICKED(Button_pcp, &CMLCMDlg::OnBnClickedpcp)
	ON_BN_CLICKED(Button_dat, &CMLCMDlg::OnBnClickeddat)
	ON_BN_CLICKED(Button_Calibrate, &CMLCMDlg::OnBnClickedCalibrate)
	ON_BN_CLICKED(Button_Model, &CMLCMDlg::OnBnClickedModel)
	ON_BN_CLICKED(Button_Validate, &CMLCMDlg::OnBnClickedValidate)
	ON_BN_CLICKED(Button_ModAndVal, &CMLCMDlg::OnBnClickedModandval)
	ON_CBN_SELCHANGE(Combo_InFormat, &CMLCMDlg::OnCbnSelchangeInformat)
	ON_CBN_SELCHANGE(Combo_OutFormat, &CMLCMDlg::OnCbnSelchangeOutformat)
END_MESSAGE_MAP()


// ����������� ��������� CMLCMDlg

BOOL CMLCMDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���������� ������ "� ���������..." � ��������� ����.

	// IDM_ABOUTBOX ������ ���� � �������� ��������� �������.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ������ ������ ��� ����� ����������� ����. ����� ������ ��� �������������,
	//  ���� ������� ���� ���������� �� �������� ����������
	SetIcon(m_hIcon, TRUE);			// ������� ������
	SetIcon(m_hIcon, FALSE);		// ������ ������

	// TODO: �������� �������������� �������������

	return TRUE;  // ������� �������� TRUE, ���� ����� �� ������� �������� ����������
}

void CMLCMDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��� ���������� ������ ����������� � ���������� ���� ����� ��������������� ����������� ���� �����,
//  ����� ���������� ������. ��� ���������� MFC, ������������ ������ ���������� ��� �������������,
//  ��� ������������� ����������� ������� ��������.

void CMLCMDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �������� ���������� ��� ���������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ������������ ������ �� ������ ����������� ��������������
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ��������� ������
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ������� �������� ��� ������� ��� ��������� ����������� ������� ��� �����������
//  ���������� ����.
HCURSOR CMLCMDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMLCMDlg::OnBnClickedTune()
{
	TuneDlg *T = new TuneDlg();
	T->setHydrograph(mH);
	T->setCalAndVal(mCalType, mFitnType, mValType);
	T->DoModal();
	T->getCalAndVal(mCalType, mFitnType, mValType);
	delete T;
}


void CMLCMDlg::OnBnClickedMaininfo()
{
	CString text = L"����� ����������\n\n";
	text += L"������� ��������, ��� ������� ������ �� ����� ������� ��������� ��������, ��� ������� ��� ���������� ��������. ";
	text += L"������ ������ ��� �������� � �������� .txt\n\n";
	text += L"������ ��������� ��� ��������� ��������� ����� ������ ������ � ������, ���� ���� ����� ����� �� ������ �����. ";
	text += L"����� ����������� ���� (��� ����������� � ����) � ��������� �� ���� �����, ���� ���� �� ������ - �� ��� �� ����������, ";
	text += L"��� � .exe ����. ���� ���� ������������� ������� ���� ������ ������ ������ - ������� ��� �� ���� ����� ����� �� ������.";
	mInfo->print(text);
}


void CMLCMDlg::OnBnClickedModelinfo()
{
	CString text = L"��������������� ������ ��������� � �������� ���� � ��� �� ������������, ��� � � �������� ������";
	mInfo->print(text);
}


void CMLCMDlg::OnBnClickedConfiginfo()
{
	CString text = L"���������� �� .config �����\n\n(���� ������� ������ ����������� ������������ ������ - ���������� �����������)";
	text += L"������ �� ��������� �������� � default.config\n";
	text += L"����� ������� ��� �������� - ������������ ���������� (1 - ������-���, 2 - ����������� ������-���, 3 - SLS), ";
	text += L"������������ ������� ������� � ������� ��� ��������� (� ��� ����, � ��� ������� 1 - MSOF, 4 - �������������������� ������)\n";
	text += L"������� ������� � �������� ������ ����� ������ (������� ��� �������� ������ ���������� � ���������� ��� ������� � �������� ������)\n";
	text += L"����������� �� �������� ������������� ���������� �����, ����� ������, ������ � ����� (0-1)\n";
	text += L"������ �� ������������ �� ���� - ������ ������ ����������� �� �������� �������������� �������� (� ��/�), ";
	text += L"����� ������ ����� �������� ����� ���� (��/�) � ������ ���� (��) ����� ������\n";
	text += L"�������� ��� �������� - ���������� ����� � ����� ��������, ���������� �������� ��� ��������\n";
	text += L"������ ��� �������-���� - ������� �������� � ����� �� ���������� ��������\n";
	text += L"������������ ��� ���������� ���������\n";
	text += L"���������� ��������� ���������� � ����������� �������-���� � ����� M ����� � �������������� ��������� ����������.\n";
	text += L"������ ��� SLS - ���, ����� �� ���������� ��������, ���������� �������� ���� - 1 - ��������-�����, ";
	text += L"2 - ����������� ��������-����� � 3 - �������\n";
	text += L"��� ���� ���������� - ����������� ������� ��� �������� �� �������� ���� � ������ ";
	text += L"(���� ������� ������ ����������� ������������ ������ - ���������� �����������)";
	mInfo->print(text);
}


void CMLCMDlg::OnBnClickedMlcminfo()
{
	CString text = L"���������� �� ����� � ������������ ����������� ������ (*.param)\n\n";
	text += L"���������� �����, N (0-10)\n�������� �������������� �������� (��/�)\n";
	text += L"�������� ������������� ���������� ����� (0-1)\n����� �����-������������� (1-20)\n";
	text += L"������� �����-������������� (1-5)\n�������� ������ ���������� ���������� (0-24)\n";
	text += L"N ����� ���������� �������: �������� ����� ���� (��/�) � ������ ���� (��) ����� ������";
	mInfo->print(text);
}


void CMLCMDlg::OnBnClickedDeckinfo()
{
	CString text = L"���������� �� ����� � ������� � ��������� (*.deck)\n\n";
	text += L"  ����� ������ (� ��)\n  ������������ (���) �� ������� ������ ������ ";
	text += L"(� �����, ������ ����� 24, �� ���� 7 ����� �� ��������, � 6 ��� 0.5 ��������)\n";
	text += L"  ������������ ����� ������� ���������� ����������\n  12 ����� �� �������� ���������� ��������� �� ����� ";
	text += L"(����������� ����������� � ���������� ������ �� ����� �����)\n\n";
	text += L"��������: ��� �������� ������ ����� deck ������������� ������������� ���� � ��������";
	mInfo->print(text);
}


void CMLCMDlg::OnBnClickedInoutformatinfo()
{
	CString text = L"���������� �� �������� ������� � �������� ������\n\n";
	text += L"1: ��� - ����� - ���� - ������\n2: ����� - ���� - ������\n3: ������ (������� ��������� 1 ������ 2013)\n4: (������ ��� �������� ������)\n";
	text += L"   ����� - ���� - ������ - �������� ������ - ��������������� ������\n\n";
	text += L"����� � ������� ������ ����-��� ����� - ����� ������, ������ ��� - ���, �� ���� 396 - ���� 1996, � 1201 - ������� 2001\n";
	text += L"����� ���������, ��� �� ������� ������ ������ ��� ����� ���. �� ����������� ��� ������������, �������, ����� �";
	text += L" ������ ������� ��������� ����� ����� ��� � ������";
	mInfo->print(text);
}


void CMLCMDlg::OnBnClickedDiminfo()
{
	CString text = L"��������������� ������ ��������� � �������� ���� � ��� �� ������������, ��� � � �������� ������";
	mInfo->print(text);
}

void CMLCMDlg::OnBnClickedLoadsett()
{
	char *loadSettChar;
	if(doFileName(1, mLoadSett, &loadSettChar))
		try {
			loadConfig(loadSettChar);
		}
		catch (const int &a) {
			printError(a);
		}
}

void CMLCMDlg::OnBnClickedSavesett()
{
	char *saveSettChar;
	if(doFileName(0, mSaveSett, &saveSettChar))
		saveConfig(saveSettChar);
}


void CMLCMDlg::OnBnClickedSavemlcm()
{
	char *saveMlcmChar;
	if (doFileName(0, mSaveMlcm, &saveMlcmChar))
		mH->printMlcm(saveMlcmChar);
}


void CMLCMDlg::OnBnClickedLoadmlcm()
{
	if (!mIsDeck) {
		CString text = L"��������� ������� ���� � ������� � ���������";
		mInfo->print(text);
		return;
	}
	char *loadMlcmChar;
	if (doFileName(1, mLoadMlcm, &loadMlcmChar)) {
		try {
			mH->loadMlcm(loadMlcmChar);
		}
		catch (const int &a) {
			printError(a);
		}
	}
}


void CMLCMDlg::OnBnClickedsetinoutformat()
{
	UpdateData(1);
	mH->setInOutFormat(mInFormat + 1, mOutFormat + 1);
}


void CMLCMDlg::OnBnClickedoutputfile()
{
	char *outputFileChar;
	if (doFileName(0, mOutFile, &outputFileChar)) {
		mIsOutfile = 1;
		mH->setOutFile(outputFileChar);
	}
}

double CMLCMDlg::doComboFormats(const CString &combo)
{
	UpdateData(1);
	if (combo == "���������/���") {
		return 1;
	}
	if (combo == "����/���") {
		return 25.4;
	}
	if (combo == "���/���") {
		return 304.8;
	}
	double value = _wtof(combo);
	if (value == 0)
		return 1;
	else
		return value;
}


void CMLCMDlg::OnBnClickeddeck()
{
	char *deckChar;
	if (doFileName(1, mEditDeck, &deckChar)) {
		mIsDeck = 1;
		double deckFormat = doComboFormats(mComboDeck);
		mH->readDeck(deckFormat, deckChar);
	}
}


void CMLCMDlg::OnBnClickedpcp()
{
	if (!mIsDeck) {
		CString text = L"��������� ������� ���� � ������� � ���������";
		mInfo->print(text);
		return;
	}
	char *pcpChar;
	if (doFileName(1, mEditPcp, &pcpChar)) {
		double pcpFormat = doComboFormats(mComboPcp);
		try {
			mH->readPcp(pcpFormat, pcpChar);
			mIsPcp = 1;
		}
		catch (const int &a) {
			printError(a);
		}
	}
}

void CMLCMDlg::OnBnClickeddat()
{
	if ((!mIsDeck) || (!mIsPcp)) {
		CString text = L"��������� ������� ���� � ������� � ��������� � ���� � ��������";
		mInfo->print(text);
		return;
	}
	char *datChar;
	if (doFileName(1, mEditDat, &datChar)) {
		double datFormat = doComboFormats(mComboDat);
		try {
			mH->readDat(datFormat, datChar);
		}
		catch (const int &a) {
			printError(a);
		}
	}
}

int* CMLCMDlg::doDate(const COleDateTime &date)
{
	UpdateData(1);
	int *dateInt = new int [3];
	dateInt[0] = date.GetDay();
	dateInt[1] = date.GetMonth();
	dateInt[2] = date.GetYear();
	return dateInt;
}

void CMLCMDlg::OnBnClickedCalibrate()
{
	if ((!mIsDeck) || (!mIsPcp)) {
		CString text = L"��������� ������� ���� � ������� � ��������� � ���� � ��������";
		mInfo->print(text);
		return;
	}
	int *calBegin = doDate(mCalFrom);
	int *calEnd = doDate(mCalTo);
	mH->click();
	try {
		double val = mH->calibrate(calBegin, calEnd);
		int countOfClicks = mH->click();
		val = mH->validate(calBegin, calEnd);
		CString valStr;
		valStr.Format(L"%f", val);
		CString text = L"���������� ���������. ��������� ";
		text += valStr;
		CString clicks;
		clicks.Format(L"%i", countOfClicks);
		text += L". ���������� ������� ������: ";
		text += clicks;
		mInfo->print(text);
	}
	catch (const int &a) {
		printError(a);
	}
}


void CMLCMDlg::OnBnClickedValidate()
{
	if ((!mIsDeck) || (!mIsPcp)) {
		CString text = L"��������� ������� ���� � ������� � ��������� � ���� � ��������";
		mInfo->print(text);
		return;
	}
	int *valBegin = doDate(mValFrom);
	int *valEnd = doDate(mValTo);
	try {
		double val = mH->validate(valBegin, valEnd);
		CString valStr;
		valStr.Format(L"%f", val);
		CString text = L"�������� ��������� ";
		text += valStr;
		mInfo->print(text);
	}
	catch (const int &a) {
		printError(a);
	}
}


void CMLCMDlg::OnBnClickedModel()
{
	if ((!mIsDeck) || (!mIsPcp) || (!mIsOutfile)) {
		CString text = L"��������� ������� ���� � ������� � ��������� � ���� � �������� � ������� �������� ����";
		mInfo->print(text);
		return;
	}
	int *modBegin = doDate(mModFrom);
	int *modEnd = doDate(mModTo);
	try {
		mH->printPrediction(modBegin, modEnd);
		CString text = L"������������� ������ �������";
		mInfo->print(text);
	}
	catch (const int &a) {
		printError(a);
	}
}

void CMLCMDlg::OnBnClickedModandval()
{
	if ((!mIsDeck) || (!mIsPcp) || (!mIsOutfile)) {
		CString text = L"��������� ������� ���� � ������� � ��������� � ���� � �������� � ������� �������� ����";
		mInfo->print(text);
		return;
	}
	int *modBegin = doDate(mModFrom);
	int *modEnd = doDate(mModTo);
	int valType;
	switch (mComboModAndVal) {
	case 0: valType = -1; break;
	case 1: valType = 1; break;
	case 2: valType = 4; break;
	}
	try {
		double val = mH->printPredAndValid(modBegin, modEnd, valType);
		CString text = L"������������� ������ �������. ";
		CString valStr;
		valStr.Format(L"%f", val);
		text += L"�������� ��������� ";
		text += valStr;
		mInfo->print(text);
	}
	catch (const int &a) {
		printError(a);
	}
}


void CMLCMDlg::OnCbnSelchangeInformat()
{
	UpdateData(1);
	mH->setInOutFormat(mInFormat + 1, mOutFormat + 1);
}


void CMLCMDlg::OnCbnSelchangeOutformat()
{
	UpdateData(1);
	mH->setInOutFormat(mInFormat + 1, mOutFormat + 1);
}
