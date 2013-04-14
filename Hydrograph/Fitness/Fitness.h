#pragma once
#include "..\ModelsShell.h"
#include "Msof.h"
#include "AbsoluteSquareError.h"

//�����, ����������� ������ ������� �������
// 0 - MSOF, 1 - ������������������ ����������

class ModelsShell;
class Msof;
class AbsoluteSquareError;

enum fitnessType
{
	FT_DefaultVal = -2,
	FT_Default,
	FT_MSOF,
	FT_ASE
};

class Fitness
{
public:
	explicit Fitness(ModelsShell *modShell);
	~Fitness();
	//���������� ������� ������� ��� ����������
	void setDefFitnessType(const fitnessType &fitnType);
	//���������� ������� ������� ��� ���������
	void setValType(const fitnessType &valType);
	//���������� ���������� ��������� � ����
	void setMeasPerDay(const int &measPerDay);
	//���������� �������� ��������
	void setRealVal(vector<double> *realDat, const int &gap);
	//��������� ������� ������� ��� ���������� (�� ���������)
	double getFitness() const;
	//��������� ������� ������� ���� fitnessType
	double getFitness(const fitnessType &fitnType);
	//��������� ������� ������� �� ����������
	double getFitness(const double *params);
	//��������� ���������
	double getValFitness() const;
	//���������� ������ � ����� �������� ������� �������
	void setBegEnd(const int &begDay, const int &endDay);
	//����� ����� ������� �������
	void getFitnessTypes(fitnessType &defType, fitnessType &valType) const;
private:
	//�������� ������� ������� ���� fitnessType
	void createFitnessFunction(const fitnessType &fitnType);
	//��� ������� ������� ��� ����������
	fitnessType mFitnessType;
	//��� ������� ������� ��� ���������
	fitnessType mValType;
	//���������� ��������� ������� �������
	static const int mMaxClassesNum;
	//���������� ��������� � ����
	int mMeasPerDay;
	//������� ����� �������� � ��������� ������� � ����
	int mGap;
	//�������� ������
	vector<double> *mRealVal;
	//�������� �� ��� ������ ������� �������
	bool *mCreatedClasses;
	//������ � ����� �������� ������� �������
	int mDayBeg, mDayEnd;
	ModelsShell *mShell;
	Msof *mMsof;
	AbsoluteSquareError *mAse;
};
