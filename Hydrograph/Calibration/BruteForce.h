#pragma once
#include "..\MlcmShell.h"
#include "..\Element.h"

//Калибровка перебором

class BruteForce
{
public:
	explicit BruteForce(MlcmShell *modelSh);
	//Устновить количество шагов и итераций
	void setBFParams(const int &stepsNum, const int &iterNum);
	//Узнать количество шагов и итераций
	void getBFParams(int &stepsNum, int &iterNum) const;
	//Калибровать один слой
	double calibrateLayer(const int &N, Element &best) const;
private:
	//Калибровать отрезок от leftBorder на ширину на width
	void calibrateGap(const int &N, double &f, const double *leftBorder, const double &width, Element &best) const;
	//Вызов рекурсии
	void recursion(double &f
				, const int &coord
				, const int &coordNum
				, const double &step
				, const double *leftBorder
				, Element now
				, Element &best) const;
	MlcmShell *mModelSh;
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