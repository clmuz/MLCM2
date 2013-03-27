#pragma once
#include <vector>

using namespace std;

class Element
{
public:
	Element();
	Element(const int &numLayers);
	Element(const int &numLayers
			 , const double &alpha0
			 , const double &C
			 , const double &kuh
			 , const double &ettauh
			 , const double &tuh
			 , const vector<double> &alpha
			 , const vector<double> &layersZ);
	Element operator+ (const Element &oth) const;
	Element operator- (const Element &oth) const;
	void	operator+=(const Element &oth);
	void	operator*=(const double &con);
	Element operator* (const double &con) const;
	int    getN() const;
	double getAlpha0() const;
	double getK() const;
	double getEtta() const;
	double getT() const;
	double getC() const;
	void setLimits(double *mA, double *mZ);
	vector<double> getAlphas() const;
	vector<double> getZ() const;
	bool checkPos() const;
	static double maxA[11], maxZ[10], minC, maxC;
protected:
	int N;
	double a, c, k, e, t;
	vector<double> Al, Z;
};
