#pragma once
#include <vector>
#include "..\Mlcm.h"

//������������������ ������

class AbsoluteSquareError
{
public:
	explicit AbsoluteSquareError(Mlcm *model);
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
	Mlcm *mModel;
	//�������� ������
	vector<double> *mRealVal;
	//������� ����� �������� � ��������� ������� � ����
	int mGap;
	//������ � ����� �������� ������������������ ������
	int mDayBeg, mDayEnd;
	//���������� ��������� � ����
	int mMeasPerDay;
};
