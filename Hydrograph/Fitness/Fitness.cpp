#include "stdafx.h"
#include "Fitness.h"

const int Fitness::mMaxClassesNum = 2;

Fitness::Fitness(ModelsShell *modShell) :
	mFitnessType(FT_ASE),
	mValType(FT_ASE),
	mShell(modShell)
{
	mCreatedClasses = new bool [mMaxClassesNum];
	for (int i = 0; i < mMaxClassesNum; i++)
		mCreatedClasses[i] = 0;
	createFitnessFunction(mFitnessType);
	createFitnessFunction(mValType);
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
	delete mCreatedClasses;
}

void Fitness::createFitnessFunction(const fitnessType &fitnType)
{
	if (mCreatedClasses[fitnType])
		return;
	mCreatedClasses[fitnType] = 1;
	switch (fitnType) {
	case FT_MSOF:
		mMsof = new Msof (mShell);
		mMsof->setMeasPerDay(mMeasPerDay);
		mMsof->setRealVal(mRealVal, mGap);
		break;
	case FT_ASE:
		mAse = new AbsoluteSquareError (mShell);
		mAse->setMeasPerDay(mMeasPerDay);
		mAse->setRealVal(mRealVal, mGap);
		break;
	}
}

void Fitness::setDefFitnessType(const fitnessType &fitnType)
{
	if (mFitnessType == fitnType)
		return;
	mFitnessType = fitnType;
	createFitnessFunction(mFitnessType);
}

void Fitness::setValType(const fitnessType &valType)
{
	if (mValType == valType)
		return;
	mValType = valType;
	createFitnessFunction(mValType);
}

double Fitness::getFitness() const
{
	switch (mFitnessType) {
	case FT_MSOF:
		return mMsof->countError();
	case FT_ASE:
		return mAse->countError();
	}
	throw(0);
}

double Fitness::getFitness(const fitnessType &fitnType)
{
	if (!mCreatedClasses[fitnType])
		createFitnessFunction(fitnType);
	switch (fitnType) {
	case FT_MSOF:
		return mMsof->countError();
	case FT_ASE:
		return mAse->countError();
	}
	throw(0);
}

double Fitness::getValFitness() const
{
	switch (mValType) {
	case FT_MSOF:
		return mMsof->countError();
	case FT_ASE:
		return mAse->countError();
	}
	throw(0);
}

double Fitness::getFitness(const double *params)
{
	mShell->changeModelParametrs(params);
	return getFitness();
}

void Fitness::setBegEnd(const int &begDay, const int &endDay)
{
	if ((mDayBeg == begDay) && (mDayEnd == endDay))
		return;
	mDayBeg = begDay;
	mDayEnd = endDay;
	for (int i = 0; i < mMaxClassesNum; i++) {
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

void Fitness::getFitnessTypes(fitnessType &defType, fitnessType &valType) const
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