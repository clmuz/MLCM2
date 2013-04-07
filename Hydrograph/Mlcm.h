#pragma once
#include <vector>
#include <queue>
#include "Element.h"

//MLCM

using namespace std;

class Mlcm
{
public:
	Mlcm();
	~Mlcm();
	//Установить длину склона и количество ординат единичного гидрографа
	void setAslopeAndNuh(const double &Aslope, const int &nuh);
	//Установить реальные данные и их начало
	void setRealData(vector<double> *realData, const int &realDatBeg);
	//Установить параметы модели
	void setParam(const Element &element);
	//Установить осадки и испарения
	void setPandET(vector<double> *P, vector<double> *ET);
	//Запуск модели
	vector<double> makeRunoff(const int &timeBeg, const int &timeEnd) const;
	//Сколько раз запускалась MLCM с момента последнего клика
	int click();
	//Получить параметры модели
	Element getParam() const;
private:
	//Сколько воды дойдет до конца склона в момент времени time
	struct Water {
		Water (const double &waterThikness, const int &newTime);
		double water;
		int time;
	};
	//Задает ординаты единичного гидрографа
	void makeHydrOrd();
	//Считает Qsum в момент времени time
	double countUhT(const vector<double> &Qsum, const int &time) const;
	//Один шаг модели (шаг = 24/<количество измерений в день>)
	double makeStep(const double &P
					, const double &ET
					, const int &time
					, queue<Water> *waterQueue
					, vector<double> &state) const;
	//Сколько воды дошло до конца склона в момент времени time
	double countChannelWater(const int &time, queue<Water> *waterQueue, vector<double> &state) const;
	//Количество слоев
	int mN;
	//Количество ординат с учетом сдвига по времени
	int mNOrd;
	//Количество ординат
	int mNuh;
	//Начало реальных данных относительно начала осадков в количестве шагов
	int mRealBeg;
	//Конец реальных данных относительно начала осадков в количестве шагов
	int mRealEnd;
	//Количество запусков модели на данный момент (int* чтобы не нарушать константность вызова модели)
	int *mClick;
	//Время за которое вода стечет по поверхности (в шагах)
	int mTime0;
	//Скорость трансформации паводочного слоя
	double mC;
	//etta и k - для подсчета ординат единичного гидрографа с помощью гамма функции
	double mEtta;
	double mK;
	//Сдвиг по времени
	double mT;
	//Скорость поверхностного стекания
	double mAlpha0;
	//Длина склона
	double mAslope;
	//Константа, меньше которой скорость считается нулем
	static const double mMin;
	//Массив времен, за которые вода стечет по i-ому слою
	double *mTime;
	//Ординаты единичного гидрографа
	vector<double> mFxOrd;
	//Вектор скоростей через слои
	vector<double> mAlpha;
	//Вектор ширин слоев
	vector<double> mZ;
	//Осадки
	vector<double> *mP;
	//Испарения
	vector<double> *mET;
	//Реальные данные
	vector<double> *mRealData;
};
