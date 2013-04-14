#pragma once
#include "..\Fitness\Fitness.h"

//Калибровка перебором

class BruteForce
{
public:
	explicit BruteForce(Fitness *fitn);
	//Устновить количество шагов и итераций
	void setBFParams(const int &stepsNum, const int &iterNum);
	//Узнать количество шагов и итераций
	void getBFParams(int &stepsNum, int &iterNum) const;
	//Калибровать один слой
	double doCalibration(const int &paramsNum, double *bestParams) const;
private:
	//Калибровать отрезок от leftBorder на ширину на width
	void calibrateGap(const int &paramsNum, double &f, const double *leftBorder, const double &width, double *bestParams) const;
	//Вызов рекурсии
	void recursion(double &f
				, const int &coord
				, const int &paramsNum
				, const double &step
				, const double *leftBorder
				, const double *nowParams
				, double *bestParams) const;
	Fitness *mFitness;
	//Количество шагов
	int mStepsNum;
	//Количество итераций
	int mIterNum;
	//Выдает это значение, если параметры модели выходят за пределы области определения.
	//Предполагается, что это выше любого значения целевой функции
	static const double mMax;
	//Минимальные значения параметров, так как при нуле могут возникнуть проблемы
	static const double mMin;
};