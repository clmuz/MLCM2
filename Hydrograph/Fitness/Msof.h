#pragma once
#include <vector>
#include "..\Mlcm.h"

using namespace std;

class Msof
{
public:
	explicit Msof(Mlcm *model);
	void setBegEnd(const int &begDay, const int &endDay);
	double countError() const;
	double countError(const vector<double> &modVal) const;
	void getSigmas(double &sigma1to2sq, double &sigma1to3sq) const;
	void getAverages(vector<double> &aveDaily, vector<double> &aveMonthly) const;
	void setMeasPerDay(const int &measPerDay);
	void setRealVal(vector<double> *realDat, const int &gap);
private:
	void countRealVal();
	void countSigmas();
	int mDayBeg, mDayEnd, mMeasPerDay, mGap;
	vector<double> *mRealVal, mAveDaily, mAveMonthly;
	vector<int> mMonth;
	double mSigma1to2sq, mSigma1to3sq, mLowerLim;
	Mlcm *mModel;
};
