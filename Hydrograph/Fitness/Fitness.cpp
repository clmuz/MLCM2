#include "stdafx.h"
#include "Fitness.h"

const int Fitness::mMaxClassesNum = 2;

Fitness::Fitness(Mlcm *model) :
	mFitnessType(1),
	mValType(1),
	mModel(model)
{
	mCreatedClasses.resize(mMaxClassesNum + 1);
}

Fitness::~Fitness()
{
	for (int i = 0; i < mMaxClassesNum; i++) {
		if(mCreatedClasses[i]) {
			switch(i) {
			case 0:
				delete mMsof;
				break;
			case 1:
				delete mAse;
				break;
			}
		}
	}
}

void Fitness::createFitnessFunction(const int &fitnessType)
{
	if (mCreatedClasses[fitnessType])
		return;
	mCreatedClasses[fitnessType] = 1;
	switch (fitnessType) {
	case 0:
		mMsof = new Msof (mModel);
		mMsof->setMeasPerDay(mMeasPerDay);
		mMsof->setRealVal(mRealVal, mGap);
		break;
	case 1:
		mAse = new AbsoluteSquareError (mModel);
		mAse->setMeasPerDay(mMeasPerDay);
		mAse->setRealVal(mRealVal, mGap);
		break;
	}
}

void Fitness::setDefFitnessType(const int &fitnessType)
{
	if (mFitnessType == fitnessType)
		return;
	mFitnessType = fitnessType;
	createFitnessFunction(mFitnessType);
}

void Fitness::setValType(const int &valType)
{
	if (mValType == valType)
		return;
	mValType = valType;
	createFitnessFunction(mValType);
}

double Fitness::getFitness() const
{
	switch (mFitnessType) {
	case 0:
		return mMsof->countError();
	case 1:
		return mAse->countError();
	}
	return 0;
}

double Fitness::getFitness(const int &fitnessType)
{
	if (!mCreatedClasses[fitnessType])
		createFitnessFunction(fitnessType);
	switch (fitnessType) {
	case 0:
		return mMsof->countError();
	case 1:
		return mAse->countError();
	}
	return 0;
}

double Fitness::getFitness(const int &fitnessType, const vector<double> &modVal)
{
	if (!mCreatedClasses[fitnessType])
		createFitnessFunction(fitnessType);
	switch (fitnessType) {
	case 0:
		return mMsof->countError(modVal);
	case 1:
		return mAse->countError(modVal);
	}
	return 0;
}

double Fitness::getValFitness() const
{
	switch (mValType) {
	case 0:
		return mMsof->countError();
	case 1:
		return mAse->countError();
	}
	return 0;
}

double Fitness::getValFitness(const vector<double> &modVal) const
{
	switch (mValType) {
	case 0:
		return mMsof->countError(modVal);
	case 1:
		return mAse->countError(modVal);
	}
	return 0;
}

void Fitness::setBegEnd(const unsigned int &begDay, const unsigned int &endDay)
{
	if ((mDayBeg == begDay) && (mDayEnd == endDay))
		return;
	mDayBeg = begDay;
	mDayEnd = endDay;
	for (int i = 0; i <= mMaxClassesNum; i++) {
		if (mCreatedClasses[i]) {
			switch (i) {
			case 0:
				mMsof->setBegEnd(begDay, endDay);
				break;
			case 1:
				mAse->setBegEnd(begDay, endDay);
				break;
			}
		}
	}
}

void Fitness::getFitnessTypes(int &defType, int &valType) const
{
	defType = mFitnessType;
	valType = mValType;
}

void Fitness::setMeasPerDay(const int &measPerDay)
{
	mMeasPerDay = measPerDay;
	for (int i = 0; i < mMaxClassesNum; i++) {
		if (mCreatedClasses[i]) {
			switch (i) {
			case 0:
				mMsof->setMeasPerDay(measPerDay);
				break;
			case 1:
				mAse->setMeasPerDay(measPerDay);
				break;
			}
		}
	}
}

void Fitness::setRealVal(vector<double> *realDat, const int &gap)
{
	mRealVal = realDat;
	mGap = gap;
	for (int i = 0; i < mMaxClassesNum; i++) {
		if (mCreatedClasses[i]) {
			switch (i) {
			case 0:
				mMsof->setRealVal(realDat, gap);
				break;
			case 1:
				mAse->setRealVal(realDat, gap);
				break;
			}
		}
	}
}