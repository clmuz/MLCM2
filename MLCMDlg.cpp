#include "stdafx.h"
#include "MLCM-PROJECT.h"
#include "TuneDlg.h"
#include "MLCMDlg.h"
#include "afxdialogex.h"
#include <fstream>
#include <vector>

using namespace std;

// ���������� ���� CMLCMDlg



CMLCMDlg::CMLCMDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMLCMDlg::IDD, pParent)
	, mSaveSett(_T(""))
	, mLoadSett(_T(""))
	, mEditPcp(_T(""))
	, mEditDeck(_T(""))
	, mEditDat(_T(""))
	, mCalFrom(COleDateTime::GetCurrentTime())
	, mCalTo(COleDateTime::GetCurrentTime())
	, mValFrom(COleDateTime::GetCurrentTime())
	, mValTo(COleDateTime::GetCurrentTime())
	, mModFrom(COleDateTime::GetCurrentTime())
	, mModTo(COleDateTime::GetCurrentTime())
	, mOutFormat(0)
	, mOutFile(_T(""))
	, mSaveMlcm(_T(""))
	, mLoadMlcm(_T(""))
	, mComboModAndVal(0)
	, mModValFrom(COleDateTime::GetCurrentTime())
	, mModValTo(COleDateTime::GetCurrentTime())
	, mWarmingDays(0)
	, mComboFormat(0)
	, mEtFormat(1)
	, mPcpFormat(1)
	, mQFormat(1)
	, mFbasinFormat(1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	mIsDeck = 0;
	mIsOutfile = 0;
	mIsPcp = 0;
	mInfo = new InfoDlg();
	mH = new Hydrograph();
	try {
		loadConfig(_T("default.config"));
	}
	catch (const int &a) {
		printError(a);
	}
}

CMLCMDlg::~CMLCMDlg()
{
	delete mInfo;
}

bool CMLCMDlg::doFileName(bool loadFile, CString &edit_str, const CString &format)
{
	UpdateData(1);
	if (edit_str != L"")
		return 1;
	CString filters = L"txt (*.txt)|*.txt|Deck file (*.deck)|*.deck|Precipitation file (*.pcp)|*.pcp|";
	filters += L"Real data (*.dat)|*.dat|Settings file (*.config)|*.config|MLCM parameters (*.param)|All Files (*.*)|*.*||";
	CString format1 = L"*" + format;
	CFileDialog fileDlg(loadFile, format, format1, 6UL, filters);
	if (fileDlg.DoModal() == IDOK) {
		edit_str = fileDlg.GetPathName();
		UpdateData(0);
	}
	else
		return 0;
	return 1;
}

void CMLCMDlg::loadConfig(const wchar_t *configName)
{
	ifstream confin(configName, ios::out);
	if (!confin)
		throw(3);
	int calType, fitnType, valType;
	confin >> calType >> fitnType >> valType;
	mH->setCalibrationType(doCalibrationType(calType));
	mH->setFitnessType(doFitnessType(fitnType), doFitnessType(valType));
	confin >> mComboFormat;
	OnCbnSelchangeFormat();
	confin >> mOutFormat;
	mH->setOutFormat(mOutFormat--);
	confin >> mWarmingDays;
	mH->setWarmingDays(mWarmingDays);
	double c1, c2;
	confin >> c1 >> c2;
	mH->setCLim(c1, c2);
	double maxA[11], maxZ[10];
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
	mH->setSlsParam(slsStep, slsLim, doCalibrationType(slsCalType));
	double minGrowth;
	if (confin.ios::eof()) {
		confin.close();
		throw(0);
	}
	confin >> minGrowth;
	mH->setMinGrowth(minGrowth);
	confin.close();
}

