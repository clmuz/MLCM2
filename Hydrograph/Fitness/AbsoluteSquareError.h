#pragma once
#include <vector>
#include "..\ModelsShell.h"

//Среднеквадратичная ошибка

class ModelsShell;

class AbsoluteSquareError
{
public:
	explicit AbsoluteSquareError(ModelsShell *modShell);
	//Посчитать среднеквадратичную ошибку
	double countError() const;
	//Посчитать ошибку от уже посчитанной модели
	double countError(const vector<double> &modVal) const;
	//Установить начало и конец подсчета среднеквадратичной ошибки
	void setBegEnd(const int &begDay, const int &endDay);
	//Установить количество измерений в день
	void setMeasPerDay(const int &measPerDay);
	//Установить реальные данные
	void setRealVal(vector<double> *realDat, const int &gap);
private:
	ModelsShell *mShell;
	//Реальные данные
	vector<double> *mRealVal;
	//Разница между осадками и реальными данными в днях
	int mGap;
	//Начало и конец подсчета среднеквадратичной ошибки
	int mDayBeg, mDayEnd;
	//Количество измерений в день
	int mMeasPerDay;
};
