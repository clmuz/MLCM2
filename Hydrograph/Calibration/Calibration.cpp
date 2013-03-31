#include "stdafx.h"
#include "Calibration.h"

Calibration::Calibration(const int &calibrationType, MlcmShell *MlcmSh)
{
	mCalType = calibrationType;
	mMlcmSh = MlcmSh;
	mMinGrowth = 0.1;
	mNM = new NelderMead(mMlcmSh);
	mBF = new BruteForce(mMlcmSh);
}

Calibration::Calibration(MlcmShell *MlcmSh)
{
	mMlcmSh = MlcmSh;
	mCalType = 0;
	mMinGrowth = 0.1;
	mNM = new NelderMead(mMlcmSh);
	mBF = new BruteForce(mMlcmSh);
}

Calibration::~Calibration()
{
	delete mNM;
	delete mMlcmSh;
}

void Calibration::setCalibrationType(const int &calType)
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

double Calibration::doCalibration()
{
	Element nowElement, bestElement;
	double best = doCalStep(0, bestElement);
	double now;
	for (int i = 1; i <= 10; i++) {
		now = doCalStep(i, nowElement);
		if (now < best) {
			best = now;
			bestElement = nowElement;
		}
		if (best - now < mMinGrowth) {
			mMlcmSh->changeModelParametrs(bestElement);
			return best;
		}
	}
	mMlcmSh->changeModelParametrs(bestElement);
	return best;
}

void Calibration::setNMKoeffs(const double &koeff1, const double &koeff2)
{
	mNM->setKoeffs(koeff1, koeff2);
}

double Calibration::doCalStep(const int &N, Element &nowElement)
{
	switch (mCalType) {
	case 0:
		mNM->changeN(N);
		return mNM->doCalibration(nowElement);
	case 1:
		return mNM->doComplexCalibration(N, nowElement);
	case 3:
		return mBF->calibrateLayer(N, nowElement);
	}
	return -1;
}

int Calibration::getCalType() const
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