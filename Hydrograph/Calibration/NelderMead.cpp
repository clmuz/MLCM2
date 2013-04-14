#include "stdafx.h"
#include <cmath>
#include "NelderMead.h"

const double NelderMead::mAlpha = 1.0, NelderMead::mBetta = .5, NelderMead::mGamma = 2.0;
const double NelderMead::mMax = 1e15, NelderMead::mMin = 1e-3;

NelderMead::NelderMead(Fitness *fitn) :
	mFitness(fitn),
	mParamsNum(0),
	mE(0.1),
	mLimit(1000),
	mComplexNum(1),
	mKoeff1(0.1),
	mKoeff2(0.2)
{
	mKoeffs.resize(2);
	mKoeffs[0] = 0.1;
	mKoeffs[1] = 0.2;
}

void NelderMead::setNMStopAndLim(const double &NMStop, const int &NMLim)
{
	if (mE > 0)
		mE = NMStop;
	if (mLimit > 0)
		mLimit = NMLim;
}

double NelderMead::doNelderMead(double *bestParams, vector<double*> &simplex) const
{
	int it = 0;
	double *Fs = new double [mParamsNum + 1];
	double *centre = new double [mParamsNum];
	double *expans = new double [mParamsNum];
	double *contr = new double [mParamsNum];
	double *refl = new double [mParamsNum];
	double fl, fg, fh, fr, fc, fe;
	int i, j, l, g, h;
	for (i = 0; i <= mParamsNum; i++)
		Fs[i] = (countF(simplex[i]));
	while ((!isLim(Fs)) && (it++ < mLimit)) {
		//Sort
		fl = mMax;
		l = 0;
		g = 1;
		h = 2;
		for (i = 0; i <= mParamsNum; i++)
			if (Fs[i] < fl) {
				fl = Fs[i];
				l = i;
			}
		swap(l, 0, simplex, Fs);
		fh = fl;
		for (i = 1; i <= mParamsNum; i++)
			if (Fs[i] > fh) {
				fh = Fs[i];
				h = i;
			}
		swap(h, mParamsNum, simplex, Fs);
		fg = fl;
		for (i = 1; i < mParamsNum; i++)
			if ((Fs[i] > fg) && (Fs[i] < fh)) {
				fg = Fs[i];
				g = i;
			}
		swap(g, mParamsNum - 1, simplex, Fs);
		//Mass centre
		for (j = 0; j < mParamsNum; j++)
			centre[j] = 0;
		for (i = 0; i < mParamsNum; i++) {
			for (j = 0; j < mParamsNum; j++) {
				centre[j] += (simplex[i][j]) / (double) mParamsNum;
			}
		}
		//Reflection
		for (j = 0; j < mParamsNum; j++)
			refl[j] = centre[j] * (1.0 + mAlpha) - simplex[mParamsNum][j] * mAlpha;
		fr = countF(refl);
		if (fr < fl) {
			//Expansion
			for (j = 0; j < mParamsNum; j++)
				expans[j] = centre[j] * (1.0 - mGamma) - refl[j] * mGamma;
			fe = countF(expans);
			if (fe < fl) {
				doEqual(simplex[mParamsNum], expans, mParamsNum);
				Fs[mParamsNum] = fe;
			}
			else
			{
				doEqual(simplex[mParamsNum], refl, mParamsNum);
				Fs[mParamsNum] = fr;
			}
		}
		else {
			if (fr < fg) {
				doEqual(simplex[mParamsNum], refl, mParamsNum);
				Fs[mParamsNum] = fr;
			}
			else {
				//Contraction
				if (fr < fh) {
					doEqual(simplex[mParamsNum], refl, mParamsNum);
					Fs[mParamsNum] = fr;
					fh = fr;
				}
				for (j = 0; j < mParamsNum; j++)
					contr[j] = simplex[mParamsNum][j] * mBetta + centre[j] * (1.0 - mBetta);
				fc = countF (contr);
				if (fc < fh) {
					doEqual(simplex[mParamsNum], contr, mParamsNum);
					Fs[mParamsNum] = fc;
				}
				else {
					for (i = 1; i <= mParamsNum; i++) {
						for (j = 0; j < mParamsNum; j++)
							simplex[i][j] = simplex[0][j] + (simplex[i][j] - simplex[0][j]) * 0.5;
						Fs[i] = countF(simplex[i]);
					}
				}
			}
		}
	}
	fl = mMax;
	l = 0;
	for (i = 0; i <= mParamsNum; i++) {
		if (Fs[i] < fl) {
			fl = Fs[i];
			l = i;
		}
	}
	for (i = 0; i < mParamsNum; i++)
		bestParams[i] = simplex[l][i];
	delete[] centre, expans, contr, refl, Fs;
	return fl;
}

