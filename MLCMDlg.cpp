
// MLCMDlg.cpp : файл реализации
//

#include "stdafx.h"
#include "MLCM-PROJECT.h"
#include "MLCMDlg.h"
#include "afxdialogex.h"
#include <fstream>
#include <vector>

using namespace std;

// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
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


// диалоговое окно CMLCMDlg



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
		text = L"Что-то пошло не так :(";
		break;
	case 1:
		text = L"Скорее всего границы калибровки заданы неверно";
		break;
	default:
		text = L"Что-то пошло не так :(";
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


// обработчики сообщений CMLCMDlg

BOOL CMLCMDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
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

	// Задает значок для этого диалогового окна. Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
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

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок. Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CMLCMDlg::OnPaint()
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
	CString text = L"Общая информация\n\n";
	text += L"Следует заметить, что форматы файлов не несут никакой смысловой нагрузки, это сделано для уменьшения путаницы. ";
	text += L"Можете делать все например с форматом .txt\n\n";
	text += L"Кнопки загрузить или сохранить открывают обзор файлов только в случае, если поле ввода слева от кнопки пусто. ";
	text += L"Иначе открывается файл (или сохраняется в файл) с названием из поля ввода, если путь не указан - из той же директории, ";
	text += L"что и .exe файл. Если есть необходимость открыть окно обзора файлов заново - удалите все из поля вводя слева от кнопки.";
	mInfo->print(text);
}


void CMLCMDlg::OnBnClickedModelinfo()
{
	CString text = L"Смоделированные данные выведутся в выходной файл с той же размерностью, что и у реальных данных";
	mInfo->print(text);
}


void CMLCMDlg::OnBnClickedConfiginfo()
{
	CString text = L"Информация по .config файлу\n\n(если функция ошибки уменьшается недостаточно быстро - калибровка завершается)";
	text += L"Данные по умолчанию хранятся в default.config\n";
	text += L"Через пробелы три значения - используемая калибровка (1 - Нелдер-Мид, 2 - расширенный Нелдер-Мид, 3 - SLS), ";
	text += L"используемая целевая функция и функция для валидации (и для того, и для другого 1 - MSOF, 4 - среднеквадратическая ошибка)\n";
	text += L"Форматы входных и выходных файлов через пробел (справку для форматов можете посмотреть в информации для входных и выходных файлов)\n";
	text += L"Ограничение на скорость трансформации паводочной волны, через пробел, сверху и снизу (0-1)\n";
	text += L"Данные об ограничениях на слои - первая строка ограничение на скорость поверхностного стекания (в мм/ч), ";
	text += L"затем десять строк скорость через слой (мм/ч) и ширина слоя (мм) через пробел\n";
	text += L"Значения для перебора - количество шагов в одном переборе, количество итераций для перебора\n";
	text += L"Данные для Нелдера-Мида - Условие останова и лимит на количество итераций\n";
	text += L"Коэффициенты для начального симплекса\n";
	text += L"Количество начальных симплексов в расширенном Нелдере-Миде и затем M строк с коэффициентами начальных симплексов.\n";
	text += L"Данные для SLS - Шаг, лимит на количество проходов, калибровка нулевого слоя - 1 - Нелдером-Мидом, ";
	text += L"2 - расширенным Нелдером-Мидом и 3 - перебор\n";
	text += L"Для всех калибровок - минимальный прирост при переходе от прошлого слоя к новому ";
	text += L"(если функция ошибки уменьшается недостаточно быстро - калибровка завершается)";
	mInfo->print(text);
}


void CMLCMDlg::OnBnClickedMlcminfo()
{
	CString text = L"Информация по файлу с калибруемыми параметрами модели (*.param)\n\n";
	text += L"Количество слоев, N (0-10)\nСкорость поверхностного стекания (мм/ч)\n";
	text += L"Скорость трансформации паводочной волны (0-1)\nФорма гамма-распределения (1-20)\n";
	text += L"Масштаб гамма-распределения (1-5)\nПараметр сдвига единичного гидрографа (0-24)\n";
	text += L"N строк следующего формата: скорость через слой (мм/ч) и ширина слоя (мм) через пробел";
	mInfo->print(text);
}


