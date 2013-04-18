#include "stdafx.h"
#include <cmath>
#include "Msof.h"

const double Msof::mLowerLim = 1e-7;

Msof::Msof(ModelsShell *modShell) :
	mShell(modShell),
	mDayBeg(-1),
	mDayEnd(-1)
{ }

void Msof::setBegEnd(const int &begDay, const int &endDay)
{
	mDayBeg = begDay;
	mDayEnd = endDay;
	countRealVal();
	countSigmas();
}

double Msof::countError() const
{
	return countError(mShell -> makeRunoff(mDayBeg, mDayEnd));
}

double Msof::countError(const vector<double> &modVal) const
{
	vector<double> modDaily, modMonthly;
	double sum;
	int i, j;
	for (i = 0; i <= mDayEnd - mDayBeg; i++) {
		sum = 0;
		for (j = 0; j < mMeasPerDay; j++)
			sum += modVal[i * mMeasPerDay + j];
		modDaily.push_back(sum / mMeasPerDay);
	}
	for (i = 0; i < floor((mDayEnd - mDayBeg - 1) / 30); i++) {
		sum = 0;
		for (j = 0; j < 30; j++) {
			sum += modDaily[30 * i + j];
		}
		modMonthly.push_back(sum / 30);
	}
	sum = 0;
	unsigned int ui = i, uj = 0;
	while (30 * ui + uj < modDaily.size()) {
		sum += modDaily[ui * 30 + uj++];
	}
	modMonthly.push_back(sum / uj);
	double J = 0;
	sum = 0;
	uj = (mDayBeg + mGap) * mMeasPerDay;
	for (ui = 0; ui < modVal.size(); ui++)
		sum += pow(modVal[ui] - (*mRealVal)[ui + uj], 2);
	J += sum / modVal.size();
	sum = 0;
	for (ui = 0; ui < mAveDaily.size(); ui++)
		sum += pow(modDaily[ui] - mAveDaily[ui], 2);
	J += mSigma1to2sq * (sum / modDaily.size());
	sum = 0;
	for (ui = 0; ui < mAveMonthly.size(); ui++)
		sum += pow(modMonthly[ui] - mAveMonthly[ui], 2);
	J += mSigma1to3sq * (sum / modMonthly.size());
	return sqrt(J);
}

void Msof::countRealVal() {
	double sum;
	int i, j;
	mAveDaily.clear();
	mAveMonthly.clear();
	int tmp = (mDayBeg + mGap) * mMeasPerDay;
	for (i = 0; i <= mDayEnd - mDayBeg; i++) {
		sum = 0;
		for (j = 0; j < mMeasPerDay; j++)
			sum += (*mRealVal)[i * mMeasPerDay + j + tmp];
		mAveDaily.push_back(sum / mMeasPerDay);
	}
	for (i = 0; i < floor((mDayEnd - mDayBeg - 1) / 30); i++) {
		sum = 0;
		for (j = 0; j < 30; j++) {
			sum += mAveDaily[30 * i + j];
		}
		mAveMonthly.push_back(sum / 30);
	}
	sum = 0;
	j = 0;
	while ((unsigned int) (30 * i + j) < mAveDaily.size()) {
		sum += mAveDaily[i * 30 + j++];
	}
	mAveMonthly.push_back(sum / j);
}

void Msof::countSigmas()
{
	double sigma1 = 0, sigma2 = 0, sigma3 = 0, aver = 0;
	for (int i = (mDayBeg - mGap) * mMeasPerDay; i < (mDayEnd - mGap + 1) * mMeasPerDay; i++)
		aver += (*mRealVal)[i];
	aver /= (mDayEnd - mDayBeg + 1) * mMeasPerDay;
	for (int i = (mDayBeg - mGap) * mMeasPerDay; i < (mDayEnd - mGap + 1) * mMeasPerDay; i++)
		sigma1 += pow((*mRealVal)[i] - aver, 2);
	for (unsigned int i = 0; i < mAveDaily.size(); i++)
		sigma2 += pow(mAveDaily[i] - aver, 2);
	for (unsigned int i = 0; i < mAveMonthly.size(); i++)
		sigma3 += pow(mAveMonthly[i] - aver, 2);
	sigma1 /= (mDayEnd - mDayBeg + 1) * mMeasPerDay;
	sigma2 /= mAveDaily.size();
	sigma3 /= mAveMonthly.size();
	if (sigma2 > mLowerLim)
		mSigma1to2sq = sigma1 / sigma2;
	else
		mSigma1to2sq = 1;
	if (sigma3 > mLowerLim)
		mSigma1to3sq = sigma1 / sigma3;
	else
		mSigma1to3sq = 1;
}

void Msof::setMeasPerDay(const int &measPerDay)
{
	mMeasPerDay = measPerDay;
}

void Msof::setRealVal(vector<double> *realDat, const int &gap)
{
	mRealVal = realDat;
	mGap = gap;
}
