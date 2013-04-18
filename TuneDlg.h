#pragma once
#include "InfoDlg.h"
#include "Hydrograph\Hydrograph.h"

// ���������� ���� ��������� ������

class TuneDlg : public CDialogEx
{
	DECLARE_DYNAMIC(TuneDlg)
public:
	TuneDlg(CWnd* pParent = NULL);
	virtual ~TuneDlg();
	//���������� ������ �� ������������� ������
	void setHydrograph(Hydrograph *H);

// ������ ����������� ����
	enum { IDD = TuneModelDialog };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV
	DECLARE_MESSAGE_MAP()

private:
	//�������, ���������� ���� ���������-��������� � �������������� � ������ ��� �������� �����
	bool doFileName(bool loadFile, CString &edit_str, const CString &format);
	//��������� ����������� �� �������� ����� ���� � ������ �����
	void saveMaxAandZ();
	//���������� ������������ ��� �������-����
	void setNMKoeffs();
	//���������� ���� ���������� � ������� �������
	void setCalAndVal();
	fitnessType doFitnessType(const int &intFitn) const;
	calibrationType doCalibrationType(const int &intCal) const;
	//������
	Hydrograph *mH;
	//�������� ���� � �������������� ��� �������-����
	bool mIsNMLoad;
	//���������� ���� ����������
	InfoDlg *mInfo;
	//���������� ���� � ����������� �������
	double mMaxA[11], mMaxZ[10];
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
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedClearnm();
};
