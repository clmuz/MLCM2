#include "stdafx.h"
#include "Calibration.h"

Calibration::Calibration(const int &calibrationType, MlcmShell *MlcmSh)
{
	mCalType = calibrationType;
	mMlcmSh = MlcmSh;
	mMinGrowth = 0.1;
	switch (mCalType) {
	case 1:
		mNM = new NelderMead (mMlcmSh);
		break;
	case 2:
		mNM = new NelderMead (mMlcmSh);
		break;
	default:
		break;
	}
}

Calibration::Calibration(MlcmShell *MlcmSh)
{
	mMlcmSh = MlcmSh;
	mCalType = -1;
}

Calibration::~Calibration()
{
	delete mNM;
	delete mMlcmSh;
}

void Calibration::setCalibrationType(const int &calType)
{
	if (mCalType == calType)
		return;
	switch (calType) {
	case 1:
		if (mCalType == 2)
			break;
		delete mNM;
		mNM = new NelderMead (mMlcmSh);
		break;
	case 2:
		if (mCalType == 1)
			break;
		delete mNM;
		mNM = new NelderMead (mMlcmSh);
		break;
	}
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
	case 1:
		mNM->changeN(N);
		return mNM->doCalibration(nowElement);
	case 2:
		return mNM->doComplexCalibration(N, nowElement);
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

int Calibration::getMinGrowth() const
{
	return mMinGrowth;
}