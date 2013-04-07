#pragma once
#include "..\Mlcm.h"
#include "Msof.h"
#include "AbsoluteSquareError.h"

//�����, ����������� ������ ������� �������
// 0 - MSOF, 1 - ������������������ ����������

class Fitness
{
public:
	explicit Fitness(Mlcm *model);
	~Fitness();
	//���������� ������� ������� ��� ����������
	void setDefFitnessType(const int &fitnessType);
	//���������� ������� ������� ��� ���������
	void setValType(const int &valType);
	//���������� ���������� ��������� � ����
	void setMeasPerDay(const int &measPerDay);
	//���������� �������� ��������
	void setRealVal(vector<double> *realDat, const int &gap);
	//��������� ������� ������� ��� ���������� (�� ���������)
	double getFitness() const;
	//��������� ������� ������� ���� fitnessType
	double getFitness(const int &fitnessType);
	//��������� ������� ������� ���� fitnessType �� ��� ����������� ������
	double getFitness(const int &fitnessType, const vector<double> &modVal);
	//��������� ���������
	double getValFitness() const;
	//��������� ��������� �� ��� ����������� ������
	double getValFitness(const vector<double> &modVal) const;
	//���������� ������ � ����� �������� ������� �������
	void setBegEnd(const unsigned int &begDay, const unsigned int &endDay);
	//����� ����� ������� �������
	void getFitnessTypes(int &defType, int &valType) const;
private:
	//�������� ������� ������� ���� fitnessType
	void createFitnessFunction(const int &fitnessType);
	//��� ������� ������� ��� ����������
	int mFitnessType;
	//��� ������� ������� ��� ���������
	int mValType;
	//���������� ��������� ������� �������
	static const int mMaxClassesNum;
	//���������� ��������� � ����
	int mMeasPerDay;
	//������� ����� �������� � ��������� ������� � ����
	int mGap;
	//�������� ������
	vector<double> *mRealVal;
	//�������� �� ��� ������ ������� �������
	vector<bool> mCreatedClasses;
	//������ � ����� �������� ������� �������
	int mDayBeg, mDayEnd;
	Mlcm *mModel;
	Msof *mMsof;
	AbsoluteSquareError *mAse;
};
