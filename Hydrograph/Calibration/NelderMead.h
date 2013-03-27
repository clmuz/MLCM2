#pragma once
#include <vector>
#include "..\MlcmShell.h"
#include "..\Element.h"

using namespace std;

class NelderMead
{
public:
	NelderMead(MlcmShell *modelSh);
	double doCalibration(Element &nowElement);
	double doComplexCalibration(const int &N, Element &nowElement);
	void setKoeffs(const double &koeff1, const double &koeff2);
	void setKoeffs(const vector<double> &koeffs);
	void changeN(const int &N);
	void setNMStopAndLim(const double &NMStop, const int &NMLim);
	void getNMParams(double &stop, int &iter) const;
	int getNMKoeffs(std::vector<double> &koeffs) const;
private:
	void doFirstSimplex(const double &koeff1, const double &koeff2);
	double countF(const Element &point) const;
	bool isLim() const;
	void swap(const int &i, const int &j);
	double mAlpha, mBetta, mGamma, mMax, mE, mKoeff1, mKoeff2;
	int mN, mSimplexN, mComplexNum, mLimit;
	MlcmShell *mModelSh;
	vector<Element> mElements;
	vector<double> mF, mKoeffs;
};
