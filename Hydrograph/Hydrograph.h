#pragma once
#include "MlcmShell.h"
#include "Calibration\Calibration.h"

//Внешний класс для работы с моделью

class Hydrograph
{
public:
	Hydrograph();
	~Hydrograph();
	void loadMlcm(const char *paramFile);
	void printMlcm(const char *outputParamFile);
	void printPrediction(const int *begDate, const int *endDate);
	double printPredAndValid(const int *begDate, const int *endDate, const int &fitnessType = -1);
	double calibrate(const int *begDate, const int *endDate);
	double validate(const int *begDate, const int *endDate);
	void setCalibrationType(const int &calType);
	void setFitnessType(const int &fitnDefType, const int &valType);
	void setOutFormat(const int &outFormat);
	void setMaxAandZ(const int *maxA, const int *maxZ);
	void setNMStopAndLim(const double &NMStop, const int &NMLim);
	void setNMKoeffs(const double &koeff1, const double &koeff2);
	void setNMKoeffs(const std::vector<double> koeffs);
	void setMinGrowth(const double minGrowth);
	void setCLim(const double &c1, const double &c2);
	void setBruteforceParams(const int &steps, const int &it);
	void getCalAndFitnessTypes(int &calType, int &defFitnType, int &valType) const;
	int getOutFormat() const;
	void getMaxAandZ(int *maxA, int *maxZ) const;
	void getBruteforceParams(int &steps, int &iter) const;
	void getNMParams(double &stop, int &iter) const;
	int getNMKoeffs(std::vector<double> &koeffs) const;
	void getSlsParams(double &slsStep, int &slsLim, int &slsCalType) const;
	void setSlsParam(const double &slsStep, const int &slsLim, const int &slsCalType);
	double getMinGrowth() const;
	void getCLim(double &c1, double &c2) const;
	void setHeatDays(const int &countOfHeatDays);
	int getHeatDays() const;
	void setOutFile(char *outFileName);
	void readDeck(const char *filename);
	void readPcp(const double &format, const char *filename);
	void readDat(const double &format, const char *filename);
	int click();
private:
	//Оболочка модели
	MlcmShell *mMlcmSh;
	//Калибровка
	Calibration *mCal;
};
