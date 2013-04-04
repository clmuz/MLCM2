#pragma once
#include "..\MlcmShell.h"
#include "Calibration.h"

class Calibration;

class SLS
{
public:
	SLS(MlcmShell *modelSh, Calibration *cal);
	void setParams(const double &step, const int &lim, const int &cal0Type);
	void getParams(double &step, int &lim, int &cal0Type) const;
	void setBFParams(const int &stepsNum, const int &iterNum);
	double doCalibration(const int &N, Element &bestElement);
private:
	double countF(Element &point) const;
	MlcmShell *mModelSh;
	Calibration *mCal;
	double mStep, mMax;
	int mLim, m0Type, mBFSteps, mBFitNum;
};