void CMLCMDlg::OnBnClickedDeckinfo()
{
	CString text = L"Информация по файлу с данными о водосборе (*.deck)\n\n";
	text += L"  Длина склона (в км)\n  Дискретность (шаг) по времени выхода модели ";
	text += L"(в часах, нацело делит 24, то есть 7 часов не подойдет, а 6 или 0.5 подойдет)\n";
	text += L"  Максимальное число ординат единичного гидрографа\n  12 строк со средними значениями испарений за месяц ";
	text += L"(размерность указывается в комбобоксе справа от имени файла)\n\n";
	text += L"Внимание: при открытии нового файла deck рекомендуется перезагрузить файл с осадками";
	mInfo->print(text);
}


void CMLCMDlg::OnBnClickedInoutformatinfo()
{
	CString text = L"Информация по форматам входных и выходных данных\n\n";
	text += L"1: Код - месяц - день - данные\n2: Месяц - день - данные\n3: Данные (началом считается 1 января 2013)\n4: (Только для выходных данных)\n";
	text += L"   Месяц - день - осадки - реальные данные - смоделированные данные\n\n";
	text += L"Месяц в формате первая одна-две цифры - номер месяца, вторые две - год, то есть 396 - март 1996, а 1201 - декабрь 2001\n";
	text += L"Может оказаться, что во входных данных вместо дня стоит час. Не обязательно все переделывать, главное, чтобы в";
	text += L" первой строчке правильно стоял номер дня в месяце";
	mInfo->print(text);
}


void CMLCMDlg::OnBnClickedDiminfo()
{
	CString text = L"Смоделированные данные выведутся в выходной файл с той же размерностью, что и у реальных данных";
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
		CString text = L"Загрузите сначала файл с данными о водосборе";
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
	if (combo == "миллиметр/час") {
		return 1;
	}
	if (combo == "дюйм/час") {
		return 25.4;
	}
	if (combo == "фут/час") {
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
		CString text = L"Загрузите сначала файл с данными о водосборе";
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
		CString text = L"Загрузите сначала файл с данными о водосборе и файл с осадками";
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
		CString text = L"Загрузите сначала файл с данными о водосборе и файл с осадками";
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
		CString text = L"Калибрация выполнена. Валидация ";
		text += valStr;
		CString clicks;
		clicks.Format(L"%i", countOfClicks);
		text += L". Количество вызовов модели: ";
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
		CString text = L"Загрузите сначала файл с данными о водосборе и файл с осадками";
		mInfo->print(text);
		return;
	}
	int *valBegin = doDate(mValFrom);
	int *valEnd = doDate(mValTo);
	try {
		double val = mH->validate(valBegin, valEnd);
		CString valStr;
		valStr.Format(L"%f", val);
		CString text = L"Значение валидации ";
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
		CString text = L"Загрузите сначала файл с данными о водосборе и файл с осадками и задайте выходной файл";
		mInfo->print(text);
		return;
	}
	int *modBegin = doDate(mModFrom);
	int *modEnd = doDate(mModTo);
	try {
		mH->printPrediction(modBegin, modEnd);
		CString text = L"Моделирование прошло успешно";
		mInfo->print(text);
	}
	catch (const int &a) {
		printError(a);
	}
}

void CMLCMDlg::OnBnClickedModandval()
{
	if ((!mIsDeck) || (!mIsPcp) || (!mIsOutfile)) {
		CString text = L"Загрузите сначала файл с данными о водосборе и файл с осадками и задайте выходной файл";
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
		CString text = L"Моделирование прошло успешно. ";
		CString valStr;
		valStr.Format(L"%f", val);
		text += L"Значение валидации ";
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
