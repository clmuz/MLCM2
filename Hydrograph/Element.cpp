#include "stdafx.h"
#include "Element.h"

double Element::maxA[11], Element::maxZ[10], Element::minC, Element::maxC;

Element::Element()
{
}

Element::Element(const int &numLayers)
{
	mMin = 1e-2;
	N = numLayers;
	a = mMin;
	c = minC;
	k = 1;
	e = 1;
	t = 0;
	vector<double> newA (N, mMin);
	vector<double> newZ (N, mMin);
	Al = newA;
	Z = newZ;
}

Element::Element(const int &numLayers
				 , const double &alpha0
				 , const double &C
				 , const double &kuh
				 , const double &ettauh
				 , const double &tuh
				 , const vector<double> &alpha
				 , const vector<double> &layersZ)
{
	N = numLayers;
	a = alpha0;
	c = C;
	k = kuh;
	e = ettauh;
	t = tuh;
	Al = alpha;
	Z = layersZ;
}

Element Element::operator +(const Element &oth) const
{
	vector<double> newA, newZ;
	for (int i = 0; i < N; i++) {
		newA.push_back(Al[i] + oth.Al[i]);
		newZ.push_back(Z[i] + oth.Z[i]);
	}
	Element newElement (N, a + oth.a, c + oth.c, k + oth.k, e + oth.e, t + oth.t, Al, Z);
	return newElement;
}

Element Element::operator -(const Element &oth) const
{
	vector<double> newA, newZ;
	for (int i = 0; i < N; i++) {
		newA.push_back(Al[i] - oth.Al[i]);
		newZ.push_back(Z[i] - oth.Z[i]);
	}
	Element newElement (N, a - oth.a, c - oth.c, k - oth.k, e - oth.e, t - oth.t, Al, Z);
	return newElement;
}

void Element::operator +=(const Element &oth)
{
	a += oth.a;
	c += oth.c;
	k += oth.k;
	e += oth.e;
	t += oth.t;
	for (int i = 0; i < N; i++) {
		Al[i] += oth.Al[i];
		Z[i] += oth.Z[i];
	}
}

void Element::operator *=(const double &con)
{
	a *= con;
	c *= con;
	k *= con;
	e *= con;
	t *= con;
	for (int i = 0; i < N; i++) {
		Al[i] *= con;
		Z[i] *= con;
	}
}

Element Element::operator *(const double &con) const
{
	double na, nc, nk, ne, nt;
	vector<double> nAl = Al, nZ = Z;
	na = a * con;
	nc = c * con;
	nk = k * con;
	ne = e * con;
	nt = t * con;
	for (int i = 0; i < N; i++) {
		nAl[i] *= con;
		nZ[i] *= con;
	}
	Element newElement (N, na, nc, nk, ne, nt, nAl, nZ);
	return newElement;
}

void Element::plus(const double &koeff, const int &coord)
{
	if (coord >= 2 * N + 5)
		return;
	switch(coord) {
	case 0:
		a += maxA[0] * koeff;
		return;
	case 1:
		c += (maxC - minC) * koeff;
		return;
	case 2:
		k += 19 * koeff;
		return;
	case 3:
		e += 4 * koeff;
		return;
	case 4:
		t = 24 * koeff;
		return;
	default:
		if (coord % 2 == 1) {
			Al[(coord - 5) / 2] += maxA[(coord - 5) / 2 + 1] * koeff;
		}
		else {
			Z[(coord - 6) / 2] += maxZ[(coord - 6) / 2] * koeff;
		}
		return;
	}
}

void Element::setCoord(const double &koeff, const int &coord)
{
	if (coord >= 2 * N + 5)
		return;
	switch(coord) {
	case 0:
		a = mMin + maxA[0] * koeff;
		return;
	case 1:
		c = minC + (maxC - minC) * koeff;
		return;
	case 2:
		k = 1 + 19 * koeff;
		return;
	case 3:
		e = 1 + 4 * koeff;
		return;
	case 4:
		t = 24 * koeff;
		return;
	default:
		if (coord % 2 == 1) {
			Al[(coord - 5) / 2] = mMin + maxA[(coord - 5) / 2 + 1] * koeff;
		}
		else {
			Z[(coord - 6) / 2] = mMin + maxZ[(coord - 6) / 2] * koeff;
		}
		return;
	}
}

double* Element::toKoeffs() const
{
	double *koeffs = new double [2 * N + 5];
	koeffs[0] = a / maxA[0];
	koeffs[1] = (c - minC) / (maxC - minC);
	koeffs[2] = (k - 1) / 19.0;
	koeffs[3] = (e - 1) / 4.0;
	koeffs[4] = t / 24.0;
	for (int i = 0; i < N; i++) {
		koeffs[2 * N + 5] = Al[i] / maxA[i + 1];
		koeffs[2 * N + 6] = Al[i] / maxZ[i];
	}
	return koeffs;
}

int Element::getN() const
{
	return N;
}

double Element::getAlpha0() const
{
	return a;
}

double Element::getC() const
{
	return c;
}

double Element::getK() const
{
	return k;
}

double Element::getEtta() const
{
	return e;
}

double Element::getT() const
{
	return t;
}

vector<double> Element::getAlphas() const
{
	return Al;
}

vector<double> Element::getZ() const
{
	return Z;
}

bool Element::checkPos() const
{
	if ((a <= 0) || (a > maxA[0]))
		return 0;
	if ((c < minC) || (c > maxC))
		return 0;
	if ((k < 1) || (k > 20))
		return 0;
	if ((e < 1) || (e > 5))
		return 0;
	if ((t < 0) || (t > 24))
		return 0;
	for (int i = 0; i < N; i++) {
		if ((Al[i] < mMin) || (Al[i] > maxA[i + 1]))
			return 0;
		if ((Z[i] < mMin) || (Z[i] > maxZ[i]))
			return 0;
	}
	return 1;
}

void Element::setLimits(double *newMaxA, double *newMaxZ)
{
	for (int i = 0; i < 11; i++)
		maxA[i] = newMaxA[i];
	for (int i = 0; i < 10; i++)
		maxZ[i] = newMaxZ[i];
}
