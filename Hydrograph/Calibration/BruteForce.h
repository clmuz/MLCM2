#pragma once
#include "..\MlcmShell.h"
#include "..\Element.h"

class BruteForce
{
public:
	explicit BruteForce(MlcmShell *modelSh);
	void setBFParams(const int &stepsNum, const int &iterNum);
	void getBFParams(int &stepsNum, int &iterNum) const;
	double calibrateLayer(const int &N, Element &best) const;
private:
	void calibrateGap(const int &N, double &f, const double *leftBorder, const double &width, Element &best) const;
	void recursion(double &f
				, const int &coord
				, const int &coordNum
				, const double &step
				, const double *leftBorder
				, Element now
				, Element &best) const;
	MlcmShell *mModelSh;
	int mStepsNum, mIterNum;
	double mMax, mMin;
};