void CMLCMDlg::saveConfig(const wchar_t *configName)
{
	ofstream confout(configName, ios::out);
	if (!confout)
		throw(3);
	calibrationType calType;
	fitnessType defFitnType, valType;
	mH->getCalAndFitnessTypes(calType, defFitnType, valType);
	confout << (int)calType << " " << (int)defFitnType << " " << (int)valType << endl;
	confout << mComboFormat << endl;
	confout << mH->getOutFormat() << endl;
	confout << mH->getWarmingDays() << endl;
	double maxA[11], maxZ[10];
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
	int slsLim;
	calibrationType slsCalType;
	mH->getSlsParams(slsStep, slsLim, slsCalType);
	confout << slsStep << " " << slsLim << " " << (int)slsCalType << endl;
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
	case 2:
		text = L"������������ ������ �������� �����";
		break;
	case 3:
		text = L"������ �������� �����";
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
	DDX_Text(pDX, Edit_pcp, mEditPcp);
	DDX_Text(pDX, Edit_deck, mEditDeck);
	DDX_Text(pDX, Edit_dat, mEditDat);
	DDX_DateTimeCtrl(pDX, Date_CalFrom, mCalFrom);
	DDX_DateTimeCtrl(pDX, Date_CalTo, mCalTo);
	DDX_DateTimeCtrl(pDX, Date_ValFrom, mValFrom);
	DDX_DateTimeCtrl(pDX, Date_ValTo, mValTo);
	DDX_DateTimeCtrl(pDX, Date_ModFrom, mModFrom);
	DDX_DateTimeCtrl(pDX, Date_ModTo, mModTo);
	DDX_CBIndex(pDX, Combo_OutFormat, mOutFormat);
	DDX_Text(pDX, Edit_OutFile, mOutFile);
	DDX_Text(pDX, Edit_SaveMlcm, mSaveMlcm);
	DDX_Text(pDX, Edit_LoadMlcm, mLoadMlcm);
	DDX_CBIndex(pDX, Combo_ModAndVal, mComboModAndVal);
	DDX_DateTimeCtrl(pDX, Date_ModValFrom, mModValFrom);
	DDX_DateTimeCtrl(pDX, Date_ModValTo, mModValTo);
	DDX_Text(pDX, Edit_WarmingDays, mWarmingDays);
	DDX_CBIndex(pDX, Combo_Format, mComboFormat);
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
	ON_BN_CLICKED(Button_outputFile, &CMLCMDlg::OnBnClickedoutputfile)
	ON_BN_CLICKED(Button_deck, &CMLCMDlg::OnBnClickeddeck)
	ON_BN_CLICKED(Button_pcp, &CMLCMDlg::OnBnClickedpcp)
	ON_BN_CLICKED(Button_dat, &CMLCMDlg::OnBnClickeddat)
	ON_BN_CLICKED(Button_Calibrate, &CMLCMDlg::OnBnClickedCalibrate)
	ON_BN_CLICKED(Button_Model, &CMLCMDlg::OnBnClickedModel)
	ON_BN_CLICKED(Button_Validate, &CMLCMDlg::OnBnClickedValidate)
	ON_BN_CLICKED(Button_ModAndVal, &CMLCMDlg::OnBnClickedModandval)
	ON_CBN_SELCHANGE(Combo_OutFormat, &CMLCMDlg::OnCbnSelchangeOutformat)
	ON_EN_CHANGE(Edit_WarmingDays, &CMLCMDlg::OnEnChangeWarmingdays)
	ON_BN_CLICKED(Button_ClearSaveSett, &CMLCMDlg::OnBnClickedClearsavesett)
	ON_BN_CLICKED(Button_ClearLoadSett, &CMLCMDlg::OnBnClickedClearloadsett)
	ON_BN_CLICKED(Button_ClearDeck, &CMLCMDlg::OnBnClickedCleardeck)
	ON_BN_CLICKED(Button_ClearPcp, &CMLCMDlg::OnBnClickedClearpcp)
	ON_BN_CLICKED(Button_ClearDat, &CMLCMDlg::OnBnClickedCleardat)
	ON_BN_CLICKED(Button_ClearOut, &CMLCMDlg::OnBnClickedClearout)
	ON_BN_CLICKED(Button_ClearSaveMlcm, &CMLCMDlg::OnBnClickedClearsavemlcm)
	ON_BN_CLICKED(Button_ClearLoadMlcm, &CMLCMDlg::OnBnClickedClearloadmlcm)
	ON_CBN_SELCHANGE(Combo_Format, &CMLCMDlg::OnCbnSelchangeFormat)
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
	CDialogEx::OnSysCommand(nID, lParam);
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
	T->DoModal();
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
	text += L"����� ������� ��� �������� - ������������ ���������� (0 - ������-���, 1 - ����������� ������-���, 2 - SLS, 3 - �������), ";
	text += L"������������ ������� ������� � ������� ��� ��������� (� ��� ����, � ��� ������� 0 - MSOF, 1 - �������������������� ������)\n";
	text += L"����������� ������� ������, 0 - RUS, 1 - US\n";
	text += L"������ �������� ������ (������� ��� �������� ������ ���������� � ���������� ��� �������� ������)\n";
	text += L"���������� ���� �������� ������\n";
	text += L"����������� �� �������� ������������� ���������� �����, ����� ������, ������ � ����� (0-1)\n";
	text += L"������ �� ������������ �� ���� - ������ ������ ����������� �� �������� �������������� �������� (� �3/�), ";
	text += L"����� ������ ����� �������� ����� ���� (�3/�) � ������ ���� (�3) ����� ������\n";
	text += L"�������� ��� �������� - ���������� ����� � ����� ��������, ���������� �������� ��� ��������\n";
	text += L"������ ��� �������-���� - ������� �������� � ����� �� ���������� ��������\n";
	text += L"������������ ��� ���������� ���������\n";
	text += L"���������� ��������� ���������� � ����������� �������-���� � ����� M ����� � �������������� ��������� ����������.\n";
	text += L"������ ��� SLS - ���, ����� �� ���������� ��������, ��� ��������������� ���������� - 0 - ��������-�����, ";
	text += L"1 - ����������� ��������-����� � 3 - �������\n";
	text += L"��� ���� ���������� - ����������� ������� ��� �������� �� �������� ���� � ������ ";
	text += L"(���� ������� ������ ����������� ������������ ������ - ���������� �����������)";
	mInfo->print(text);
}


