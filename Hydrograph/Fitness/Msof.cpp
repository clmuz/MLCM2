#include "stdafx.h"
#include <cmath>
#include "Msof.h"

Msof::Msof(Mlcm *model) :
	mModel(model),
	mLowerLim(1e-10),
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
	vector<double> modDaily, modMonthly, modVal;
	modVal = mModel -> makeRunoff(mDayBeg * mMeasPerDay, (mDayEnd + 1) * mMeasPerDay);
	double sum;
	unsigned int i, j;
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
	j = 0;
	while (30 * i + j < modDaily.size()) {
		sum += modDaily[i * 30 + j++];
	}
	modMonthly.push_back(sum / j);
	double J = 0;
	sum = 0;
	j = (mDayBeg - mGap) * mMeasPerDay;
	for (i = 0; i < modVal.size(); i++)
		sum += pow(modVal[i] - (*mRealVal)[i + j], 2);
	J += sum;
	sum = 0;
	for (i = 0; i < mAveDaily.size(); i++)
		sum += pow(modDaily[i] - mAveDaily[i], 2);
	J += mSigma1to2sq * sum;
	sum = 0;
	for (i = 0; i < mAveMonthly.size(); i++)
		sum += pow(modMonthly[i] - mAveMonthly[i], 2);
	J += mSigma1to3sq * sum;
	return sqrt(J);
}

double Msof::countError(const vector<double> &modVal) const
{
	vector<double> modDaily, modMonthly;
	double sum;
	unsigned int i, j;
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
	j = 0;
	while (30 * i + j < modDaily.size()) {
		sum += modDaily[i * 30 + j++];
	}
	modMonthly.push_back(sum / j);
	double J = 0;
	sum = 0;
	j = (mDayBeg - mGap) * mMeasPerDay;
	for (i = 0; i < modVal.size(); i++)
		sum += pow(modVal[i] - (*mRealVal)[i + j], 2);
	J += sum;
	sum = 0;
	for (i = 0; i < mAveDaily.size(); i++)
		sum += pow(modDaily[i] - mAveDaily[i], 2);
	J += mSigma1to2sq * sum;
	sum = 0;
	for (i = 0; i < mAveMonthly.size(); i++)
		sum += pow(modMonthly[i] - mAveMonthly[i], 2);
	J += mSigma1to3sq * sum;
	return sqrt(J);
}

void Msof::getSigmas(double &sigma1to2sq, double &sigma1to3sq) const
{
	sigma1to2sq = mSigma1to2sq;
	sigma1to3sq = mSigma1to3sq;
}

void Msof::getAverages(vector<double> &aveDaily, vector<double> &aveMonthly) const
{
	aveDaily = mAveDaily;
	aveMonthly = mAveMonthly;
}

void Msof::countRealVal() {
	double sum;
	unsigned int i, j;
	mAveDaily.clear();
	mAveMonthly.clear();
	int tmp = (mDayBeg - mGap) * mMeasPerDay;
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
	while (30 * i + j < mAveDaily.size()) {
		sum += mAveDaily[i * 30 + j++];
	}
	mAveMonthly.push_back(sum / j);
}

void Msof::countSigmas()
{
	double sigma1 = 0, sigma2 = 0, sigma3 = 0, aver = 0;
	unsigned int i;
	for (i = (mDayBeg - mGap) * mMeasPerDay; i < (mDayEnd - mGap + 1) * mMeasPerDay; i++)
		aver += (*mRealVal)[i];
	aver /= (mDayEnd - mDayBeg + 1) * mMeasPerDay;
	for (i = (mDayBeg - mGap) * mMeasPerDay; i < (mDayEnd - mGap + 1) * mMeasPerDay; i++)
		sigma1 += pow((*mRealVal)[i] - aver, 2);
	for (i = 0; i < mAveDaily.size(); i++)
		sigma2 += pow(mAveDaily[i] - aver, 2);
	for (i = 0; i < mAveMonthly.size(); i++)
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
