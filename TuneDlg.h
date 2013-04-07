#pragma once
#include "InfoDlg.h"
#include "Hydrograph\Hydrograph.h"

// Диалоговое окно настройки модели

class TuneDlg : public CDialogEx
{
	DECLARE_DYNAMIC(TuneDlg)
public:
	TuneDlg(CWnd* pParent = NULL);
	virtual ~TuneDlg();
	//Установить ссылку на настраиваемую модель
	void setHydrograph(Hydrograph *H);

// Данные диалогового окна
	enum { IDD = TuneModelDialog };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	DECLARE_MESSAGE_MAP()

private:
	//Функция, вызывающая окно Загрузить-сохранить и конвертирующая в нужный тип название файла
	bool doFileName(bool loadFile, CString &edit_str, char **charName);
	//Сохранить ограничение на скорости через слой и ширины слоев
	void saveMaxAandZ();
	//Установить коэффициенты для Нелдера-Мида
	void setNMKoeffs();
	//Установить типы калибровки и целевых функций
	void setCalAndVal();
	//Модель
	Hydrograph *mH;
	//Имя файла с коэффициентами для Нелдера-Мида
	char *mCharNMLoad;
	//Загружен файл с коэффициентами для Нелдера-Мида
	bool mIsNMLoad;
	//Диалоговое окно информации
	InfoDlg *mInfo;
	//Переменные окна и обработчики событий
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
