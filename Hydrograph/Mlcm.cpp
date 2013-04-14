#include "stdafx.h"
#include <fstream>
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
	mAlpha0(100),
	mWarmingSteps(0),
	mMinC(0.5),
	mMaxC(1)
{
	mClick = new int (0);
	mTime = new double [10];
	mAlpha = new double [11];
	mZ = new double [10];
	mMaxA = new double [11];
	int i;
	for (i = 0; i < 11; i++) {
		mAlpha[i] = 100;
		mMaxA[i] = 100;
	}
	mMaxZ = new double [10];
	for (i = 0; i < 10; i++) {
		mZ[i] = 100;
		mMaxZ[i] = 100;
	}
}

Mlcm::~Mlcm()
{
	delete mClick, mMaxA, mMaxZ, mTime, mAlpha, mZ;
}

void Mlcm::setAslopeAndNuh(const double &Aslope, const int &nuh)
{
	mAslope = Aslope;
	mNuh = nuh;
	mNOrd = mNuh + ceil(mT);
	makeHydrOrd();
}

void Mlcm::setParam(const double *params)
{
	mAlpha0 = mMaxA[0] * params[0];
	mC = mMinC + (mMaxC - mMinC) * params[1];
	mK = 1 + 19 * params[2];
	mEtta = 1 + 4 * params[3];
	mT = 24 * params[4];
	mNOrd = mNuh + ceil(mT);
	if (mAlpha0 > mMin)
		mTime0 = ceil((double) mAslope / mAlpha0);
	else 
		mTime0 = 1000000;
	for (int i = 0; i < mN; i++) {
		mAlpha[i] = mMaxA[i + 1] * params[5 + 2 * i];
		if (mAlpha[i] > mMin)
			mTime[i] = (double) mAslope / mAlpha[i];
		else
			mTime[i] = 1e10;
		mZ[i] = mMaxZ[i] * params[6 + 2 * i];
	}
	makeHydrOrd();
}

void Mlcm::setPandET(vector<double> *P, vector<double> *ET)
{
	mP = P;
	mET = ET;
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
	int WarmingBeg = max (timeBeg - mWarmingSteps, 0);
	for (i = WarmingBeg; i < timeEnd; i++) {
		Qsum.push_back(makeStep((*mP)[i], (*mET)[i], i, waterQueue, state));
	}
	vector<double> Q;
	int a2 = min(mRealBeg + 1, timeEnd);
	int a3 = max(timeBeg, a2);
	int a4 = min(timeEnd, mRealEnd + 1);
	for (i = timeBeg; i < a2; i++) {
		Q.push_back(mC * countUhT(Qsum, i - WarmingBeg));
	}
	for (i = a3; i < a4; i++) {
		Q.push_back(mC * countUhT(Qsum, i - WarmingBeg) + (1 - mC) * (*mRealData)[i - 1 - mRealBeg]);
	}
	for (i = max(timeBeg, a4); i < timeEnd; i++) {
		Q.push_back(mC * countUhT(Qsum, i - WarmingBeg));
	}
	return Q;
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

void Mlcm::setWarmingSteps(const int &countOfWarmingSteps)
{
	if (countOfWarmingSteps >= 0)
		mWarmingSteps = countOfWarmingSteps;
}

int Mlcm::getWarmingSteps() const
{
	return mWarmingSteps;
}

void Mlcm::setMaxAandZ(const int *maxA, const int *maxZ)
{
	int i;
	for (i = 0; i < 11; i++)
		mMaxA[i] = maxA[i];
	for (i = 0; i < 10; i++)
		mMaxZ[i] = maxZ[i];
}

void Mlcm::setCLim(const double &minC, const double &maxC)
{
	if (minC > maxC)
		return;
	if ((minC >= 0) && (minC <= 1))
		mMinC = minC;
	if ((maxC >= 0) && (maxC <= 1))
		mMaxC = maxC;
}

void Mlcm::getMaxAandZ(int *maxA, int *maxZ) const
{
	int i;
	for (i = 0; i < 11; i++)
		maxA[i] = mMaxA[i];
	for (i = 0; i < 10; i++)
		maxZ[i] = mMaxZ[i];
}

void Mlcm::getCLim(double &minC, double &maxC) const
{
	minC = mMinC;
	maxC = mMaxC;
}

void Mlcm::setN(const int &n)
{
	mN = n;
}

int Mlcm::getN() const {
	return mN;
}

void Mlcm::printParams(const wchar_t *outputParamFile) const
{
	ofstream fout(outputParamFile, ios::out);
	if (!fout)
		throw(3);
	fout << mN << endl << mAlpha0 << endl << mC << endl << mK << endl << mEtta << endl << mT;
	for (int i = 0; i < mN; i++)
		fout << endl << mAlpha[i] << " " << mZ[i];
	fout.close();
}

void Mlcm::loadParams(const wchar_t *inputParamFile)
{
	ifstream fin(inputParamFile, ios::in);
	if (!fin)
		throw(3);
	fin >> mN >> mAlpha0 >> mC >> mK >> mEtta >> mT;
	mNOrd = mNuh + ceil(mT);
	if (mAlpha0 > mMin)
		mTime0 = ceil((double) mAslope / mAlpha0);
	else 
		mTime0 = 1000000;
	for (int i = 0; i < mN; i++) {
		fin >> mAlpha[i] >> mZ[i];
		if (mAlpha[i] > mMin)
			mTime[i] = ceil((double) mAslope / mAlpha[i]);
		else
			mTime[i] = 1000000;
	}
	fin.close();

}