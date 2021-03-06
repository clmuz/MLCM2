#pragma once
#include <vector>
#include "..\ModelsShell.h"

//������� ������� MSOF

using namespace std;

class ModelsShell;

class Msof
{
public:
	explicit Msof(ModelsShell *modShell);
	//���������� ������ � ����� �������� MSOF
	void setBegEnd(const int &begDay, const int &endDay);
	//������� �������� MSOF
	double countError() const;
	//������� �������� �� ��� ����������� ������
	double countError(const vector<double> &modVal) const;
	//���������� ���������� ��������� � ����
	void setMeasPerDay(const int &measPerDay);
	//���������� �������� ������
	void setRealVal(vector<double> *realDat, const int &gap);
private:
	//������� ������� �������� ������ � ���� � � �����
	void countRealVal();
	//������� ����
	void countSigmas();
	//������ � ����� �������� MSOF
	int mDayBeg, mDayEnd;
	//���������� ��������� � ����
	int mMeasPerDay;
	//������� ����� �������� � ��������� ������� � ����
	int mGap;
	//�������� ������
	vector<double> *mRealVal;
	//�������������� ��� �������� ������
	vector<double> mAveDaily;
	//�������������� ��� �������� ������
	vector<double> mAveMonthly;
	//���������� ��������� ����
	double mSigma1to2sq, mSigma1to3sq;
	//������ ������ ����
	static const double mLowerLim;
	//MLCM
	ModelsShell *mShell;
};
