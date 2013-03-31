#pragma once
#include "InfoDlg.h"
#include "Hydrograph\Hydrograph.h"

// диалоговое окно TuneDlg

class TuneDlg : public CDialogEx
{
	DECLARE_DYNAMIC(TuneDlg)

public:
	TuneDlg(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~TuneDlg();
	void setHydrograph(Hydrograph *H);

// Данные диалогового окна
	enum { IDD = TuneModelDialog };

private:
	bool doFileName(bool loadFile, CString &edit_str, char **charName);
	void saveMaxAandZ();
	void setNMKoeffs();
	void setCalAndVal();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
private:
	Hydrograph *mH;
	char *mCharNMLoad;
	bool mIsNMLoad;
	InfoDlg *mInfo;
	int mMaxA[11], mMaxZ[10];
	int mBFStepsNum;
	int mBFItNum;
	CString mNMLoad;
	double mNMStop;
	int mNMLim;
	int mSlsLim;
	double mSlsStep;
	int mSls0;
	double mMinGrowth;
	int mCalType;
	int mFitnType;
	int mValType;
	double mNMKoeff1;
	double mNMKoeff2;
	double mMinC;
	double mMaxC;
public:
	afx_msg void OnBnClickedNminfo();
	afx_msg void OnBnClickedNmstopinfo();
	afx_msg void OnBnClickedLoadnmkoeffs();
	afx_msg void OnBnClickedOk();
};
