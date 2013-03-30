#pragma once
#include "..\Mlcm.h"
#include "Msof.h"
#include "AbsoluteSquareError.h"

class Fitness
{
public:
	explicit Fitness(Mlcm *model);
	~Fitness();
	void createFitnessFunction(const int &fitnessType);
	void setDefFitnessType(const int &fitnessType);
	void setValType(const int &valType);
	void setMeasPerDay(const int &measPerDay);
	void setRealVal(vector<double> *realDat, const int &gap);
	double getFitness() const;
	double getFitness(const int &fitnessType);
	double getFitness(const int &fitnessType, const vector<double> &modVal);
	double getValFitness() const;
	double getValFitness(const vector<double> &modVal) const;
	void setBegEnd(const unsigned int &begDay, const unsigned int &endDay);
	void getFitnessTypes(int &defType, int &valType) const;
private:
	int mFitnessType, mValType, mMaxClassesNum;
	int mMeasPerDay, mGap;
	vector<double> *mRealVal;
	vector<bool> mCreatedClasses;
	int mDayBeg, mDayEnd;
	Mlcm *mModel;
	Msof *mMsof;
	AbsoluteSquareError *mAse;
};
