#pragma once
#include <vector>
#include "..\Fitness\Fitness.h"

//���������� �������-����

class NelderMead
{
public:
	explicit NelderMead(Fitness *fitn);
	//��������� ���������� �� ������ ���������� ���������
	double doCalibration(const int &paramsNum, double *nowParams);
	//��������� ���������� ��� ���������� ��������� ����������
	double doComplexCalibration(const int &paramsNum, double *nowParams);
	//���������� ������������ ���������� ��������� ��� ��������� ����������
	void setKoeffs(const double &koeff1, const double &koeff2);
	//���������� ������������ ���������� ��� ����������������� ����������
	void setKoeffs(const vector<double> &koeffs);
	//���������� ������� �������� � ����� �� ���������� ��������
	void setNMStopAndLim(const double &NMStop, const int &NMLim);
	//������ ������� �������� � ����� �� ���������� ��������
	void getNMParams(double &stop, int &iter) const;
	//����� ���� ������������� ����������
	int getNMKoeffs(vector<double> &koeffs) const;
private:
	//��������� ���������� �� ���������
	double doNelderMead(double *bestParams, vector<double*> &simplex) const;
	//������� �������� �� �������������
	void doFirstSimplex(const double &koeff1, const double &koeff2, vector<double*> &simplex) const;
	//��������� ������� ������� �� ������ ���������� ������ point
	double countF(const double *param) const;
	//�������� �� ������� �������� (�������� �������� ������� ������� � �������� ���������)
	bool isLim(const double *Fs) const;
	//swap ������ ��������� � �� �������� �������
	void swap(const int &i, const int &j, vector<double*> &simplex, double *Fs) const;
	//������������� ������� a ������ b
	void doEqual(double *a, const double *b, const int &arraySize) const;
	//������������ �������-����
	static const double mAlpha, mBetta, mGamma;
	//������ ��� ��������, ���� ��������� ������ ������� �� ������� ������� �����������.
	//��������������, ��� ��� ���� ������ �������� ������� �������
	static const double mMax;
	//����������� �������� ��������� (����� �� ���� ������� � ����)
	static const double mMin;
	//������� �������� - ��� �������� �� �������� �������� ������� �������
	double mE;
	//������������ ��������� ��� ��������� ����������
	double mKoeff1, mKoeff2;
	//���������� ����������� �����������
	int mParamsNum;
	//���������� ���������� � ����������������� ����������
	int mComplexNum;
	//����� �� ���������� ��������
	int mLimit;
	Fitness *mFitness;
	//������������ ���������� ��� ����������������� ����������
	vector<double> mKoeffs;
};