void CMLCMDlg::OnBnClickedMlcminfo()
{
	CString text = L"���������� �� ����� � ������������ ����������� ������ (*.param)\n\n";
	text += L"���������� �����, N (0-10)\n�������� �������������� �������� (�3/�)\n";
	text += L"�������� ������������� ���������� ����� (0-1)\n����� �����-������������� (1-20)\n";
	text += L"������� �����-������������� (1-5)\n�������� ������ ���������� ���������� (0-24)\n";
	text += L"N ����� ���������� �������: �������� ����� ���� (�3/�) � ������ ���� (�3) ����� ������";
	mInfo->print(text);
}


void CMLCMDlg::OnBnClickedDeckinfo()
{
	CString text = L"���������� �� ����� � ������� � ��������� (*.deck)\n\n";
	text += L"����� �������� ������� ���������� ��� �������:\n\n";
	text += L"  ������� ���������\n  ������������ ������ -";
	text += L"��������� ��������� � ����\n";
	text += L"  ������������ ����� ������� ���������� ����������\n";
	text += L"����� ���� ����� �����������, ����� ��������� ��������� ��������\n";
	text += L"  C������������� �������� �������� ���������, ��� � ������������ ����� ������\n";
	text += L"  ��������� ��������� ���� ��������� ����� ���� � ����� ����� ������\n";
	text += L"  ��������� ��������� ���� ����� ����� ���� � ����� ����� ������\n";
	text += L"  (��������������, ��� ���� �������� �� ������ ����, � ������ �����)\n\n";
	text += L"��������: ��� �������� ������ ����� deck ������������� ������������� ���� � ��������";
	mInfo->print(text);
}


