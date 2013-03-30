#include "stdafx.h"
#include "BruteForce.h"

BruteForce::BruteForce(MlcmShell *modelSh)
{
	mMlcmSh = modelSh;
	mStepsNum = 1;
	mIterNum = 1;
	mMax = 1e15;
	mMin = 1e-2;
}

void BruteForce::setBFParams(const int &stepsNum, const int &iterNum)
{
	mStepsNum = stepsNum;
	mIterNum = iterNum;
}

void BruteForce::getBFParams(int &stepsNum, int &iterNum) const
{
	stepsNum = mStepsNum;
	iterNum = mIterNum;
}

double BruteForce::calibrate0(Element &best) const
{
	Element newBest(0);
	best = newBest;
	return calibrateLayer(0, best);
}

double BruteForce::calibrateLayer(const int &N, Element &best) const
{
	Element now(N);
	int coordNum = 2 * N + 5;
	double *leftBorder = new double[coordNum];
	int i, j;
	for (i = 0; i < coordNum; i++)
		leftBorder[i] = 0;
	double f = mMax, width = 1;
	double step = width / (2.0 * (mStepsNum + 1));
	calibrateGap(0, f, leftBorder, width, best);
	for (i = 1; i < mIterNum; i++) {
		delete leftBorder;
		leftBorder = best.toKoeffs();
		for (j = 0; j < coordNum; j++) {
			leftBorder[j] = max(mMin, leftBorder[i] - step);
		}
		width = step;
		calibrateGap(N, f, leftBorder, width, best);
	}
	return f;
}

void BruteForce::calibrateGap(const int &N, double &f, const double *leftBorder, const double &width, Element &best) const
{
	double step = width / (mStepsNum + 1);
	int coordNum = 2 * N + 5;
	Element now(N);
	for (int i = 0; i < coordNum; i++) {
		now.setCoord(leftBorder[i], i);
	}
	recursion(f, 0, coordNum, step, leftBorder, now, best);
}

void BruteForce::recursion(double &f
						   , const int &coord
						   , const int &coordNum
						   , const double &step
						   , const double *leftBorder
						   , Element now
						   , Element &best) const
{
	if (coord == coordNum) {
		double fNow = countF(now);
		if (fNow < f) {
			best = now;
			f = fNow;
		}
		return;
	}
	now.setCoord(leftBorder[coord], coord);
	for (int i = 0; i <= mStepsNum; i++) {
		recursion(f, coord + 1, coordNum, step, leftBorder, now, best);
		now.plus(step, coord);
	}
}

double BruteForce::countF(const Element &point) const
{
	mMlcmSh->changeModelParametrs(point);
	return mMlcmSh->getFitness();
}