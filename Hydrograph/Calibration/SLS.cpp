#include "stdafx.h"
#include "SLS.h"


SLS::SLS(MlcmShell *modelSh, Calibration *cal) :
	mModelSh(modelSh),
	mCal(cal),
	mStep(0.1),
	mLim(0),
	m0Type(0),
	mBFSteps(1),
	mMax(1e15)
{ }

void SLS::setParams(const double &step, const int &lim, const int &cal0Type)
{
	mStep = step;
	mLim = lim;
	if (cal0Type != 2)
		m0Type = cal0Type;
}

void SLS::getParams(double &step, int &lim, int &cal0Type) const
{
	step = mStep;
	lim = mLim;
	cal0Type = m0Type;
}

void SLS::setBFParams(const int &stepsNum, const int &iterNum)
{
	mBFSteps = stepsNum;
	mBFitNum = iterNum;
}

double SLS::doCalibration(const int &N, Element &bestElement)
{
	double f;
	f = mCal->doCalStep(N, bestElement, m0Type);
	double *searchPoint = bestElement.toKoeffs();
	double fLeft, fRight;
	int *paramCounter = new int [2 * N + 5];
	int i, coordNum = 2 * N + 5;
	int it = 0;
	bool stop = 0;
	Element now(N);
	for (i = 0; i < coordNum; i++) {
		paramCounter[i] = 0;
		now.setCoord(searchPoint[i], i);
	}
	while ((!stop) && (it++ < mLim)) {
		stop = 1;
		for (i = 0; i < coordNum; i++) {
			if (paramCounter[i]++ < 3) {
				now.plus(-mStep, i);
				fLeft = countF(now);
				now.plus(2.0 * mStep, i);
				fRight = countF(now);
				if ((fLeft >= f) && (fRight >= f)) {
					now.plus(-mStep, i);
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
					now.plus(-2.0 * mStep, i);
					continue;
				}
				f = fRight;
			}
		}
	}
	bestElement = now;
	delete searchPoint, paramCounter;
	return f;
}

double SLS::countF(Element &point) const
{
	if (!point.checkPos())
		return mMax;
	return mModelSh->countF(point);
}