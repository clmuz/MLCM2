#pragma once
#include "..\Fitness\Fitness.h"
#include "..\ModelsShell.h"
#include "NelderMead.h"
#include "SLS.h"
#include "BruteForce.h"
#include <vector>

//�����, ����������� ��������� ����������
//0 - ������-��� �� ������ ���������, 1 - ����������������� ������-���, 2 - SLS, 3 - �������

class SLS;

enum calibrationType
{
	CT_NULL = -1,
	CT_NelderMead,
	CT_ComplexNelderMead,
	CT_SLS,
	CT_BruteForce
};

class Calibration
{
public:
	Calibration(ModelsShell *modShell, Fitness *fitn);
	~Calibration();
	//��������� ����������
	double doCalibration();
	//��������� ���������� ��� N �����, � ����� ���������� calType
	double doCalStep(const int &paramNum, double *bestParams, calibrationType calType = CT_NULL);
	//���������� ��� ����������
	void setCalibrationType(const calibrationType &calType);
	//���������� ������� �������� � ����� �� ���������� �������� � �������-����
	void setNMStopAndLim(const double &NMStop, const int &NMLim);
	//���������� ������������ ��������� ��� ���������������� �������-����
	void setNMKoeffs(const double &koeff1, const double &koeff2);
	//���������� ������������ ���������� ��� ���������������� �������-����
	void setNMKoeffs(const vector<double> &koeffs);
	//���������� ���������� ����� � �������� � ��������
	void setBFParams(const int &stepsNum, const int &iterNum);
	//���������� ���, ����� �� ���������� �������� � ��� ��������������� ���������� ��� SLS
	void setSlsParam(const double &slsStep, const int &slsLim, const calibrationType &slsCalType);
	//������ ���, ����� �� ���������� �������� � ��� ��������������� ���������� ��� SLS
	void getSlsParams(double &slsStep, int &slsLim, calibrationType &slsCalType) const;
	//���������� ����������� ������� �� ����������� ���� � ������
	void setMinGrowth(const double &minGrowth);
	//������ ��� ����������
	 calibrationType getCalType() const;
	//������ ������� �������� � ����� �� ���������� �������� � �������-����
	void getNMParams(double &stop, int &iter) const;
	//������ ������������ ���������� � �������-����
	int getNMKoeffs(vector<double> &koeffs) const;
	//������ ����������� ������� �� ����������� ���� � ������
	double getMinGrowth() const;
	//������ ���������� ����� � �������� � ��������
	void getBFParams(int &stepsNum, int &iterNum) const;
private:
	//��� ����������
	 calibrationType mCalType;
	//����������� ������� �� ����������� ���� � ������
	double mMinGrowth;
	ModelsShell *mShell;
	Fitness *mFitness;
	NelderMead *mNM;
	SLS *mSLS;
	BruteForce *mBF;
};