double NelderMead::doCalibration(const int &paramsNum, double *nowParams)
{
	mParamsNum = paramsNum;
	vector<double*> simplex (mParamsNum + 1);
	for (int i = 0; i <= mParamsNum; i++)
		simplex[i] = new double [mParamsNum];
	doFirstSimplex(mKoeff1, mKoeff2, simplex);
	double f = doNelderMead(nowParams, simplex);
	for (int i = 0; i <= mParamsNum; i++)
		delete[] simplex[i];
	return f;
}

double NelderMead::doComplexCalibration(const int &paramsNum, double *nowParams)
{
	mParamsNum = paramsNum;
	double *nowPar = new double [mParamsNum], *bestPar = new double [mParamsNum];
	double now, best;
	vector<double*> simplex (mParamsNum + 1);
	int i, j;
	for (i = 0; i <= mParamsNum; i++)
		simplex[i] = new double [mParamsNum];
	doFirstSimplex(mKoeffs[0], mKoeffs[1], simplex);
	best = doNelderMead(nowPar, simplex);
	for (j = 0; j < mParamsNum; j++)
		bestPar[j] = nowPar[j];
	for (i = 1; i < mComplexNum; i++) {
		doFirstSimplex(mKoeffs[2 * i], mKoeffs[2 * i + 1], simplex);
		now = doNelderMead(nowPar, simplex);
		if (now < best) {
			best = now;
			for (j = 0; j < mParamsNum; j++)
				bestPar[j] = nowPar[j];
		}
	}
	for (i = 0; i <= mParamsNum; i++)
		delete[] simplex[i];
	for (i = 0; i < mParamsNum; i++)
		nowParams[i] = bestPar[i];
	delete[] nowPar, bestPar;
	return best;
}

void NelderMead::setKoeffs(const double &koeff1, const double &koeff2)
{
	mKoeff1 = koeff1;
	mKoeff2 = koeff2;
}

void NelderMead::setKoeffs(const vector<double> &koeffs)
{
	mComplexNum = floor(koeffs.size() * 0.5);
	mKoeffs = koeffs;
	if (mComplexNum < 1) {
		mComplexNum = 1;
		mKoeffs.resize(2, 0.1);
	}
}

void NelderMead::doFirstSimplex(const double &koeff1, const double &koeff2, vector<double*> &simplex) const
{
	int i, j;
	for (j = 0; j < mParamsNum; j++) {
		simplex[0][j] = koeff1;
	}
	for (i = 1; i <= mParamsNum; i++) {
		for (j = 0; j < mParamsNum; j++) {
			simplex[i][j] = koeff1;
		}
		simplex[i][i - 1] = koeff2;
	}
}

double NelderMead::countF(const double *param) const
{
	int i;
	for (i = 0; i < mParamsNum; i++) {
		if ((param[i] < mMin) || (param[i] > 1))
			break;
	}
	if (i < mParamsNum)
		return mMax;
	return mFitness->getFitness(param);
}

bool NelderMead::isLim(const double *Fs) const
{
	int i;
	double f = 0;
	for (i = 0; i <= mParamsNum; i++)
		f += Fs[i];
	f /= mParamsNum + 1;
	double sigma = 0;
	for (i = 0; i <= mParamsNum; i++)
		sigma += pow (Fs[i] - f, 2);
	sigma /= mParamsNum + 1;
	sigma = sqrt(sigma);
	if (sigma < mE)
		return 1;
	return 0;
}

void NelderMead::swap(const int &i, const int &j, vector<double*> &simplex, double *Fs) const
{
	if (i == j)
		return;
	double tmpF = Fs[i];
	double *tmpParam = simplex[i];
	Fs[i] = Fs[j];
	simplex[i] = simplex[j];
	Fs[j] = tmpF;
	simplex[j] = tmpParam;
}

void NelderMead::getNMParams(double &stop, int &iter) const
{
	stop = mE;
	iter = mLimit;
}

int NelderMead::getNMKoeffs(vector<double> &koeffs) const
{
	koeffs.clear();
	koeffs.push_back(mKoeff1);
	koeffs.push_back(mKoeff2);
	for (int i = 0; i < mComplexNum * 2; i++) {
		koeffs.push_back(mKoeffs[i]);
	}
	return mComplexNum;
}

void NelderMead::doEqual(double *a, const double *b, const int &arraySize) const
{
	for (int i = 0; i < arraySize; i++)
		a[i] = b[i];
}