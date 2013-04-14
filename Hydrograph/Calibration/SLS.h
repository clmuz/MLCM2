#pragma once
#include "..\Fitness\Fitness.h"
#include "Calibration.h"

//Калибровка SLS

class Calibration;
enum calibrationType;
class Fitness;

class SLS
{
public:
	SLS(Fitness *fitn, Calibration *cal);
	//Установить параметры SLS
	void setParams(const double &step, const int &lim, const calibrationType &cal0Type);
	//Узнать параметры SLS
	void getParams(double &step, int &lim, calibrationType &cal0Type) const;
	//Выполнить калибровку
	double doCalibration(const int &paramsNum, double *bestParams);
private:
	//Посчитать целевую функцию от набора параметров модели point
	double countF(double *param, const int &paramsNum) const;
	Fitness *mFitness;
	//Внешний класс калибровки. Ссылка на него нужна для того, чтобы выполнять первичную глобальную калибровку
	Calibration *mCal;
	//Один шаг (обычно от 0.01 до 0.05)
	double mStep;
	//Выдает это значение, если параметры модели выходят за пределы области определения.
	//Предполагается, что это выше любого значения целевой функции
	static const double mMax;
	//Минимальное значение параметра (чтобы не было проблем в нуле)
	static const double mMin;
	//Лимит на количество итераций
	int mLim;
	//Тип первичной глобальной калибровки
	calibrationType m0Type;
};