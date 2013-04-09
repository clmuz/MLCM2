#include "stdafx.h"
#include "Hydrograph.h"

Hydrograph::Hydrograph()
{
	mMlcmSh = new MlcmShell();
	mCal = new Calibration(mMlcmSh);
}

Hydrograph::~Hydrograph()
{
	delete mCal;
}

void Hydrograph::setCalibrationType(const int &calType)
{
	mCal->setCalibrationType(calType);
}

void Hydrograph::setFitnessType(const int &fitnDefType, const int &valType)
{
	mMlcmSh->setDefFitnessType(fitnDefType);
	mMlcmSh->setValType(valType);
}

void Hydrograph::setOutFormat(const int &outFormat)
{
	mMlcmSh->setOutFormat(outFormat);
}

void Hydrograph::setMaxAandZ(const int *maxA, const int *maxZ)
{
	mMlcmSh->setMaxAandZ(maxA, maxZ);
}

void Hydrograph::setNMStopAndLim(const double &NMStop, const int &NMLim)
{
	mCal->setNMStopAndLim(NMStop, NMLim);
}

void Hydrograph::setNMKoeffs(const double &koeff1, const double &koeff2)
{
	mCal->setNMKoeffs(koeff1, koeff2);
}

void Hydrograph::setNMKoeffs(const std::vector<double> koeffs)
{
	mCal->setNMKoeffs(koeffs);
}

void Hydrograph::setMinGrowth(const double minGrowth)
{
	mCal->setMinGrowth(minGrowth);
}

void Hydrograph::setCLim(const double &c1, const double &c2)
{
	mMlcmSh->setCLim(c1, c2);
}

void Hydrograph::getCLim(double &c1, double &c2) const
{
	mMlcmSh->getCLim(c1, c2);
}

void Hydrograph::setBruteforceParams(const int &steps, const int &it)
{
	mCal->setBFParams(steps, it);
}

void Hydrograph::loadMlcm(const char *paramFile)
{
	mMlcmSh->loadParametrs(paramFile);
}

void Hydrograph::printMlcm(const char *outputParamFile)
{
	mMlcmSh->printParams(outputParamFile);
}

void Hydrograph::printPrediction(const int *begDate, const int *endDate)
{
	mMlcmSh->printPrediction(begDate, endDate);
}

double Hydrograph::printPredAndValid(const int *begDate, const int *endDate, const int &fitnessType)
{
	mMlcmSh->setFitnessBegEnd(begDate, endDate);
	return mMlcmSh->printPrediction(begDate, endDate, fitnessType);
}

double Hydrograph::calibrate(const int *begDate, const int *endDate)
{
	mMlcmSh->setFitnessBegEnd(begDate, endDate);
	return mCal->doCalibration();
}

double Hydrograph::validate(const int *begDate, const int *endDate)
{
	mMlcmSh->setFitnessBegEnd(begDate, endDate);
	return mMlcmSh->getValFitness();
}

void Hydrograph::getCalAndFitnessTypes(int &calType, int &defFitnType, int &valType) const
{
	calType = mCal->getCalType();
	mMlcmSh->getFitnessTypes(defFitnType, valType);
}

int Hydrograph::getOutFormat() const
{
	return mMlcmSh->getOutFormat();
}

void Hydrograph::getMaxAandZ(int *maxA, int *maxZ) const
{
	mMlcmSh->getMaxAandZ(maxA, maxZ);
}

void Hydrograph::getBruteforceParams(int &steps, int &iter) const
{
	mCal->getBFParams(steps, iter);
}

void Hydrograph::getNMParams(double &stop, int &iter) const
{
	mCal->getNMParams(stop, iter);
}

int Hydrograph::getNMKoeffs(std::vector<double> &koeffs) const
{
	return mCal->getNMKoeffs(koeffs);
}

void Hydrograph::getSlsParams(double &slsStep, int &slsLim, int &slsCalType) const
{
	mCal->getSlsParams(slsStep, slsLim, slsCalType);
}

void Hydrograph::setSlsParam(const double &slsStep, const int &slsLim, const int &slsCalType)
{
	mCal->setSlsParam(slsStep, slsLim, slsCalType);
}

double Hydrograph::getMinGrowth() const
{
	return mCal->getMinGrowth();
}

void Hydrograph::setOutFile(char *outFileName)
{
	mMlcmSh->setOutFile(outFileName);
}

void Hydrograph::readDeck(const char *filename)
{
	mMlcmSh->readDeck(filename);
}

void Hydrograph::readPcp(const double &format, const char *filename)
{
	mMlcmSh->readPcp(format, filename);
}

void Hydrograph::readDat(const double &format, const char *filename)
{
	mMlcmSh->readDat(format, filename);
}

int Hydrograph::click()
{
	return mMlcmSh->click();
}

void Hydrograph::setHeatDays(const int &countOfHeatDays)
{
	mMlcmSh->setHeatDays(countOfHeatDays);
}

int Hydrograph::getHeatDays() const
{
	return mMlcmSh->getHeatDays();
}