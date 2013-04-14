#pragma once

#include "ModelsShell.h"
#include "Fitness\Fitness.h"
#include "Calibration\Calibration.h"

//Внешний класс для работы с моделью

class Hydrograph
{
public:
	Hydrograph();
	~Hydrograph();
	void loadMlcm(const wchar_t *paramFile);
	void printMlcm(const wchar_t *outputParamFile);
	void printPrediction(const int *begDate, const int *endDate);
	double printPredAndValid(const int *begDate, const int *endDate, const fitnessType fitnessType = FT_DefaultVal);
	double calibrate(const int *begDate, const int *endDate);
	double validate(const int *begDate, const int *endDate);
	void setCalibrationType(const calibrationType &calType);
	void setFitnessType(const fitnessType fitnDefType, const fitnessType valType);
	void setOutFormat(const int outFormat);
	void setMaxAandZ(const int *maxA, const int *maxZ);
	void setNMStopAndLim(const double NMStop, const int NMLim);
	void setNMKoeffs(const double koeff1, const double koeff2);
	void setNMKoeffs(const vector<double> koeffs);
	void setMinGrowth(const double minGrowth);
	void setCLim(const double minC, const double maxC);
	void setBruteforceParams(const int steps, const int it);
	void getCalAndFitnessTypes(calibrationType &calType, fitnessType &defFitnType, fitnessType &valType) const;
	int getOutFormat() const;
	void getMaxAandZ(int *maxA, int *maxZ) const;
	void getBruteforceParams(int &steps, int &iter) const;
	void getNMParams(double &stop, int &iter) const;
	int getNMKoeffs(vector<double> &koeffs) const;
	void getSlsParams(double &slsStep, int &slsLim, calibrationType &slsCalType) const;
	void setSlsParam(const double slsStep, const int slsLim, const calibrationType slsCalType);
	double getMinGrowth() const;
	void getCLim(double &minC, double &maxC) const;
	void setWarmingDays(const int countOfWarmingDays);
	int getWarmingDays() const;
	void setOutFile(const wchar_t *outFileName);
	void readDeck(const wchar_t *filename);
	void readPcp(const double format, const wchar_t *filename);
	void readDat(const double format, const wchar_t *filename);
	int click();
private:
	//Оболочка модели
	ModelsShell *mShell;
	//Целевые функции
	Fitness *mFitness;
	//Калибровка
	Calibration *mCal;
};