#pragma once
#include "..\Fitness\Fitness.h"
#include "Calibration.h"

//���������� SLS

class Calibration;
enum calibrationType;
class Fitness;

class SLS
{
public:
	SLS(Fitness *fitn, Calibration *cal);
	//���������� ��������� SLS
	void setParams(const double &step, const int &lim, const calibrationType &cal0Type);
	//������ ��������� SLS
	void getParams(double &step, int &lim, calibrationType &cal0Type) const;
	//��������� ����������
	double doCalibration(const int &paramsNum, double *bestParams);
private:
	//��������� ������� ������� �� ������ ���������� ������ point
	double countF(double *param, const int &paramsNum) const;
	Fitness *mFitness;
	//������� ����� ����������. ������ �� ���� ����� ��� ����, ����� ��������� ��������� ���������� ����������
	Calibration *mCal;
	//���� ��� (������ �� 0.01 �� 0.05)
	double mStep;
	//������ ��� ��������, ���� ��������� ������ ������� �� ������� ������� �����������.
	//��������������, ��� ��� ���� ������ �������� ������� �������
	static const double mMax;
	//����������� �������� ��������� (����� �� ���� ������� � ����)
	static const double mMin;
	//����� �� ���������� ��������
	int mLim;
	//��� ��������� ���������� ����������
	calibrationType m0Type;
};