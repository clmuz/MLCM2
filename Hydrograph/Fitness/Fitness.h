#pragma once
#include "..\ModelsShell.h"
#include "Msof.h"
#include "AbsoluteSquareError.h"

//Класс, связывающий разные целевые функции
// 0 - MSOF, 1 - Среднеквадратичное отклонение

class ModelsShell;
class Msof;
class AbsoluteSquareError;

enum fitnessType
{
	FT_DefaultVal = -2,
	FT_Default,
	FT_MSOF,
	FT_ASE
};

class Fitness
{
public:
	explicit Fitness(ModelsShell *modShell);
	~Fitness();
	//Установить целевую функцию для калибровки
	void setDefFitnessType(const fitnessType &fitnType);
	//Установить целевую функцию для валидации
	void setValType(const fitnessType &valType);
	//Установить количество измерений в день
	void setMeasPerDay(const int &measPerDay);
	//Установить реальные значения
	void setRealVal(vector<double> *realDat, const int &gap);
	//Посчитать целевую функцию для калибровки (по умолчанию)
	double getFitness() const;
	//Посчитать целевую функцию типа fitnessType
	double getFitness(const fitnessType &fitnType);
	//Посчитать целевую функцию от параметров
	double getFitness(const double *params);
	//Посчитать валидацию
	double getValFitness() const;
	//Установить начало и конец действия целевых функций
	void setBegEnd(const int &begDay, const int &endDay);
	//Гетер типов целевых функций
	void getFitnessTypes(fitnessType &defType, fitnessType &valType) const;
private:
	//Добавить целевую функцию типа fitnessType
	void createFitnessFunction(const fitnessType &fitnType);
	//Тип целевой функции для калибровки
	fitnessType mFitnessType;
	//Тип целевой функции для валидации
	fitnessType mValType;
	//Количество известных целевых функций
	static const int mMaxClassesNum;
	//Количество измерений в день
	int mMeasPerDay;
	//Разница между осадками и реальными данными в днях
	int mGap;
	//Реальные данные
	vector<double> *mRealVal;
	//Добавлен ли уже данная целевая функция
	bool *mCreatedClasses;
	//Начало и конец действия целевых функций
	int mDayBeg, mDayEnd;
	ModelsShell *mShell;
	Msof *mMsof;
	AbsoluteSquareError *mAse;
};
