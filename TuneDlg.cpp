#include "stdafx.h"
#include <fstream>
#include "MLCM-PROJECT.h"
#include "TuneDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(TuneDlg, CDialogEx)

TuneDlg::TuneDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(TuneDlg::IDD, pParent)
	, mBFStepsNum(0)
	, mBFItNum(0)
	, mNMLoad(_T(""))
	, mNMStop(0)
	, mNMLim(0)
	, mSlsLim(0)
	, mSlsStep(0)
	, mSls0(0)
	, mMinGrowth(0)
	, mCalType(0)
	, mFitnType(0)
	, mValType(0)
	, mNMKoeff1(0)
	, mNMKoeff2(0)
	, mIsNMLoad(0)
	, mMinC(0)
	, mMaxC(0)
{
	for (int i = 0; i < 10; i++) {
		mMaxA[i] = 0;
		mMaxZ[i] = 0;
	}
	mMaxA[10] = 0;
	mInfo = new InfoDlg();
}

TuneDlg::~TuneDlg()
{
	delete mInfo;
}

void TuneDlg::setHydrograph(Hydrograph *H)
{
	mH = H;
	mH->getCalAndFitnessTypes(mCalType, mFitnType, mValType);
	mH->getMaxAandZ(mMaxA, mMaxZ);
	mH->getCLim(mMinC, mMaxC);
	mH->getBruteforceParams(mBFStepsNum, mBFItNum);
	mMinGrowth = mH->getMinGrowth();
	vector<double> koeffs;
	mH->getNMKoeffs(koeffs);
	mNMKoeff1 = koeffs[0];
	mNMKoeff2 = koeffs[1];
	mH->getNMParams(mNMStop, mNMLim);
	mH->getSlsParams(mSlsStep, mSlsLim, mSls0);
	if (mSls0 > 1)
		mSls0--;
}

void TuneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, Edit_A0, mMaxA[0]);
	DDX_Text(pDX, Edit_A1, mMaxA[1]);
	DDX_Text(pDX, Edit_A2, mMaxA[2]);
	DDX_Text(pDX, Edit_A3, mMaxA[3]);
	DDX_Text(pDX, Edit_A4, mMaxA[4]);
	DDX_Text(pDX, Edit_A5, mMaxA[5]);
	DDX_Text(pDX, Edit_A6, mMaxA[6]);
	DDX_Text(pDX, Edit_A7, mMaxA[7]);
	DDX_Text(pDX, Edit_A8, mMaxA[8]);
	DDX_Text(pDX, Edit_A9, mMaxA[9]);
	DDX_Text(pDX, Edit_A10, mMaxA[10]);
	DDX_Text(pDX, Edit_Z1, mMaxZ[0]);
	DDX_Text(pDX, Edit_Z2, mMaxZ[1]);
	DDX_Text(pDX, Edit_Z3, mMaxZ[2]);
	DDX_Text(pDX, Edit_Z4, mMaxZ[3]);
	DDX_Text(pDX, Edit_Z5, mMaxZ[4]);
	DDX_Text(pDX, Edit_Z6, mMaxZ[5]);
	DDX_Text(pDX, Edit_Z7, mMaxZ[6]);
	DDX_Text(pDX, Edit_Z8, mMaxZ[7]);
	DDX_Text(pDX, Edit_Z9, mMaxZ[8]);
	DDX_Text(pDX, Edit_Z10, mMaxZ[9]);
	DDX_Text(pDX, Edit_BFStepsNum, mBFStepsNum);
	DDV_MinMaxInt(pDX, mBFStepsNum, 0, 200);
	DDX_Text(pDX, Edit_BFItNum, mBFItNum);
	DDV_MinMaxInt(pDX, mBFItNum, 0, 20000);
	DDX_Text(pDX, Edit_NMLoad, mNMLoad);
	DDX_Text(pDX, Edit_NMStop, mNMStop);
	DDX_Text(pDX, Edit_NMLim, mNMLim);
	DDV_MinMaxInt(pDX, mNMLim, 0, 20000);
	DDX_Text(pDX, Edit_SlsLim, mSlsLim);
	DDV_MinMaxInt(pDX, mSlsLim, 0, 20000);
	DDX_Text(pDX, Edit_SlsStep, mSlsStep);
	DDX_CBIndex(pDX, Combo_Sls0, mSls0);
	DDX_Text(pDX, Edit_MinGrowth, mMinGrowth);
	DDX_CBIndex(pDX, Combo_CalibrationType, mCalType);
	DDX_CBIndex(pDX, Combo_FitnessType, mFitnType);
	DDX_CBIndex(pDX, Combo_ValidationType, mValType);
	DDX_Text(pDX, Edit_NMKoeff1, mNMKoeff1);
	DDX_Text(pDX, Edit_NMKoeff2, mNMKoeff2);
	DDX_Text(pDX, Edit_MinC, mMinC);
	DDX_Text(pDX, Edit_MaxC, mMaxC);
}


