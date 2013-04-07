#include "stdafx.h"
#include "BruteForce.h"

const double BruteForce::mMax = 1e15, BruteForce::mMin = 1e-2;

BruteForce::BruteForce(MlcmShell *modelSh) :
	mModelSh(modelSh),
	mStepsNum(1),
	mIterNum(1)
{ }

void BruteForce::setBFParams(const int &stepsNum, const int &iterNum)
{
	if (stepsNum >= 0)
		mStepsNum = stepsNum;
	if (iterNum > 0)
		mIterNum = iterNum;
}

void BruteForce::getBFParams(int &stepsNum, int &iterNum) const
{
	stepsNum = mStepsNum;
	iterNum = mIterNum;
}

double BruteForce::calibrateLayer(const int &N, Element &best) const
{
	int coordNum = 2 * N + 5;
	double *leftBorder = new double[coordNum];
	int i, j;
	for (i = 0; i < coordNum; i++)
		leftBorder[i] = 0;
	double f = mMax, width = 1;
	double halfStep = width / (2.0 * (mStepsNum + 1));
	calibrateGap(N, f, leftBorder, width, best);
	for (i = 1; i < mIterNum; i++) {
		delete leftBorder;
		leftBorder = best.toKoeffs();
		for (j = 0; j < coordNum; j++) {
			leftBorder[j] = max(mMin, leftBorder[i] - halfStep);
		}
		width = halfStep * 2.0;
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
		double fNow = mModelSh->countF(now);
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