#pragma once
#include <vector>
#include "..\MlcmShell.h"
#include "..\Element.h"

//Калибровка Нелдера-Мида

class NelderMead
{
public:
	explicit NelderMead(MlcmShell *modelSh);
	//Выполнить калибровку от одного начального симплекса
	double doCalibration(const int &N, Element &nowElement);
	//Выполнить калибровку для нескольких начальных симплексов
	double doComplexCalibration(const int &N, Element &nowElement);
	//Установить коэффициенты начального симплекса для одиночной калибровки
	void setKoeffs(const double &koeff1, const double &koeff2);
	//Установить коэффициенты симплексов для многосимплексовой калибровки
	void setKoeffs(const vector<double> &koeffs);
	//Установить условие останова и лимит на количество проходов
	void setNMStopAndLim(const double &NMStop, const int &NMLim);
	//Узнать условие останова и лимит на количество проходов
	void getNMParams(double &stop, int &iter) const;
	//Гетер всех коэффициентов симплексов
	int getNMKoeffs(vector<double> &koeffs) const;
private:
	//Выполнить калибровку от симплекса
	double doNelderMead(Element &nowElement, vector<Element*> &elements) const;
	//Создать симплекс по коэффициентам
	void doFirstSimplex(const double &koeff1, const double &koeff2, vector<Element*> &elements) const;
	//Посчитать целевую функцию от набора параметров модели point
	double countF(const Element &point) const;
	//Проверка на условие останова (близость значений целевых функций в вершинах симплекса)
	bool isLim(const vector<double> &Fs) const;
	//swap вершин симплекса и их значений функций
	void swap(const int &i, const int &j, vector<Element*> &elements, vector<double> &Fs) const;
	//Коэффициенты Нелдера-Мида
	static const double mAlpha, mBetta, mGamma;
	//Выдает это значение, если параметры модели выходят за пределы области определения.
	//Предполагается, что это выше любого значения целевой функции
	static const double mMax;
	//Условие останова - для проверки на близость значений целевых функций
	double mE;
	//Коэффициенты симплекса для одиночной калибровки
	double mKoeff1, mKoeff2;
	//Количество слоев
	int mN;
	//Количество вершин симплекса
	int mSimplexN;
	//Количество симплексов в многосимплексовой калибровке
	int mComplexNum;
	//Лимит на количество итераций
	int mLimit;
	MlcmShell *mModelSh;
	//Коэффициенты симплексов для многосимплексовой калибровки
	vector<double> mKoeffs;
};
