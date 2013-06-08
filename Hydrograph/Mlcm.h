#pragma once
#include <vector>
#include <queue>

//MLCM

using namespace std;

class Mlcm
{
public:
	Mlcm(vector<double> *P, vector<double> *ET);
	~Mlcm();
	//���������� ���������� ������� ���������� ����������
	void setNuh(const int &nuh);
	//���������� �������� ������ � �� ������
	void setRealData(vector<double> *realData, const int &realDatBeg);
	//���������� ���������� �����
	void setN(const int &n);
	int getN() const;
	//���������� �������� ������
	void setParam(const double *params);
	//���������� ���������� ����� �������� ������
	void setWarmingSteps(const int &countOfWarmingSteps);
	//������ ���������� ����� �������� ������
	int getWarmingSteps() const;
	//���������� ����������� �� �������� ����� ���� � ������ �����
	void setMaxAandZ(const double *maxA, const double *maxZ);
	//���������� ����������� �� �������� ������������� ���������� �����
	void setCLim(const double &minC, const double &maxC);
	void getMaxAandZ(double *maxA, double *maxZ) const;
	void getCLim(double &minC, double &maxC) const;
	//������ ������
	vector<double> makeRunoff(const int &timeBeg, const int &timeEnd) const;
	//������� ��� ����������� MLCM � ������� ���������� �����
	int click();
	void printParams(const wchar_t *outputParamFile) const;
	void loadParams(const wchar_t *inputParamFile);
private:
	//������ �������� ���������� ����������
	void makeHydrOrd();
	//������� Qsum � ������ ������� time
	double countUhT(const vector<double> &Qsum, const int &time) const;
	//���� ��� ������ (��� = 24/<���������� ��������� � ����>)
	double makeStep(double P, double ET, vector<double> &state) const;
	//���������� �����
	int mN;
	//���������� ������� � ������ ������ �� �������
	int mNOrd;
	//���������� �������
	int mNuh;
	//������ �������� ������ ������������ ������ ������� � ���������� �����
	int mRealBeg;
	//����� �������� ������ ������������ ������ ������� � ���������� �����
	int mRealEnd;
	//���������� �������� ������ �� ������ ������ (int* ����� �� �������� ������������� ������ ������)
	int *mClick;
	//���������� ����� �������� ������
	int mWarmingSteps;
	//�������� ������������� ����������� ����
	double mC;
	//etta � k - ��� �������� ������� ���������� ���������� � ������� ����� �������
	double mEtta;
	double mK;
	//����� �� �������
	double mT;
	//�������� ���������� ����������
	vector<double> mFxOrd;
	//������ ��������� ����� ����
	double *mAlpha;
	//������ ����� �����
	double *mZ;
	//������
	vector<double> *mP;
	//���������
	vector<double> *mET;
	//�������� ������
	vector<double> *mRealData;
	//����������� �� �������� ����� ���� (�3/�) (0 - �������������, 10 - 10-�� ����)
	double *mMaxA;
	//����������� �� ������ ����� (�3) (0 - 1-�� ����)
	double *mMaxZ;
	//����������� �� �������� ������������� ���������� �����
	double mMinC, mMaxC;
};
