#include "stdafx.h"
#include <cmath>
#include "NelderMead.h"

const double NelderMead::mAlpha = 1.0, NelderMead::mBetta = .5, NelderMead::mGamma = 2.0;
const double NelderMead::mMax = 1e15;

NelderMead::NelderMead(MlcmShell *modelSh) :
	mModelSh(modelSh),
	mN(0),
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

double NelderMead::doNelderMead(Element &nowElement, vector<Element*> &elements) const
{
	int it = 0;
	vector<double> Fs;
	for (int i = 0; i < mSimplexN; i++)
		Fs.push_back(countF(*elements[i]));
	while ((!isLim(Fs)) && (it++ < mLimit)) {
		//Sort
		double fl = mMax, fg, fh;
		int i, l = 0, g = 1, h = 2;
		for (i = 0; i < mSimplexN; i++)
			if (Fs[i] < fl) {
				fl = Fs[i];
				l = i;
			}
		swap(l, 0, elements, Fs);
		fh = fl;
		for (i = 1; i < mSimplexN; i++)
			if (Fs[i] > fh) {
				fh = Fs[i];
				h = i;
			}
		swap(h, mSimplexN - 1, elements, Fs);
		fg = fl;
		for (i = 1; i < mSimplexN - 1; i++)
			if ((Fs[i] > fg) && (Fs[i] < fh)) {
				fg = Fs[i];
				g = i;
			}
		swap(g, mSimplexN - 2, elements, Fs);
		//Mass centre
		Element centre (mN);
		for (i = 0; i < mSimplexN - 1; i++)
			centre += *elements[i];
		centre *= (1.0 / (mSimplexN - 1));
		//Reflection
		Element refl (mN);
		refl = centre * (1.0 + mAlpha) - *elements[mSimplexN - 1] * mAlpha;
		double fr = countF(refl);
		if (fr < fl) {
			//Expansion
			Element expans (mN);
			expans = centre * (1.0 - mGamma) - refl * mGamma;
			double fe = countF(expans);
			if (fe < fl) {
				*elements[mSimplexN - 1] = expans;
				Fs[mSimplexN - 1] = fe;
			}
			else
			{
				*elements[mSimplexN - 1] = refl;
				Fs[mSimplexN - 1] = fr;
			}
		}
		else {
			if (fr < fg) {
				*elements[mSimplexN - 1] = refl;
				Fs[mSimplexN - 1] = fr;
			}
			else {
				//Contraction
				if (fr < fh) {
					*elements[mSimplexN - 1] = refl;
					Fs[mSimplexN - 1] = fr;
					fh = fr;
				}
				Element contr (mN);
				contr = *elements[mSimplexN - 1] * mBetta + centre * (1.0 - mBetta);
				double fs = countF (contr);
				if (fs < fh) {
					*elements[mSimplexN - 1] = contr;
					Fs[mSimplexN - 1] = fs;
				}
				else {
					for (i = 1; i < mSimplexN; i++) {
						*elements[i] = *elements[0] + (*elements[i] - *elements[0]) * 0.5;
						Fs[i] = countF(*elements[i]);
					}
				}
			}
		}
	}
	double fl = mMax;
	int l = 0;
	for (int i = 0; i < mSimplexN; i++)
		if (Fs[i] < fl) {
			fl = Fs[i];
			l = i;
		}
	nowElement = *elements[l];
	return fl;
}

double NelderMead::doCalibration(const int &N, Element &nowElement)
{
	mN = N;
	mSimplexN = mN * 2 + 6;
	vector<Element*> elements (mSimplexN);
	for (int i = 0; i < mSimplexN; i++)
		elements[i] = new Element(mN);
	doFirstSimplex(mKoeff1, mKoeff2, elements);
	double f = doNelderMead(nowElement, elements);
	for (int i = 0; i < mSimplexN; i++)
		delete elements[i];
	return f;
}

double NelderMead::doComplexCalibration(const int &N, Element &nowElement)
{
	mN = N;
	mSimplexN = mN * 2 + 6;
	Element nowE, bestE;
	double now, best;
	vector<Element*> elements (mSimplexN);
	for (int i = 0; i < mSimplexN; i++)
		elements[i] = new Element(mN);
	doFirstSimplex(mKoeffs[0], mKoeffs[1], elements);
	best = doNelderMead(nowE, elements);
	bestE = nowE;
	for (int i = 1; i < mComplexNum; i++) {
		doFirstSimplex(mKoeffs[2 * i], mKoeffs[2 * i + 1], elements);
		now = doNelderMead(nowE, elements);
		if (now < best) {
			best = now;
			bestE = nowE;
		}
	}
	for (int i = 0; i < mSimplexN; i++)
		delete elements[i];
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
	mComplexNum = floor(koeffs.size() * 0.5);
	mKoeffs = koeffs;
	if (mComplexNum < 1) {
		mComplexNum = 1;
		mKoeffs.resize(2, 0.1);
	}
}

void NelderMead::doFirstSimplex(const double &koeff1, const double &koeff2, vector<Element*> &elements) const
{
	int i, j;
	for (j = 0; j < 2 * mN + 5; j++) {
		elements[0]->setCoord(koeff1, j);
	}
	for (i = 1; i < mSimplexN; i++) {
		for (j = 0; j < 2 * mN + 5; j++) {
			elements[i]->setCoord(koeff1, j);
		}
		elements[i]->setCoord(koeff2, i - 1);
	}
}

double NelderMead::countF(const Element &point) const
{
	if (!point.checkPos())
		return mMax;
	return mModelSh->countF(point);
}

bool NelderMead::isLim(const vector<double> &Fs) const
{
	int i;
	double f = 0;
	for (i = 0; i < mSimplexN; i++)
		f += Fs[i];
	f /= mSimplexN;
	double sigma = 0;
	for (i = 0; i < mSimplexN; i++)
		sigma += pow (Fs[i] - f, 2);
	sigma /= mSimplexN;
	sigma = sqrt(sigma);
	if (sigma < mE)
		return 1;
	return 0;
}

void NelderMead::swap(const int &i, const int &j, vector<Element*> &elements, vector<double> &Fs) const
{
	if (i == j)
		return;
	double tmpF = Fs[i];
	Element *tmpE = elements[i];
	Fs[i] = Fs[j];
	elements[i] = elements[j];
	Fs[j] = tmpF;
	elements[j] = tmpE;
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