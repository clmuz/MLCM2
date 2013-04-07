#include "stdafx.h"
#include <cmath>
#include "Gamma.h"
#include "Mlcm.h"

const double Mlcm::mMin = 1e-3;

Mlcm::Mlcm() :
	mRealBeg(0),
	mRealEnd(0),
	mN(0),
	mC(0.5),
	mK(2),
	mEtta(2),
	mT(0),
	mAlpha0(100)
{
	mClick = new int (0);
	mTime = new double [10];
}

Mlcm::~Mlcm()
{
	delete mClick;
}

void Mlcm::setAslopeAndNuh(const double &Aslope, const int &nuh)
{
	mAslope = Aslope;
	mNuh = nuh;
	mNOrd = mNuh + ceil(mT);
	makeHydrOrd();
}

void Mlcm::setParam(const Element &element)
{
	mN = element.getN();
	mC = element.getC();
	mK = element.getK();
	mEtta = element.getEtta();
	mT = element.getT();
	mNOrd = mNuh + ceil(mT);
	mAlpha0 = element.getAlpha0();
	if (mAlpha0 > mMin)
		mTime0 = ceil((double) mAslope / mAlpha0);
	else 
		mTime0 = 1000000;
	mAlpha = element.getAlphas();
	for (int i = 0; i < mN; i++) {
		if (mAlpha[i] > mMin)
			mTime[i] = (double) mAslope / mAlpha[i];
		else
			mTime[i] = 1e10;
	}
	mZ = element.getZ();
	makeHydrOrd();
}

void Mlcm::setPandET(vector<double> *P, vector<double> *ET)
{
	mP = P;
	mET = new vector<double>;
	for (int i = 0; i < ET->size(); i++)
		mET->push_back((*ET)[i]);
}

void Mlcm::setRealData(vector<double> *realData, const int &realDatBeg)
{
	mRealData = realData;
	mRealBeg = realDatBeg;
	mRealEnd = mRealBeg + mRealData->size();
}

int Mlcm::click()
{
	int tmp = *mClick;
	*mClick = 0;
	return tmp;
}

vector<double> Mlcm::makeRunoff(const int &timeBeg, const int &timeEnd) const
{
	(*mClick)++;
	queue<Water> waterQueue[11];
	vector<double> state (mN + 1, 0);
	vector<double> Qsum;
	int i;
	for (i = timeBeg; i < timeEnd; i++) {
		Qsum.push_back(makeStep((*mP)[i], (*mET)[i], i, waterQueue, state));
	}
	vector<double> Q;
	int a2 = min(mRealBeg + 1, timeEnd);
	int a3 = max(timeBeg, a2);
	int a4 = min(timeEnd, mRealEnd + 1);
	for (i = timeBeg; i < a2; i++) {
		Q.push_back(mC * countUhT(Qsum, i - timeBeg));
	}
	for (i = a3; i < a4; i++) {
		Q.push_back(mC * countUhT(Qsum, i - timeBeg) + (1 - mC) * (*mRealData)[i - 1 - mRealBeg]);
	}
	for (i = max(timeBeg, a4); i < timeEnd; i++) {
		Q.push_back(mC * countUhT(Qsum, i - timeBeg));
	}
	return Q;
}

Element Mlcm::getParam() const
{
	Element nElement(mN, mAlpha0, mC, mK, mEtta, mT, mAlpha, mZ);
	return nElement;
}

void Mlcm::makeHydrOrd()
{
	mFxOrd.clear();
	double koeff = pow(mEtta, mK) * Gamma(mK), tmp;
	for (int i = 1; i <= mNOrd; i++) {
		if (i > mT) {
			tmp = pow(i - mT, mK - 1) * exp((-i + mT)/mEtta) / koeff;
			mFxOrd.push_back(tmp);
		}
		else
			mFxOrd.push_back(0);
	}
	double sum = 0;
	for (int i = 0; i < mFxOrd.size(); i++) {
		sum += mFxOrd[i];
	}
}

double Mlcm::countUhT(const vector<double> &Qsum, const int &time) const
{
	double sum = 0;
	for (int i = 0; i <= min(time, mNOrd - 1); i++)
		sum += Qsum[time - i] * mFxOrd[i];
	return sum;
}

double Mlcm::makeStep(const double &P
					  , const double &ET
					  , const int &time
					  , queue<Water> *waterQueue
					  , vector<double> &state) const
{
	if (P <= ET)
		return countChannelWater(time, waterQueue, state);
	double P0 = P - ET;
	if (mN == 0) {
		Water newWater (P0, mTime0 + time - 1);
		waterQueue[0].push(newWater);
	}
	else {
		if (P0 > mAlpha[0]) {
			Water newWater (P0 - mAlpha[0], mTime0 + time - 1);
			waterQueue[0].push(newWater);
			P0 = mAlpha[0];
		}
		double time1 = -1;
		for (int i = 0; i < mN; i++) {
			if (P0 + state[i + 1] < mZ[i]) {
				state[i + 1] += P0;
				Water newWater (P0, ceil(time1 + mTime[i]) + time);
				waterQueue[i + 1].push(newWater);
				break;
			}
			time1 += (mZ[i] - state[i + 1]) * 1.0 / mAlpha[i];
			if (i == mN - 1) {
				state[i + 1] = mZ[i];
				Water newWater (P0, ceil(time1 + mTime[i]) + time);
				waterQueue[i + 1].push(newWater);
				break;
			}
			if (P0 > mAlpha[i + 1]) {
				state[i + 1] = max(0.0, mZ[i] - mAlpha[i + 1]);
				Water newWater (P0 - mAlpha[i + 1], ceil(time1 + mTime[i + 1]) + time);
				waterQueue[i + 1].push(newWater);
				P0 = mAlpha[i + 1];
			}
		}
	}
	return countChannelWater(time, waterQueue, state);
}

double Mlcm::countChannelWater(const int &time, queue<Water> *waterQueue, vector<double> &state) const
{
	double sum = 0, tmp;
	for (int i = 0; i <= mN; i++) {
		while ((!waterQueue[i].empty()) && (waterQueue[i].front().time <= time)) {
			tmp = waterQueue[i].front().water;
			sum += tmp;
			state[i] = max(state[i] - tmp, .0);
			waterQueue[i].pop();
		}
	}
	return sum;
}

Mlcm::Water::Water(const double &waterThikness, const int &newTime) : water(waterThikness), time(newTime) {}
