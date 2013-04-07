#pragma once
#include "..\Mlcm.h"
#include "Msof.h"
#include "AbsoluteSquareError.h"

//Класс, связывающий разные целевые функции
// 0 - MSOF, 1 - Среднеквадратичное отклонение

class Fitness
{
public:
	explicit Fitness(Mlcm *model);
	~Fitness();
	//Установить целевую функцию для калибровки
	void setDefFitnessType(const int &fitnessType);
	//Установить целевую функцию для валидации
	void setValType(const int &valType);
	//Установить количество измерений в день
	void setMeasPerDay(const int &measPerDay);
	//Установить реальные значения
	void setRealVal(vector<double> *realDat, const int &gap);
	//Посчитать целевую функцию для калибровки (по умолчанию)
	double getFitness() const;
	//Посчитать целевую функцию типа fitnessType
	double getFitness(const int &fitnessType);
	//Посчитать целевую функцию типа fitnessType от уже посчитанной модели
	double getFitness(const int &fitnessType, const vector<double> &modVal);
	//Посчитать валидацию
	double getValFitness() const;
	//Посчитать валидацию от уже посчитанной модели
	double getValFitness(const vector<double> &modVal) const;
	//Установить начало и конец действия целевых функций
	void setBegEnd(const unsigned int &begDay, const unsigned int &endDay);
	//Гетер типов целевых функций
	void getFitnessTypes(int &defType, int &valType) const;
private:
	//Добавить целевую функцию типа fitnessType
	void createFitnessFunction(const int &fitnessType);
	//Тип целевой функции для калибровки
	int mFitnessType;
	//Тип целевой функции для валидации
	int mValType;
	//Количество известных целевых функций
	static const int mMaxClassesNum;
	//Количество измерений в день
	int mMeasPerDay;
	//Разница между осадками и реальными данными в днях
	int mGap;
	//Реальные данные
	vector<double> *mRealVal;
	//Добавлен ли уже данная целевая функция
	vector<bool> mCreatedClasses;
	//Начало и конец действия целевых функций
	int mDayBeg, mDayEnd;
	Mlcm *mModel;
	Msof *mMsof;
	AbsoluteSquareError *mAse;
};
