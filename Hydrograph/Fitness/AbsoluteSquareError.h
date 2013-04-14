#pragma once
#include <vector>
#include "..\ModelsShell.h"

//������������������ ������

class ModelsShell;

class AbsoluteSquareError
{
public:
	explicit AbsoluteSquareError(ModelsShell *modShell);
	//��������� ������������������ ������
	double countError() const;
	//��������� ������ �� ��� ����������� ������
	double countError(const vector<double> &modVal) const;
	//���������� ������ � ����� �������� ������������������ ������
	void setBegEnd(const int &begDay, const int &endDay);
	//���������� ���������� ��������� � ����
	void setMeasPerDay(const int &measPerDay);
	//���������� �������� ������
	void setRealVal(vector<double> *realDat, const int &gap);
private:
	ModelsShell *mShell;
	//�������� ������
	vector<double> *mRealVal;
	//������� ����� �������� � ��������� ������� � ����
	int mGap;
	//������ � ����� �������� ������������������ ������
	int mDayBeg, mDayEnd;
	//���������� ��������� � ����
	int mMeasPerDay;
};
