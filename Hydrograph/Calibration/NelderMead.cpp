#include "stdafx.h"
#include <cmath>
#include "NelderMead.h"
#include <iostream>

NelderMead::NelderMead(MlcmShell *modelSh)
{
	mModelSh = modelSh;
	mAlpha = 1;
	mBetta = 0.5;
	mGamma = 2;
	mN = 0;
	mMax = 1e15;
	mE = 0.1;
	mLimit = 1000;
	mComplexNum = 1;
	mKoeff1 = 0.1;
	mKoeff2 = 0.2;
	mKoeffs.resize(2);
	mKoeffs[0] = 0.1;
	mKoeffs[1] = 0.2;
}

void NelderMead::setNMStopAndLim(const double &NMStop, const int &NMLim)
{
	mE = NMStop;
	mLimit = NMLim;
}

double NelderMead::doCalibration(Element &nowElement)
{
	int it = 0;
	mF.clear();
	for (int i = 0; i < mSimplexN; i++)
		mF.push_back(countF(mElements[i]));
	while ((!isLim()) && (it++ < mLimit)) {
		//Sort
		double fl = mMax, fg, fh;
		int i, l = 0, g = 1, h = 2;
		for (i = 0; i < mSimplexN; i++)
			if (mF[i] < fl) {
				fl = mF[i];
				l = i;
			}
		swap(l, 0);
		fh = fl;
		for (i = 1; i < mSimplexN; i++)
			if (mF[i] > fh) {
				fh = mF[i];
				h = i;
			}
		swap(h, mSimplexN - 1);
		fg = fl;
		for (i = 1; i < mSimplexN - 1; i++)
			if ((mF[i] > fg) && (mF[i] < fh)) {
				fg = mF[i];
				g = i;
			}
		swap(g, mSimplexN - 2);
		//Mass centre
		Element centre (mN);
		for (i = 0; i < mSimplexN - 1; i++)
			centre += mElements[i];
		centre *= (1.0 / (mSimplexN - 1));
		//Reflection
		Element refl (mN);
		refl = centre * (1.0 + mAlpha) - mElements[mSimplexN - 1] * mAlpha;
		double fr = countF(refl);
		if (fr < fl) {
			//Expansion
			Element expans (mN);
			expans = centre * (1.0 - mGamma) - refl * mGamma;
			double fe = countF(expans);
			if (fe < fl) {
				mElements[mSimplexN - 1] = expans;
				mF[mSimplexN - 1] = fe;
			}
			else
			{
				mElements[mSimplexN - 1] = refl;
				mF[mSimplexN - 1] = fr;
			}
		}
		else {
			if (fr < fg) {
				mElements[mSimplexN - 1] = refl;
				mF[mSimplexN - 1] = fr;
			}
			else {
				//Contraction
				if (fr < fh) {
					mElements[mSimplexN - 1] = refl;
					mF[mSimplexN - 1] = fr;
					fh = fr;
				}
				Element contr (mN);
				contr = mElements[mSimplexN - 1] * mBetta + centre * (1.0 - mBetta);
				double fs = countF (contr);
				if (fs < fh) {
					mElements[mSimplexN - 1] = contr;
					mF[mSimplexN - 1] = fs;
				}
				else {
					for (i = 1; i < mSimplexN; i++) {
						mElements[i] = mElements[0] + (mElements[i] - mElements[0]) * 0.5;
						mF[i] = countF(mElements[i]);
					}
				}
			}
		}
	}
	double fl = mMax;
	int l = 0;
	for (int i = 0; i < mSimplexN; i++)
		if (mF[i] < fl) {
			fl = mF[i];
			l = i;
		}
	nowElement = mElements[l];
	return fl;
}

double NelderMead::doComplexCalibration(const int &N, Element &nowElement) {
	mN = N;
	Element nowE, bestE;
	double now, best;
	doFirstSimplex(mKoeffs[0], mKoeffs[1]);
	best = doCalibration(nowE);
	bestE = nowE;
	for (int i = 1; i < mComplexNum; i++) {
		doFirstSimplex(mKoeffs[2 * i], mKoeffs[2 * i + 1]);
		now = doCalibration(nowE);
		if (now < best) {
			best = now;
			bestE = nowE;
		}
	}
	nowElement = bestE;
	return best;
}

