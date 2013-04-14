#include "stdafx.h"
#include "BruteForce.h"

const double BruteForce::mMax = 1e15, BruteForce::mMin = 1e-2;

BruteForce::BruteForce(Fitness *fitn) :
	mFitness(fitn),
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

double BruteForce::doCalibration(const int &paramsNum, double *bestParams) const
{
	double *leftBorder = new double[paramsNum];
	int i, j;
	for (i = 0; i < paramsNum; i++)
		leftBorder[i] = 0;
	double f = mMax, width = 1;
	double halfStep = width / (2.0 * (mStepsNum + 1));
	calibrateGap(paramsNum, f, leftBorder, width, bestParams);
	for (i = 1; i < mIterNum; i++) {
		for (j = 0; j < paramsNum; j++) {
			leftBorder[j] = max(mMin, bestParams[j] - halfStep);
		}
		width = halfStep * 2.0;
		calibrateGap(paramsNum, f, leftBorder, width, bestParams);
	}
	delete[] leftBorder;
	return f;
}

void BruteForce::calibrateGap(const int &paramsNum, double &f, const double *leftBorder, const double &width, double *bestParams) const
{
	double step = width / (mStepsNum + 1);
	double *now = new double [paramsNum];
	for (int i = 0; i < paramsNum; i++) {
		now[i] = leftBorder[i];
	}
	recursion(f, 0, paramsNum, step, leftBorder, now, bestParams);
}

void BruteForce::recursion(double &f
						   , const int &coord
						   , const int &paramsNum
						   , const double &step
						   , const double *leftBorder
						   , const double *nowParams
						   , double *bestParams) const
{
	int i;
	if (coord == paramsNum) {
		double fNow = mFitness->getFitness(nowParams);
		if (fNow < f) {
			for (i = 0; i < paramsNum; i++)
				bestParams[i] = nowParams[i];
			f = fNow;
		}
		return;
	}
	double *nowRecParams = new double [paramsNum];
	for (i = 0; i < paramsNum; i++)
		nowRecParams[i] = nowParams[i];
	nowRecParams[coord] = leftBorder[coord];
	for (int i = 0; i <= mStepsNum; i++) {
		recursion(f, coord + 1, paramsNum, step, leftBorder, nowRecParams, bestParams);
		nowRecParams[coord] += step;
	}
	delete[] nowRecParams;
}