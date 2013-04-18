#pragma once
#include <vector>
#include "Mlcm.h"
#include "Fitness\Fitness.h"

//Оболочка моделей

using namespace std;

class Fitness;
enum fitnessType;

enum modelType {
	MT_MLCM = 0
};

class ModelsShell
{
public:
	ModelsShell();
	~ModelsShell();
	void setFitnessClass(Fitness *fitn);
	//Установить формат выходного файла
	void setOutFormat(const int &outFormat);
	//Вывести в файл смоделированные данные
	void printPrediction(const int *begDate, const int *endDate);
	//Вывести в файл смоделированные данные и произвести валидаци типом fitnessType
	double printPrediction(const int *begDate, const int *endDate, const fitnessType &fitnType);
	//Изменить параметры MLCM
	void changeModelParametrs(const double *params);
	//Установить ограничения на скорости через слои и ширины слоев
	void setMaxAandZ(const int *maxA, const int *maxZ);
	//Установить трансформацию паводочного слоя
	void setCLim(const double &minC, const double &maxC);
	//Установить начало и конец действия целевой функции
	void setFitnessBegEnd(const int *begDate, const int *endDate);
	//Установить количество дней прогрева модели
	void setWarmingDays(const int &countOfWarmingDays);
	//Узнать количество дней прогрева модели
	int getWarmingDays() const;
	//Распечатать в файл параметры модели
	void printParams(const wchar_t *outputParamFile);
	void loadParams(const wchar_t *inputParamFile);
	//гетеры
	int getOutFormat() const;
	void getMaxAandZ(int *maxA, int *maxZ) const;
	void getCLim(double &minC, double &maxC) const;
	//Задать выходной файл
	void setOutFile(const wchar_t *outFileName);
	//Загрузить данные о водосборе
	void readDeck(const double fbasinFormat, const double etFormat, const wchar_t *filename);
	//Загрузить файл с осдаками
	void readPcp(const double &format, const wchar_t *filename);
	//Загрузить файл с релаьными значениями
	void readDat(const double &format, const wchar_t *filename);
	//Сколько раз запускалась MLCM с момента последнего клика
	int click();
	//Посчитать целевую функцию от параметров
	double countF(const double *params);
	//Заупск модели
	vector<double> makeRunoff(const int &dayBeg, const int &dayEnd) const;
	//Выдает количество параметров модели при этой итерации
	int getModelParams(const int &iter) const;
	//Задает итерацию модели
	void setModelIter(const int &iter);
	int getIterNum() const;
private:
	//Выдает испарения в данный момент
	double makeET(const int *date, const double &p) const;
	//Считает разницу в днях между двумя датами в формате int[3] - день-месяц-год
	int makeTheGap(const int *date1, const int *date2) const;
	//Возвращает количество дней в месяце
	int giveDaysInMonth(const int &month, const int &year) const;
	//одна итерация чтения из файла
	void readInFormat(ifstream &fin, int &code, int &month, int &day, double &value) const;
	//прочесть первую строчку файла и установить формат
	void readAndSetFormat(ifstream &fin, int &code, int &month, int &day, double &value);
	//одна итерация записи в файл
	void writeOutFormat(ofstream &fout, const int *date, const int &i, const int &begPoint, const double &value) const;
	//Дает значение реальных данных в области определения и вне
	double getRealData(const int &i) const;
	//Инкремент даты
	void incDate(int *date) const;
	//Активная модель
	modelType mActiveModel;
	//MLCM
	Mlcm *mMlcm;
	//Целевые функции
	Fitness *mFitness;
	//Длина склона
	double mAslope;
	//Площадь водосбора
	double mFbasin;
	//Количество измерений в день
	int mMeasPerDay;
	//Формат входных данных
	int mInfileFormat;
	//Формат выходных данных
	int mOutfileFormat;
	//Код водосбора
	int mCode;
	//Разница между началом данных об осадках и началом данных о реальных данных
	int mGap;
	//Количество дней прогрева модели
	int mWarmingDays;
	//Начало реальных данных и осадков
	int mDatBeg[3], mPcpBeg[3]; //0 - day, 1 - month, 2 - year;
	//Имя выходного файла
	const wchar_t *mOutput;
	//Осадки
	vector<double> mP;
	//Испарения
	vector<double> mET;
	//Реальные данные
	vector<double> mDat;
	//Размерность осадков (на сколько надо умножить, чтобы получить мм/ч)
	double mPcpFormat;
	//Размерность реальных данных
	double mDatFormat;
	//Размерность выходных данных
	double mOutFormat;
	//Способ задания испарений - 0 - через среднемесячные значения, 1 - через EE и d
	bool mEtWay;
	//Среднемесячные испарения
	double *mAvET;
	//Испаряемость
	double mEtEE;
	//Среднесуточное значение дефицита влажности
	double mEtD;
	//Даты связанные со снегом в формате: Выпадение снега: 0 - день, 1 - месяц; Сход снега: 2 - день, 3 - месяц;
	//Две недели от снега: 4 - день, 5 - месяц
	int mSnow[6];
};