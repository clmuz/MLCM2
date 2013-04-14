#pragma once
#include "..\Fitness\Fitness.h"

//���������� ���������

class BruteForce
{
public:
	explicit BruteForce(Fitness *fitn);
	//��������� ���������� ����� � ��������
	void setBFParams(const int &stepsNum, const int &iterNum);
	//������ ���������� ����� � ��������
	void getBFParams(int &stepsNum, int &iterNum) const;
	//����������� ���� ����
	double doCalibration(const int &paramsNum, double *bestParams) const;
private:
	//����������� ������� �� leftBorder �� ������ �� width
	void calibrateGap(const int &paramsNum, double &f, const double *leftBorder, const double &width, double *bestParams) const;
	//����� ��������
	void recursion(double &f
				, const int &coord
				, const int &paramsNum
				, const double &step
				, const double *leftBorder
				, const double *nowParams
				, double *bestParams) const;
	Fitness *mFitness;
	//���������� �����
	int mStepsNum;
	//���������� ��������
	int mIterNum;
	//������ ��� ��������, ���� ��������� ������ ������� �� ������� ������� �����������.
	//��������������, ��� ��� ���� ������ �������� ������� �������
	static const double mMax;
	//����������� �������� ����������, ��� ��� ��� ���� ����� ���������� ��������
	static const double mMin;
};