#pragma once
#include "..\Fitness\Fitness.h"
#include "..\ModelsShell.h"
#include "NelderMead.h"
#include "SLS.h"
#include "BruteForce.h"
#include <vector>

//Класс, связывающий различные калибровки
//0 - Нелдер-Мид от одного симплекса, 1 - многосимплексовый Нелдер-Мид, 2 - SLS, 3 - перебор

class SLS;

enum calibrationType
{
	CT_NULL = -1,
	CT_NelderMead,
	CT_ComplexNelderMead,
	CT_SLS,
	CT_BruteForce
};

class Calibration
{
public:
	Calibration(ModelsShell *modShell, Fitness *fitn);
	~Calibration();
	//Выполнить калибровку
	double doCalibration();
	//Посчитать калибровку для N слоев, с типом калибровки calType
	double doCalStep(const int &paramNum, double *bestParams, calibrationType calType = CT_NULL);
	//Установить тип калибровки
	void setCalibrationType(const calibrationType &calType);
	//Установить условие останова и лимит на количество итераций в Нелдере-Миде
	void setNMStopAndLim(const double &NMStop, const int &NMLim);
	//Установить коэффициенты симплекса для одиносимплексого Нелдера-Мида
	void setNMKoeffs(const double &koeff1, const double &koeff2);
	//Установить коэффициенты симплексов для многосимплексого Нелдера-Мида
	void setNMKoeffs(const vector<double> &koeffs);
	//Установить количество шагов и итераций в переборе
	void setBFParams(const int &stepsNum, const int &iterNum);
	//Установить шаг, лимит на количество итераций и тип предварительной калибровки для SLS
	void setSlsParam(const double &slsStep, const int &slsLim, const calibrationType &slsCalType);
	//Узнать шаг, лимит на количество итераций и тип предварительной калибровки для SLS
	void getSlsParams(double &slsStep, int &slsLim, calibrationType &slsCalType) const;
	//Установить минимальный прирост от предыдущего слоя к новому
	void setMinGrowth(const double &minGrowth);
	//Узнать тип калибровки
	 calibrationType getCalType() const;
	//Узнать условие останова и лимит на количество итераций в Нелдере-Миде
	void getNMParams(double &stop, int &iter) const;
	//Узнать коэффициенты симплексов в Нелдере-Миде
	int getNMKoeffs(vector<double> &koeffs) const;
	//Узнать минимальный прирост от предыдущего слоя к новому
	double getMinGrowth() const;
	//Узнать количество шагов и итераций в переборе
	void getBFParams(int &stepsNum, int &iterNum) const;
private:
	//Тип калибровки
	 calibrationType mCalType;
	//Минимальный прирост от предыдущего слоя к новому
	double mMinGrowth;
	ModelsShell *mShell;
	Fitness *mFitness;
	NelderMead *mNM;
	SLS *mSLS;
	BruteForce *mBF;
};
