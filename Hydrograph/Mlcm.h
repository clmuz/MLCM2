#pragma once
#include <vector>
#include <queue>
#include "Element.h"

using namespace std;

class Mlcm
{
public:
	Mlcm();
	~Mlcm();
	void setAslopeAndNuh(const double &Aslope, const int &nuh);
	void setRealData(vector<double> *realData, const int &realDatBeg);
	void setParam(const Element &element);
	void setPandET(vector<double> *P, vector<double> *ET);
	vector<double> makeRunoff(const int &timeBeg, const int &timeEnd) const;
	int click();
	Element getParam() const;
private:
	struct Water {
		Water (const double &waterThikness, const int &newTime);
		double water;
		int time;
	};
	void makeHydrOrd();
	double countUhT(const vector<double> &Qsum, const int &time) const;
	double makeStep(const double &P
					, const double &ET
					, const int &time
					, queue<Water> *waterQueue
					, vector<double> &state) const;
	void toChannel(const double &P, const int &time, const int &layer, queue<Water> *waterQueue) const;
	double countChannelWater(const int &time, queue<Water> *waterQueue, vector<double> &state) const;
	int mN, mNOrd, mNuh, mRealBeg, mRealEnd, *mClick;
	double mC, mEtta, mK, mT, mAlpha0, mAslope;
	vector<double> mFxOrd, mAlpha, mZ, *mP, *mET, *mRealData;
};
