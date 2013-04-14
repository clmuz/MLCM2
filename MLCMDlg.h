#pragma once
#include "atlcomtime.h"
#include "TuneDlg.h"
#include "InfoDlg.h"
#include "Hydrograph\Hydrograph.h"

// Главное диалоговое окно

class CMLCMDlg : public CDialogEx
{
public:
	CMLCMDlg(CWnd* pParent = NULL);
	~CMLCMDlg();

// Данные диалогового окна
	enum { IDD = IDD_MLCM_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	//Загрузка настроек модели
	void loadConfig(const wchar_t *configName);
	//Сохранение настроек
	void saveConfig(const wchar_t *configName);
	//Функция, вызывающая окно Загрузить-сохранить и конвертирующая в нужный тип название файла
	bool doFileName(bool loadFile, CString &edit_str, const CString &format);
	//Функция, по комбобоксу выдающая размерность данных
	double doComboFormats(const CString &combo);
	//По дате выдающая дату в формате int[3] день - месяц - год
	int *doDate(const COleDateTime &date);
	//Обработчик эксепшенов
	void printError(const int &a);
	fitnessType doFitnessType(const int &intFitn) const;
	calibrationType doCalibrationType(const int &intCal) const;
	//Модель
	Hydrograph *mH;
	//Окно справки
	InfoDlg *mInfo;
	//Был загружен файл водосбора
	bool mIsDeck;
	//Был задан выходной файл
	bool mIsOutfile;
	//Был загружен файл с осадками
	bool mIsPcp;

	//Переменные окна и обработчики событий
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