void CMLCMDlg::OnBnClickedInoutformatinfo()
{
	CString text = L"���������� �� �������� ������� � �������� ������\n\n";
	text += L"1: ��� - ����� - ���� - ������\n2: ����� - ���� - ������\n3: ������ (������� ��������� 1 ������ 2013)\n";
	text += L"4 (��� ������): (������ ��� �������� ������)\n";
	text += L"   ����� - ���� - ������ ����� ��������� - ����������� ������ - ��������������� ������\n";
	text += L"5 (��� ������ ���� ���������): (������ ��� �������� ������)\n";
	text += L"   ����� - ���� - ������ - ��������� - ����������� ������ - ��������������� ������\n\n";
	text += L"����� � ������� ������ ����-��� ����� - ����� ������, ������ ��� - ���, �� ���� 396 - ���� 1996, � 1201 - ������� 2001\n";
	text += L"����� ���������, ��� �� ������� ������ ������ ��� ����� ���. �� ����������� ��� ������������, �������, ����� �";
	text += L" ������ ������� ��������� ����� ����� ��� � ������";
	mInfo->print(text);
}


void CMLCMDlg::OnBnClickedDiminfo()
{
	CString text = L"��������������� ������ ��������� � �������� ���� � ��� �� ������������, ��� � � �������� ������";
	text += L"RUS :";
	text += L"      ������ : ��/�";
	text += L"      ��������� : ��/�";
	text += L"      ���� : �3/�";
	text += L"      ������� : ��2";
	text += L"US :";
	text += L"      ������ : ����/�";
	text += L"      ��������� : ����/�";
	text += L"      ���� : ���3/�";
	text += L"      ������� : ����2";
	mInfo->print(text);
}

void CMLCMDlg::OnBnClickedLoadsett()
{
	if(doFileName(1, mLoadSett, L".config")) {
		try {
			loadConfig((const wchar_t *) mLoadSett);
		}
		catch (const int &a) {
			printError(a);
		}
	}
}

void CMLCMDlg::OnBnClickedSavesett()
{
	if(doFileName(0, mSaveSett, L".config")) {
		saveConfig((const wchar_t *) mSaveSett);
	}
}


void CMLCMDlg::OnBnClickedSavemlcm()
{
	if (doFileName(0, mSaveMlcm, L".param"))
		mH->printMlcm((const wchar_t *) mSaveMlcm);
}


void CMLCMDlg::OnBnClickedLoadmlcm()
{
	if (!mIsDeck) {
		CString text = L"��������� ������� ���� � ������� � ���������";
		mInfo->print(text);
		return;
	}
	if (doFileName(1, mLoadMlcm, L".param")) {
		try {
			mH->loadMlcm((const wchar_t *) mLoadMlcm);
		}
		catch (const int &a) {
			printError(a);
		}
	}
}

void CMLCMDlg::OnBnClickedoutputfile()
{
	if (doFileName(0, mOutFile, L".dat")) {
		mIsOutfile = 1;
		mH->setOutFile((const wchar_t *) mOutFile);
	}
}


void CMLCMDlg::OnBnClickeddeck()
{
	if (doFileName(1, mEditDeck, L".deck")) {
		mIsDeck = 1;
		mH->readDeck(mFbasinFormat, mEtFormat, (const wchar_t *) mEditDeck);
	}
}


