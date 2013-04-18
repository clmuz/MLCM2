#pragma once
#include <vector>
#include <queue>

//MLCM

using namespace std;

class Mlcm
{
public:
	Mlcm();
	~Mlcm();
	//Установить длину склона и количество ординат единичного гидрографа
	void setAslopeAndNuh(const double &faslope, const int &nuh);
	//Установить реальные данные и их начало
	void setRealData(vector<double> *realData, const int &realDatBeg);
	//Установить количество слоев
	void setN(const int &n);
	int getN() const;
	//Установить параметы модели
	void setParam(const double *params);
	//Установить осадки и испарения
	void setPandET(vector<double> *P, vector<double> *ET);
	//Установить количество шагов прогрева модели
	void setWarmingSteps(const int &countOfWarmingSteps);
	//Узнать количество шагов прогрева модели
	int getWarmingSteps() const;
	//Установить ограничения на скорости через слои и ширины слоев
	void setMaxAandZ(const double *maxA, const double *maxZ);
	//Установить ограничения на скорость трансформации паводочной волны
	void setCLim(const double &minC, const double &maxC);
	void getMaxAandZ(double *maxA, double *maxZ) const;
	void getCLim(double &minC, double &maxC) const;
	//Запуск модели
	vector<double> makeRunoff(const int &timeBeg, const int &timeEnd) const;
	//Сколько раз запускалась MLCM с момента последнего клика
	int click();
	void printParams(const wchar_t *outputParamFile) const; //TODO
	void loadParams(const wchar_t *inputParamFile); //TODO
private:
	//Задает ординаты единичного гидрографа
	void makeHydrOrd();
	//Считает Qsum в момент времени time
	double countUhT(const vector<double> &Qsum, const int &time) const;
	//Один шаг модели (шаг = 24/<количество измерений в день>)
	double makeStep(const double &P
					, const double &ET
					, vector<double> &state) const;
	//Сколько воды дошло до конца склона в момент времени time
	double countChannelWater(vector<double> &state) const;
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
	//Количество шагов прогрева модели
	int mWarmingSteps;
	//Скорость трансформации паводочного слоя
	double mC;
	//etta и k - для подсчета ординат единичного гидрографа с помощью гамма функции
	double mEtta;
	double mK;
	//Сдвиг по времени
	double mT;
	//Площадь склона
	double mFbasin;
	//Ординаты единичного гидрографа
	vector<double> mFxOrd;
	//Вектор скоростей через слои
	double *mAlpha;
	//Вектор ширин слоев
	double *mZ;
	//Осадки
	vector<double> *mP;
	//Испарения
	vector<double> *mET;
	//Реальные данные
	vector<double> *mRealData;
	//Ограничения на скорости через слои и на скорости, помноженные на площадь (0 - поверхностный, 10 - 10-ый слой)
	double *mMaxA, *mMaxAS;
	//Ограничения на ширины слоев (0 - 1 слой)
	double *mMaxZ, *mMaxZS;
	//Ограничения на скорость трансформации паводочной волны
	double mMinC, mMaxC;
};
