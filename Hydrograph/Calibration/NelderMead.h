#pragma once
#include <vector>
#include "..\MlcmShell.h"
#include "..\Element.h"

using namespace std;

class NelderMead
{
public:
	explicit NelderMead(MlcmShell *modelSh);
	double doCalibration(const int &N, Element &nowElement);
	double doComplexCalibration(const int &N, Element &nowElement);
	void setKoeffs(const double &koeff1, const double &koeff2);
	void setKoeffs(const vector<double> &koeffs);
	void setNMStopAndLim(const double &NMStop, const int &NMLim);
	void getNMParams(double &stop, int &iter) const;
	int getNMKoeffs(std::vector<double> &koeffs) const;
private:
	double doNelderMead(Element &nowElement, vector<Element*> &elements) const;
	void doFirstSimplex(const double &koeff1, const double &koeff2, vector<Element*> &elements) const;
	double countF(const Element &point) const;
	bool isLim(const vector<double> &Fs) const;
	void swap(const int &i, const int &j, vector<Element*> &elements, vector<double> &Fs) const;
	double mAlpha, mBetta, mGamma, mMax, mE, mKoeff1, mKoeff2;
	int mN, mSimplexN, mComplexNum, mLimit;
	MlcmShell *mModelSh;
	vector<double> mKoeffs;
};