void CMLCMDlg::OnBnClickedpcp()
{
	if (!mIsDeck) {
		CString text = L"��������� ������� ���� � ������� � ���������";
		mInfo->print(text);
		return;
	}
	if (doFileName(1, mEditPcp, L".pcp")) {
		try {
			mH->readPcp(mPcpFormat, (const wchar_t *) mEditPcp);
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
	if (doFileName(1, mEditDat, L".dat")) {
		try {
			mH->readDat(mQFormat, (const wchar_t *) mEditDat);
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
		mH->calibrate(calBegin, calEnd);
		int countOfClicks = mH->click();
		double val = mH->validate(calBegin, calEnd);
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
	delete[] calBegin, calEnd;
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
	delete[] valBegin, valEnd;
}


void CMLCMDlg::OnBnClickedModel()
{
	if ((!mIsDeck) || (!mIsPcp)) {
		CString text = L"��������� ������� ���� � ������� � ��������� � ���� � ��������";
		mInfo->print(text);
		return;
	}
	if (!mIsOutfile) {
		if (doFileName(0, mOutFile, L".dat")) {
			mIsOutfile = 1;
			mH->setOutFile((const wchar_t *) mOutFile);
		}
		else
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
	delete[] modBegin, modEnd;
}

void CMLCMDlg::OnBnClickedModandval()
{
	if ((!mIsDeck) || (!mIsPcp)) {
		CString text = L"��������� ������� ���� � ������� � ��������� � ���� � ��������";
		mInfo->print(text);
		return;
	}
	if (!mIsOutfile) {
		if (doFileName(0, mOutFile, L".dat")) {
			mIsOutfile = 1;
			mH->setOutFile((const wchar_t *) mOutFile);
		}
		else
			return;
	}
	int *modValBegin = doDate(mModValFrom);
	int *modValEnd = doDate(mModValTo);
	try {
		double val = mH->printPredAndValid(modValBegin, modValEnd, doFitnessType(mComboModAndVal));
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
	delete[] modValBegin, modValEnd;
}

void CMLCMDlg::OnCbnSelchangeOutformat()
{
	UpdateData(1);
	mH->setOutFormat(mOutFormat + 1);
}

void CMLCMDlg::OnEnChangeWarmingdays()
{
	UpdateData(1);
	mH->setWarmingDays(mWarmingDays);
}

fitnessType CMLCMDlg::doFitnessType(const int &intFitn) const
{
	switch (intFitn) {
	case 0:
		return FT_MSOF;
	case 1:
		return FT_ASE;
	default:
		return FT_Default;
	}
}

calibrationType CMLCMDlg::doCalibrationType(const int &intCal) const
{
	switch (intCal) {
	case 0:
		return CT_NelderMead;
	case 1:
		return CT_ComplexNelderMead;
	case 2:
		return CT_SLS;
	case 3:
		return CT_BruteForce;
	default:
		return CT_NULL;
	}
}


void CMLCMDlg::OnBnClickedClearsavesett()
{
	mSaveSett = L"";
	UpdateData(0);
}


void CMLCMDlg::OnBnClickedClearloadsett()
{
	mLoadSett = L"";
	UpdateData(0);
}


void CMLCMDlg::OnBnClickedCleardeck()
{
	mEditDeck = L"";
	UpdateData(0);
}


void CMLCMDlg::OnBnClickedClearpcp()
{
	mEditPcp = L"";
	UpdateData(0);
}


void CMLCMDlg::OnBnClickedCleardat()
{
	mEditDat = L"";
	UpdateData(0);
}


void CMLCMDlg::OnBnClickedClearout()
{
	mOutFile = L"";
	UpdateData(0);
}


void CMLCMDlg::OnBnClickedClearsavemlcm()
{
	mSaveMlcm = L"";
	UpdateData(0);
}


void CMLCMDlg::OnBnClickedClearloadmlcm()
{
	mLoadMlcm = L"";
	UpdateData(0);
}

void CMLCMDlg::OnCbnSelchangeFormat()
{
	switch (mComboFormat) {
	//RUS
	case 0:
		mPcpFormat =  2.7777778e-7;	// ��/���
		mEtFormat = 2.7777778e-7;	// ��/���
		mQFormat = 1;			// �3/�
		mFbasinFormat = 1e6;	// ��2
		break;
	//US
	case 1:
		mPcpFormat = 7.05555556e-6;	// inch/hour
		mEtFormat = 7.05555556e-6;		// inch/hour
		mQFormat = 0.0283168466;	// foot3/s
		mFbasinFormat = 2.58998811e6;	// mi2
		break;
	}
}
