#include "stdafx.h"
#include "Hydrograph.h"

Hydrograph::Hydrograph()
{
	mShell = new ModelsShell();
	mFitness = new Fitness(mShell);
	mShell->setFitnessClass(mFitness);
	mCal = new Calibration(mShell, mFitness);
}

Hydrograph::~Hydrograph()
{
	delete mCal, mFitness, mShell;
}

void Hydrograph::setCalibrationType(const calibrationType &calType)
{
	mCal->setCalibrationType(calType);
}

void Hydrograph::setFitnessType(const fitnessType fitnDefType, const fitnessType valType)
{
	mFitness->setDefFitnessType(fitnDefType);
	mFitness->setValType(valType);
}

void Hydrograph::setOutFormat(const int outFormat)
{
	mShell->setOutFormat(outFormat);
}

void Hydrograph::setMaxAandZ(const double *maxA, const double *maxZ)
{
	mShell->setMaxAandZ(maxA, maxZ);
}

void Hydrograph::setNMStopAndLim(const double NMStop, const int NMLim)
{
	mCal->setNMStopAndLim(NMStop, NMLim);
}

void Hydrograph::setNMKoeffs(const double koeff1, const double koeff2)
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

void Hydrograph::setCLim(const double minC, const double maxC)
{
	mShell->setCLim(minC, maxC);
}

void Hydrograph::getCLim(double &minC, double &maxC) const
{
	mShell->getCLim(minC, maxC);
}

void Hydrograph::setBruteforceParams(const int steps, const int it)
{
	mCal->setBFParams(steps, it);
}

void Hydrograph::loadMlcm(const wchar_t *paramFile)
{
	mShell->loadParams(paramFile);
}

void Hydrograph::printMlcm(const wchar_t *outputParamFile)
{
	mShell->printParams(outputParamFile);
}

void Hydrograph::printPrediction(const int *begDate, const int *endDate)
{
	mShell->printPrediction(begDate, endDate);
}

double Hydrograph::printPredAndValid(const int *begDate, const int *endDate, const fitnessType fitnessType)
{
	mShell->setFitnessBegEnd(begDate, endDate);
	return mShell->printPrediction(begDate, endDate, fitnessType);
}

double Hydrograph::calibrate(const int *begDate, const int *endDate)
{
	mShell->setFitnessBegEnd(begDate, endDate);
	return mCal->doCalibration();
}

double Hydrograph::validate(const int *begDate, const int *endDate)
{
	mShell->setFitnessBegEnd(begDate, endDate);
	return mFitness->getValFitness();
}

void Hydrograph::getCalAndFitnessTypes(calibrationType &calType, fitnessType &defFitnType, fitnessType &valType) const
{
	calType = mCal->getCalType();
	mFitness->getFitnessTypes(defFitnType, valType);
}

int Hydrograph::getOutFormat() const
{
	return mShell->getOutFormat();
}

void Hydrograph::getMaxAandZ(double *maxA, double *maxZ) const
{
	mShell->getMaxAandZ(maxA, maxZ);
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

void Hydrograph::getSlsParams(double &slsStep, int &slsLim, calibrationType &slsCalType) const
{
	mCal->getSlsParams(slsStep, slsLim, slsCalType);
}

void Hydrograph::setSlsParam(const double slsStep, const int slsLim, const calibrationType slsCalType)
{
	mCal->setSlsParam(slsStep, slsLim, slsCalType);
}

double Hydrograph::getMinGrowth() const
{
	return mCal->getMinGrowth();
}

void Hydrograph::setOutFile(const wchar_t *outFileName)
{
	mShell->setOutFile(outFileName);
}

void Hydrograph::readDeck(const double fbasinFormat, const double etFormat, const wchar_t *filename)
{
	mShell->readDeck(fbasinFormat, etFormat, filename);
}

void Hydrograph::readPcp(const double pcpFormat, const wchar_t *filename)
{
	mShell->readPcp(pcpFormat, filename);
}

void Hydrograph::readDat(const double qFormat, const wchar_t *filename)
{
	mShell->readDat(qFormat, filename);
}

int Hydrograph::click()
{
	return mShell->click();
}

void Hydrograph::setWarmingDays(const int countOfWarmingDays)
{
	mShell->setWarmingDays(countOfWarmingDays);
}

int Hydrograph::getWarmingDays() const
{
	return mShell->getWarmingDays();
}