BEGIN_MESSAGE_MAP(TuneDlg, CDialogEx)
	ON_BN_CLICKED(Button_NMInfo, &TuneDlg::OnBnClickedNminfo)
	ON_BN_CLICKED(Button_NMStopInfo, &TuneDlg::OnBnClickedNmstopinfo)
	ON_BN_CLICKED(Button_LoadNMKoeffs, &TuneDlg::OnBnClickedLoadnmkoeffs)
	ON_BN_CLICKED(IDOK, &TuneDlg::OnBnClickedOk)
END_MESSAGE_MAP()


bool TuneDlg::doFileName(bool loadFile, CString &edit_str, char **charName)
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

// обработчики сообщений TuneDlg


void TuneDlg::OnBnClickedNminfo()
{
	CString text = L"Метод Нелдера-Мида состоит в передвижении симплекса (обобщенного треугольника) по многомерной ";
	text += L"поверхности функции ошибки. Коэффициенты (от 0 до 1) задают начальные координаты симплекса. В двухмерном пространстве";
	text += L" коэффициенты (0.1, 0.2) зададут треугольник A(0.1, 0.1) B(0.1, 0.2) C(0.2, 0.1). Следует понимать, ";
	text += L"что симплекс с коэффициентами (0.1, 0.2) и симплекс с коэффициентами (0.2, 0.1) - различны.\n\n";
	text += L"Подключаемый файл (*.nm) выглядит следующим образом:\n";
	text += L"Количество начальных симплексов, M\nM строк с записанными через пробел коэффициентами.\n";
	text += L"Это нужно для расширенного Нелдера-Мида, в котором идет поиск минимума ошибки от различных начальных симплексов";
	mInfo->print(text);
}


void TuneDlg::OnBnClickedNmstopinfo()
{
	CString text = L"Условие на разность значений функций, чтобы калибровка остановилась (>0). ";
	text += L"Чем меньше значение - тем дольше будет работать. ";
	text += L"Однако при больших значениях есть вероятность, что калибровка остановится раньше времени";
	mInfo->print(text);
}


void TuneDlg::OnBnClickedLoadnmkoeffs()
{
	if (doFileName(1, mNMLoad, &mCharNMLoad)) {
		mIsNMLoad = 1;
	}
}

void TuneDlg::saveMaxAandZ()
{
	int *maxA = new int [11];
	int *maxZ = new int [10];
	if ((mMaxA[0] > 0) && (mMaxA[0] < 1e6))
		maxA[0] = mMaxA[0];
	else
		maxA[0] = -1;
	for (int i = 0; i < 10; i++) {
		if ((mMaxA[i + 1] > 0) && (mMaxA[i + 1] < 1e6))
			maxA[i + 1] = mMaxA[i + 1];
		else
			maxA[i + 1] = -1;
		if ((mMaxZ[i] > 0) && (mMaxZ[i] < 1e5))
			maxZ[i] = mMaxZ[i];
		else
			maxZ[i] = -1;
	}
	mH->setMaxAandZ(maxA, maxZ);
}

void TuneDlg::setCalAndVal()
{
	mH->setCalibrationType(mCalType);
	mH->setFitnessType(mFitnType, mValType);
}

void TuneDlg::setNMKoeffs()
{
	if ((mNMKoeff1 > 0) && (mNMKoeff1 < 1) && (mNMKoeff2 > 0) && (mNMKoeff2 < 1))
		mH->setNMKoeffs(mNMKoeff1, mNMKoeff2);
	if (mIsNMLoad) {
		ifstream nmin(mCharNMLoad, ios::in);
		int n;
		nmin >> n;
		double tmp;
		vector<double> NMKoeffs;
		for (int i = 0; i < 2 * n; i += 2) {
			nmin >> tmp;
			NMKoeffs.push_back(tmp);
		}
		mH->setNMKoeffs(NMKoeffs);
		nmin.close();
	}
}

void TuneDlg::OnBnClickedOk()
{
	UpdateData(1);
	saveMaxAandZ();
	setCalAndVal();
	mH->setCLim(mMinC, mMaxC);
	mH->setBruteforceParams(mBFStepsNum, mBFItNum);
	mH->setMinGrowth(mMinGrowth);
	setNMKoeffs();
	mH->setNMStopAndLim(mNMStop, mNMLim);
	if (mSls0 > 1)
		mSls0++;
	mH->setSlsParam(mSlsStep, mSlsLim, mSls0);
	CDialogEx::OnOK();
}