void NelderMead::setKoeffs(const double &koeff1, const double &koeff2)
{
	mKoeff1 = koeff1;
	mKoeff2 = koeff2;
}

void NelderMead::setKoeffs(const vector<double> &koeffs)
{
	mComplexNum = koeffs.size() / 2;
	mKoeffs = koeffs;
	if (mComplexNum < 1) {
		mComplexNum = 1;
		mKoeffs.resize(2, 0.1);
	}
}

void NelderMead::changeN(const int &N)
{
	mN = N;
	doFirstSimplex(mKoeff1, mKoeff2);
}

void NelderMead::doFirstSimplex(const double &koeff1, const double &koeff2)
{
	mElements.clear();
	int i;
	double a0 = Element::maxA[0], k = 19, minC = Element::minC, c = Element::maxC - minC, e = 4, t = 24;
	vector<double> A, Z, A1, Z1;
	for (i = 0; i < mN; i++) {
		A1.push_back(Element::maxA[i + 1]);
		Z1.push_back(Element::maxZ[i]);
		A.push_back(A1.back() * koeff1);
		Z.push_back(Z1.back() * koeff1);
	}
	Element nE0 (mN, a0 * koeff1, minC + c * koeff1, 1 + k * koeff1, 1 + e * koeff1, t * koeff1, A, Z);
	mElements.push_back(nE0);
	Element nE1 (mN, a0 * koeff2, minC + c * koeff1, 1 + k * koeff1, 1 + e * koeff1, t * koeff1, A, Z);
	mElements.push_back(nE1);
	Element nE2 (mN, a0 * koeff1, minC + c * koeff2, 1 + k * koeff1, 1 + e * koeff1, t * koeff1, A, Z);
	mElements.push_back(nE2);
	Element nE3 (mN, a0 * koeff1, minC + c * koeff1, 1 + k * koeff2, 1 + e * koeff1, t * koeff1, A, Z);
	mElements.push_back(nE3);
	Element nE4 (mN, a0 * koeff1, minC + c * koeff1, 1 + k * koeff1, 1 + e * koeff2, t * koeff1, A, Z);
	mElements.push_back(nE4);
	Element nE5 (mN, a0 * koeff1, minC + c * koeff1, 1 + k * koeff1, 1 + e * koeff1, t * koeff2, A, Z);
	mElements.push_back(nE5);
	for (i = 0; i < mN; i++) {
		A[i] = A1[i] * koeff2;
		Element nEl (mN, a0 * koeff1, minC + c * koeff1, 1 + k * koeff1, 1 + e * koeff1, t * koeff1, A, Z);
		mElements.push_back(nEl);
		A[i] = A1[i] * koeff1;
		Z[i] = Z1[i] * koeff2;
		Element nEl1 (mN, a0 * koeff1, minC + c * koeff1, 1 + k * koeff1, 1 + e * koeff1, t * koeff1, A, Z);
		mElements.push_back(nEl1);
		Z[i] = Z1[i] * koeff1;
	}
	mSimplexN = mN * 2 + 6;
}

double NelderMead::countF(const Element &point) const
{
	if (!point.checkPos())
		return mMax;
	mModelSh->changeModelParametrs(point);
	return mModelSh->getFitness();
}

bool NelderMead::isLim() const
{
	int i;
	double f = 0;
	for (i = 0; i < mSimplexN; i++)
		f += mF[i];
	f /= mSimplexN;
	double sigma = 0;
	for (i = 0; i < mSimplexN; i++)
		sigma += pow (mF[i] - f, 2);
	sigma /= mSimplexN;
	sigma = sqrt(sigma);
	if (sigma < mE)
		return 1;
	return 0;
}

void NelderMead::swap(const int &i, const int &j)
{
	if (i == j)
		return;
	double tmpF = mF[i];
	Element tmpE = mElements[i];
	mF[i] = mF[j];
	mElements[i] = mElements[j];
	mF[j] = tmpF;
	mElements[j] = tmpE;
}

void NelderMead::getNMParams(double &stop, int &iter) const
{
	stop = mE;
	iter = mLimit;
}

int NelderMead::getNMKoeffs(std::vector<double> &koeffs) const
{
	koeffs.clear();
	koeffs.push_back(mKoeff1);
	koeffs.push_back(mKoeff2);
	for (int i = 0; i < mComplexNum * 2; i++) {
		koeffs.push_back(mKoeffs[i]);
	}
	return mComplexNum;
}