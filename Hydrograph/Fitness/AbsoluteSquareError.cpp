#include "stdafx.h"
#include <cmath>
#include "AbsoluteSquareError.h"

AbsoluteSquareError::AbsoluteSquareError(ModelsShell *modShell) :
	mShell(modShell),
	mDayBeg(-1),
	mDayEnd(-1)
{ }

double AbsoluteSquareError::countError() const
{
	int beg = mDayBeg * mMeasPerDay;
	vector<double> modVal;
	modVal = mShell -> makeRunoff(mDayBeg, mDayEnd);
	double sum = 0;
	for (int i = 0; i < modVal.size(); i++)
		sum += pow(modVal[i] - (*mRealVal)[i + beg], 2);
	return sqrt(sum / modVal.size());
}

double AbsoluteSquareError::countError(const vector<double> &modVal) const
{
	int beg = (mGap + mDayBeg) * mMeasPerDay;
	double sum = 0;
	for (int i = 0; i < modVal.size(); i++)
		sum += pow(modVal[i] - (*mRealVal)[i + beg], 2);
	return sqrt(sum / modVal.size());
}

void AbsoluteSquareError::setBegEnd(const int &begDay, const int &endDay)
{
	mDayBeg = begDay;
	mDayEnd = endDay;
}

void AbsoluteSquareError::setMeasPerDay(const int &measPerDay)
{
	mMeasPerDay = measPerDay;
}

void AbsoluteSquareError::setRealVal(vector<double> *realDat, const int &gap)
{
	mRealVal = realDat;
	mGap = gap;
}