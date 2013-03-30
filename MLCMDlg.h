
// MLCMDlg.h : файл заголовка
//

#pragma once
#include "atlcomtime.h"
#include "TuneDlg.h"
#include "InfoDlg.h"
#include "Hydrograph\Hydrograph.h"


// диалоговое окно CMLCMDlg
class CMLCMDlg : public CDialogEx
{
// Создание
public:
	CMLCMDlg(CWnd* pParent = NULL);	// стандартный конструктор
	~CMLCMDlg();

// Данные диалогового окна
	enum { IDD = IDD_MLCM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV

private:
	void loadConfig(char *configName);
	void saveConfig(char *configName);
	bool doFileName(bool loadFile, CString &edit_str, char **charName);
	double doComboFormats(const CString &combo);
	int *doDate(const COleDateTime &date);
	void printError(const int &a);
	Hydrograph *mH;
	InfoDlg *mInfo;
	bool mIsDeck, mIsOutfile, mIsPcp;
// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CString mSaveSett;
	CString mLoadSett;
	CString mComboPcp;
	CString mComboDeck;
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
	int mCalType, mFitnType, mValType;
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
	afx_msg void OnBnClickedsetinoutformat();
	afx_msg void OnBnClickedoutputfile();
	afx_msg void OnBnClickeddeck();
	afx_msg void OnBnClickedpcp();
	afx_msg void OnBnClickeddat();
	afx_msg void OnBnClickedCalibrate();
	afx_msg void OnBnClickedModel();
	afx_msg void OnBnClickedValidate();
	afx_msg void OnBnClickedModandval();
	afx_msg void OnCbnSelchangeInformat();
	afx_msg void OnCbnSelchangeOutformat();
};
