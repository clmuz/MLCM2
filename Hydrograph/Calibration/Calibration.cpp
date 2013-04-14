#include "stdafx.h"
#include "Calibration.h"

Calibration::Calibration(ModelsShell *modShell, Fitness *fitn) :
	mShell(modShell),
	mFitness(fitn)
{
	mCalType = CT_ComplexNelderMead;
	mMinGrowth = 0.1;
	mNM = new NelderMead(mFitness);
	mSLS = new SLS(mFitness, this);
	mBF = new BruteForce(mFitness);
}

Calibration::~Calibration()
{
	delete mNM, mSLS, mBF;
}

void Calibration::setCalibrationType(const calibrationType &calType)
{
	mCalType = calType;
}

void Calibration::setNMStopAndLim(const double &NMStop, const int &NMLim)
{
	mNM->setNMStopAndLim(NMStop, NMLim);
}

void Calibration::setNMKoeffs(const std::vector<double> &koeffs)
{
	mNM->setKoeffs(koeffs);
}

void Calibration::setMinGrowth(const double &minGrowth)
{
	mMinGrowth = minGrowth;
}

void Calibration::setBFParams(const int &stepsNum, const int &iterNum)
{
	mBF->setBFParams(stepsNum, iterNum);
}

void Calibration::getBFParams(int &stepsNum, int &iterNum) const
{
	mBF->getBFParams(stepsNum, iterNum);
}

void Calibration::setSlsParam(const double &slsStep, const int &slsLim, const calibrationType &slsCalType)
{
	mSLS->setParams(slsStep, slsLim, slsCalType);
}

void Calibration::getSlsParams(double &slsStep, int &slsLim, calibrationType &slsCalType) const
{
	mSLS->getParams(slsStep, slsLim, slsCalType);
}

double Calibration::doCalibration()
{
	double *nowParams, *bestParams;
	int nowParamsNum = mShell->getModelParams(0);
	bestParams = new double [nowParamsNum];
	mShell->setModelIter(0);
	int bestIter = 0;
	double best = doCalStep(nowParamsNum, bestParams);
	double now, prev;
	for (int i = 1; i < mShell->getIterNum(); i++) {
		int nowParamsNum = mShell->getModelParams(i);
		nowParams = new double [nowParamsNum];
		mShell->setModelIter(i);
		now = doCalStep(nowParamsNum, nowParams);
		prev = best;
		if (now < best) {
			best = now;
			bestIter = i;
			delete bestParams;
			bestParams = new double [nowParamsNum];
			for (int j = 0; j < nowParamsNum; j++)
				bestParams[j] = nowParams[j];
		}
		if (prev - now < mMinGrowth) {
			break;
		}
	}
	mShell->setModelIter(bestIter);
	mShell->changeModelParametrs(bestParams);
	delete[] bestParams, nowParams;
	return best;
}

void Calibration::setNMKoeffs(const double &koeff1, const double &koeff2)
{
	mNM->setKoeffs(koeff1, koeff2);
}

double Calibration::doCalStep(const int &paramsNum, double *bestParams, calibrationType calType)
{
	if (calType == CT_NULL)
		calType = mCalType;
	switch (calType) {
	case CT_NelderMead:
		return mNM->doCalibration(paramsNum, bestParams);
	case CT_ComplexNelderMead:
		return mNM->doComplexCalibration(paramsNum, bestParams);
	case CT_SLS:
		return mSLS->doCalibration(paramsNum, bestParams);
	case CT_BruteForce:
		return mBF->doCalibration(paramsNum, bestParams);
	}
	throw(0);
}

calibrationType Calibration::getCalType() const
{
	return mCalType;
}

void Calibration::getNMParams(double &stop, int &iter) const
{
	mNM->getNMParams(stop, iter);
}

int Calibration::getNMKoeffs(std::vector<double> &koeffs) const
{
	return mNM->getNMKoeffs(koeffs);
}

double Calibration::getMinGrowth() const
{
	return mMinGrowth;
}