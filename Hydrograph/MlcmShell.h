#pragma once
#include <vector>
#include "Mlcm.h"
#include "Fitness\Fitness.h"
#include "Element.h"

//Оболочка MLCM

using namespace std;

class MlcmShell
{
public:
	MlcmShell();
	~MlcmShell();
	//Установить целевую функцию для калибровки
	void setDefFitnessType(const int &defFitnType);
	//Установить целевую фунцкию для валидации
	void setValType(const int &valType);
	//Установить формат выходного файла
	void setOutFormat(const int &outFormat);
	//Вывести в файл смоделированные данные
	void printPrediction(const int *begDate, const int *endDate);
	//Вывести в файл смоделированные данные и произвести валидаци типом fitnessType
	double printPrediction(const int *begDate, const int *endDate, const int &fitnessType);
	//Загрузить параметры MLCM
	void loadParametrs(const char *paramFile);
	//Изменить параметры MLCM
	void changeModelParametrs(const Element &element);
	//Установить ограничения на скорости через слои и ширины слоев
	void setMaxAandZ(const int *newMaxA, const int *newMaxZ);
	//Установить трансформацию паводочного слоя
	void setCLim(const double &c1, const double &c2);
	//Узнать целевую функцию
	double getFitness() const;
	//Произвести валидацию
	double getValFitness() const;
	//Установить начало и конец действия целевой функции
	void setFitnessBegEnd(const int *begDate, const int *endDate);
	//Распечатать в файл параметры модели
	void printParams(const char *outputParamFile);
	//Получить параметры
	Element getParams() const;
	//гетеры
	void getFitnessTypes(int &defType, int &valType) const;
	int getOutFormat() const;
	void getMaxAandZ(int *maxA, int *maxZ) const;
	void getCLim(double &c1, double &c2) const;
	//Задать выходной файл
	void setOutFile(char *outFileName);
	//Загрузить данные о водосборе
	void readDeck(const double &format, const char *filename);
	//Загрузить файл с осдаками
	void readPcp(const double &format, const char *filename);
	//Загрузить файл с релаьными значениями
	void readDat(const double &format, const char *filename);
	//Сколько раз запускалась MLCM с момента последнего клика
	int click();
	//Посчитать целевую функцию от параметров
	double countF(const Element &point);
private:
	//Выдает испарения от месяца
	double makeET(const int &month);
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
	//Дает значение реальных данных в области определения и 0 вне
	double getRealData(const int &i) const;
	//Инкремент даты
	void incDate(int *date) const;
	//MLCM
	Mlcm *mModel;
	//Целевые функции
	Fitness *mFitness;
	//Длина склона
	int mAslope;
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
	//Начало реальных данных и осадков
	int mDatBeg[3], mPcpBeg[3]; //0 - day, 1 - month, 2 - year;
	//Имя выходного файла
	const char *mOutput;
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
	//Размерноесть испарений
	double mETFormat;
	//Размерность выходных данных
	double mOutFormat;
};

