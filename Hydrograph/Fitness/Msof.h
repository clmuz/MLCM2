#pragma once
#include <vector>
#include "..\Mlcm.h"

//Целевая функция MSOF

using namespace std;

class Msof
{
public:
	explicit Msof(Mlcm *model);
	//Установить начало и конец действия MSOF
	void setBegEnd(const int &begDay, const int &endDay);
	//Вернуть значение MSOF
	double countError() const;
	//Вернуть значение от уже посчитанной модели
	double countError(const vector<double> &modVal) const;
	//Установить количество измерений в день
	void setMeasPerDay(const int &measPerDay);
	//Установить реальные данные
	void setRealVal(vector<double> *realDat, const int &gap);
private:
	//Подсчет средних реальных данных в день и в месяц
	void countRealVal();
	//Подсчет сигм
	void countSigmas();
	//Начало и конец действия MSOF
	int mDayBeg, mDayEnd;
	//Количество измерений в день
	int mMeasPerDay;
	//Разница между осадками и реальными данными в днях
	int mGap;
	//Реальные данные
	vector<double> *mRealVal;
	//Среднесуточное для реальных данных
	vector<double> mAveDaily;
	//Среднемесячное для реальных данных
	vector<double> mAveMonthly;
	//квадратное отношение сигм
	double mSigma1to2sq, mSigma1to3sq;
	//нижний предел сигм
	static const double mLowerLim;
	//MLCM
	Mlcm *mModel;
};
