#pragma once
#include "..\MlcmShell.h"
#include "Calibration.h"

//Калибровка SLS

class Calibration;

class SLS
{
public:
	SLS(MlcmShell *modelSh, Calibration *cal);
	//Установить параметры SLS
	void setParams(const double &step, const int &lim, const int &cal0Type);
	//Узнать параметры SLS
	void getParams(double &step, int &lim, int &cal0Type) const;
	//Выполнить калибровку
	double doCalibration(const int &N, Element &bestElement);
private:
	//Посчитать целевую функцию от набора параметров модели point
	double countF(Element &point) const;
	//Оболочка MLCM
	MlcmShell *mModelSh;
	//Внешний класс калибровки. Ссылка на него нужна для того, чтобы выполнять первичную глобальную калибровку
	Calibration *mCal;
	//Один шаг (обычно от 0.01 до 0.05)
	double mStep;
	//Выдает это значение, если параметры модели выходят за пределы области определения.
	//Предполагается, что это выше любого значения целевой функции
	static const double mMax;
	//Лимит на количество итераций
	int mLim;
	//Тип первичной глобальной калибровки
	int m0Type;
};