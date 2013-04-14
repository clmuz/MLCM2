#pragma once
#include "atlcomtime.h"
#include "TuneDlg.h"
#include "InfoDlg.h"
#include "Hydrograph\Hydrograph.h"

// ������� ���������� ����

class CMLCMDlg : public CDialogEx
{
public:
	CMLCMDlg(CWnd* pParent = NULL);
	~CMLCMDlg();

// ������ ����������� ����
	enum { IDD = IDD_MLCM_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// ��������� DDX/DDV
	HICON m_hIcon;

	// ��������� ������� ����� ���������
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	//�������� �������� ������
	void loadConfig(const wchar_t *configName);
	//���������� ��������
	void saveConfig(const wchar_t *configName);
	//�������, ���������� ���� ���������-��������� � �������������� � ������ ��� �������� �����
	bool doFileName(bool loadFile, CString &edit_str, const CString &format);
	//�������, �� ���������� �������� ����������� ������
	double doComboFormats(const CString &combo);
	//�� ���� �������� ���� � ������� int[3] ���� - ����� - ���
	int *doDate(const COleDateTime &date);
	//���������� ����������
	void printError(const int &a);
	fitnessType doFitnessType(const int &intFitn) const;
	calibrationType doCalibrationType(const int &intCal) const;
	//������
	Hydrograph *mH;
	//���� �������
	InfoDlg *mInfo;
	//��� �������� ���� ���������
	bool mIsDeck;
	//��� ����� �������� ����
	bool mIsOutfile;
	//��� �������� ���� � ��������
	bool mIsPcp;

	//���������� ���� � ����������� �������
	CString mSaveSett;
	CString mLoadSett;
	CString mComboPcp;
	CString mComboDat;
	CString mEditPcp;
	CString mEditDeck;
	CString mEditDat;
	COleDateTime mCalFrom;
	COleDateTime mCalTo;
	COleDateTime mValFrom;
	COleDateTime mValTo;
	COleDateTime mModFrom;
	COleDateTime mModTo;
	int mInFormat;
	int mOutFormat;
	CString mOutFile;
	CString mSaveMlcm;
	CString mLoadMlcm;
	int mComboModAndVal;
	COleDateTime mModValFrom;
	COleDateTime mModValTo;
	int mWarmingDays;
public:
	afx_msg void OnBnClickedTune();
	afx_msg void OnBnClickedMaininfo();
	afx_msg void OnBnClickedModelinfo();
	afx_msg void OnBnClickedConfiginfo();
	afx_msg void OnBnClickedMlcminfo();
	afx_msg void OnBnClickedDeckinfo();
	afx_msg void OnBnClickedInoutformatinfo();
	afx_msg void OnBnClickedDiminfo();
	afx_msg void OnBnClickedLoadsett();
	afx_msg void OnBnClickedSavesett();
	afx_msg void OnBnClickedSavemlcm();
	afx_msg void OnBnClickedLoadmlcm();
	afx_msg void OnBnClickedoutputfile();
	afx_msg void OnBnClickeddeck();
	afx_msg void OnBnClickedpcp();
	afx_msg void OnBnClickeddat();
	afx_msg void OnBnClickedCalibrate();
	afx_msg void OnBnClickedModel();
	afx_msg void OnBnClickedValidate();
	afx_msg void OnBnClickedModandval();
	afx_msg void OnCbnSelchangeOutformat();
	afx_msg void OnEnChangeWarmingdays();
	afx_msg void OnBnClickedClearsavesett();
	afx_msg void OnBnClickedClearloadsett();
	afx_msg void OnBnClickedCleardeck();
	afx_msg void OnBnClickedClearpcp();
	afx_msg void OnBnClickedCleardat();
	afx_msg void OnBnClickedClearout();
	afx_msg void OnBnClickedClearsavemlcm();
	afx_msg void OnBnClickedClearloadmlcm();
};
