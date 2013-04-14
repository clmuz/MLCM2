#include "stdafx.h"
#include "SLS.h"

const double SLS::mMax = 1e15, SLS::mMin = 1e-3;

SLS::SLS(Fitness *fitn, Calibration *cal) :
	mFitness(fitn),
	mCal(cal),
	mStep(0.1),
	mLim(0),
	m0Type(CT_ComplexNelderMead)
{ }

void SLS::setParams(const double &step, const int &lim, const calibrationType &cal0Type)
{
	mStep = step;
	mLim = lim;
	if (cal0Type != CT_SLS)
		m0Type = cal0Type;
}

void SLS::getParams(double &step, int &lim, calibrationType &cal0Type) const
{
	step = mStep;
	lim = mLim;
	cal0Type = m0Type;
}

double SLS::doCalibration(const int &paramsNum, double *bestParams)
{
	double f;
	f = mCal->doCalStep(paramsNum, bestParams, m0Type);
	double fLeft, fRight;
	int *paramCounter = new int [paramsNum];
	int i;
	int it = 0;
	bool stop = 0;
	for (i = 0; i < paramsNum; i++) {
		paramCounter[i] = 0;
	}
	while ((!stop) && (it++ < mLim)) {
		stop = 1;
		for (i = 0; i < paramsNum; i++) {
			if (paramCounter[i]++ < 3) {
				bestParams[i] -= mStep;
				fLeft = countF(bestParams, paramsNum);
				bestParams[i] += 2.0 * mStep;
				fRight = countF(bestParams, paramsNum);
				if ((fLeft >= f) && (fRight >= f)) {
					bestParams[i] -= mStep;
					continue;
				}
				stop = 0;
				paramCounter[i] = 0;
				if (fLeft < f) {
					if (fRight < fLeft) {
						f = fRight;
						continue;
					}
					f = fLeft;
					bestParams[i] -= 2.0 * mStep;
					continue;
				}
				f = fRight;
			}
		}
	}
	delete[] paramCounter;
	return f;
}

double SLS::countF(double *param, const int &paramsNum) const
{
	int i;
	for (i = 0; i < paramsNum; i++) {
		if ((param[i] < mMin) || (param[i] > 1))
			break;
	}
	if (i < paramsNum)
		return mMax;
	return mFitness->getFitness(param);
}